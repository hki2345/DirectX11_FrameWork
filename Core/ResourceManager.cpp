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
	// 현재 '이'파일이 구동되는 위치를 Data에 넣어주는 함수
	wchar_t Collider2D_Data[1000];
	GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Collider2D_Data);
	m_FilePath = Collider2D_Data;

	// 프로젝트 혹은 게임이 구동되는 파일 디렉토리 바로 위에서 초기화를 한다.
	m_FilePath.replace(m_FilePath.find_last_of('\\', m_FilePath.size()),
		m_FilePath.size(), L"\\bin\\");
}

void PathManager::Create_FullPath(const wchar_t* _Name, const wchar_t* _FullPath)
{
	// 방어코드 ->지만 터뜨린다.ㅎㅎ
	if (nullptr != Find_Path(_Name))
	{
		KASSERT(true);
	}

	m_map_GamePath.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Name, _FullPath));
}

void PathManager::Create_ForderPath(const wchar_t* _Name, const wchar_t* _Path)
{
	// 방어코드 ->지만 터뜨린다.ㅎㅎ
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

// 파일과 파일 명을 합쳐 저장하게 해준다.
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
	// 해당 경로에 파일이 있는지 검사 -> +1을 하는 이유는
	// 없을 시 -1 있을 시 0 반환하기 때문

	// 함수 인자 -> MSDN
	// 00 존재 합니다.
	// 02 쓰기 권한.
	// 04 읽기 권한이 있습니다.
	// 06 읽기 및 쓰기 권한입니다

	int R = _waccess_s(_Path, 0) + 1;

	if (0 == R)
	{
		return false;
	}

	return true;
}


// 짜피 텍스쳐만 모두 받을 것이기 대문에
// afx가 헤더에 추가되니 애가 이상해진다. b.b

bool ResourceManager<KImage>::All_Image_Load(const wchar_t* _Target)
{	
	std::wstring NewString = PathManager::Get_FilePath();
	NewString += _Target;
	
	if (false == ResourceManager<KImage>::All_Load_Sub(NewString.c_str(), _Target))
	{
		return false; 
	}
	
	return true;
}


bool ResourceManager<KSound>::All_Sound_Load(const wchar_t* _Target)
{
	std::wstring NewString = PathManager::Get_FilePath();
	NewString += _Target;

	if (false == ResourceManager<KSound>::All_Load_Sub(NewString.c_str(), _Target))
	{
		return false;
	}

	return true;
}





template<typename KS>
bool ResourceManager<KS>::All_Load_Sub(const wchar_t* _Value, const wchar_t* _Target)
{
	struct _wfinddata_t TempFD;
	intptr_t Handle;

	std::wstring Path = PathManager::Get_FilePath();;
	Path += _Target;
	Path += L"\\*.*";

	Handle = _wfindfirst(Path.c_str(), &TempFD);

	if (Handle == -1)
	{
		return false;
	}



	std::wstring FTN = TempFD.name;

	

	if (FTN == L".")
	{
		_wfindnext(Handle, &TempFD);
	}

	FTN = TempFD.name;

	if (FTN == L"..")
	{
		_wfindnext(Handle, &TempFD);
	}


	// 이 이후가 FD에는 폴더 이름이 들어간다.
	std::wstring NextFolder = _Target;
	NextFolder += L"\\";
	NextFolder += TempFD.name;

	do
	{
		bool Check = ResourceManager<KS>::IsDot(FTN);
		std::wstring arr = PathManager::Get_FilePath();;
		arr += NextFolder;

		arr += L"\\";
		arr += TempFD.name;

		wchar_t ArrDrive[128] = {0, };
		wchar_t ArrFolder[128] = {0, };
		wchar_t ArrFile[128] = {0, };
		wchar_t ArrExist[128] = {0, };

		// 파일 경로를 뜯는다.
		_wsplitpath_s(arr.c_str(), ArrDrive, ArrFolder, ArrFile, ArrExist);

		std::wstring Drive = ArrDrive;
		std::wstring Exi = ArrExist;
		std::wstring Name = ArrFile;
		std::wstring Folder = _Target;

		Name += Exi;

		// 사진과 음악
		if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" || Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" || Exi == L".bmp" ||
			Exi == L".mp3")
		{
			if (nullptr == PathManager::Find_Path(Folder.c_str()))
			{
				PathManager::Create_ForderPath(Folder.c_str());
			}
			ResourceManager<KS>::All_DataLoad(Folder.c_str(), Name.c_str());
		}
	} while (_wfindnext(Handle, &TempFD) == 0);

	std::wstring Tes;
	All_Load_Sub(Tes.c_str(), NextFolder.c_str());
	_findclose(Handle);

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