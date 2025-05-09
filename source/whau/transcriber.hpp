﻿#pragma once

//
// このクラスは文字起こしアプリを管理します。
//
inline struct Transcriber
{
	//
	// コンストラクタです。
	//
	Transcriber()
	{
		// 原点となるフォルダのパスです。
		hive.origin = get_module_file_name(nullptr).parent_path().lexically_normal();

		// ダウンロード元のURLです。
		hive.download_url =  L"https://github.com/Purfview/whisper-standalone-win/releases/download/Faster-Whisper-XXL/Faster-Whisper-XXL_r245.4_windows.7z";
	}

	//
	// 各種ファイルパスを更新します。
	//
	BOOL update_path()
	{
		// ダウンロード先のファイルパスです。
		hive.download_path = hive.download_url.filename();

		// Faster-Whisperがあるフォルダのパスです。
		auto faster_whisper_folder = hive.origin / hive.download_path.stem() / L"Faster-Whisper-XXL";

		// Faster-Whisperのファイルパスです。
		hive.whisper_path = faster_whisper_folder / L"faster-whisper-xxl.exe";

		// ffmpegのファイルパスです。
		hive.ffmpeg_path = faster_whisper_folder / L"ffmpeg.exe";

		return TRUE;
	}

	//
	// 文字起こしアプリ用のコマンドを作成して返します。
	//
	std::wstring get_command()
	{
		std::wstring command;

		command += std::format(LR"("{}" "{}")", hive.whisper_path.wstring(), hive.audio_file_path.wstring());

		if (is_arg_valid(hive.task)) command += std::format(L" --task {}", hive.task);
		if (is_arg_valid(hive.language)) command += std::format(L" -l {}", hive.language);
		if (is_arg_valid(hive.model)) command += std::format(L" -m {}", hive.model);
		if (is_arg_valid(hive.diarize)) command += std::format(L" --diarize {}", hive.diarize);
		if (is_arg_valid(hive.vad_method)) command += std::format(L" --vad_method {}", hive.vad_method);
		if (is_arg_valid(hive.ff)) command += std::format(L" --{}", hive.ff);
		if (is_arg_valid(hive.japanese_mode)) command += std::format(L" --japanese {}", hive.japanese_mode);
		if (hive.vad_speech_pad_ms.length()) command += std::format(L" --vad_speech_pad_ms {}", hive.vad_speech_pad_ms);

		command += L" " + hive.additional_command;
		command += std::format(LR"( --beep_off --print_progress --output_format all -o "{}")", hive.interim_folder_path.wstring());

		return command;
	}

	//
	// 文字起こしアプリが使用可能な場合はTRUEを返します。
	//
	BOOL is_available()
	{
		return std::filesystem::exists(hive.whisper_path);
	}

	//
	// 文字起こしアプリを導入します。
	//
	BOOL install()
	{
		// サブスレッドが既に存在する場合は
		if (hive.current_thread)
		{
			// ユーザーに通知します。
			AfxMessageBox(_T("サブスレッドを実行中です"));

			// 処理を続けることができないのでFALSEを返します。
			return FALSE;
		}

		// サブスレッドを作成します。
		auto install_thread = std::make_unique<InstallThread>();

		// サブスレッドを実行します。
		AfxBeginThread([](LPVOID param) -> UINT {
			return std::unique_ptr<InstallThread>((InstallThread*)param)->run();
		}, install_thread.release());

		return TRUE;
	}

	//
	// 文字起こしアプリを実行します。
	//
	BOOL execute()
	{
		// 文字起こしアプリが使用不可能な場合は
		if (!is_available())
		{
			// 文字起こしアプリを導入するかユーザーに問い合わせます。
			auto response = AfxMessageBox(
				_T("文字起こしアプリが存在しません") _T("\n")
				_T("導入しますか？"), MB_YESNO | MB_ICONASTERISK);

			// ユーザーが導入を拒否した場合はFALSEを返します。
			if (response != IDYES) return FALSE;

			// 文字起こしアプリを導入します。
			return install();
		}

		try
		{
			// サブスレッドが既に存在する場合は
			if (hive.current_thread)
			{
				// ユーザーに通知します。
				AfxMessageBox(_T("サブスレッドを実行中です"));

				// 処理を続けることができないのでFALSEを返します。
				return FALSE;
			}

			// 音声ファイルが存在しない場合は
			if (!std::filesystem::exists(hive.audio_file_path))
			{
				// ユーザーに通知します。
				AfxMessageBox(_T("音声ファイルが無効です"));

				// 処理を続けることができないのでFALSEを返します。
				return FALSE;
			}

			// 中間フォルダのパスを取得します。
			// 相対パスの可能性があるので絶対パスに変換します。
			auto interim_folder_path = std::filesystem::absolute(hive.interim_folder_path);

			// 文字起こしを実行したときに中間フォルダを選択する場合は
			if (hive.choose_folder_on_transcribe)
			{
				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = interim_folder_path.wstring();

				// フォルダ選択ダイアログを作成します。
				CFolderPickerDialog dialog(default_path.c_str());

				// ユーザーがフォルダを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたフォルダを中間フォルダのパスにします。
					interim_folder_path = to_string(dialog.GetPathName());
				}
				// ユーザーがフォルダを選択しなかった場合は
				else
				{
					// FALSEを返して処理を中断します。
					return FALSE;
				}
			}

			// 中間フォルダを作成します。
			std::filesystem::create_directories(interim_folder_path);

			// 文字起こしアプリ用のコマンドを作成します。
			auto command = get_command();

			{
				// コマンドファイルのパスを作成します。
				auto command_file_path = std::filesystem::absolute(hive.json_file_path);
				command_file_path.replace_extension(L".command.txt");

				// 出力ストリームを開きます。
				std::ofstream command_stream(command_file_path, std::ios::binary);

				// 文字起こしコマンドを出力ストリームに書き込みます。
				command_stream << wide_to_cp(command, CP_UTF8);
			}

			// サブスレッドを作成します。
			auto execute_thread = std::make_unique<ExecuteThread>(interim_folder_path, command);

			// サブスレッドを実行します。
			AfxBeginThread([](LPVOID param) -> UINT {
				return std::unique_ptr<ExecuteThread>((ExecuteThread*)param)->run();
			}, execute_thread.release());

			return TRUE;
		}
		catch (const std::exception& error)
		{
			AfxMessageBox(ts(error.what()).c_str());
		}
		catch (...)
		{
			AfxMessageBox(_T("例外が発生しました"));
		}

		return FALSE;
	}
} transcriber;
