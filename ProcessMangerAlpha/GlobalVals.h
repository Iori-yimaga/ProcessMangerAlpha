#pragma once
// 全局变量
extern CString g_ProcID;
extern CString g_ThreadID;
extern int nRow;

extern char* fileBuff;
// 获取DOS头
extern PIMAGE_DOS_HEADER dosHead;
// 获取NT头
extern PIMAGE_NT_HEADERS ntHead;
// 获取文件头
extern PIMAGE_FILE_HEADER fileHead;
// 获取扩展头
extern PIMAGE_OPTIONAL_HEADER optHead;
// RVA=======>FOA
extern DWORD rvaTofoa(DWORD rva);
// FOA=========>RVA
extern DWORD foaTorva(DWORD foa);
// 定位此地址落在了哪一个区段
extern CString sectionName;