﻿#pragma once

//
// このクラスはアプリケーションのインターフェイスです。
//
inline struct AppInterface {
	virtual BOOL read_config() = 0;
	virtual BOOL write_config() = 0;
	virtual std::filesystem::path get_default_interim_folder_path() = 0;
	virtual std::filesystem::path get_default_json_file_path() = 0;
	virtual std::filesystem::path get_default_exo_file_path() = 0;
	virtual std::filesystem::path get_default_wav_folder_path() = 0;
	virtual std::filesystem::path get_actual_command() = 0;
	virtual BOOL transcribe() = 0;
	virtual BOOL output_exo_file() = 0;
} *app = {};
