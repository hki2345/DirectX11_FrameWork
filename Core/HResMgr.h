#pragma once
#include "RefBase.h"
#include "StlHelperFunc.h"

#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include "MCRI.h"

class HCore;
class GamePath 
{
	friend HCore;

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
	static std::unordered_map<std::wstring, HPTR<Res>> m_ResMap;
	static std::unordered_map<std::wstring, std::vector<HPTR<Res>>> m_MultiResMap;

public:
	static std::vector<HPTR<Res>> GetAllSingleRes() 
	{
		std::vector<HPTR<Res>> vec;

		std::unordered_map<std::wstring, HPTR<Res>>::iterator StartIter = m_ResMap.begin();
		std::unordered_map<std::wstring, HPTR<Res>>::iterator EndIter = m_ResMap.end();

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

	static std::list<HPTR<Res>> GetAllSingleResList() 
	{
		std::list<HPTR<Res>> ReturnList;

		std::unordered_map<std::wstring, HPTR<Res>>::iterator StartMapIter = m_ResMap.begin();
		std::unordered_map<std::wstring, HPTR<Res>>::iterator EndMapIter = m_ResMap.end();

		for (; StartMapIter != EndMapIter; ++StartMapIter)
		{
			ReturnList.push_back(StartMapIter->second);
		}

		return ReturnList;
	}

public:
	static HPTR<Res> Find(const wchar_t* _Name)
	{
		return MapFind<HPTR<Res>>(m_ResMap, _Name);
	}

	static HPTR<Res> Create(const wchar_t* _Name)
	{

		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1>
	static HPTR<Res> Create(const wchar_t* _Name, V1 _1)
	{

		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	template<typename V1, typename V2>
	static HPTR<Res> Create(const wchar_t* _Name, V1 _1, V2 _2)
	{
		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4>
	static HPTR<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4)
	{
		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9>
	static HPTR<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9)
	{
		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	template<typename V1, typename V2, typename V3, typename V4, typename V5, typename V6, typename V7, typename V8, typename V9, typename V10>
	static HPTR<Res> Create(const wchar_t* _Name, V1 _1, V2 _2, V3 _3, V4 _4, V5 _5, V6 _6, V7 _7, V8 _8, V9 _9, V10 _10)
	{
		Res* NewRes = new Res();
		NewRes->TypeSetting();
		NewRes->Name(_Name);

		if (false == NewRes->Create(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10))
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}


	static HPTR<Res> Load(const wchar_t* _Path)
	{
		Res* NewRes = new Res();
		NewRes->Path(_Path);
		NewRes->Name(NewRes->FullFileName());
		if (false == NewRes->Load())
		{
			delete NewRes;
			return nullptr;
		}

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(NewRes->FullFileName(), NewRes));
		return NewRes;
	}



	static HPTR<Res> Load(const wchar_t* _Path, const wchar_t* _Name)
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

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	template<typename V1, typename V2>
	static HPTR<Res> Load(const wchar_t* _Path, const wchar_t* _Name, V1 _1, V2 _2)
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

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Name, NewRes));
		return NewRes;
	}

	static HPTR<Res> LoadToKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName)
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

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}
	template<typename V1>
	static HPTR<Res> LoadToKey(const wchar_t* _Key, const wchar_t* _PathKey, const wchar_t* _FileName, V1 _1)
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

		m_ResMap.insert(std::unordered_map<std::wstring, HPTR<Res>>::value_type(_Key, NewRes));
		return NewRes;
	}


private:
	HResMgr() {}
	virtual ~HResMgr() = 0 {}
};

template<typename Res>
std::unordered_map<std::wstring, HPTR<Res>> HResMgr<Res>::m_ResMap;