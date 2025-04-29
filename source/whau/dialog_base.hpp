#pragma once

//
// このクラスはダイアログの共通ベースクラスです。
//
class DialogBase : public CDialogEx
{
public:

	//
	// UIの値を使用してコンフィグを更新するタイミングで呼び出されます。
	//
	virtual BOOL on_ui_to_config() = 0;

	//
	// コンフィグの値を使用してUIを更新するタイミングで呼び出されます。
	//
	virtual BOOL on_config_to_ui() = 0;

	//
	// コントロールからのコマンドを処理するタイミングで呼び出されます。
	//
	virtual BOOL on_command(UINT code, UINT control_id, HWND control) = 0;

	//
	// TRUEの場合はWM_COMMANDを処理しません。
	//
	BOOL is_command_locked = FALSE;

	//
	// WM_COMMANDをロックします。
	//
	struct CommandLocker
	{
		DialogBase* main_dialog;

		CommandLocker(DialogBase* main_dialog)
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
	// コンストラクタです。
	//
	DialogBase(UINT dialog_id, CWnd* parent)
		: CDialogEx(dialog_id, parent)
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

		return on_ui_to_config();
	}

	//
	// コンフィグの値を使用してUIを更新します。
	//
	BOOL config_to_ui()
	{
		CommandLocker command_locker(this);

		return on_config_to_ui();
	}

	//
	// コントロールからのコマンドを処理します。
	//
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override
	{
		// WM_COMMANDがロックされていない場合は
		if (!is_command_locked)
		{
			// サブクラスに処理させます。
			if (on_command(HIWORD(wParam), LOWORD(wParam), (HWND)lParam))
				return TRUE;
		}

		return __super::OnCommand(wParam, lParam);
	}
};
