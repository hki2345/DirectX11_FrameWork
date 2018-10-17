#pragma once
#include "FileStream.h"
class BRStream : public FileStream
{
public:
	void Read(void* _RBuf, int _BufByteSize);
	void Read(void* _RBuf, int _BufByteSize, int _ReadByteSize);
	void Seek(long _Size, int _Mode = SEEK_SET);

public:
	BRStream(const wchar_t* _Path);
	~BRStream();
};

