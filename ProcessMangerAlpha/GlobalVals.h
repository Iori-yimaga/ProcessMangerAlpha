#pragma once
// ȫ�ֱ���
extern CString g_ProcID;
extern CString g_ThreadID;
extern int nRow;

extern char* fileBuff;
// ��ȡDOSͷ
extern PIMAGE_DOS_HEADER dosHead;
// ��ȡNTͷ
extern PIMAGE_NT_HEADERS ntHead;
// ��ȡ�ļ�ͷ
extern PIMAGE_FILE_HEADER fileHead;
// ��ȡ��չͷ
extern PIMAGE_OPTIONAL_HEADER optHead;
// RVA=======>FOA
extern DWORD rvaTofoa(DWORD rva);
// FOA=========>RVA
extern DWORD foaTorva(DWORD foa);
// ��λ�˵�ַ��������һ������
extern CString sectionName;