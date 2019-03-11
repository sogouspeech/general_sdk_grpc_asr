#pragma once

#include "ss_export.h"

#ifdef __cplusplus
extern "C" {
#endif

// 获取版本号描述
__SOGOU_API__ const char * sogou_speech_get_sdk_version();

// 设置全局初始化，所有函数必须在此函数调用之后才可以调用，传入json格式：
//
// {
// 	 "appid": "xxxx",				     # 知音平台上申请得来
// 	 "skey": "xxxx",                     # 知音平台上申请得来
// 	 "appname": "xxxx",					 # 应用程序名,开发者自己定义
// 	 "imei": "xxxxxxxxxxxxxxxx"          # 机器码，手机应填串号，其他平台应获取唯一机器码，例如Windows端应使用hash(硬盘号+物理网卡)等方式生成
// }

__SOGOU_API__ int32_t sogou_speech_init(const char* global_init_json);


/*全局反初始化，进程退出或者不再使用识别引擎的时候需要调用
  调用本函数之前，必须先调用sogou_speech_release_engine等函数释放local资源，
  然后再调用本函数全局清理,否则将导致未定义的行为，可能引发崩溃
*/
__SOGOU_API__ void sogou_speech_cleanup();


// 引擎handle，标识一次语音唤醒、识别session
typedef struct _sogou_speech_engine  {
	int unused;
} sogou_speech_engine;


// 创建语音识别引擎
// 返回引擎指针，如果返回0则创建失败
// 注意，引擎并非线程安全，多线程调用sogou_speech_engine为第一个参数的函数，需要使用方互斥
__SOGOU_API__ sogou_speech_engine * sogou_speech_create_engine();

// 析构引擎
__SOGOU_API__ void sogou_speech_release_engine(sogou_speech_engine* engine);

// 由于回调的必要，应在process_sound_data的线程中，循环定时调用本函数，直到sogou_speech_release_engine
// 返回值0表示成功，> 0 表示仍有待处理请求，后续需要继续调用本函数
__SOGOU_API__ int32_t sogou_speech_do_idle(sogou_speech_engine* engine);

// 平时按顺序传入音频数据的时候，packet_id >= 0 且自增，当标识最后一包数据（音频结束）时，传入packet_id < 0，此时强制结束
// 回传最后的识别结果
// 返回值： 0 正常， 其余错误
__SOGOU_API__ int32_t sogou_speech_process_sound_data(sogou_speech_engine* engine, const uint16_t* data, int32_t len, int32_t packet_id);

// 语音识别结果的回调函数
// 结果以json方式提供
// context: 用户自定义上下文
// json 结果如下示例：
// {
//     "status": 0,                      # 状态码，0正常，其余错误
//     "message": "ok",                  # 错误原因
//     "result": [
//         {
//             "text": "打开支付宝",     # 识别结果
//             "isbreak": true,          # 是否是最终结果，true为最终，否则是中间结果
//             "service": "remote"       # remote: 在线
//         }
//     ]
//	   semantic_result: null             # 语义识别结果
// }
typedef void (*sogou_speech_on_asr_result_pfn_t) (sogou_speech_engine* engine, const char* json_result, void* context);

// 语音识别初始化,返回0成功，其它失败
// 初始化参数是json格式
// 参数示例如下：
// {
//				"service":"remote",
//				"remote_mode":"grpc",
//				"lang":"zh-cmn-Hans-CN",
//              "prefix":"xxxx"
// }
__SOGOU_API__ int32_t sogou_speech_asr_init(sogou_speech_engine* engine, const char* json_config);

// 语音识别启动，返回0成功，其它失败
// 启动参数是json格式
// 参数示例如下：
// {
//		"asr":{
//				"token":"xxx",
//				"cer_path":"xxx",
//				"interim_results":"true",
//				"recognition_config":{
//					"sample_rate":16000
//					"audio_encoding":2
//				}
//		}
// }
__SOGOU_API__ int32_t sogou_speech_asr_start(sogou_speech_engine* engine, const char* json_config, sogou_speech_on_asr_result_pfn_t cb, void* context);

//语音识别暂停,0成功，其它失败
__SOGOU_API__ int32_t sogou_speech_asr_stop(sogou_speech_engine* engine);

__SOGOU_API__ void* sogou_speech_asr_get_context(sogou_speech_engine* engine);

//语音识别关闭，释放语音识别相关的资源
__SOGOU_API__ void sogou_speech_asr_close(sogou_speech_engine* engine);

// 申请一段内存，SDK内部使用和此函数相同的方式分配内存，比如字符串
__SOGOU_API__ void * sogou_speech_malloc(size_t size);

// 释放一段内存，从SDK传出的内存（字符串），必须使用本函数释放
// 使用裸 free、delete 等函数无法跨越编译器边界，可能导致崩溃
__SOGOU_API__ void sogou_speech_free(void * ptr);

//获取grpc协议语音识别所需要的token
__SOGOU_API__ const char* sogou_speech_get_token(const char* json_config);

#ifdef __cplusplus
}
#endif
