#include "ResourceManager.h"
#include <afxwin.h>
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
	// ���� '��'������ �����Ǵ� ��ġ�� Data�� �־��ִ� �Լ�
	wchar_t Collider2D_Data[1000];
	GetCurrentDirectoryW(sizeof(wchar_t) * 1000, Collider2D_Data);
	m_FilePath = Collider2D_Data;

	// ������Ʈ Ȥ�� ������ �����Ǵ� ���� ���丮 �ٷ� ������ �ʱ�ȭ�� �Ѵ�.
	m_FilePath.replace(m_FilePath.find_last_of('\\', m_FilePath.size()),
		m_FilePath.size(), L"\\bin\\");
}

void PathManager::Create_FullPath(const wchar_t* _Name, const wchar_t* _FullPath)
{
	// ����ڵ� ->���� �Ͷ߸���.����
	if (nullptr != Find_Path(_Name))
	{
		KASSERT(true);
	}

	m_map_GamePath.insert(std::unordered_map<std::wstring, std::wstring>::value_type(_Name, _FullPath));
}

void PathManager::Create_ForderPath(const wchar_t* _Name, const wchar_t* _Path)
{
	// ����ڵ� ->���� �Ͷ߸���.����
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

// ���ϰ� ���� ���� ���� �����ϰ� ���ش�.
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
	// �ش� ��ο� ������ �ִ��� �˻� -> +1�� �ϴ� ������
	// ���� �� -1 ���� �� 0 ��ȯ�ϱ� ����

	// �Լ� ���� -> MSDN
	// 00 ���� �մϴ�.
	// 02 ���� ����.
	// 04 �б� ������ �ֽ��ϴ�.
	// 06 �б� �� ���� �����Դϴ�

	int R = _waccess_s(_Path, 0) + 1;

	if (0 == R)
	{
		return false;
	}

	return true;
}


// ¥�� �ؽ��ĸ� ��� ���� ���̱� �빮��
// afx�� ����� �߰��Ǵ� �ְ� �̻�������. b.b
bool ResourceManager<KImage>::All_Image_Load(const wchar_t* _Target)
{
	CFileFind	Finder;
	bool		thisFind = false;

	CString NewString = PathManager::Get_FilePath();
	NewString += _Target;

	if (false == ResourceManager<KImage>::All_Load_Sub(NewString, _Target))
	{
		return false; 
	}

	return true;
}


bool ResourceManager<KSound>::All_Sound_Load(const wchar_t* _Target)
{
	CFileFind	Finder;
	bool		thisFind = false;

	CString NewString = PathManager::Get_FilePath();
	NewString += _Target;

	if (false == ResourceManager<KSound>::All_Load_Sub(NewString, _Target))
	{
		return false;
	}

	return true;
}


template<typename KS>
bool ResourceManager<KS>::All_Load_Sub(const wchar_t* _Value, const wchar_t* _Target)
{
	CFileFind	Finder;
	BOOL		thisFind = false;

	CString NewString = _Value;
	NewString += L"\\*.*";

	thisFind = Finder.FindFile(NewString);
	while (TRUE == thisFind)
	{
		thisFind = Finder.FindNextFileW();
		if (Finder.IsDots())
		{
			continue;
		}

		// ������ ã�Ҵ�.
		else
		{
			CString arr;
			arr = Finder.GetFilePath();

			wchar_t ArrDrive[128] = {0, };
			wchar_t ArrFolder[128] = {0, };
			wchar_t ArrFile[128] = {0, };
			wchar_t ArrExist[128] = {0, };

			// ���� ��θ� ��´�.
			_wsplitpath_s(arr, ArrDrive, ArrFolder, ArrFile, ArrExist);

			std::wstring Drive = ArrDrive;
			std::wstring Exi = ArrExist;
			std::wstring Name = ArrFile;
			std::wstring Folder = ArrFolder;

			Name += Exi;

			size_t a = Folder.find(_Target);
			Folder.clear();

			for (size_t i = a; i < 128; i++)
			{
				Folder += ArrFolder[i];
			}

			Folder.replace(Folder.find_last_of('\\', Folder.size()),
				Folder.size(), L"");

			// ������ ����
			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" || Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg" ||
				Exi == L".mp3")
			{
				if (nullptr == PathManager::Find_Path(Folder.c_str()))
				{
					PathManager::Create_ForderPath(Folder.c_str());
				}
				ResourceManager<KS>::All_DataLoad(Folder.c_str(), Name.c_str());
			}
			All_Load_Sub(arr, _Target);
		}
	}

	return true;
}