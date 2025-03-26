#pragma once

//
// このクラスは文字起こしを実行します。
//
struct ExecuteThread : Hive::SubThread
{
	std::filesystem::path output_folder_path;
	std::wstring command;

	//
	// コンストラクタです。
	//
	ExecuteThread(const std::filesystem::path& output_folder_path, const std::wstring& command)
		: output_folder_path(output_folder_path)
		, command(command)
	{
	}

	//
	// コンソールを使用して文字起こしを実行します。
	//
	UINT run()
	{
		// コンソールを開きます。
		Console console;

		// 文字起こしの開始をユーザーに通知します。
		console.write(L"文字起こしを開始します" L"\n");

		// 文字起こしコマンドを実行します。完了するまで待機します。
		SubProcess(hive.origin, command).wait();

		// 出力フォルダを開きます。
//		shell_execute(L"open", output_folder_path.c_str(), nullptr);

		// 文字起こしの完了をユーザーに通知します。
		console.write(L"文字起こしが完了しました" L"\n");

		return 0;
	}
};
