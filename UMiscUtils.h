#pragma once

#include <afxwin.h>
#include <string>
#include <fstream>

namespace UMiscUtils {

	CString GetRuntimeFilePath(TCHAR* filename, TCHAR* subFolder = NULL);
	CString GetExeFileVersion();
	CString GetProgramDataPath(TCHAR* subFolder, TCHAR* filename);

	std::string UnicodeToANSI(const std::wstring& wstr);
	std::wstring ANSIToUnicode(const std::string& str);

	void PrintThreadInfo(const std::string& tag);
	void EnableConsoleWindow();

	void WriteWaveFileHeader(std::fstream& file, UINT32 sampleRate, UINT16 numChannels, UINT16 bitsPerSample, UINT32 pcmDataSize = 0);

	bool RunExternalApp(TCHAR* appPath, TCHAR* appParams, bool bSync = false);
}