#include "ReadStream.h"


ReadStream::ReadStream(const wchar_t* _Path) : BaseStream(_Path, L"rb")
{
}


ReadStream::~ReadStream()
{
}

void ReadStream::read(void* _Buf, int _Bytesize)
{
	BaseStream::read(_Buf, _Bytesize, _Bytesize);
}

void ReadStream::read(void* _RBuf, int _BufByteSize, int _ReadByteSize)
{
	BaseStream::read(_RBuf, _BufByteSize, _ReadByteSize);
}

void ReadStream::seek(long _Size, int _Mode) {
	BaseStream::seek(_Size, _Mode);
}