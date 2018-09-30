#include "WriteStream.h"


WriteStream::WriteStream(const wchar_t* _Path) : BaseStream(_Path, L"wb")
{
}


WriteStream::~WriteStream()
{
}

void WriteStream::write(void* _WBuf, int _ByteSize)
{
	BaseStream::write(_WBuf, _ByteSize);
}