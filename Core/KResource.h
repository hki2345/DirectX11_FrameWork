#pragma once
#include <string>
#include "Begin.h"

class KResource : public Begin
{
public:
	KResource();
	KResource(const KResource& _Other);
	~KResource();


private:
	std::wstring m_sForderPath;
	std::wstring m_sPath;

	std::wstring m_sNameExi;
	std::wstring m_sFileName;
	std::wstring m_Exist;

public:
	const wchar_t* path()				{ return m_sPath.c_str(); }
	const wchar_t* file_name()			{ return m_sFileName.c_str(); }
	const wchar_t* exist()				{ return m_Exist.c_str(); }
	const wchar_t* name_exist()			{ return m_sNameExi.c_str(); }
	const wchar_t* forder_path()		{ return m_sForderPath.c_str(); }
	void forder_path(const wchar_t* _pforderPath)
	{
		m_sForderPath = _pforderPath;
	}


	void path(const wchar_t* _Value);

	bool is_Exist(const wchar_t* _Value)
	{
		return m_Exist == _Value;
	}

private:
	virtual bool Create();
	virtual bool Load();

};

