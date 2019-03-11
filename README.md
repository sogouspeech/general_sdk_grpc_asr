# C++版通用SDK基于grpc协议在线语音识别Demo

## 支持的目标平台


| OS                    | CPU                       |
| :-------------------- | ------------------------- |
| Linux                 | x86 x64 (gcc,g++版本4.9.2)|
| Windows 7 or higher   | x86 x64                   |

## 目录结构介绍

```bash
general_sdk_grpc_asr
├─include                                   # 开发者使用的头文件目录
│  ├─ss_engine_wrapper.h                    # 通用SDK语音识别API头文件，开发者使用此头文件集成语音识别的功能
│  └─ss_export                              # 此头文件开发者无需关心
│            
├─lib		                                # 存放不同平台库文件的目录
│  ├─Linux                              	# Linux平台下库文件目录
│  │  ├─x64		
│  │  │  └─libssasr.so						# Linux系统，x64架构的库文件
│  │  │      
│  │  └─x86
│  │	 └─libssasr.so						# Linux系统，x86架构的库文件
│  │
│  └─Windows								# Windows平台下库文件目录
│     ├─x64
│     │  ├─libprotobuf.dll                  # Windows系统，x64架构下使用C++版通用SDK的API编译出来的可执行文件运行时依赖的动态库
│     │  ├─ssasr.dll						# Windows系统，x64架构下使用C++版通用SDK的API编译出来的可执行文件运行时依赖的动态库
│     │  └─ssasr.lib						# Windows系统，x64架构下使用C++版通用SDK的API编译时需要的库文件
│     └─x86
│        ├─libprotobuf.dll                  # Windows系统，x86架构下可执行文件运行时依赖的动态库
│        ├─ssasr.dll						# Windows系统，x86架构下使用C++版通用SDK的API编译出来的可执行文件运行时依赖的动态库
│        └─ssasr.lib						# Windows系统，x86架构下使用C++版通用SDK的API编译时需要的库文件
│  
├─resource								    # 保存测试音频,https所需要证书文件目录
│  ├─audio									# 测试音频文件目录 
│  └─roots.pem								# 根证书文件，gprc协议需要
│                                 
├─src		                                # 测试Demo源代码目录				
│  └─grpc_asr.cpp                           # grpc协议在线语音识别Demo
│      
└─CMakeLists.txt                            # Demo工程的编译脚本,CMake语法
```

## 集成grpc协议语音识别功能

请开发者参考general_sdk_grpc_asr/src/grpc_asr.cpp集成grpc协议语音识别功能


[C++版通用SDK基于grpc协议在线语音识别接口参考](https://docs.zhiyin.sogou.com/docs/asr/sdk/reference/gsdk)


## Windows平台下编译grpc协议在线语音识别Demo

开发者使用Visual Studio打开general_sdk_grpc_asr,按照CMake(K)->更改CMake设置(S)->grpc_asr_demo打开CMakeSettings.json,请按以下配置修改CMakeSettings.json内容

```
{
  // 请参阅 https://go.microsoft.com//fwlink//?linkid=834763 了解有关此文件的详细信息。
  "configurations": [
    {
      "name": "x86-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x86" ],
      "buildRoot": "${workspaceRoot}\\build\\${name}",
      "installRoot": "${workspaceRoot}\\build\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": ""
    },
    {
      "name": "x86-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "inheritEnvironments": [ "msvc_x86" ],
      "buildRoot": "${workspaceRoot}\\build\\${name}",
      "installRoot": "${workspaceRoot}\\build\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": ""
    },
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${workspaceRoot}\\build\\${name}",
      "installRoot": "${workspaceRoot}\\build\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": ""
    },
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "${workspaceRoot}\\build\\${name}",
      "installRoot": "${workspaceRoot}\\build\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": ""
    }
  ]
}
```

CMake(K)->调动和启动设置(L)->grpc_asr_demo.exe打开launch.vs.json,请按以下配置修改launch.vs.json内容
```
{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "grpc_asr_demo.exe",
      "currentDir": "${workspaceRoot}\\resource",
      "name": "grpc_asr_demo.exe"
    }
  ]
}
```

CMake(K)->全部重新生成(R)编译Demo工程<br>

如果开发者选择是x86-Debug平台，请将general_sdk_grpc_asr/lib/Windows/x86下的libprotobuf.dll和ssasr.dll复制到build/x86-debug目录下，grpc_asr_demo.exe运行时依赖这2个库;选择启动项下拉选择grpc_asr_demo运行测试demo;x64平台通x86平台操作一致。


## Linux平台下编译grpc协议在线语音识别Demo

请按以下命令编译grpc协议在线语音识别Demo
```
	cd general_sdk_grpc_asr
	//如果是x64将"cp lib/Linux/x86/libssasr.so /usr/local/lib"替换成"cp lib/Linux/x64/libssasr.so /usr/local/lib"
	cp lib/Linux/x86/libssasr.so /usr/local/lib
	cp include/* /usr/local/include
	mkdir build
	cp -R resource/audio ./build
	cp resource/roots.pem ./build 
	cd build
	cmake ..
	make
	./grpc_asr_demo 
```
