#pragma once

//
// このクラスは文字起こしアプリを管理します。
//
inline struct Hive
{
	inline static constexpr struct {
		inline static constexpr int c_segment = 0;
		inline static constexpr int c_token = 1;
		inline static constexpr int c_both = 2;
	} c_split_mode;

	inline static constexpr struct {
		inline static constexpr int c_4 = 0;
		inline static constexpr int c_8 = 1;
		inline static constexpr int c_12 = 2;
		inline static constexpr int c_16 = 3;
		inline static constexpr int c_20 = 4;
		inline static constexpr int c_24 = 5;
		inline static constexpr int c_28 = 6;
		inline static constexpr int c_32 = 7;
		inline static constexpr int c_36 = 8;
		inline static constexpr int c_40 = 9;
	} c_slider_count;

	//
	// Faster-Whisper用の設定です。
	//
	std::filesystem::path audio_file_path = L"sample.wav";
	std::wstring task = L"指定なし";
	std::wstring language = L"ja";
	std::wstring model = L"large-v2";
	std::wstring diarize = L"pyannote_v3.1";
	std::wstring vad_method = L"pyannote_v3";
	std::wstring vad_speech_pad_ms;
	std::wstring ff = L"指定なし";
	std::wstring japanese_mode = L"blend";
	std::wstring output_sub_folder_name;
	std::wstring additional_command;
	std::wstring actual_command;

	//
	// exoファイルの設定です。
	//
	struct {
		int video_w = 1920;
		int video_h = 1080;
		int video_rate = 60;
		int video_scale = 1;
		int audio_rate = 48000;
		int audio_ch = 2;
	} exo;

	//
	// AviUtl用の設定です。
	//
	std::filesystem::path json_file_path;
	int split_mode = c_split_mode.c_both;
	int layer_offset = 0;
	std::wstring start_margin = L"0.0";
	std::wstring end_margin = L"0.0";
	std::filesystem::path template_file_path = L"未実装";

	//
	// PSDToolKit用の設定です。
	//
	std::filesystem::path wav_folder_path;
	BOOL use_lip_sync = TRUE;
	BOOL use_slider = FALSE;
	int slider_count = c_slider_count.c_8;
	BOOL use_subtitle = TRUE;
	BOOL all_in_one = TRUE;

	//
	// その他の設定です。
	//
	BOOL choose_file_on_output = FALSE;

	//
	// フォルダ構成の原点となるパスです。
	//
	std::filesystem::path origin;

	//
	// ダウンロード元のurlです。
	//
	std::filesystem::path download_url;

	//
	// ダウンロード先のパスです。
	//
	std::filesystem::path download_path;

	//
	// 文字起こしアプリのパスです。
	//
	std::filesystem::path whisper_path;

	//
	// 動画変換アプリのパスです。
	//
	std::filesystem::path ffmpeg_path;

	//
	// このクラスはサブスレッドのベースクラスです。
	//
	struct SubThread
	{
		SubThread()
		{
			hive.current_thread = this;
		}

		~SubThread()
		{
			hive.current_thread = nullptr;
		}
	};

	//
	// 現在処理中のスレッドです。
	//
	SubThread* current_thread = nullptr;
} hive;
