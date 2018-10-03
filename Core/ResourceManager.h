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
	// 이 폴더가 실행되는 파일경로
	// 게임에 쓰일 데이터들을 폴더에 넣을 것인데
	// 그 이름을 저장하는 맵이다.
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


template<typename KS>
class ResourceManager
{
private:
	static std::unordered_map<std::wstring, KPtr<KS>> m_map_Resource;
	static std::unordered_map<std::wstring, KPtr<KS>> m_Multi_ResMap;

	// ㄷㅣㄹ레마 -> 초기화하면서 여러가지 변수를 모두 초기화 할수있게 할 것이냐//
	// 그러려면 과연 노가다 하드코딩을 해야하는데 다른 방법이 있겠느냐


public:
	static std::list<KPtr<KS>> All_SingleResList()
	{
		std::list<KPtr<KS>> ReturnList;

		std::unordered_map<std::wstring, KPtr<KS>>::iterator StartMapIter = m_map_Resource.begin();
		std::unordered_map<std::wstring, KPtr<KS>>::iterator EndMapIter = m_map_Resource.end();

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


	static KPtr<KS> Find(const wchar_t* _Name)
	{
		if (nullptr == Map_Find<KPtr<KS>>(m_map_Resource, _Name))
		{
			return Map_Find<KPtr<KS>>(m_Multi_ResMap, _Name);
		}
		return Map_Find<KPtr<KS>>(m_map_Resource, _Name);
	}

	static KPtr<KS> Create(const wchar_t* _Name)
	{
		KS* NewRS = new KS();
		NewRS->set_type();
		NewRS->name(_Name);

		// 여기서 오버로딩이 안되거나 오류가 나면 false를 받겠다.
		if (false == NewRS->Create())
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert (std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	template<typename V1>
	static KPtr<KS> Create(const wchar_t* _Name, V1 _1)
	{
		KS* NewRS = new KS();
		NewRS->set_type();
		NewRS->name(_Name);

		// 여기서 오버로딩이 안되거나 오류가 나면 false를 받겠다.
		if (false == NewRS->Create(_1))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}


	// 인자 9개 -- 이건 메쉬를 저격하는 것이다.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static KPtr<KS> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		KS* NewRS = new KS();
		NewRS->set_type();
		NewRS->name(_Name);

		// 여기서 오버로딩이 안되거나 오류가 나면 false를 받겠다.
		if (false == NewRS->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}


	// 인자 10개 -- 이건 메쉬의 라인 지정을 저격하는 것이다.
	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static KPtr<KS> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		KS* NewRS = new KS();
		NewRS->set_type();
		NewRS->name(_Name);

		// 여기서 오버로딩이 안되거나 오류가 나면 false를 받겠다.
		if (false == NewRS->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRS;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	static void Delete(const wchar_t* _Name)
	{
		std::unordered_map<std::wstring, KPtr<KS>>::iterator FI = m_map_Resource.find(_Name);
		m_map_Resource.erase(FI);
	}

private:
	// 멀티 리소스 나중을 위한 - 인자 하나도 없는 경우
	// 사운드 -> 로드시 하나도 어
	static KPtr<KS> All_DataLoad(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		KS* NewRS = new KS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load())
		{
			delete NewRS;
			return nullptr;
		}

	
		m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}

	// 멀티 리소스 - 이미지 인자 2개
	template<typename T1, typename T2>
	static KPtr<KS> All_DataLoad(const wchar_t* _Path, const wchar_t* _Name, T1 _V1, T2 _V2)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		KS* NewRS = new KS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load(_V1, _V2, true))
		{
			delete NewRS;
			return nullptr;
		}

		m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		return NewRS;
	}


public:
	static KPtr<KS> Load(const wchar_t* _Path, const wchar_t* _Name, const bool& _Multi = false)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		KS* NewRS = new KS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load())
		{
			delete NewRS;
			return nullptr;
		}

		
		// 데이터 중복방지를 위한 가정문 
		if (false == _Multi)
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		}
		else
		{
			m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		}

		return NewRS;
	}

	template<typename T1, typename T2>
	static KPtr<KS> Load(const wchar_t* _Path, const wchar_t* _Name, T1 _V1, T2 _V2, const bool& _Multi = false)
	{
		std::wstring thisPath = PathManager::Find_Path(_Path);
		thisPath += _Name;

		KS* NewRS = new KS();
		NewRS->name(_Name);
		NewRS->forder_path(_Path);
		NewRS->path(thisPath.c_str());

		if (false == NewRS->Load(_V1, _V2, _Multi))
		{
			delete NewRS;
			return nullptr;
		}

		// 데이터 중복방지를 위한 가정문 
		if (false == _Multi)
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		}
		else
		{
			m_Multi_ResMap.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Name, NewRS));
		}

		return NewRS;
	}

	// 맵의 Key를 이용해 찾은 뒤 (사실 Key가 패스로 저장됌)
	// 이 키를 바탕으로 로드를 생성함
	static KPtr<KS> Load_FromKey(const wchar_t* _Key, const wchar_t*  _Path_Key, const wchar_t* _FileName)
	{
		std::wstring NewFind = PathManager::Find_Path(_Path_Key);
		NewFind += _FileName;

		KS* NewRes = new KS();
		NewRes->name(_Key);
		NewRS->forder_path(_Path_Key);
		NewRes->path(NewFind.c_str());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Key, NewRS));
		return NewRes;
	}

	template<typename T1>
	static KPtr<KS> Load_FromKey(const wchar_t* _Key, const wchar_t*  _Path_Key, const wchar_t* _FileName, T1 _V1)
	{
		std::wstring NewFind = PathManager::Find_Path(_Path_Key);
		NewFind += _FileName;

		KS* NewRes = new KS();
		NewRes->name(_Key);
		NewRes->forder_path(_Path_Key);
		NewRes->path(NewFind.c_str());

		if (false == NewRes->Load(_V1))
		{
			delete NewRes;
			return nullptr;
		}

		m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(_Key, NewRes));
		return NewRes;
	}


	// 맵의 Key를 이용해 찾은 뒤 (사실 Key가 패스로 저장됌)
	// 이 키를 바탕으로 로드를 생성함
	static KPtr<KS> Load_FullPath(const wchar_t* _Path)
	{
		KS* NewRes = new KS();
		NewRes->path(_Path);
		NewRes->name(NewRes->path());

		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		std::unordered_map<std::wstring, KPtr<KS>>::iterator findIter = m_map_Resource.find(NewRes->name_exist());

		// 난 아예 여기서 있을 경우 하지 않는 것으로 수저ㅏㅇ함
		if (findIter == m_map_Resource.end())
		{
			m_map_Resource.insert(std::unordered_map<std::wstring, KPtr<KS>>::value_type(NewRes->name_exist(), NewRes));
		}

		return NewRes;
	}



public:
	// 모든 리소스를 다 가져온다
	static bool All_Load(const wchar_t* _Target);

private:
	// 실제 인제 파일을 불러오려는 함수
	static bool All_LoadOrigin(const wchar_t* _Target);

	// 모든 리소스를 가져오려는 제귀 함수
	static bool All_LoadSub(const intptr_t& _Handle, _wfinddata_t& _FD, const wchar_t* _Target);
	static bool IsDot(const std::wstring& _Value);


public:
	ResourceManager() {};
	~ResourceManager() {};
};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_map_Resource;
template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> ResourceManager<Res>::m_Multi_ResMap;