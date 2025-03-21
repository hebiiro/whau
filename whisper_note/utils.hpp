#pragma once

//
// std::stringをstd::wstringに変換して返します。
// https://nprogram.hatenablog.com/entry/2018/01/03/113206
//
inline std::wstring cp_to_wide(const std::string &s, UINT codepage)
{
	int in_length = (int)s.length();
	int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0); 
	std::wstring result(out_length, L'\0');
	if (out_length) MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0], out_length);
	return result;
}

//
// std::wstringをstd::stringに変換して返します。
// https://nprogram.hatenablog.com/entry/2018/01/03/113206
//
inline std::string wide_to_cp(const std::wstring &s, UINT codepage)
{
	int in_length = (int)s.length();
	int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0); 
	std::string result(out_length, '\0');
	if (out_length) WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0], out_length, 0, 0); 
	return result;
}

#ifdef UNICODE
inline std::wstring ts(const std::string &s) { return cp_to_wide(s, CP_ACP); }
inline std::wstring ts(const std::wstring &s) { return s; }
#else
inline std::string ts(const std::string &s) { return s; }
inline std::string ts(const std::wstring &s) { return wide_to_cp(s, CP_ACP); }
#endif

//
// 実行可能ファイルのパスを返します。
//
inline std::filesystem::path get_module_file_name(HINSTANCE instance, size_t buffer_length = MAX_PATH)
{
	auto buffer = std::make_unique<char[]>(buffer_length);
	::GetModuleFileNameA(instance, buffer.get(), (DWORD)buffer_length);
	return buffer.get();
}

//
// 指定されたファイルを実行します。
//
inline BOOL shell_execute(LPCTSTR verb, LPCTSTR file, LPCTSTR parameters)
{
	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = verb;
	sei.nShow = SW_SHOWNORMAL;
	sei.lpFile = file;
	sei.lpParameters = parameters;
	return ::ShellExecuteEx(&sei);
}

//
// 指定された引数を文字列化して返します。
//
inline std::wstring to_string(const CString& s)
{
	return (LPCWSTR)s;
}

//
// 指定された文字列が有効な引数の場合はTRUEを返します。
//
inline BOOL is_arg_valid(const std::wstring& s)
{
	return s.empty() ? FALSE : (s != L"指定なし");
}

//
// 指定された文字列を16進数文字列にして返します。
//
inline std::string to_hex_string(const std::wstring& text)
{
	uint8_t code[2048] = {};
	memcpy(code, text.data(), min(sizeof(code), text.size() * sizeof(WCHAR)));

	std::string hex_string;
	for (size_t i = 0; i < std::size(code); i++)
		hex_string += std::format("{:02x}", code[i]);
	return hex_string;
}

//
// このクラスはコンソールです。
//
struct Console
{
	//
	// コンソールの出力ハンドルです。
	//
	HANDLE output = {};

	//
	// コンストラクタです。
	//
	Console()
		: output(::GetStdHandle(STD_OUTPUT_HANDLE))
	{
	}

	//
	// 指定された文字列をコンソールに書き込みます。
	//
	BOOL write(const std::wstring& s)
	{
		return !!::WriteConsoleW(output, s.c_str(), (DWORD)s.size(), nullptr, nullptr);
	}
};

//
// このクラスはサブプロセスです。
//
struct SubProcess
{
	PROCESS_INFORMATION pi = {};

	//
	// コンストラクタです。
	//
	SubProcess(const std::wstring& dir, const std::wstring& _command_line, BOOL show = TRUE)
	{
		// 一旦コマンドラインをコピーします。
		WCHAR command_line[4096] = {};
		wcsncpy_s(command_line, std::size(command_line), _command_line.c_str(), _TRUNCATE);

		// サブプロセスを起動します。
		STARTUPINFOW si = { sizeof(si) };
		if (!show)
		{
			// 非表示で起動するようにします。
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
		}
		auto r1 = ::CreateProcessW(
			nullptr,		// application_name
			command_line,	// command_line
			nullptr,		// process_attributes
			nullptr,		// thread_attributes
			TRUE,			// inherit_handles
			0,				// create_flags
			nullptr,		// enviroment
			dir.c_str(),	// current_directory
			&si,			// startup_info
			&pi);			// process_information
	}

	//
	// デストラクタです。
	//
	~SubProcess()
	{
		// サブプロセスのハンドルを閉じます。
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}

	//
	// サブプロセスが終了するまで待機します。
	//
	DWORD wait(DWORD time = INFINITE)
	{
		return ::WaitForSingleObject(pi.hProcess, time);
	}
};
