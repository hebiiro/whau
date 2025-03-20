#pragma once

//
// このクラスは文字起こしを実行します。
//
struct InstallThread : Hive::SubThread
{
	//
	// コンソールを使用して文字起こしを実行します。
	//
	UINT run()
	{
		// ヘルプのファイルパスです。
		auto usage_path = std::filesystem::path(L"usage-faster-whisper-xxl.txt");

		// コンソールを開きます。
		Console console;

		// 導入の開始をユーザーに通知します。
		console.write(L"文字起こしアプリの導入を開始します" L"\n");

//		if (0)
		{
			// ダウンロードコマンドを実行します。完了するまで待機します。
			SubProcess(hive.origin, std::format(
				L"PowerShell -NoProfile -ExecutionPolicy Bypass -Command "
				LR"("Start-BitsTransfer -Source {} -Destination {}")" L"\n",
				hive.download_url.wstring(), hive.download_path.wstring())).wait();
		}

//		if (0)
		{
			// 解凍コマンドを実行します。完了するまで待機します。
			SubProcess(hive.origin, std::format(LR"(7za x {} -aoa -o{}")" L"\n",
				hive.download_path.wstring(), hive.download_path.stem().wstring())).wait();
		}

//		if (0)
		{
			// ヘルプを作成します。完了するまで待機します。
			SubProcess(hive.origin, std::format(LR"(cmd /c "{}" --help > {})" L"\n",
				hive.whisper_path.wstring(), usage_path.wstring())).wait();

			// ヘルプを表示します。
			shell_execute(L"open", usage_path.c_str(), nullptr);
		}

		// 導入の完了をユーザーに通知します。
		console.write(L"文字起こしアプリの導入が完了しました" L"\n");

		return 0;
	}
};
