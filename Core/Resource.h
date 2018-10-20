#pragma once
#include <string>
#include "Begin.h"

class Resource : public Begin
{

public:
	Resource();
	Resource(const Resource& _Other);
	~Resource();

private:
	std::wstring m_AllPath;

	std::wstring m_FileForder;
	std::wstring m_FileNameExi;
	std::wstring m_FileName;
	std::wstring m_FileExt;

public:
	const wchar_t* AllPath() { return m_AllPath.c_str(); }
	const wchar_t* FileName() { return m_FileName.c_str(); }
	const wchar_t* FileNameExt() { return m_FileNameExi.c_str(); }
	const wchar_t* FileExt() { return m_FileExt.c_str(); }
	const wchar_t* FileForder() { return m_FileForder.c_str(); }

	void FileForder(const wchar_t* _pPathKey) { m_FileForder = _pPathKey; }
	bool Compare_Ext(const wchar_t* _pExt) { return m_FileExt == _pExt; }

	void Split_Path(const wchar_t* _pPath);

private:
	virtual bool Create();
	virtual bool Load();

};

