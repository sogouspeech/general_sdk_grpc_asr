#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <iterator>
#include <cstdlib>
#include <random>
#include <regex>
#include <deque>
#include <vector>

#include "ss_engine_wrapper.h"

#ifdef _WIN32
#  include <windows.h>
#endif

const char* token = NULL;

void getToken() {
	token = sogou_speech_get_token(R"JSON({
				"app_id":"xxxx",
				"app_key":"xxxx",
				"cer_path":"roots.pem",
				"expire":2592000
			})JSON");
}

void print_asr_result(sogou_speech_engine* eng, const char* json, void* context) {

	std::cout << u8">>>>>>>>>>识别结果>>>>>>>>>>" << std::endl;
	std::cout << json << std::endl;

	// 此处应使用json库解析结果，这里简单处理
	std::regex reg(R"("status": ([+-]?\d+))");
	std::smatch matches;
	std::string result(json);

	if (std::regex_search(result, matches, reg)) {

		auto m = matches[1].str();
		auto status = std::strtol(m.c_str(), nullptr, 10);

		// 此处检测有错误，则通知线程重新开始识别
		// 也可以按照需求判断得到最终结果(isbreak = true)就停止识别等等
		if (status != 0)
			*static_cast<bool*>(context) = true;
	}
}

void asr_thread_func() {

	getToken();

	if (!token) {
		return;
	}

	std::cout << u8"语音识别线程启动..." << std::endl;
	sogou_speech_init(
		R"JSON({
			"appid":"xxxx",
			"skey":"xxxx",
			"appname":"com.sogou.speech.demo_online_asr",
			"imei":"862066060926230"
		})JSON"
	);

	auto eng = sogou_speech_create_engine();

	auto ret = sogou_speech_asr_init(
		eng,
		R"JSON({
			"service": "remote",
			"remote_mode":"grpc",
			"lang":"zh-cmn-Hans-CN",
			"prefix":"canary.speech.sogou.com:443"
		})JSON"
	);

	if (ret != 0) {
		std::cout << "sogou_speech_asr_init failed" << std::endl;
		sogou_speech_release_engine(eng);
		sogou_speech_cleanup();
		return;
	}

	int test_count = 5;
	int index = 0;
	uint16_t buf[8000];

	bool isError = false;

	std::string token_string = token;

	std::string start_arg =
		R"JSON({"asr":{"token":")JSON" + token_string + R"JSON(",)JSON" +
		R"JSON("cer_path":"roots.pem","interim_results":true,)JSON" +
		R"JSON("recognition_config":{"sample_rate":16000,"audio_encoding":2})JSON" +
		R"JSON(}})JSON";

	ret = sogou_speech_asr_start(eng, start_arg.c_str(), print_asr_result, &isError);

	if (ret != 0) {
		std::cout << "sogou_speech_asr_start failed" << std::endl;
		sogou_speech_release_engine(eng);
		sogou_speech_cleanup();
		return;
	}

	std::ifstream fs("audio/open_alipay_twice.wav", std::ios::binary);
	int32_t pack_id = 0;
	int32_t readcount = 0;
	fs.seekg(0, std::ios::end);
	int32_t totalSize = fs.tellg();
	fs.seekg(44, std::ios::beg);

	int32_t readPosition = 44;

	for (;;) {

		if (fs.good()) {
			fs.read(reinterpret_cast<char*>(buf), 8000 * sizeof(uint16_t));
			readcount = static_cast<int32_t>(fs.gcount()) / sizeof(uint16_t);
			readPosition = readPosition + readcount * 2;
			if (readcount > 0) {
				++pack_id;
				if (readPosition == totalSize) {
					pack_id = -1 * pack_id;
					std::cout << "read audio end" << std::endl;
				}
				sogou_speech_process_sound_data(eng, buf, readcount, pack_id);
			}
		}

		if (isError) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}
	
	fs.close();
	sogou_speech_asr_stop(eng);
	sogou_speech_asr_close(eng);

	sogou_speech_release_engine(eng);
	sogou_speech_cleanup();

	if (token) {
		sogou_speech_free((void*)token);
	}

	std::cout << u8"语音识别线程退出..." << std::endl;

}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(65001);
	setvbuf(stdout, nullptr, _IOFBF, 10240);
#endif

	std::cout << u8"搜狗语音SDK示例程序(在线识别grpc)" << sogou_speech_get_sdk_version() << std::endl;

	std::thread asr_thread(asr_thread_func);

	asr_thread.join();

	std::cout << "Press any key to exit" << std::endl;
	std::cin.get();
	return 0;
}