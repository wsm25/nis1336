# 日程管理软件

是 SJTU NIS1336 课程大作业，详情请参见[设计文档](./DESIGN.md)  

github仓库：<https://github.com/wsm25/nis1336>  

## 文件结构 
`CMakeLists.txt`: cmake配置文件  
`.gitignore`: git配置文件  

`.gitmodules`: 引入HASH模块  
`Hash/`: 使用的HASH加密模块  

`include/`: .h头文件  
`src/`: .cc源代码  
`test.sh`: 测试程序

`static/`: 设计文档.md使用的图片  
`DESIGN.md`: 设计文档  
`README.md`: 本文件  

## 编译
由于引用了 submodule，在初次 clone 后需要 `git submodule update --init --recursive`

已测试平台
- Linux, gcc 11 + gmake
- Windows, gcc 13 (x86_64-posix-seh-rev0, mingw) + gmake
- Windows, Visual Studio 2022
- Windows 11, gcc (Rev3, Built by MSYS2 project) 14.1.0 + cmake