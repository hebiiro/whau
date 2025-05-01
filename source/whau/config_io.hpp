#pragma once

//
// このクラスはコンフィグの入出力を担当します。
//
inline struct ConfigIO
{
	//
	// コンフィグをファイルから読み込みます。
	//
	BOOL read_config()
	{
		auto path = get_module_file_name(nullptr).replace_extension(L".ini").wstring();

		const auto read_string = [&](LPCWSTR section_name, LPCWSTR key_name, std::wstring& value) {
			value.resize(MAX_PATH);
			auto length = ::GetPrivateProfileStringW(section_name, key_name,
				value.c_str(), value.data(), (DWORD)value.size(), path.c_str());
			value.resize(length);
		};

		const auto read_int = [&](LPCWSTR section_name, LPCWSTR key_name, int& value) {
			value = ::GetPrivateProfileIntW(section_name, key_name, value, path.c_str());
		};

		const auto read_path = [&](LPCWSTR section_name, LPCWSTR key_name, std::filesystem::path& value) {
			std::wstring s = value.wstring();
			read_string(section_name, key_name, s);
			value = s;
		};

		read_path(L"Faster-Whisper", L"download_url", hive.download_url);
		read_path(L"Faster-Whisper", L"audio_file_path", hive.audio_file_path);
		read_path(L"Faster-Whisper", L"interim_folder_path", hive.interim_folder_path);
		read_string(L"Faster-Whisper", L"task", hive.task);
		read_string(L"Faster-Whisper", L"language", hive.language);
		read_string(L"Faster-Whisper", L"japanese_mode", hive.japanese_mode);
		read_string(L"Faster-Whisper", L"model", hive.model);
		read_string(L"Faster-Whisper", L"diarize", hive.diarize);
		read_string(L"Faster-Whisper", L"ff", hive.ff);
		read_string(L"Faster-Whisper", L"vad_method", hive.vad_method);
		read_string(L"Faster-Whisper", L"vad_speech_pad_ms", hive.vad_speech_pad_ms);
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
		read_int(L"AviUtl", L"tail_layer_offset", hive.tail_layer_offset);
		read_int(L"AviUtl", L"segment_layer_offset", hive.segment_layer_offset);
		read_string(L"AviUtl", L"start_margin", hive.start_margin);
		read_string(L"AviUtl", L"end_margin", hive.end_margin);

		read_path(L"PSDToolKit", L"wav_folder_path", hive.wav_folder_path);
		read_int(L"PSDToolKit", L"use_lip_sync", hive.use_lip_sync);
		read_int(L"PSDToolKit", L"use_subtitle", hive.use_subtitle);
		read_int(L"PSDToolKit", L"slider_count", hive.slider_count);
		read_int(L"PSDToolKit", L"all_in_one", hive.all_in_one);

		read_int(L"etc", L"use_speaker", hive.use_speaker);
		read_int(L"etc", L"choose_folder_on_transcribe", hive.choose_folder_on_transcribe);
		read_int(L"etc", L"choose_file_on_output_exo_file", hive.choose_file_on_output_exo_file);

		return TRUE;
	}

	//
	// コンフィグをファイルに書き込みます。
	//
	BOOL write_config()
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

		write_path(L"Faster-Whisper", L"download_url", hive.download_url);
		write_path(L"Faster-Whisper", L"audio_file_path", hive.audio_file_path);
		write_path(L"Faster-Whisper", L"interim_folder_path", hive.interim_folder_path);
		write_string(L"Faster-Whisper", L"task", hive.task);
		write_string(L"Faster-Whisper", L"language", hive.language);
		write_string(L"Faster-Whisper", L"japanese_mode", hive.japanese_mode);
		write_string(L"Faster-Whisper", L"model", hive.model);
		write_string(L"Faster-Whisper", L"diarize", hive.diarize);
		write_string(L"Faster-Whisper", L"ff", hive.ff);
		write_string(L"Faster-Whisper", L"vad_method", hive.vad_method);
		write_string(L"Faster-Whisper", L"vad_speech_pad_ms", hive.vad_speech_pad_ms);
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
		write_int(L"AviUtl", L"tail_layer_offset", hive.tail_layer_offset);
		write_string(L"AviUtl", L"start_margin", hive.start_margin);
		write_string(L"AviUtl", L"end_margin", hive.end_margin);

		write_path(L"PSDToolKit", L"wav_folder_path", hive.wav_folder_path);
		write_int(L"PSDToolKit", L"use_lip_sync", hive.use_lip_sync);
		write_int(L"PSDToolKit", L"use_subtitle", hive.use_subtitle);
		write_int(L"PSDToolKit", L"slider_count", hive.slider_count);
		write_int(L"PSDToolKit", L"all_in_one", hive.all_in_one);

		write_int(L"etc", L"use_speaker", hive.use_speaker);
		write_int(L"etc", L"choose_folder_on_transcribe", hive.choose_folder_on_transcribe);
		write_int(L"etc", L"choose_file_on_output_exo_file", hive.choose_file_on_output_exo_file);

		return TRUE;
	}
} config_io;
