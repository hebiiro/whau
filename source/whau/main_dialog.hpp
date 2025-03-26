#pragma once

//
// このクラスはメインダイアログです。
//
inline class MainDialog : public CDialogEx
{
public:

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

	//
	// TRUEの場合はWM_COMMANDを処理しません。
	//
	BOOL is_command_locked = FALSE;

	//
	// WM_COMMANDをロックします。
	//
	struct CommandLocker
	{
		MainDialog* main_dialog;

		CommandLocker(MainDialog* main_dialog)
			: main_dialog(main_dialog)
		{
			main_dialog->is_command_locked = TRUE;
		}

		~CommandLocker()
		{
			main_dialog->is_command_locked = FALSE;
		}
	};

	//
	// Faster-Whisper用の設定です。
	//
	CMFCEditBrowseCtrl audio_file_path;
	CMFCEditBrowseCtrl interim_folder_path;
	CComboBox task;
	CComboBox language;
	CComboBox japanese_mode;
	CComboBox model;
	CComboBox diarize;
	CComboBox ff;
	CComboBox vad_method;
	CEdit vad_speech_pad_ms;
	CEdit additional_command;
	CEdit actual_command;

	//
	// exoファイルの設定です。
	//
	struct {
		CEdit video_w;
		CEdit video_h;
		CEdit video_rate;
		CEdit video_scale;
		CEdit audio_rate;
		CEdit audio_ch;
	} exo;

	//
	// AviUtl用の設定です。
	//
	CMFCEditBrowseCtrl json_file_path;
	CButton create_token_item;
	CButton create_segment_item;
	CButton create_psdtoolkit_item;
	CEdit token_layer_offset;
	CEdit segment_layer_offset;
	CEdit start_margin;
	CEdit end_margin;

	//
	// PSDToolKit用の設定です。
	//
	CMFCEditBrowseCtrl wav_folder_path;
	CButton use_lip_sync;
	CButton use_subtitle;
	CComboBox slider_count;
	CButton all_in_one;

	//
	// コンソールのプレースホルダーです。
	//
	CStatic console_placeholder;

	//
	// コンストラクタです。
	//
	MainDialog(CWnd* parent = nullptr)
		: CDialogEx(IDD_MAIN_DIALOG, parent)
	{
	}

	void from_ui(std::wstring& value, CEdit& ui)
	{
		CString s; ui.GetWindowText(s);
		value = to_string(s);
	}

	void from_ui(std::filesystem::path& value, CEdit& ui)
	{
		CString s; ui.GetWindowText(s);
		value = to_string(s);
	}

	void from_ui(int& value, CEdit& ui)
	{
		CString s; ui.GetWindowText(s);
		value = _ttoi(s);
	}

	void from_ui(BOOL& value, CButton& ui)
	{
		value = ui.GetCheck() == BST_CHECKED;
	}

	void from_ui(int& value, CComboBox& ui)
	{
		value = max(0, ui.GetCurSel());
	}

	void from_ui(std::wstring& value, CComboBox& ui)
	{
		auto index = max(0, ui.GetCurSel());
		CString s; ui.GetLBText(index, s);
		value = to_string(s);
	}

	void to_ui(const std::wstring& value, CEdit& ui)
	{
		ui.SetWindowText(ts(value).c_str());
	}

	void to_ui(const std::filesystem::path& value, CEdit& ui)
	{
		ui.SetWindowText(ts(value.wstring()).c_str());
	}

	void to_ui(int value, CEdit& ui)
	{
		ui.SetWindowText(ts(std::format(L"{}", value)).c_str());
	}

	void to_ui(const BOOL& value, CButton& ui)
	{
		ui.SetCheck(value ? BST_CHECKED : BST_UNCHECKED);
	}

	void to_ui(const int& value, CComboBox& ui)
	{
		ui.SetCurSel(value);
	}

	void to_ui(const std::wstring& value, CComboBox& ui)
	{
		int c = ui.GetCount();
		for (int i = 0; i < c; i++)
		{
			CString s; ui.GetLBText(i, s);
			if (value == to_string(s))
			{
				ui.SetCurSel(i);

				break;
			}
		}
	}

	//
	// UIの値を使用してコンフィグを更新します。
	//
	BOOL ui_to_config()
	{
		CommandLocker command_locker(this);

		from_ui(hive.audio_file_path, audio_file_path);
		from_ui(hive.interim_folder_path, interim_folder_path);
		from_ui(hive.task, task);
		from_ui(hive.language, language);
		from_ui(hive.japanese_mode, japanese_mode);
		from_ui(hive.model, model);
		from_ui(hive.diarize, diarize);
		from_ui(hive.ff, ff);
		from_ui(hive.vad_method, vad_method);
		from_ui(hive.vad_speech_pad_ms, vad_speech_pad_ms);

		from_ui(hive.additional_command, additional_command);
		from_ui(hive.actual_command, actual_command);

		from_ui(hive.exo.video_w, exo.video_w);
		from_ui(hive.exo.video_h, exo.video_h);
		from_ui(hive.exo.video_rate, exo.video_rate);
		from_ui(hive.exo.video_scale, exo.video_scale);
		from_ui(hive.exo.audio_rate, exo.audio_rate);
		from_ui(hive.exo.audio_ch, exo.audio_ch);

		from_ui(hive.json_file_path, json_file_path);
		from_ui(hive.create_token_item, create_token_item);
		from_ui(hive.create_segment_item, create_segment_item);
		from_ui(hive.create_psdtoolkit_item, create_psdtoolkit_item);
		from_ui(hive.token_layer_offset, token_layer_offset);
		from_ui(hive.segment_layer_offset, segment_layer_offset);
		from_ui(hive.start_margin, start_margin);
		from_ui(hive.end_margin, end_margin);

		from_ui(hive.wav_folder_path, wav_folder_path);
		from_ui(hive.use_lip_sync, use_lip_sync);
		from_ui(hive.use_subtitle, use_subtitle);
		from_ui(hive.slider_count, slider_count);
		from_ui(hive.all_in_one, all_in_one);

		return TRUE;
	}

	//
	// コンフィグの値を使用してUIを更新します。
	//
	BOOL config_to_ui()
	{
		CommandLocker command_locker(this);

		to_ui(hive.audio_file_path, audio_file_path);
		to_ui(hive.interim_folder_path, interim_folder_path);
		to_ui(hive.task, task);
		to_ui(hive.language, language);
		to_ui(hive.japanese_mode, japanese_mode);
		to_ui(hive.model, model);
		to_ui(hive.diarize, diarize);
		to_ui(hive.ff, ff);
		to_ui(hive.vad_method, vad_method);
		to_ui(hive.vad_speech_pad_ms, vad_speech_pad_ms);

		to_ui(hive.additional_command, additional_command);
		to_ui(hive.actual_command, actual_command);

		to_ui(hive.exo.video_w, exo.video_w);
		to_ui(hive.exo.video_h, exo.video_h);
		to_ui(hive.exo.video_rate, exo.video_rate);
		to_ui(hive.exo.video_scale, exo.video_scale);
		to_ui(hive.exo.audio_rate, exo.audio_rate);
		to_ui(hive.exo.audio_ch, exo.audio_ch);

		to_ui(hive.json_file_path, json_file_path);
		to_ui(hive.create_token_item, create_token_item);
		to_ui(hive.create_segment_item, create_segment_item);
		to_ui(hive.create_psdtoolkit_item, create_psdtoolkit_item);
		to_ui(hive.token_layer_offset, token_layer_offset);
		to_ui(hive.segment_layer_offset, segment_layer_offset);
		to_ui(hive.start_margin, start_margin);
		to_ui(hive.end_margin, end_margin);

		to_ui(hive.wav_folder_path, wav_folder_path);
		to_ui(hive.use_lip_sync, use_lip_sync);
		to_ui(hive.use_subtitle, use_subtitle);
		to_ui(hive.slider_count, slider_count);
		to_ui(hive.all_in_one, all_in_one);

		return TRUE;
	}

	//
	// コンソールウィンドウを返します。
	//
	CWnd* get_console_window()
	{
		return CWnd::FromHandle(::GetConsoleWindow());
	}

	//
	// コンソールを初期化します。
	//
	BOOL init_console()
	{
		// コンソールを開きます。
		::AllocConsole();

		// コンソールウィンドウを取得します。
		auto console_window = get_console_window();
		if (!console_window) return FALSE;

		// コンソールウィンドウのウィンドウスタイルを変更します。
		console_window->ModifyStyle(WS_OVERLAPPEDWINDOW, WS_CHILD);

		// コンソールウィンドウをこのダイアログの子ウィンドウにします。
		console_window->SetParent(this);

		return TRUE;
	}

	//
	// ダイアログのレイアウトを更新します。
	//
	void update_layout()
	{
		// コンソールウィンドウを取得できる場合は
		if (auto console_window = get_console_window())
		{
			// コンソールウィンドウをプレースホルダーの位置に移動します。
			CRect rc; console_placeholder.GetWindowRect(&rc);
			ScreenToClient(&rc);
			console_window->SetWindowPos({},
				rc.left, rc.top, rc.Width(), rc.Height(),
				SWP_NOZORDER | SWP_NOACTIVATE);

			// コンソールウィンドウを再描画します。
			console_window->RedrawWindow({}, {},
				RDW_ERASE | RDW_INVALIDATE | RDW_FRAME |
				RDW_INTERNALPAINT | RDW_ALLCHILDREN);
		}
	}

	//
	// 変数とコントロールをバインドします。
	//
	virtual void DoDataExchange(CDataExchange* dx) override
	{
		CDialogEx::DoDataExchange(dx);

		DDX_Control(dx, IDC_AUDIO_FILE_PATH, audio_file_path);
		DDX_Control(dx, IDC_INTERIM_FOLDER_PATH, interim_folder_path);
		DDX_Control(dx, IDC_TASK, task);
		DDX_Control(dx, IDC_LANGUAGE, language);
		DDX_Control(dx, IDC_MODEL, model);
		DDX_Control(dx, IDC_DIARIZE, diarize);
		DDX_Control(dx, IDC_VAD_METHOD, vad_method);
		DDX_Control(dx, IDC_FF, ff);
		DDX_Control(dx, IDC_JAPANESE_MODE, japanese_mode);
		DDX_Control(dx, IDC_VAD_SPEECH_PAD_MS, vad_speech_pad_ms);

		DDX_Control(dx, IDC_ADDITIONAL_COMMAND, additional_command);
		DDX_Control(dx, IDC_ACTUAL_COMMAND, actual_command);

		DDX_Control(dx, IDC_EXO_VIDEO_W, exo.video_w);
		DDX_Control(dx, IDC_EXO_VIDEO_H, exo.video_h);
		DDX_Control(dx, IDC_EXO_VIDEO_RATE, exo.video_rate);
		DDX_Control(dx, IDC_EXO_VIDEO_SCALE, exo.video_scale);
		DDX_Control(dx, IDC_EXO_AUDIO_RATE, exo.audio_rate);
		DDX_Control(dx, IDC_EXO_AUDIO_CHANNEL, exo.audio_ch);

		DDX_Control(dx, IDC_JSON_FILE_PATH, json_file_path);
		DDX_Control(dx, IDC_CREATE_TOKEN_ITEM, create_token_item);
		DDX_Control(dx, IDC_CREATE_SEGMENT_ITEM, create_segment_item);
		DDX_Control(dx, IDC_CREATE_PSDTOOLKIT_ITEM, create_psdtoolkit_item);
		DDX_Control(dx, IDC_TOKEN_LAYER_OFFSET, token_layer_offset);
		DDX_Control(dx, IDC_SEGMENT_LAYER_OFFSET, segment_layer_offset);
		DDX_Control(dx, IDC_START_MARGIN, start_margin);
		DDX_Control(dx, IDC_END_MARGIN, end_margin);

		DDX_Control(dx, IDC_WAV_FOLDER_PATH, wav_folder_path);
		DDX_Control(dx, IDC_USE_LIP_SYNC, use_lip_sync);
		DDX_Control(dx, IDC_USE_SUBTITLE, use_subtitle);
		DDX_Control(dx, IDC_SLIDER_COUNT, slider_count);
		DDX_Control(dx, IDC_ALL_IN_ONE, all_in_one);

		DDX_Control(dx, IDC_CONSOLE_PLACEHOLDER, console_placeholder);
	}

	//
	// ダイアログの初期化処理を実行します。
	//
	virtual BOOL OnInitDialog() override
	{
		CDialogEx::OnInitDialog();

		// コンソールを初期化します。
		init_console();

		// コンフィグを設定ファイルから読み込みます。
		app->read_config();

		// コンフィグの値でUIを更新します。
		config_to_ui();

		// 実際のコマンドのUIを更新します。
		to_ui(app->get_actual_command(), actual_command);

		// レイアウトを更新します。
		update_layout();

		// ダイアログを前面に表示します。
		SetForegroundWindow();
		SetActiveWindow();

		return TRUE;
	}

	//
	// コントロールからのコマンドを処理します。
	//
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override
	{
		// WM_COMMANDがロックされている場合は
		if (is_command_locked)
		{
			// デフォルト処理だけを実行します。
			return CDialogEx::OnCommand(wParam, lParam);
		}

		auto code = HIWORD(wParam);
		auto control_id = LOWORD(wParam);
		auto control = (HWND)lParam;

		switch (control_id)
		{
		// コンボボックスのコマンドです。
		case IDC_TASK:
		case IDC_LANGUAGE:
		case IDC_MODEL:
		case IDC_DIARIZE:
		case IDC_VAD_METHOD:
		case IDC_FF:
		case IDC_JAPANESE_MODE:
			{
				if (code == CBN_SELCHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();

					// 文字起こしコマンドをUIに適用します。
					to_ui(app->get_actual_command(), actual_command);
				}

				break;
			}
		// エディットボックスのコマンドです。
		case IDC_VAD_SPEECH_PAD_MS:
		case IDC_ADDITIONAL_COMMAND:
			{
				if (code == EN_CHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();

					// 文字起こしコマンドをUIに適用します。
					to_ui(app->get_actual_command(), actual_command);
				}

				break;
			}
		// エディットボックスのコマンドです。
		case IDC_AUDIO_FILE_PATH:
			{
				if (code == EN_CHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();

					// デフォルトの中間フォルダのパスをUIに適用します。
					to_ui(app->get_default_interim_folder_path(), interim_folder_path);

					// デフォルトのjsonファイルのパスをUIに適用します。
					to_ui(app->get_default_json_file_path(), json_file_path);

					// デフォルトのwavフォルダのパスをUIに適用します。
					to_ui(app->get_default_wav_folder_path(), wav_folder_path);

					// 文字起こしコマンドをUIに適用します。
					to_ui(app->get_actual_command(), actual_command);
				}

				break;
			}
		// エディットボックスのコマンドです。
		case IDC_INTERIM_FOLDER_PATH:
			{
				if (code == EN_CHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();

					// デフォルトのjsonファイルのパスをUIに適用します。
					to_ui(app->get_default_json_file_path(), json_file_path);

					// 文字起こしコマンドをUIに適用します。
					to_ui(app->get_actual_command(), actual_command);
				}

				break;
			}
#if 0
		// エディットボックスのコマンドです。
		case IDC_EXO_VIDEO_W:
		case IDC_EXO_VIDEO_H:
		case IDC_EXO_VIDEO_RATE:
		case IDC_EXO_VIDEO_SCALE:
		case IDC_EXO_AUDIO_RATE:
		case IDC_EXO_AUDIO_CHANNEL:
		case IDC_WAV_FOLDER_PATH:
		case IDC_TOKEN_LAYER_OFFSET:
		case IDC_SEGMENT_LAYER_OFFSET:
		case IDC_START_MARGIN:
		case IDC_END_MARGIN:
			{
				if (code == EN_CHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();
				}

				break;
			}
		// コンボボックスのコマンドです。
		case IDC_SLIDER_COUNT:
			{
				if (code == CBN_SELCHANGE)
				{
					// UIの値でコンフィグを更新します。
					ui_to_config();
				}

				break;
			}
		// ボタンのコマンドです。
		case IDC_CREATE_SEGMENT_ITEM:
		case IDC_CREATE_TOKEN_ITEM:
		case IDC_CREATE_PSDTOOLKIT_ITEM:
		case IDC_USE_LIP_SYNC:
		case IDC_USE_SUBTITLE:
		case IDC_ALL_IN_ONE:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				break;
			}
#endif
		// ボタンのコマンドです。
		case IDC_AUDIO_FILE_PATH_REF:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = std::filesystem::absolute(hive.audio_file_path).wstring();

				// ファイル選択ダイアログを作成します。
				CFileDialog dialog(
					TRUE, // 読み込みの場合はTRUEです。
					nullptr, // デフォルト拡張子です。
					default_path.c_str(), // 初期ファイル名です。
					OFN_FILEMUSTEXIST, // 既存のファイルだけ選択できるようにします。
					_T("audio files (wav, mp3, flac, aac, ogg, oga, m4a, mp4, avi, wma, wmv|")
					_T("*.wav;*.mp3;*.flac;*.aac;*.ogg;*.oga;*.m4a;*.mp4;*.avi;*.wma;*.wmv|") // 音声ファイル用のフィルタです。
					_T("all files (*.*)|*.*||"), // すべてのファイル用のフィルタです。
					this); // オーナーウィンドウです。

				// ユーザーがファイルを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたファイルを音声ファイルのパスにします。
					hive.audio_file_path = to_string(dialog.GetPathName());

					// 音声ファイルのパスをUIに適用します。
					to_ui(hive.audio_file_path, audio_file_path);
				}

				break;
			}
		// ボタンのコマンドです。
		case IDC_INTERIM_FOLDER_PATH_REF:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = std::filesystem::absolute(hive.interim_folder_path).wstring();

				// フォルダ選択ダイアログを作成します。
				CFolderPickerDialog dialog(default_path.c_str(), 0, this);

				// ユーザーがフォルダを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたフォルダを中間フォルダのパスにします。
					hive.interim_folder_path = to_string(dialog.GetPathName());

					// 中間フォルダのパスをUIに適用します。
					to_ui(hive.interim_folder_path, interim_folder_path);
				}

				break;
			}
		// ボタンのコマンドです。
		case IDC_JSON_FILE_PATH_REF:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = std::filesystem::absolute(hive.json_file_path).wstring();

				// ファイル選択ダイアログを作成します。
				CFileDialog dialog(
					TRUE, // 読み込みの場合はTRUEです。
					nullptr, // デフォルト拡張子です。
					default_path.c_str(), // 初期ファイル名です。
					OFN_FILEMUSTEXIST, // 既存のファイルだけ選択できるようにします。
					_T("json files (*.json|*.json|") // jsonファイル用のフィルタです。
					_T("all files (*.*)|*.*||"), // すべてのファイル用のフィルタです。
					this); // オーナーウィンドウです。

				// ユーザーがファイルを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたファイルをjsonファイルのパスにします。
					hive.json_file_path = to_string(dialog.GetPathName());

					// jsonファイルのパスをUIに適用します。
					to_ui(hive.json_file_path, json_file_path);
				}

				break;
			}
		// ボタンのコマンドです。
		case IDC_WAV_FOLDER_PATH_REF:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = std::filesystem::absolute(hive.wav_folder_path).wstring();

				// フォルダ選択ダイアログを作成します。
				CFolderPickerDialog dialog(default_path.c_str(), 0, this);

				// ユーザーがフォルダを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたフォルダをwavフォルダのパスにします。
					hive.wav_folder_path = to_string(dialog.GetPathName());

					// wavフォルダのパスをUIに適用します。
					to_ui(hive.wav_folder_path, wav_folder_path);
				}

				break;
			}
		// ボタンのコマンドです。
		case IDC_TRANSCRIBE:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// 文字起こしを実行します。
				return app->transcribe();
			}
		// ボタンのコマンドです。
		case IDC_OUTPUT_EXO_FILE:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// AviUtl用のexoファイルを出力します。
				return app->output_exo_file();
			}
		// ボタンのコマンドです。
		case IDOK:
			{
				return TRUE;
			}
		// ボタンのコマンドです。
		case IDCANCEL:
			{
				// UIの値でコンフィグを更新します。
				ui_to_config();

				// コンフィグを設定ファイルに書き込みます。
				app->write_config();

				break;
			}
		}

		return CDialogEx::OnCommand(wParam, lParam);
	}

	//
	// WM_SIZEを処理します。
	//
	LRESULT on_size(UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto lr = __super::WindowProc(message, wParam, lParam);

		update_layout();

		return lr;
	}

	//
	// WM_CONTEXTMENUを処理します。
	//
	LRESULT on_context_menu(UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto control = CWnd::FromHandle((HWND)wParam);
		auto point = CPoint(lParam);

		switch (control->GetDlgCtrlID())
		{
		case IDC_TRANSCRIBE:
			{
				constexpr struct {
					const int c_choose_folder_on_transcribe = 1;
				} c_command_id;

				CMenu menu; menu.CreatePopupMenu();
				menu.AppendMenu(MF_STRING, c_command_id.c_choose_folder_on_transcribe, _T("実行時にフォルダを選択"));
				if (hive.choose_folder_on_transcribe)
					menu.CheckMenuItem(c_command_id.c_choose_folder_on_transcribe, MF_CHECKED);

				auto id = menu.TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);

				switch (id)
				{
				case c_command_id.c_choose_folder_on_transcribe:
					{
						hive.choose_folder_on_transcribe = !hive.choose_folder_on_transcribe;

						break;
					}
				}

				break;
			}
		case IDC_OUTPUT_EXO_FILE:
			{
				constexpr struct {
					const int c_choose_file_on_output_exo_file = 1;
				} c_command_id;

				CMenu menu; menu.CreatePopupMenu();
				menu.AppendMenu(MF_STRING, c_command_id.c_choose_file_on_output_exo_file, _T("出力時にファイルを選択"));
				if (hive.choose_file_on_output_exo_file)
					menu.CheckMenuItem(c_command_id.c_choose_file_on_output_exo_file, MF_CHECKED);

				auto id = menu.TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);

				switch (id)
				{
				case c_command_id.c_choose_file_on_output_exo_file:
					{
						hive.choose_file_on_output_exo_file = !hive.choose_file_on_output_exo_file;

						break;
					}
				}

				break;
			}
		}

		return __super::WindowProc(message, wParam, lParam);
	}

	//
	// WM_DROPFILESを処理します。
	//
	LRESULT on_drop_files(UINT message, WPARAM wParam, LPARAM lParam)
	{
		// ドラッグアンドドロップで渡されたドロップファイルのパスを取得します。
		auto drop = (HDROP)wParam;
		TCHAR drop_file_path[MAX_PATH] = {};
		::DragQueryFile(drop, 0, drop_file_path, std::size(drop_file_path));
		::DragFinish(drop);

		// ドロップファイルのパスを音声ファイルのパスに設定します。
		audio_file_path.SetWindowText(drop_file_path);

		return __super::WindowProc(message, wParam, lParam);
	}

	//
	// ウィンドウメッセージを処理します。
	//
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override
	{
		switch (message)
		{
		case WM_SIZE: return on_size(message, wParam, lParam);
		case WM_CONTEXTMENU: return on_context_menu(message, wParam, lParam);
		case WM_DROPFILES: return on_drop_files(message, wParam, lParam);
		}

		return __super::WindowProc(message, wParam, lParam);
	}
} main_dialog;
