#pragma once
#include "SmartPtr.h"
#include "Stl_AID.h"

#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include "MCRI.h"

class KCore;
class GamePath 
{
	friend KCore;

private:
	static std::wstring RootPath;
	static std::unordered_map<std::wstring, std::wstring> m_PathMap;

private:
	GamePath() {}
	~GamePath() {}

public:
	static void CreatePath(const wchar_t* _Key, const wchar_t* _FullPath);
	static void CreateRootPath(const wchar_t* _Key, const wchar_t* _Folder);
	static void CreateRootPath(const wchar_t* _Key);
	static std::wstring CreateFilePath(const wchar_t* _FolderKey, const wchar_t* _FileName);
	static const wchar_t* FindPath(const wchar_t* _Key);
	static std::wstring FindPath_ToString(const wchar_t* _Key);
	static bool IsFile(const wchar_t* _Path);
	// static std::vector<std::wstring> FolderAllFile(const wchar_t* _Path);

private:
	static void Init();

public:
	static const wchar_t* GetRootPath() { return RootPath.c_str();	}
};

template<typename Res>
class HResMgr
{
private:
	static std::unordered_map<std::wstring, KPtr<Res>> m_ResMap;
	static std::unordered_map<std::wstring, std::vector<KPtr<Res>>> m_MultiResMap;

public:
	static std::vector<KPtr<Res>> GetAllSingleRes() 
	{
		std::vector<KPtr<Res>> vec;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartIter = m_ResMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndIter = m_ResMap.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			vec.push_back(StartIter->second);
		}

		return vec;
	}

public:
	static size_t AllResCount()
	{
		return m_ResMap.size();
	}

	static std::list<KPtr<Res>> GetAllSingleResList() 
	{
		std::list<KPtr<Res>> ReturnList;

		std::unordered_map<std::wstring, KPtr<Res>>::iterator StartMapIter = m_ResMap.begin();
		std::unordered_map<std::wstring, KPtr<Res>>::iterator EndMapIter = m_ResMap.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

public:
	static KPtr<Res> Find(const wchar_t* _Name)
	{
		return Map_Find<KPtr<Res>>(m_ResMap, _Name);
	}

	static KPtr<Res> Create(const wchar_t* _Name)
	{

		Res* NewRes = new Res();
		NewRes->Set_Type();
		NewRes->Name(_Name);

		if (false == NewRes->Create())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


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

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	static KPtr<Res> Load(const wchar_t* _Path)
	{
		Res* NewRes = new Res();
		NewRes->Path(_Path);
		NewRes->Name(NewRes->FullFileName());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(NewRes->FullFileName(), NewRes));
		return NewRes;
	}



	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name)
	{
		std::wstring FindPath = GamePath::FindPath(_Path);
		FindPath += _Name;

		Res* NewRes = new Res();
		NewRes->Name(_Name);
		NewRes->PathKey(_Path);
		NewRes->Path(FindPath.c_str());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2>
	static KPtr<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1, V2 _2)
	{
		std::wstring FindPath = GamePath::FindPath(_Path);
		FindPath += _Name;

		Res* NewRes = new Res();
		NewRes->Name(_Name);
		NewRes->PathKey(_Path);
		NewRes->Path(FindPath.c_str());
		if (false == NewRes->Load(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	static KPtr<Res> LoadToKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName)
	{
		std::wstring FindPath = GamePath::FindPath(_PathKey);
		FindPath += _FileName;

		Res* NewRes = new Res();
		NewRes->Name(_Key);
		NewRes->PathKey(_PathKey);
		NewRes->Path(FindPath.c_str());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}
	template<typename V1>
	static KPtr<Res> LoadToKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1)
	{
		std::wstring FindPath = GamePath::FindPath(_PathKey);
		FindPath += _FileName;

		Res* NewRes = new Res();
		NewRes->Name(_Key);
		NewRes->PathKey(_PathKey);
		NewRes->Path(FindPath.c_str());
		if (false == NewRes->Load(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, KPtr<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}


private:
	HResMgr() {}
	virtual ~HResMgr() = 0 {}
};

template<typename Res>
std::unordered_map<std::wstring, KPtr<Res>> HResMgr<Res>::m_ResMap;