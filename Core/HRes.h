#pragma once
#include <string>
#include "Begin.h"

class HRes : public Begin
{

public:
	HRes();
	HRes(const HRes& _Other);
	~HRes();

private:
	std::wstring m_PathKey;
	std::wstring m_Path;
	std::wstring m_FullFileName;
	std::wstring m_FileName;
	std::wstring m_Ext;

public:
	const wchar_t* Path() 
	{
		return m_Path.c_str();
	}

	const wchar_t* FileName() 
	{
		return m_FileName.c_str();
	}

	const wchar_t* FullFileName()
	{
		return m_FullFileName.c_str();
	}

	const wchar_t* Ext() {
		return m_Ext.c_str();
	}

	bool IsExt(const wchar_t* _pExt) 
	{
		return m_Ext == _pExt;
	}

	void PathKey(const wchar_t* _pPathKey) {
		m_PathKey = _pPathKey;
	}

	const wchar_t* PathKey() {
		return m_PathKey.c_str();
	}

	void Path(const wchar_t* _pPath);

private:
	virtual bool Create();
	virtual bool Load();

};

