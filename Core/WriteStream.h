#pragma once
#include "BaseStream.h"
class WriteStream : public BaseStream
{
public:
	template<typename T>
	void WriteT(T* _WBuf, int _ByteSize) 
	{
		BaseStream::Write((void*)_WBuf, _ByteSize);
	}

	void Write(void* _WBuf, int _ByteSize);

public:
	WriteStream(const wchar_t* _Path);
	~WriteStream();
};

