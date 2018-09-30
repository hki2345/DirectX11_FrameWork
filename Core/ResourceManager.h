#pragma once
#include "SmartPtr.h"
#include "Stl_Assistor.h"
#include <unordered_map>
#include <list>



class KCore;
class PathManager
{
public:
	friend KCore;

private:
	// �� ������ ����Ǵ� ���ϰ��
	// ���ӿ� ���� �����͵��� ������ ���� ���ε�
	// �� �̸��� �����ϴ� ���̴�.
	static std::wstring m_FilePath;
	static std::unordered_map<std::wstring, std::wstring> m_map_GamePath;

private:
	PathManager();
	~PathManager();

private:
	static void Init();

public:
	static void Create_FullPath(const wchar_t* _Name, const wchar_t* _FullPath);
	static void Create_ForderPath(const wchar_t* _Name, const wchar_t* _Path);
	static void Create_ForderPath(const wchar_t* _Name);
	static std::wstring Create_ForderFilePath(const wchar_t* _FolderName, const wchar_t* _Name);
	static const wchar_t* Find_Path(const wchar_t* _Name);
	static bool IsFile(const wchar_t* _Path);

	static const wchar_t* Get_FilePath()
	{
		return m_FilePath.c_str();
	}

};


template<typename RS>
class ResourceManager
{
private:
	static std::unordered_map<std::wstring, KPtr<RS>> m_map_Resource;

	static std::unordered_map<std::wstring, KPtr<RS>> m_Multi_ResMap;

	// ���Ӥ����� -> �ʱ�ȭ�ϸ鼭 �������� ������ ��� �ʱ�ȭ �Ҽ��ְ� �� ���̳�//
	// �׷����� ���� �밡�� �ϵ��ڵ��� �ؾ��ϴµ� �ٸ� ����� �ְڴ���



private:
	// ���͵������� �Լ�
	static bool All_Load_Sub(const wchar_t* _Value, const wchar_t* _Target);

public:
	static std::list<KPtr<RS>> All_SingleResList()
	{
		std::list<KPtr<RS>> ReturnList;

		std::unordered_map<std::wstring, KPtr<RS>>::iterator StartMapIter = m_map_Resource.begin();
		std::unordered_map<std::wstring, KPtr<RS>>::iterator EndMapIter = m_map_Resource.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

	static size_t All_Res_Count()
	{
		return m_map_Resource.size();
	}

	// ����� ��� ���� �޾ƿ����� �Լ�
	static bool All_Image_Load(const wchar_t* _Target);
	static bool All_Sound_Load(const wchar_t* _Target);

	static KPtr<RS> Find(const wchar_t* _Name)
	{
		if (nullptr == Map_Find<KPtr<RS>>(m_map_Resource, _Name))
		{
			return Map_Find<KPtr<RS>>(m_Multi_ResMap, _Name);
		}
		return Map_Find<KPtr<RS>>(m_map_Resource, _Name);
	}

	static KPtr<RS> Create(const wchar_t* _Name)
	{
		RS* NewRS = new RS();
		NewRS->set_type();
		NewRS->name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRS->Create())
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert (std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	template<typename V1>
	static KPtr<RS> Create(const wchar_t* _Name, V1 _1)
	{
		RS* NewRS = new RS();
		NewRS->set_type();
		NewRS->name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRS->Create(_1))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}


	// ���� 9�� -- �̰� �޽��� �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static KPtr<RS> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		RS* NewRS = new RS();
		NewRS->set_type();
		NewRS->name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRS->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}


	// ���� 10�� -- �̰� �޽��� ���� ������ �����ϴ� ���̴�.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static KPtr<RS> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		RS* NewRS = new RS();
		NewRS->set_type();
		NewRS->name(_Name);

		// ���⼭ �����ε��� �ȵǰų� ������ ���� false�� �ްڴ�.
		if (false == NewRS->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	static void Delete(const wchar_t* _Name)
	{
		std::unordered_map<std::wstring, KPtr<RS>>::iterator FI = m_map_Resource.find(_Name);
		m_map_Resource.erase(FI);
	}

private:
	// ��Ƽ ���ҽ� ������ ���� - ���� �ϳ��� ���� ���
	// ���� -> �ε�� �ϳ��� ��
	static KPtr<RS> All_DataLoad(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		RS* NewRS = new RS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load())
		{
			delete NewRS;
			return nullptr;
		}

	
		m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	// ��Ƽ ���ҽ� - �̹��� ���� 2��
	template<typename T1, typename T2>
	static KPtr<RS> All_DataLoad(const wchar_t* _Path, const wchar_t* _Name, T1 _V1, T2 _V2)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		RS* NewRS = new RS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load(_V1, _V2, true))
		{
			delete NewRS;
			return nullptr;
		}

		m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		return NewRS;
	}


public:
	static KPtr<RS> Load(const wchar_t* _Path, const wchar_t* _Name, const bool& _Multi = false)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		RS* NewRS = new RS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load())
		{
			delete NewRS;
			return nullptr;
		}

		
		// ������ �ߺ������� ���� ������ 
		if (false == _Multi)
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		}
		else
		{
			m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		}

		return NewRS;
	}

	template<typename T1, typename T2>
	static KPtr<RS> Load(const wchar_t* _Path, const wchar_t* _Name, T1 _V1, T2 _V2, const bool& _Multi = false)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		RS* NewRS = new RS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load(_V1, _V2, _Multi))
		{
			delete NewRS;
			return nullptr;
		}

		// ������ �ߺ������� ���� ������ 
		if (false == _Multi)
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		}
		else
		{
			m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Name, NewRS));
		}

		return NewRS;
	}

	// ���� Key�� �̿��� ã�� �� (��� Key�� �н��� �����)
	// �� Ű�� �������� �ε带 ������
	static KPtr<RS> Load_FromKey(const wchar_t* _Key, const wchar_t*  _Path_Key, const wchar_t* _FileName)
	{
		std::wstring NewFind = PathManager::Find_Path(_Path_Key);
		NewFind += _FileName;

		RS* NewRes = new RS();
		NewRes->name(_Key);
		NewRS->forder_path(_Path_Key);
		NewRes->path(NewFind.c_str());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Key, NewRS));
		return NewRes;
	}

	template<typename T1>
	static KPtr<RS> Load_FromKey(const wchar_t* _Key, const wchar_t*  _Path_Key, const wchar_t* _FileName, T1 _V1)
	{
		std::wstring NewFind = PathManager::Find_Path(_Path_Key);
		NewFind += _FileName;

		RS* NewRes = new RS();
		NewRes->name(_Key);
		NewRes->forder_path(_Path_Key);
		NewRes->path(NewFind.c_str());

		if (false == NewRes->Load(_V1))
		{
			delete NewRes;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(_Key, NewRes));
		return NewRes;
	}


	// ���� Key�� �̿��� ã�� �� (��� Key�� �н��� �����)
	// �� Ű�� �������� �ε带 ������
	static KPtr<RS> Load_FullPath(const wchar_t* _Path)
	{
		RS* NewRes = new RS();
		NewRes->path(_Path);
		NewRes->name(NewRes->path());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		std::unordered_map<std::wstring, KPtr<RS>>::iterator findIter = m_map_Resource.find(NewRes->name_exist());

		// �� �ƿ� ���⼭ ���� ��� ���� �ʴ� ������ ����������
		if (findIter == m_map_Resource.end())
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<RS>>::value_type(NewRes->name_exist(), NewRes));
		}

		return NewRes;
	}

public:
	ResourceManager() {};
	~ResourceManager() {};
};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_map_Resource;
template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_Multi_ResMap;