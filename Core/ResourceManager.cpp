#include "ResourceManager.h"

// #include <afxwin.h>
#include <iostream>
#include <io.h>

#include <Windows.h>
#include "KMacro.h"
#include "KImage.h"
#include "KSound.h"
#include "MultiCritical.h"	

std::unordered_map<std::wstring, std::wstring> PathManager::m_map_GamePath;
std::wstring PathManager::m_FilePath = {};


PathManager::PathManager()
{
	Init();
}


PathManager::~PathManager()
{
}

void PathManager::Init()
{
	// 薄仙 '戚'督析戚 姥疑鞠澗 是帖研 Data拭 隔嬢爽澗 敗呪
	wchar_t Collider2D_Data[1000];
	GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Collider2D_Data);
	m_FilePath = Collider2D_Data;

	// 覗稽詮闘 箸精 惟績戚 姥疑鞠澗 督析 巨刑塘軒 郊稽 是拭辞 段奄鉢研 廃陥.
	m_FilePath.replace(m_FilePath.find_last_of('\\', m_FilePath.size()),
		m_FilePath.size(), L"\\bin\\");
}

void PathManager::Create_FullPath(const wchar_t* _Name, const wchar_t* _FullPath)
{
	// 号嬢坪球 ->走幻 斗襟鍵陥.ぞぞ
	if (nullptr != Find_Path(_Name))
	{
		KASSERT(true);
	}

	m_map_GamePath.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Name, _FullPath));
}

void PathManager::Create_ForderPath(const wchar_t* _Name, const wchar_t* _Path)
{
	// 号嬢坪球 ->走幻 斗襟鍵陥.ぞぞ
	if (nullptr != Find_Path(_Name))
	{
		KASSERT(true);
	}

	std::wstring NewPath = m_FilePath + _Path + L"\\";
	m_map_GamePath.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Name, NewPath));
}

void PathManager::Create_ForderPath(const wchar_t* _Name)
{
	Create_ForderPath(_Name, _Name);
}

// 督析引 督析 誤聖 杯団 煽舌馬惟 背層陥.
std::wstring PathManager::Create_ForderFilePath(const wchar_t* _FolderName, const wchar_t* _Name)
{
	std::wstring NewPath = Find_Path(_FolderName);
	return NewPath += _Name;
}

const wchar_t* PathManager::Find_Path(const wchar_t* _Name)
{
	std::unordered_map<std::wstring, std::wstring>::iterator findIter = m_map_GamePath.find(_Name);

	if (m_map_GamePath.end() == findIter)
	{
		return nullptr;
	}

	return findIter->second.c_str();
}

bool PathManager::IsFile(const wchar_t* _Path)
{
	// 背雁 井稽拭 督析戚 赤澗走 伊紫 -> +1聖 馬澗 戚政澗
	// 蒸聖 獣 -1 赤聖 獣 0 鋼発馬奄 凶庚

	// 敗呪 昔切 -> MSDN
	// 00 糎仙 杯艦陥.
	// 02 床奄 映廃.
	// 04 石奄 映廃戚 赤柔艦陥.
	// 06 石奄 貢 床奄 映廃脊艦陥

	int R = _waccess_s(_Path, 0) + 1;

	if (0 == R)
	{
		return false;
	}

	return true;
}





/********************* All Resource Load **********************/
// 析曽税 荻企奄 敗呪 - 奴巴鹸聖 伯希拭 広生檎 鞠澗汽 益君奄殖 伯希亜 走榎 格巷 弦戚 床食閃赤嬢辞
// 戚係惟 荻霜 敗呪 幻級嬢辞 姥薄敗 しし
bool ResourceManager<KSound>::All_Load(const wchar_t* _Target)
{
	return ResourceManager<KSound>::All_LoadOrigin(_Target);
}
bool ResourceManager<KImage>::All_Load(const wchar_t* _Target)
{
	return ResourceManager<KImage>::All_LoadOrigin(_Target);
}


// 促杷 努什団幻 乞砧 閤聖 依戚奄 企庚拭
// afx亜 伯希拭 蓄亜鞠艦 蕉亜 戚雌背遭陥. b.b
template<typename KS>
bool ResourceManager<KS>::All_LoadOrigin(const wchar_t* _Target)
{
	struct _wfinddata_t FD;
	intptr_t Handle;

	std::wstring Path = PathManager::Get_FilePath();;
	Path += _Target;
	Path += L"\\*.*";

	Handle = _wfindfirst(Path.c_str(), &FD);

	if (Handle == -1)
	{
		return false;
	}


	// 費廃馬惟 dot引 double dot 猿走 姶走廃陥. 巷辞錘 橿汐 せせ
	std::wstring FTN = FD.name;	

	if (FTN == L".")
	{
		_wfindnext(Handle, &FD);
	}

	FTN = FD.name;

	if (FTN == L"..")
	{
		_wfindnext(Handle, &FD);
	}

	// 魚虞辞 handle聖 希鷺 翫 格袴猿走 廃 員拭辞 採斗 端滴敗しし	
	ResourceManager<KS>::All_LoadSub(Handle, FD, _Target);

	return true;
}


template<typename KS>
bool ResourceManager<KS>::All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target)
{
	std::list<_wfinddata_t > FolderMap;

	do
	{
		// 督析戚陥.
		if (true == ResourceManager<KS>::IsDot(_FD.name))
		{
			std::wstring FilePath = PathManager::Get_FilePath();
			FilePath += _Target;

			FilePath += L"\\";
			FilePath += _FD.name;

			wchar_t ArrDrive[128] = { 0, };
			wchar_t ArrFolder[128] = { 0, };
			wchar_t ArrFile[128] = { 0, };
			wchar_t ArrExist[128] = { 0, };

			// 督析 井稽研 金澗陥.
			_wsplitpath_s(FilePath.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExist);

			std::wstring Drive = ArrDrive;
			std::wstring Exi = ArrExist;
			std::wstring Name = ArrFile;
			std::wstring Folder = _Target;

			Name += Exi;

			// 紫遭引 製焦
			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" || Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" || Exi == L".bmp" ||
				Exi == L".mp3")
			{
				if (nullptr == PathManager::Find_Path(Folder.c_str()))
				{
					PathManager::Create_ForderPath(Folder.c_str());
				}
				ResourceManager<KS>::All_DataLoad(Folder.c_str(), Name.c_str());
			}
		}

		// 虹希陥 - 薦瑛
		else
		{
			std::wstring Temp;
			Temp = _Target;
			Temp += L"\\";
			Temp += _FD.name;

			ResourceManager<KS>::All_LoadOrigin( Temp.c_str());
		}
	} while (_wfindnext(_Handle, &_FD) == 0);


	return true;
}

template<typename KS>
bool ResourceManager<KS>::IsDot(const std::wstring& _Value)
{
	for (size_t i = 0; i < _Value.size(); i++)
	{
		if (_Value[i] == L'.')
		{
			return true;
		}
	}

	return false;
}