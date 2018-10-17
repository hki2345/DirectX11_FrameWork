#include "HMACRO.h"
#include "HResMgr.h"
#include "MCRI.h"

//#include <WinSDKVer.h>
//#define _WIN32_WINNT _WIN32_WINNT_VISTA  
//#define _AFXDLL
//#include <afxwin.h>

#include <Windows.h>

std::unordered_map<std::wstring, std::wstring> GamePath::m_PathMap;
std::wstring GamePath::RootPath;

void GamePath::Init()
{
	// CFileFind NewFind;

	wchar_t Arr[1000];
	GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Arr);
	RootPath = Arr;

	size_t CutCount = RootPath.find_last_of('\\', RootPath.size());
	RootPath.replace(CutCount, RootPath.size(), L"\\BIN\\");
}

void GamePath::CreatePath(const wchar_t* _Key, const wchar_t* _FullPath) {
	if (nullptr != FindPath(_Key))
	{
		TASSERT(true);
	}

	m_PathMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, _FullPath));
}

void GamePath::CreateRootPath(const wchar_t* _Key, const wchar_t* _Folder)
{
	if (nullptr != FindPath(_Key))
	{
		TASSERT(true);
	}

	std::wstring NewPath = RootPath + _Folder + L"\\";
	m_PathMap.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Key, NewPath));
}

void GamePath::CreateRootPath(const wchar_t* _Key) {
	CreateRootPath(_Key, _Key);
}

std::wstring GamePath::CreateFilePath(const wchar_t* _FolderKey, const wchar_t* _FileName)
{
	std::wstring Path = FindPath(_FolderKey);
	return Path += _FileName;
}

const wchar_t* GamePath::FindPath(const wchar_t* _Key)
{

	std::unordered_map<std::wstring, std::wstring>::iterator PathIter = m_PathMap.find(_Key);

	if (PathIter == m_PathMap.end())
	{
		return nullptr;
	}

	return PathIter->second.c_str();
}

bool GamePath::IsFile(const wchar_t* _Path)
{
	int R = _waccess_s(_Path, 0) + 1;

	if (0 == R)
	{
		return false;
	}

	return true;
}
