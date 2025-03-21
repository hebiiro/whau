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
		auto path = get_module_file_name(nullptr).replace_extension(L".ini").wstring();

		const auto read_string = [&](LPCWSTR section_name, LPCWSTR key_name, std::wstring& value) {
			value.resize(MAX_PATH);
			::GetPrivateProfileStringW(section_name, key_name,
				value.c_str(), value.data(), (DWORD)value.size(), path.c_str());
			value.resize(::lstrlenW(value.data()));
		};

		const auto read_int = [&](LPCWSTR section_name, LPCWSTR key_name, int& value) {
			value = ::GetPrivateProfileIntW(section_name, key_name, value, path.c_str());
		};

		const auto read_path = [&](LPCWSTR section_name, LPCWSTR key_name, std::filesystem::path& value) {
			std::wstring s = value.wstring();
			read_string(section_name, key_name, s);
			value = s;
		};

		hive.json_file_path = get_json_file_path();
		hive.wav_folder_path = get_wav_folder_path();

		read_path(L"Faster-Whisper", L"audio_file_path", hive.audio_file_path);
		read_string(L"Faster-Whisper", L"task", hive.task);
		read_string(L"Faster-Whisper", L"language", hive.language);
		read_string(L"Faster-Whisper", L"model", hive.model);
		read_string(L"Faster-Whisper", L"diarize", hive.diarize);
		read_string(L"Faster-Whisper", L"vad_method", hive.vad_method);
		read_string(L"Faster-Whisper", L"ff", hive.ff);
		read_string(L"Faster-Whisper", L"vad_speech_pad_ms", hive.vad_speech_pad_ms);

		read_string(L"Faster-Whisper", L"japanese_mode", hive.japanese_mode);
		read_string(L"Faster-Whisper", L"output_sub_folder_name", hive.output_sub_folder_name);
		read_string(L"Faster-Whisper", L"additional_command", hive.additional_command);

		read_int(L"exo", L"video_w", hive.exo.video_w);
		read_int(L"exo", L"video_h", hive.exo.video_h);
		read_int(L"exo", L"video_rate", hive.exo.video_rate);
		read_int(L"exo", L"video_scale", hive.exo.video_scale);
		read_int(L"exo", L"audio_rate", hive.exo.audio_rate);
		read_int(L"exo", L"audio_ch", hive.exo.audio_ch);

		read_path(L"AviUtl", L"json_file_path", hive.json_file_path);
		read_int(L"AviUtl", L"create_token_item", hive.create_token_item);
		read_int(L"AviUtl", L"create_segment_item", hive.create_segment_item);
		read_int(L"AviUtl", L"create_psdtoolkit_item", hive.create_psdtoolkit_item);
		read_int(L"AviUtl", L"token_layer_offset", hive.token_layer_offset);
		read_int(L"AviUtl", L"segment_layer_offset", hive.segment_layer_offset);
		read_string(L"AviUtl", L"start_margin", hive.start_margin);
		read_string(L"AviUtl", L"end_margin", hive.end_margin);

		read_path(L"PSDToolKit", L"wav_folder_path", hive.wav_folder_path);
		read_int(L"PSDToolKit", L"use_lip_sync", hive.use_lip_sync);
		read_int(L"PSDToolKit", L"use_slider", hive.use_slider);
		read_int(L"PSDToolKit", L"slider_count", hive.slider_count);
		read_int(L"PSDToolKit", L"use_subtitle", hive.use_subtitle);
		read_int(L"PSDToolKit", L"all_in_one", hive.all_in_one);

		read_int(L"etc", L"choose_file_on_output", hive.choose_file_on_output);

		return TRUE;
	}

	//
	// コンフィグをファイルに書き込みます。
	//
	virtual BOOL write_config() override
	{
		auto path = get_module_file_name(nullptr).replace_extension(L".ini").wstring();

		const auto write_string = [&](LPCWSTR section_name, LPCWSTR key_name, const std::wstring& value) {
			::WritePrivateProfileStringW(section_name, key_name, value.c_str(), path.c_str());
		};

		const auto write_int = [&](LPCWSTR section_name, LPCWSTR key_name, int value) {
			write_string(section_name, key_name, std::format(L"{}", value));
		};

		const auto write_path = [&](LPCWSTR section_name, LPCWSTR key_name, const std::filesystem::path& value) {
			write_string(section_name, key_name, value.wstring());
		};

		write_path(L"Faster-Whisper", L"audio_file_path", hive.audio_file_path);
		write_string(L"Faster-Whisper", L"task", hive.task);
		write_string(L"Faster-Whisper", L"language", hive.language);
		write_string(L"Faster-Whisper", L"model", hive.model);
		write_string(L"Faster-Whisper", L"diarize", hive.diarize);
		write_string(L"Faster-Whisper", L"vad_method", hive.vad_method);
		write_string(L"Faster-Whisper", L"ff", hive.ff);
		write_string(L"Faster-Whisper", L"vad_speech_pad_ms", hive.vad_speech_pad_ms);

		write_string(L"Faster-Whisper", L"japanese_mode", hive.japanese_mode);
		write_string(L"Faster-Whisper", L"output_sub_folder_name", hive.output_sub_folder_name);
		write_string(L"Faster-Whisper", L"additional_command", hive.additional_command);

		write_int(L"exo", L"video_w", hive.exo.video_w);
		write_int(L"exo", L"video_h", hive.exo.video_h);
		write_int(L"exo", L"video_rate", hive.exo.video_rate);
		write_int(L"exo", L"video_scale", hive.exo.video_scale);
		write_int(L"exo", L"audio_rate", hive.exo.audio_rate);
		write_int(L"exo", L"audio_ch", hive.exo.audio_ch);

		write_path(L"AviUtl", L"json_file_path", hive.json_file_path);
		write_int(L"AviUtl", L"create_token_item", hive.create_token_item);
		write_int(L"AviUtl", L"create_segment_item", hive.create_segment_item);
		write_int(L"AviUtl", L"create_psdtoolkit_item", hive.create_psdtoolkit_item);
		write_int(L"AviUtl", L"token_layer_offset", hive.token_layer_offset);
		write_int(L"AviUtl", L"segment_layer_offset", hive.segment_layer_offset);
		write_string(L"AviUtl", L"start_margin", hive.start_margin);
		write_string(L"AviUtl", L"end_margin", hive.end_margin);

		write_path(L"PSDToolKit", L"wav_folder_path", hive.wav_folder_path);
		write_int(L"PSDToolKit", L"use_lip_sync", hive.use_lip_sync);
		write_int(L"PSDToolKit", L"use_slider", hive.use_slider);
		write_int(L"PSDToolKit", L"slider_count", hive.slider_count);
		write_int(L"PSDToolKit", L"use_subtitle", hive.use_subtitle);
		write_int(L"PSDToolKit", L"all_in_one", hive.all_in_one);

		write_int(L"etc", L"choose_file_on_output", hive.choose_file_on_output);

		return TRUE;
	}

	//
	// jsonファイルのパスを返します。
	//
	virtual std::filesystem::path get_json_file_path() override
	{
		return transcriber.get_output_file_path(L".json");
	}

	//
	// wavフォルダのパスを返します。
	//
	virtual std::filesystem::path get_wav_folder_path() override
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
		try
		{
			// コンソールを開きます。
			Console console;

			// exoファイル出力の開始をユーザーに通知します。
			console.write(L"exoファイル出力を開始します" L"\n");

			// exoファイルのパスを作成します。
			auto exo_path = hive.audio_file_path;
			exo_path.replace_extension(L".exo");

			// exoファイルのパスをユーザーに通知します。
			console.write(std::format(L"exo_path = {}" L"\n", exo_path.wstring()));

			// 出力時に出力ファイルを選択する場合は
			if (hive.choose_file_on_output)
			{
				// そのまま渡すとデストラクトしてしまうのでコピーを作成します。
				auto default_path = exo_path.wstring();

				CFileDialog dialog(
					FALSE, // 保存ダイアログの場合はFALSEです。
					_T("exo"), // デフォルト拡張子です。
					default_path.c_str(), // デフォルトファイル名です。
					OFN_HIDEREADONLY | // 読み取り専用ファイルは表示しないようにします。
					OFN_OVERWRITEPROMPT, // 上書き確認するようにします。
					_T("exo files (*.exo)|*.exo|") // exoファイル用のフィルタです。
					_T("all files (*.*)|*.*||"), // すべてのファイル用のフィルタです。
					&main_dialog); // オーナーウィンドウです。

				// ファイルが選択された場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたファイルをexoファイルのパスにします。
					exo_path = to_string(dialog.GetPathName());
				}
				else
				{
					// ファイルが選択されなかった場合は失敗します。
					return FALSE;
				}
			}

			//
			// プロジェクトのプロパティです。
			//
			struct ExEdit {
				int width = hive.exo.video_w;
				int height = hive.exo.video_h;
				int rate = hive.exo.video_rate;
				int scale = hive.exo.video_scale;
				int length = 0;
				int audio_rate = hive.exo.audio_rate;
				int audio_ch = hive.exo.audio_ch;
			} exedit;

			//
			// 指定された時間(秒)をフレーム番号に変換して返します。
			//
			const auto to_frame = [&](float sec) {
				return (int)(sec * exedit.rate / exedit.scale);
			};

			//
			// 指定されたUTF-8文字列をテキストアイテム用に変換して返します。
			//
			const auto to_text = [&](const std::string& u8s) {
				return to_hex_string(cp_to_wide(u8s, CP_UTF8));
			};

			//
			// 与えられた文字列をエスケープして返します。
			//
			const auto escape = [](const std::wstring& s)
			{
				std::wstring result;
				for (auto ch : s)
				{
					if (ch == L'\\')
						result += L'\\';
					result += ch;
				}
				return wide_to_cp(result, CP_ACP);
			};

			// jsonファイルのパスを取得します。
			auto json_path = transcriber.get_output_file_path(L".json");

			// jsonファイルのストリームを開きます。
			std::ifstream json_stream(json_path);

			// ストリームを読み込んでルート要素を取得します。
			json json_root = json::parse(json_stream);

			// セグメント配列の要素を取得します。
			json json_segments = json_root["segments"];

			// セグメントが存在しない場合は
			if (json_segments.empty())
			{
				// ユーザーに通知します。
				AfxMessageBox(
					_T("exoファイルの作成に失敗しました") _T("\n")
					_T("原因：セグメントが存在しませんでした"));

				// exoファイルの作成に失敗したのでFALSEを返します。
				return FALSE;
			}

			// 音声全体の長さを取得します。
			exedit.length = to_frame(json_segments.back().at("end")) - 1 + 1;

			// exoファイルのストリームを開きます。
			std::ofstream exo_stream(exo_path, std::ios::binary);

			//
			// exoファイルのストリームにセクションを書き込みます。
			//
			const auto write_section = [&](const std::string& section_name) {
				exo_stream << std::format("[{}]\r\n", section_name);
			};

			//
			// exoファイルのストリームにプロパティを書き込みます。
			//
			const auto write_property = [&](const std::string& key_name, const std::string& value) {
				exo_stream << std::format("{}={}\r\n", key_name, value);
			};

			//
			// exoファイルのストリームにテキストアイテムを書き込みます。
			//
			const auto write_text_item = [&](size_t i, int layer, int group,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("camera", "0");

				write_section(std::format("{}.0", i));
				write_property("_name", "テキスト");
				write_property("サイズ", "34");
				write_property("表示速度", "0.0");
				write_property("文字毎に個別オブジェクト", "0");
				write_property("移動座標上に表示する", "0");
				write_property("自動スクロール", "0");
				write_property("B", "0");
				write_property("I", "0");
				write_property("type", "0");
				write_property("autoadjust", "0");
				write_property("soft", "1");
				write_property("monospace", "0");
				write_property("align", "0");
				write_property("spacing_x", "0");
				write_property("spacing_y", "0");
				write_property("precision", "1");
				write_property("color", "ffffff");
				write_property("color2", "000000");
				write_property("font", "MS UI Gothic");
				write_property("text", to_text(text));

				write_section(std::format("{}.1", i));
				write_property("_name", "標準描画");
				write_property("X", "0.0");
				write_property("Y", "0.0");
				write_property("Z", "0.0");
				write_property("拡大率", "100.00");
				write_property("透明度", "0.0");
				write_property("回転", "0.00");
				write_property("blend", "0");
			};

			//
			// exoファイルのストリームに音声ファイルアイテムを書き込みます。
			//
			const auto write_audio_file_item = [&](size_t i, int layer,
				int group, const std::filesystem::path& audio_file_path,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("audio", "1");

				write_section(std::format("{}.0", i));
				write_property("_name", "音声ファイル");
				write_property("再生位置", "0.00");
				write_property("再生速度", "100.0");
				write_property("ループ再生", "0");
				write_property("動画ファイルと連携", "0");
				write_property("file", audio_file_path.string());

				write_section(std::format("{}.1", i));
				write_property("_name", "標準再生");
				write_property("音量", "100.0");
				write_property("左右", "0.0");
			};

			//
			// exoファイルのストリームに口パク準備アイテムを書き込みます。
			//
			const auto write_lip_sync_item = [&](size_t i, int layer,
				int group, const std::filesystem::path& audio_file_path,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("camera", "0");

				write_section(std::format("{}.0", i));
				write_property("_name", "カスタムオブジェクト");
				write_property("track0", "0.00");
				write_property("track1", "0.00");
				write_property("track2", "0.00");
				write_property("track3", "0.00");
				write_property("check0", "0");
				write_property("type", "0");
				write_property("filter", "2");
				write_property("name", "口パク準備@PSDToolKit");
				write_property("param", std::format(R"(file="{}")", escape(audio_file_path.wstring())));

				write_section(std::format("{}.1", i));
				write_property("_name", "標準描画");
				write_property("X", "0.0");
				write_property("Y", "0.0");
				write_property("Z", "0.0");
				write_property("拡大率", "100.00");
				write_property("透明度", "0.0");
				write_property("回転", "0.00");
				write_property("blend", "0");
			};

			//
			// exoファイルのストリームに多目的スライダーアイテムを書き込みます。
			//
			const auto write_slider_item = [&](size_t i, int layer,
				int group, const std::filesystem::path& audio_file_path,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("camera", "0");

				int sub_index = 0;

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "カスタムオブジェクト");
				write_property("track0", "0.00");
				write_property("track1", "0.00");
				write_property("track2", "0.00");
				write_property("track3", "0.00");
				write_property("check0", "0");
				write_property("type", "0");
				write_property("filter", "2");
				write_property("name", "多目的スライダー@PSDToolKit");
				write_property("param", "");

				for (int j = 0; j < hive.slider_count; j++)
				{
					write_section(std::format("{}.{}", i, sub_index++));
					write_property("_name", "アニメーション効果");
					write_property("track0", "0.00");
					write_property("track1", "0.00");
					write_property("track2", "0.00");
					write_property("track3", "0.00");
					write_property("check0", "0");
					write_property("type", "0");
					write_property("filter", "2");
					write_property("name", "多目的スライダー@PSDToolKit");
					write_property("param", "");
				}

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "標準描画");
				write_property("X", "0.0");
				write_property("Y", "0.0");
				write_property("Z", "0.0");
				write_property("拡大率", "100.00");
				write_property("透明度", "0.0");
				write_property("回転", "0.00");
				write_property("blend", "0");
			};

			//
			// exoファイルのストリームに字幕準備アイテムを書き込みます。
			//
			const auto write_subtitle_item = [&](size_t i, int layer,
				int group, const std::filesystem::path& audio_file_path,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("camera", "0");

				int sub_index = 0;

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "テキスト");
				write_property("サイズ", "34");
				write_property("表示速度", "0.0");
				write_property("文字毎に個別オブジェクト", "0");
				write_property("移動座標上に表示する", "0");
				write_property("自動スクロール", "0");
				write_property("B", "0");
				write_property("I", "0");
				write_property("type", "0");
				write_property("autoadjust", "0");
				write_property("soft", "1");
				write_property("monospace", "0");
				write_property("align", "0");
				write_property("spacing_x", "0");
				write_property("spacing_y", "0");
				write_property("precision", "1");
				write_property("color", "ffffff");
				write_property("color2", "000000");
				write_property("font", "MS UI Gothic");
				write_property("text", to_text(std::format(
"<?s=[==[" "\r\n"
"{}" "\r\n"
"]==];require(\"PSDToolKit\").subtitle:set(s,obj,true);s=nil?>", text)));

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "標準描画");
				write_property("X", "0.0");
				write_property("Y", "0.0");
				write_property("Z", "0.0");
				write_property("拡大率", "100.00");
				write_property("透明度", "0.0");
				write_property("回転", "0.00");
				write_property("blend", "0");
			};

			//
			// exoファイルのストリームに一括アイテムを書き込みます。
			//
			const auto write_all_in_one_item = [&](size_t i, int layer,
				int group, const std::filesystem::path& audio_file_path,
				int frame_begin, int frame_end, const std::string& text)
			{
				write_section(std::format("{}", i));
				write_property("start", std::format("{}", frame_begin + 1));
				write_property("end", std::format("{}", frame_end + 1));
				write_property("layer", std::format("{}", layer + 1));
				if (group) write_property("group", std::format("{}", group));
				write_property("overlay", "1");
				write_property("camera", "0");

				int sub_index = 0;

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "テキスト");
				write_property("サイズ", "34");
				write_property("表示速度", "0.0");
				write_property("文字毎に個別オブジェクト", "0");
				write_property("移動座標上に表示する", "0");
				write_property("自動スクロール", "0");
				write_property("B", "0");
				write_property("I", "0");
				write_property("type", "0");
				write_property("autoadjust", "0");
				write_property("soft", "1");
				write_property("monospace", "0");
				write_property("align", "0");
				write_property("spacing_x", "0");
				write_property("spacing_y", "0");
				write_property("precision", "1");
				write_property("color", "ffffff");
				write_property("color2", "000000");
				write_property("font", "MS UI Gothic");
				write_property("text", to_text(std::format(
"<?s=[==[" "\r\n"
"{}" "\r\n"
"]==];require(\"PSDToolKit\").prep.init({{ls_mgl=0,ls_mgr=0,st_mgl=0,st_mgr=0,sl_mgl=0,sl_mgr=0,}},obj,s)?>", text)));

				if (hive.use_lip_sync)
				{
					write_section(std::format("{}.{}", i, sub_index++));
					write_property("_name", "アニメーション効果");
					write_property("track0", "0.00");
					write_property("track1", "0.00");
					write_property("track2", "0.00");
					write_property("track3", "0.00");
					write_property("check0", "0");
					write_property("type", "0");
					write_property("filter", "2");
					write_property("name", "口パク準備@PSDToolKit");
					write_property("param", std::format(R"(file="{}")", escape(audio_file_path.wstring())));
				}

				if (hive.use_slider)
				{
					for (int j = 0; j < hive.slider_count + 1; j++)
					{
						write_section(std::format("{}.{}", i, sub_index++));
						write_property("_name", "アニメーション効果");
						write_property("track0", "0.00");
						write_property("track1", "0.00");
						write_property("track2", "0.00");
						write_property("track3", "0.00");
						write_property("check0", "0");
						write_property("type", "0");
						write_property("filter", "2");
						write_property("name", "多目的スライダー@PSDToolKit");
						write_property("param", "");
					}
				}

				write_section(std::format("{}.{}", i, sub_index++));
				write_property("_name", "標準描画");
				write_property("X", "0.0");
				write_property("Y", "0.0");
				write_property("Z", "0.0");
				write_property("拡大率", "100.00");
				write_property("透明度", "0.0");
				write_property("回転", "0.00");
				write_property("blend", "0");
			};

			write_section("exedit");
			write_property("width", std::format("{}", exedit.width));
			write_property("height", std::format("{}", exedit.height));
			write_property("rate", std::format("{}", exedit.rate));
			write_property("scale", std::format("{}", exedit.scale));
			write_property("length", std::format("{}", exedit.length));
			write_property("audio_rate", std::format("{}", exedit.audio_rate));
			write_property("audio_ch", std::format("{}", exedit.audio_ch));

			// 最初のレイヤーのインデックスです。常に0です。
			auto layer_begin = 0;

			// 重複するアイテムをずらす量です。
			auto layer_offset = 0;

			// グループIDです。使用される度にインクリメントされます。
			auto group_id = 1;

			// トークン用アイテムを作成する場合は
			if (hive.create_token_item)
			{
				// テキストアイテムとオフセットの分だけずらす量を増やします。
				layer_offset += 1 + hive.token_layer_offset;
			}

			// セグメント用アイテムを作成する場合は
			if (hive.create_segment_item)
			{
				// テキストアイテムとオフセットの分だけずらす量を増やします。
				layer_offset += 1 + hive.segment_layer_offset;
			}

			// WAVファイルを格納するフォルダのパスを取得します。
			// 相対パスの可能性があるので絶対パスに変換します。
			auto wav_folder_path = std::filesystem::absolute(hive.wav_folder_path);

			// PSDToolKit用アイテムを作成する場合は
			if (hive.create_psdtoolkit_item)
			{
				// WAVファイルを格納するフォルダを作成します。
				std::filesystem::create_directories(wav_folder_path);

				// 既存のWAVファイルを削除するために
				// WAVファイルを格納するフォルダを走査します。
				for (auto& entry : std::filesystem::directory_iterator(wav_folder_path))
				{
					// WAVファイルの場合は削除します。
					if (entry.path().extension() == L".wav")
						std::filesystem::remove(entry.path());
				}

				// 音声ファイルアイテムの分だけずらす量を増やします。
				layer_offset += 1;

				if (hive.all_in_one)
				{
					// 一括アイテムの分だけずらす量を増やします。
					layer_offset += 1;
				}
				else
				{
					// 口パク準備アイテムの分だけずらす量を増やします。
					if (hive.use_lip_sync) layer_offset += 1;

					// 多目的スライダーアイテムの分だけずらす量を増やします。
					if (hive.use_slider) layer_offset += 1;

					// 字幕準備アイテムの分だけずらす量を増やします。
					if (hive.use_subtitle) layer_offset += 1;
				}
			}

			//
			// この構造体はタイムラインアイテムです。
			//
			struct Item
			{
				int frame_begin;
				int frame_end;
			};

			//
			// この構造体はタイムラインレイヤーです。
			//
			struct Layer
			{
				std::vector<Item> items;
			};

			// タイムラインレイヤーのコレクションです。
			std::vector<Layer> layers;

			//
			// 指定されたアイテムが交差する場合はTRUEを返します。
			//
			const auto intersect = [&](const Item& a, const Item& b) -> BOOL
			{
				return
					(a.frame_end >= b.frame_begin) &&
					(a.frame_begin <= b.frame_end) &&
					(b.frame_end >= a.frame_begin) &&
					(b.frame_begin <= a.frame_end);
/*
				return
					(a.frame_end < b.frame_begin) ||
					(a.frame_begin > b.frame_end) ||
					(b.frame_end < a.frame_begin) ||
					(b.frame_begin > a.frame_end);
*/
			};

			//
			// 指定されたアイテムが指定されたレイヤーに配置可能な場合はTRUEを返します。
			//
			const auto is_placeable = [&](const Layer& layer, const Item& item) -> BOOL
			{
				// アイテムのコレクションを走査します。
				for (size_t i = 0; i < layer.items.size(); i++)
				{
					// レイヤー内のアイテムと指定されたアイテムが交差する場合は
					if (intersect(layer.items[i], item))
						return FALSE; // 交差するアイテムがあるのでFALSEを返します。
				}

				return TRUE; // 交差するアイテムがないのでTRUEを返します。
			};

			//
			// 指定されたアイテムを配置可能なレイヤーのインデックスを返します。
			//
			const auto get_layer_index = [&](const Item& item) -> int
			{
				// レイヤーのコレクションを走査します。
				for (size_t i = 0; i < layers.size(); i++)
				{
					// レイヤーを取得します。
					auto& layer = layers[i];

					// アイテムがこのレイヤーに配置可能な場合は
					if (is_placeable(layer, item))
					{
						// レイヤーにアイテムを追加します。
						layer.items.emplace_back(item);

						// レイヤーのインデックスを算出して返します。
						return (int)i * layer_offset + layer_begin;
					}
				}

				// 新しいレイヤーの配列インデックスです。
				auto i = layers.size();

				// 新しいレイヤーを追加します。
				auto& layer = layers.emplace_back();

				// レイヤーにアイテムを追加します。
				layer.items.emplace_back(item);

				// レイヤーのインデックスを算出して返します。
				return (int)i * layer_offset + layer_begin;
			};

			// 開始時間と終了時間に加えるマージンです。
			auto start_margin = wcstof(hive.start_margin.c_str(), nullptr);
			auto end_margin = wcstof(hive.end_margin.c_str(), nullptr);

			// 一つ前のセグメントの終了時間です。
			auto prev_end = 0.0f;

			// タイムラインアイテムのexoファイル内のインデックスです。
			// アイテムをexoファイルに書き込む度にインクリメントされます。
			auto item_index = 0;

			// セグメント配列を走査します。
			auto segment_c = json_segments.size();
			for (decltype(segment_c) i = 0; i < segment_c; i++)
			{
				// セグメントの要素を取得します。
				json json_segment = json_segments[i];

				// セグメントの要素のプロパティを取得します。
				float start = json_segment["start"];
				float end = json_segment["end"];
				std::string text = json_segment["text"]; // ※UTF-8です。

				// セグメントの処理を開始することをユーザーに通知します。
				console.write(std::format(LR"(segment {}/{} [{:.2f} --> {:.2f}] "{}")" L"\n",
					i + 1, segment_c, start, end, cp_to_wide(text, CP_UTF8)));

				// 開始時間が一つ前の終了時間より前の場合は
				if (start < prev_end)
				{
					// 話し手が違う可能性があるので最大で0.0秒まで変更します。
					start = max(start - start_margin, 0.0f);
				}
				else
				{
					// 同じ話し手の可能性があるので最大で一つ前の終了時間まで変更します。
					start = max(start - start_margin, prev_end);
				}

				// 一つ先のセグメントの開始時間を取得します。
				auto next_start = end;
				if (i < segment_c - 1)
					next_start = json_segments[i + 1]["start"];

				// 終了時間が一つ先の開始時間より後の場合は
				if (end > next_start)
				{
					// 話し手が違う可能性があるので制限無しで変更します。
					end = end + end_margin;
				}
				else
				{
					// 同じ話し手の可能性があるので最大で一つ先の開始時間まで変更します。
					end = min(end + end_margin, next_start);
				}

				// 次のセグメント用に終了時間を更新します。
				prev_end = end;

				// アイテムを作成します。
				Item item { to_frame(start), to_frame(end) - 1 };

				// アイテムを配置可能なレイヤーインデックスを取得します。
				auto layer_index = get_layer_index(item);

				// [トークン][...]
				// [セグメント][...]
				// [音声ファイル]
				// [口パク準備]
				// [多目的スライダー]
				// [字幕準備(セグメント)]

				// トークン用アイテムを作成する場合は
				if (hive.create_token_item)
				{
					// トークンが存在しない場合などに例外が発生する可能性があるので
					// 例外処理を行います。
					try
					{
						// トークン配列の要素を取得します。
						json json_words = json_segment["words"];

						// トークン配列を走査します。
						auto token_c = json_words.size();
						for (decltype(token_c) j = 0; j < token_c; j++)
						{
							// トークンの要素を取得します。
							json json_word = json_words[j];

							// トークンの要素のプロパティを取得します。
							float start = json_word["start"];
							float end = json_word["end"];
							std::string word = json_word["word"]; // ※UTF-8です。

							// トークンの処理を開始することをユーザーに通知します。
							console.write(std::format(LR"(token {}/{} [{:.2f} --> {:.2f}] "{}")" L"\n",
								j + 1, token_c, start, end, cp_to_wide(word, CP_UTF8)));

							// アイテムを作成します。
							Item item { to_frame(start), to_frame(end) - 1 };

							// トークンをテキストアイテムとして書き込みます。
							write_text_item(item_index++, layer_index, group_id, item.frame_begin, item.frame_end, word);
						}
					}
					catch (...)
					{
					}

					// トークンアイテムを配置したのでレイヤーインデックスを増やします。
					layer_index += 1 + hive.token_layer_offset;

					// 使用済みのグループIDをインクリメントします。
					group_id++;
				}

				// セグメント用アイテムを作成する場合は
				if (hive.create_segment_item)
				{
					// セグメントをテキストアイテムとして書き込みます。
					write_text_item(item_index++, layer_index, 0, item.frame_begin, item.frame_end, text);

					// セグメントアイテムを配置したのでレイヤーインデックスを増やします。
					layer_index += 1 + hive.segment_layer_offset;
				}

				// PSDToolKit用アイテムを作成する場合は
				if (hive.create_psdtoolkit_item)
				{
					// WAVファイルのステムの最大長です。
					constexpr auto c_max_stem_length = 16;

					// WAVファイルのステムを作成します。
					auto audio_file_stem = cp_to_wide(text, CP_UTF8);
					if (audio_file_stem.length() >= c_max_stem_length)
					{
						audio_file_stem.resize(c_max_stem_length);
						audio_file_stem.back() = L'…';
					}

					// WAVファイルのパスを作成します。
					auto wav_file_path = wav_folder_path / std::format(L"{:03d}_{}.wav", i + 1, audio_file_stem);

					// WAVファイルを抽出するコマンドを作成します。
					auto command = std::format(
						LR"("{}" -i "{}" -ss {:.2f} -t {:.2f} -ar 16000 -ac 1 -c:a pcm_s16le -y "{}")",
						hive.ffmpeg_path.wstring(),
						std::filesystem::absolute(hive.audio_file_path).wstring(),
						start,
						end - start,
						wav_file_path.wstring());

					// サブプロセスを起動します。完了するまで待機します。
					SubProcess(wav_folder_path.wstring(), command).wait();

					// セグメントを音声ファイルアイテムとして書き込みます。
					write_audio_file_item(item_index++, layer_index++, group_id, wav_file_path, item.frame_begin, item.frame_end, text);

					if (hive.all_in_one)
					{
						write_all_in_one_item(item_index++, layer_index++, group_id, wav_file_path, item.frame_begin, item.frame_end, text);
					}
					else
					{
						// セグメントを口パク準備アイテムとして書き込みます。
						if (hive.use_lip_sync)
							write_lip_sync_item(item_index++, layer_index++, group_id, wav_file_path, item.frame_begin, item.frame_end, text);

						// セグメントを多目的スライダーアイテムとして書き込みます。
						if (hive.use_slider)
							write_slider_item(item_index++, layer_index++, group_id, wav_file_path, item.frame_begin, item.frame_end, text);

						// セグメントを字幕準備アイテムとして書き込みます。
						if (hive.use_subtitle)
							write_subtitle_item(item_index++, layer_index++, group_id, wav_file_path, item.frame_begin, item.frame_end, text);
					}

					// 使用済みのグループIDをインクリメントします。
					group_id++;
				}
			}

			// exoファイル出力の完了をユーザーに通知します。
			console.write(L"exoファイル出力が完了しました" L"\n");

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

			// コンフィグを読み込みます。
			read_config();

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
