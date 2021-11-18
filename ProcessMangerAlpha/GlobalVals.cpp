#include "pch.h"
#include "GlobalVals.h"

CString sectionName;
// RVA=======>FOA
DWORD rvaTofoa(DWORD rva) {
	// TODO: �ڴ˴����ʵ�ִ���.
	// ��ȡDOSͷ
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// ��ȡNTͷ
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// ��ȡ�ļ�ͷ
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// ��ȡ��������
	DWORD secCount = fileHead->NumberOfSections;
	// ��ȡ��һ�����ε�ͷ
	PIMAGE_SECTION_HEADER secHead = IMAGE_FIRST_SECTION(ntHead);
	// �������е�����
	for (unsigned int i = 0; i < secCount; i++) {
		if (rva < secHead->VirtualAddress) {
			MessageBox(0, L"Out Of Range!", L"Tips", 0);
			return -1;
		}
		// �ж��Ƿ�������ĳ��������
		if (rva >= secHead->VirtualAddress && rva <= secHead->VirtualAddress + secHead->SizeOfRawData) {
			// rva - ������ʼƫ�� = foa - �������ļ��е�ƫ��
			DWORD foa = rva - secHead->VirtualAddress + secHead->PointerToRawData;
			// ��¼��������
			sectionName.Format(_T("%S"), secHead->Name);
			return foa;
		}
		secHead++;
	}
}

// FOA=========>RVA
DWORD foaTorva(DWORD foa) {
	// ��ȡDOSͷ
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// ��ȡNTͷ
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// ��ȡ�ļ�ͷ
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// ��ȡ��������
	DWORD secCount = fileHead->NumberOfSections;
	// ��ȡ��һ�����ε�ͷ
	PIMAGE_SECTION_HEADER secHead = IMAGE_FIRST_SECTION(ntHead);
	if (foa < secHead->PointerToRawData) {
		MessageBox(0, L"Out Of Range!", L"Tips", 0);
		return -1;
	}
	else {
		// �������е�����
		for (unsigned int i = 0; i < secCount; i++) {
			// �ж��Ƿ�������ĳ��������
			if (foa >= secHead->PointerToRawData && foa <= secHead->VirtualAddress + secHead->SizeOfRawData) {
				// rva - ������ʼƫ�� = foa - �������ļ��е�ƫ��
				DWORD rva = foa - secHead->PointerToRawData + secHead->VirtualAddress;
				// ��¼��������
				sectionName.Format(_T("%S"), secHead->Name);
				return rva;
			}
			secHead++;
		}
	}
}