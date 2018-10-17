#include "BWStream.h"


BWStream::BWStream(const wchar_t* _Path) : FileStream(_Path, L"wb")
{
}


BWStream::~BWStream()
{
}

void BWStream::Write(void* _WBuf, int _ByteSize) 
{
	FileStream::Write(_WBuf, _ByteSize);
}