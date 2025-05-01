#pragma once

//
// このクラスはアプリケーションです。
//
inline class App : public CWinApp, AppInterface
{
public:

	//
	// コンストラクタです。
	//
	App()
	{
		app = this;
	}

	//
	// コンフィグをファイルから読み込みます。
	//
	virtual BOOL read_config() override
	{
		// デフォルトのコンフィグ値をセットします。
		hive.interim_folder_path = get_default_interim_folder_path();
		hive.json_file_path = get_default_json_file_path();
		hive.wav_folder_path = get_default_wav_folder_path();

		// コンフィグを読み込みます。
		config_io.read_config();

		// ダウンロードURLが変更された可能性があるので
		// 各種ファイルパスを更新します。
		transcriber.update_path();

		return TRUE;
	}

	//
	// コンフィグをファイルに書き込みます。
	//
	virtual BOOL write_config() override
	{
		// コンフィグを書き込みます。
		config_io.write_config();

		return TRUE;
	}

	//
	// デフォルトの中間フォルダのパスを返します。
	//
	virtual std::filesystem::path get_default_interim_folder_path() override
	{
		auto interim_folder_path = hive.audio_file_path;
		interim_folder_path.replace_extension(L"");
		interim_folder_path /= L"whau";
		return interim_folder_path;
	}

	//
	// デフォルトのjsonファイルのパスを返します。
	//
	virtual std::filesystem::path get_default_json_file_path() override
	{
		auto json_file_path = hive.interim_folder_path;
		json_file_path /= hive.audio_file_path.filename();
		json_file_path.replace_extension(L".json");
		return json_file_path;
	}

	//
	// デフォルトのexoファイルのパスを返します。
	//
	virtual std::filesystem::path get_default_exo_file_path() override
	{
		auto exo_file_path = hive.json_file_path;
		exo_file_path.replace_extension(L".exo");
		return exo_file_path;
	}

	//
	// デフォルトのwavフォルダのパスを返します。
	//
	virtual std::filesystem::path get_default_wav_folder_path() override
	{
		auto wav_folder_path = hive.audio_file_path;
		wav_folder_path.replace_extension(L"");
		return wav_folder_path;
	}

	//
	// 実際のコマンドを返します。
	//
	virtual std::filesystem::path get_actual_command() override
	{
		return transcriber.get_command();
	}

	//
	// 文字起こしを実行します。
	//
	virtual BOOL transcribe() override
	{
		// 文字起こしを実行します。
		return transcriber.execute();
	}

	//
	// exoファイルを出力します。
	//
	virtual BOOL output_exo_file() override
	{
		ExoCreator exo_creator;

		return exo_creator(get_default_exo_file_path());
	}

	//
	// インスタンスの初期化処理を実行します。
	//
	virtual BOOL InitInstance() override
	{
		{
			INITCOMMONCONTROLSEX icc = { sizeof(icc) };
			icc.dwICC = ICC_WIN95_CLASSES;
			::InitCommonControlsEx(&icc);
		}

		CWinApp::InitInstance();

		AfxEnableControlContainer();

		{
			// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
			// 含まれている場合にシェル マネージャーを作成します。
			auto shell_manager = std::make_unique<CShellManager>();

			// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
			CMFCVisualManager::SetDefaultManager(
				RUNTIME_CLASS(CMFCVisualManagerWindows));

			// メインダイアログをMFCのメインウィンドウに設定します。
			m_pMainWnd = &main_dialog;

			// メインダイアログをモーダル表示します。
			switch (main_dialog.DoModal())
			{
			case IDOK:
			case IDCANCEL:
				{
					break;
				}
			default:
				{
					TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
					TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");

					break;
				}
			}
		}
#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
		ControlBarCleanUp();
#endif
		return FALSE;
	}

	DECLARE_MESSAGE_MAP()
} theApp;

BEGIN_MESSAGE_MAP(App, CWinApp)
END_MESSAGE_MAP()
