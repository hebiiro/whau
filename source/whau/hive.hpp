#pragma once

//
// このクラスは文字起こしアプリを管理します。
//
inline struct Hive
{
	inline static constexpr struct {
		inline static constexpr int c_0 = 0;
		inline static constexpr int c_4 = 1;
		inline static constexpr int c_8 = 2;
		inline static constexpr int c_12 = 3;
		inline static constexpr int c_16 = 4;
		inline static constexpr int c_20 = 5;
		inline static constexpr int c_24 = 6;
		inline static constexpr int c_28 = 7;
		inline static constexpr int c_32 = 8;
		inline static constexpr int c_36 = 9;
		inline static constexpr int c_40 = 10;
	} c_slider_count;

	//
	// Faster-Whisper用の設定です。
	//
	std::filesystem::path audio_file_path = L"assets\\samples\\電車.wav";
	std::filesystem::path interim_folder_path;
	std::wstring task = L"指定なし";
	std::wstring language = L"ja";
	std::wstring model = L"large-v2";
	std::wstring diarize = L"pyannote_v3.1";
	std::wstring vad_method = L"pyannote_v3";
	std::wstring vad_speech_pad_ms;
	std::wstring ff = L"指定なし";
	std::wstring japanese_mode = L"blend";
	std::wstring additional_command = L"--compute_type default";
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
	BOOL create_token_item = TRUE;
	BOOL create_segment_item = TRUE;
	BOOL create_psdtoolkit_item = TRUE;
	int token_layer_offset = 1;
	int segment_layer_offset = 1;
	int tail_layer_offset = 1;
	std::wstring start_margin = L"0.0";
	std::wstring end_margin = L"0.0";

	//
	// PSDToolKit用の設定です。
	//
	std::filesystem::path wav_folder_path;
	BOOL use_lip_sync = TRUE;
	BOOL use_subtitle = TRUE;
	int slider_count = c_slider_count.c_0;
	BOOL all_in_one = TRUE;

	//
	// TRUEの場合は文字起こしを実行したときにフォルダを選択します。
	//
	BOOL choose_folder_on_transcribe = FALSE;

	//
	// TRUEの場合はexoファイルを出力するときにファイルを選択します。
	//
	BOOL choose_file_on_output_exo_file = FALSE;

	//
	// TRUEの場合は話者番号を使用します。
	//
	BOOL use_speaker = TRUE;

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
