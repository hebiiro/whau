#pragma once

//
// このクラスはexoファイルを作成します。
//
struct ExoCreator
{
	//
	// exoファイルを出力します。
	//
	BOOL operator()(const std::filesystem::path& default_exo_path)
	{
		try
		{
			// コンソールを開きます。
			Console console;

			// exoファイル出力の開始をユーザーに通知します。
			console.write(L"exoファイル出力を開始します" L"\n");

			// exoファイルのパスを作成します。
			// 相対パスの可能性があるので絶対パスに変換します。
			auto exo_path = std::filesystem::absolute(default_exo_path);

			// 出力時にexoファイルを選択する場合は
			if (hive.choose_file_on_output_exo_file)
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
					AfxGetMainWnd()); // オーナーウィンドウです。

				// ユーザーがファイルを選択した場合は
				if (IDOK == dialog.DoModal())
				{
					// 選択されたファイルをexoファイルのパスにします。
					exo_path = to_string(dialog.GetPathName());
				}
				// ユーザーがファイルを選択しなかった場合は
				else
				{
					// 処理を中止します。
					return FALSE;
				}
			}

			// exoファイルのパスをユーザーに通知します。
			console.write(std::format(L"exo_path = {}" L"\n", exo_path.wstring()));

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
			// 相対パスの可能性があるので絶対パスに変換します。
			auto json_path = std::filesystem::absolute(hive.json_file_path);

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

			// セグメントに話者番号を追加します。
			if (hive.use_speaker)
			{
				//
				// この構造体はtsvファイルを読み込みます。
				//
				struct Tsv
				{
					//
					// tsvファイルの行データです。
					//
					struct Line {
						int start = 0;
						int end = 0;
						int speaker = 0;
					};

					//
					// tsvファイルの行毎のデータです。
					//
					std::vector<Line> lines;

					//
					// コンストラクタです。
					//
					Tsv(const std::filesystem::path& json_path)
					{
						// tsvファイルのパスを取得します。
						auto tsv_path = json_path;
						tsv_path.replace_extension(L".tsv");

						// tsvファイルのストリームを開きます。
						std::ifstream file(tsv_path);

						// tsvファイルを一行ずつ読み込みます。
						std::string line; // ※UTF-8です。
						std::getline(file, line); // 一行目は読み飛ばします。
						while (std::getline(file, line))
						{
							// 末尾のキャリッジリターンを除去します。
							if (line.length() && line.back() == '\r') line.pop_back();

							// 文字列ストリームを開きます。
							std::wstringstream stream(cp_to_wide(line, CP_UTF8));

							constexpr size_t c_start = 0;
							constexpr size_t c_end = 1;
							constexpr size_t c_text = 2;
							constexpr size_t c_max_size = 3;

							std::vector<std::wstring> cells(c_max_size);

							// タブ区切りでセルを読み込みます。
							for (auto& cell : cells)
								std::getline(stream, cell, L'\t');

							// 話者番号を取得するための正規表現パターンです。
							std::wregex reg(LR"(^\[SPEAKER_(\d+)\]\: (.*)$)");
							std::wsmatch results;

							// パターンにマッチしなかった場合は
							if (!std::regex_search(cells[c_text], results, reg) || results.size() != 3)
								continue; // この行を読み飛ばします。

							// 行データを取得します。
							auto start = to_int(cells[c_start]);
							auto end = to_int(cells[c_end]);
							auto speaker = to_int(results[1].str());

							// 行データを追加します。
							lines.emplace_back(start, end, speaker);
						}
					}

					//
					// 文字列をintに変換して返します。
					//
					int to_int(const std::wstring& s)
					{
						return wcstol(s.c_str(), nullptr, 10);
					}
				} tsv(json_path);

				// セグメントを走査します。
				for (auto& json_segment : json_segments)
				{
					//
					// 秒(float)をミリ秒(int)に変換して返します。
					//
					const auto sec_to_ms = [](float sec) { return (int)(sec * 1000 + 0.5f); };

					// セグメントのプロパティを取得します。
					auto start = sec_to_ms(json_segment["start"]);
					auto end = sec_to_ms(json_segment["end"]);

					int speaker = 0;

					// tsv行を走査します。
					for (auto& line : tsv.lines)
					{
						if (line.start > start) break;

						speaker = line.speaker;
					}

					// セグメントに話者番号をセットします。
					json_segment["speaker"] = speaker;
				}
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

				if (hive.slider_count != hive.c_slider_count.c_0)
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
			auto layer_offset = hive.tail_layer_offset;

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

			// wavファイルを格納するフォルダのパスを取得します。
			// 相対パスの可能性があるので絶対パスに変換します。
			auto wav_folder_path = std::filesystem::absolute(hive.wav_folder_path);

			// PSDToolKit用アイテムを作成する場合は
			if (hive.create_psdtoolkit_item)
			{
				// wavファイルを格納するフォルダを作成します。
				std::filesystem::create_directories(wav_folder_path);

				// 既存のwavファイルを削除するために
				// wavファイルを格納するフォルダを走査します。
				for (auto& entry : std::filesystem::directory_iterator(wav_folder_path))
				{
					// wavファイルの場合は削除します。
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
					if (hive.slider_count != hive.c_slider_count.c_0) layer_offset += 1;

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
			const auto get_layer_index = [&](const Item& item, size_t first_pos) -> int
			{
				// レイヤーのコレクションを走査します。
				size_t i;
				for (i = first_pos; i < layers.size(); i++)
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

				// レイヤーのコレクションを拡張します。
				if (i >= layers.size()) layers.resize(i + 1);

				// レイヤーにアイテムを追加します。
				layers[i].items.emplace_back(item);

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

				// 話者番号の要素を取得します。
				json json_speaker = json_segment["speaker"];

				// 最初のレイヤーインデックスを取得します。
				auto first_pos = json_speaker.is_null() ? 0 : (int)json_speaker;

				// アイテムを配置可能なレイヤーインデックスを取得します。
				auto layer_index = get_layer_index(item, first_pos);

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
					//
					// 文字列をエスケープして返します。
					//
					const auto escape = [](const std::wstring& str)
					{
						const std::unordered_map<wchar_t, wchar_t> escape_map =
						{
							{ L'\\', L'￥' },
							{ L'/', L'／' },
//							{ L'(', L'（' },
//							{ L')', L'）' },
//							{ L'{', L'｛' },
//							{ L'}', L'｝' },
//							{ L'[', L'［' },
//							{ L']', L'］' },
							{ L'^', L'＾' },
							{ L'$', L'＄' },
//							{ L'.', L'．' },
							{ L'*', L'＊' },
//							{ L'+', L'＋' },
							{ L'?', L'？' },
							{ L'|', L'｜' },
						};
						std::wstring ret_value;
						for (auto ch : str) {
							if (auto it = escape_map.find(ch); it != escape_map.end())
								ret_value += it->second;
							else
								ret_value += ch;
						}
						return ret_value;
					};

					// wavファイルのステムの最大長です。
					constexpr auto c_max_stem_length = 16;

					// wavファイルのステムを作成します。
					auto audio_file_stem = cp_to_wide(text, CP_UTF8);
					if (audio_file_stem.length() >= c_max_stem_length)
					{
						audio_file_stem.resize(c_max_stem_length);
						audio_file_stem.back() = L'…';
					}
					audio_file_stem = escape(audio_file_stem);

					// wavファイルのパスを作成します。
					auto wav_file_path = wav_folder_path / std::format(L"{:03d}_{}.wav", i + 1, audio_file_stem);

					// wavファイルを抽出するコマンドを作成します。
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
						if (hive.slider_count != hive.c_slider_count.c_0)
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
};
