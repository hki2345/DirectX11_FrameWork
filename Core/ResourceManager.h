#pragma once
#include "SmartPtr.h"
#include "Stl_AID.h"

#include <unordered_map>
#include "MultiCritical.h"

class KCore;
class PathManager 
{
	friend KCore;

private:
	// �� ������ ����Ǵ� ��ü���
	// ���ӿ� ���� �����͵��� ������ ���� ���ε�
	// �� �̸��� �����ϴ� ���̴�.
	static std::wstring m_PathForder;
	static std::unordered_map<std::wstring, std::wstring> m_PathMap;

private:
	PathManager() {}
	~PathManager() {}

public:
	static const void		Create_FilePath(const wchar_t* _Key, const wchar_t* _FullPath);
	static const void		Create_ForderPath(const wchar_t* _Key, const wchar_t* _Folder);
	static const void		Create_ForderPath(const wchar_t* _Key);
	static const bool		IsFile(const wchar_t* _Path);
	static const wchar_t*	PathForder() { return m_PathForder.c_str();	}
	// static std::vector<std::wstring> FolderAllFile(const wchar_t* _Path);

	// string ��ȯ�� �ʿ���
	static const wchar_t*	Find_ForderPath(const wchar_t* _Key);
	static std::wstring		Find_ForderPathStr(const wchar_t* _Key);
	static std::wstring		Add_FolderFile(const wchar_t* _FolderKey, const wchar_t* _FileName);

private:
	static void Init();

public:
};

template<typename Res>
class ResourceManager
{
private:
	ResourceManager() {}
	virtual ~ResourceManager() = 0 {}

private:
	static std::unordered_map<std::wstring, KPtr<Res>> m_RSMap;
	static std::unordered_map<std::wstring, KPtr<Res>> m_MRSMap;

public:
	static std::vector<KPtr<Res>> All_SingleVec() 
	{
		std::vector<KPtr<Res>> vec;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndIter = m_RSMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			vec.push_back(StartIter->second);
		}

		return vec;
	}

	static std::list<KPtr<Res>> All_SingleList()
	{
		std::list<KPtr<Res>> ReturnList;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartMapIter = m_RSMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndMapIter = m_RSMap.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

	// ��� ���ҽ� ����
	static size_t All_Count() { return m_RSMap.size();	}


public:
	static KPtr<Res> Find(const wchar_t* _Name)
	{
		KPtr<Res> pTemp = Map_Find<KPtr<Res>>(m_RSMap, _Name);

		// �ܵ����� ������ ��Ƽ���� �޴´�.
		if (nullptr == pTemp)
		{
			return Map_Find<KPtr<Res>>(m_MRSMap, _Name);
		}
		return pTemp;
	}


#pragma region CREATE
	static KPtr<Res> Create(const wchar_t* _Name)
	{

		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRes->Create())
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1)
	{

		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	// ���� 2�� -- ���� Ÿ���� ��Ƽ�� ���� �� �����ϴ� ���̴�.
	template<typename V1, typename V2>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 4�� -- �̰� ����Ÿ���� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 5�� -- �׳� ���� ���߱�
	template<typename V1, typename V2, typename V3, typename V4, typename V5>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	// ���� 9�� -- �̰� �޽��� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}



	// ���� 10�� -- �̰� �޽��� ���� ������ �����ϴ� ���̴�. - ������
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static KPtr<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}
#pragma endregion

#pragma region LOAD
	static KPtr<Res> Load(const wchar_t* _Path, const bool& _Multi = false)
	{
		Res* NewRes = new Res();
		NewRes->Split_Path(_Path);
		NewRes->Name(NewRes->FileNameExt());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}


		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(NewRes->FileForder(), NewRes));

		return NewRes;
	}



	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, const bool& _Multi = false)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_Path);
		TempPath += _Name;

		Res* NewRes = new Res();
		NewRes->Name(_Name);
		NewRes->FileForder(_Path);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}


		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2>
	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1, V2 _2, const bool& _Multi = false)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_Path);
		TempPath += _Name;

		Res* NewRes = new Res();
		NewRes->Name(_Name);
		NewRes->FileForder(_Path);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}


			m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		
		return NewRes;
	}

#pragma endregion

	static KPtr<Res> Load_FromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_PathKey);
		TempPath += _FileName;

		Res* NewRes = new Res();
		NewRes->Name(_Key);
		NewRes->FileForder(_PathKey);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}
	template<typename V1>
	static KPtr<Res> Load_FromKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1)
	{
		std::wstring TempPath = PathManager::Find_ForderPath(_PathKey);
		TempPath += _FileName;

		Res* NewRes = new Res();
		NewRes->Name(_Key);
		NewRes->FileForder(_PathKey);
		NewRes->Split_Path(TempPath.c_str());
		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_RSMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}



public:
	// ��� ���ҽ��� �� �����´�
	static bool All_Load(const wchar_t* _Target);

private:
	// ���� ���� ������ �ҷ������� �Լ�
	static bool All_LoadOrigin(const wchar_t* _Target);

	// ��� ���ҽ��� ���������� ���� �Լ�
	static bool All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target);
	static bool IsDot(const std::wstring& _Value);

};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_RSMap;

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_MRSMap;