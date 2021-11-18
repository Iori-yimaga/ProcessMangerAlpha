#include "pch.h"
#include "GlobalVals.h"

CString sectionName;
// RVA=======>FOA
DWORD rvaTofoa(DWORD rva) {
	// TODO: 在此处添加实现代码.
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取文件头
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// 获取区段数量
	DWORD secCount = fileHead->NumberOfSections;
	// 获取第一个区段的头
	PIMAGE_SECTION_HEADER secHead = IMAGE_FIRST_SECTION(ntHead);
	// 遍历所有的区段
	for (unsigned int i = 0; i < secCount; i++) {
		if (rva < secHead->VirtualAddress) {
			MessageBox(0, L"Out Of Range!", L"Tips", 0);
			return -1;
		}
		// 判断是否落在了某个区段内
		if (rva >= secHead->VirtualAddress && rva <= secHead->VirtualAddress + secHead->SizeOfRawData) {
			// rva - 区段起始偏移 = foa - 区段在文件中的偏移
			DWORD foa = rva - secHead->VirtualAddress + secHead->PointerToRawData;
			// 记录区段名字
			sectionName.Format(_T("%S"), secHead->Name);
			return foa;
		}
		secHead++;
	}
}

// FOA=========>RVA
DWORD foaTorva(DWORD foa) {
	// 获取DOS头
	dosHead = PIMAGE_DOS_HEADER(fileBuff);
	// 获取NT头
	ntHead = PIMAGE_NT_HEADERS(dosHead->e_lfanew + (LONG)fileBuff);
	// 获取文件头
	fileHead = PIMAGE_FILE_HEADER(&ntHead->FileHeader);
	// 获取区段数量
	DWORD secCount = fileHead->NumberOfSections;
	// 获取第一个区段的头
	PIMAGE_SECTION_HEADER secHead = IMAGE_FIRST_SECTION(ntHead);
	if (foa < secHead->PointerToRawData) {
		MessageBox(0, L"Out Of Range!", L"Tips", 0);
		return -1;
	}
	else {
		// 遍历所有的区段
		for (unsigned int i = 0; i < secCount; i++) {
			// 判断是否落在了某个区段内
			if (foa >= secHead->PointerToRawData && foa <= secHead->VirtualAddress + secHead->SizeOfRawData) {
				// rva - 区段起始偏移 = foa - 区段在文件中的偏移
				DWORD rva = foa - secHead->PointerToRawData + secHead->VirtualAddress;
				// 记录区段名字
				sectionName.Format(_T("%S"), secHead->Name);
				return rva;
			}
			secHead++;
		}
	}
}