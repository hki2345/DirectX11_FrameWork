#pragma once
#include "BaseStream.h"
class ReadStream : public BaseStream
{
public:
	void read(void* _RBuf, int _BufByteSize, int _ReadByteSize);
	void read(void* _Buf, int _Bytesize);
	void seek(long _Size, int _Mode = SEEK_SET);


public:
	ReadStream(const wchar_t* _Path);
	~ReadStream();
};

