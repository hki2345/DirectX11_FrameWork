#pragma once
#include "BaseStream.h"
class WriteStream : public BaseStream
{
public:
	template<typename T>
	void write_template(T* _WBuf, int _ByteSize)
	{
		BaseStream::write((void*)_WBuf, _ByteSize);
	}

	void write(void* _WBuf, int _ByteSize);

public:
	WriteStream(const wchar_t* _Path);
	~WriteStream();
};

