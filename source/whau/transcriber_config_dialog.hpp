#pragma once

//
// このクラスは文字起こしのコンフィグダイアログです。
//
class TranscriberConfigDialog : public DialogBase
{
public:

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSCRIBER_CONFIG };
#endif

	//
	// Faster-Whisper用の設定です。
	//
	CEdit download_url;

	//
	// コンストラクタです。
	//
	TranscriberConfigDialog(CWnd* parent = nullptr)
		: DialogBase(IDD_TRANSCRIBER_CONFIG, parent)
	{
	}

	//
	// UIの値を使用してコンフィグを更新します。
	//
	virtual BOOL on_ui_to_config() override
	{
		from_ui(hive.download_url, download_url);

		return TRUE;
	}

	//
	// コンフィグの値を使用してUIを更新します。
	//
	virtual BOOL on_config_to_ui() override
	{
		to_ui(hive.download_url, download_url);

		// ダウンロードURLが変更された可能性があるので
		// 各種ファイルパスを更新します。
		transcriber.update_path();

		return TRUE;
	}

	//
	// コントロールからのコマンドを処理します。
	//
	virtual BOOL on_command(UINT code, UINT control_id, HWND control) override
	{
		switch (control_id)
		{
		// ボタンのコマンドです。
		case IDOK:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				break;
			}
		}

		return FALSE;
	}

	//
	// 変数とコントロールをバインドします。
	//
	virtual void DoDataExchange(CDataExchange* dx) override
	{
		__super::DoDataExchange(dx);

		DDX_Control(dx, IDC_DOWNLOAD_URL, download_url);
	}

	//
	// ダイアログの初期化処理を実行します。
	//
	virtual BOOL OnInitDialog() override
	{
		__super::OnInitDialog();

		// コンフィグの値でUIを更新します。
		config_to_ui();

		// ダイアログを前面に表示します。
		SetForegroundWindow();
		SetActiveWindow();

		return TRUE;
	}
};
