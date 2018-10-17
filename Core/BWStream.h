#pragma once
#include "FileStream.h"
class BWStream : public FileStream
{
public:
	template<typename T>
	void WriteT(T* _WBuf, int _ByteSize) 
	{
		FileStream::Write((void*)_WBuf, _ByteSize);
	}

	void Write(void* _WBuf, int _ByteSize);

public:
	BWStream(const wchar_t* _Path);
	~BWStream();
};

