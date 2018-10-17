#include "BRStream.h"


BRStream::BRStream(const wchar_t* _Path) : FileStream(_Path, L"rb")
{
}


BRStream::~BRStream()
{
}

void BRStream::Read(void* _RBuf, int _BufByteSize) {
	Read(_RBuf, _BufByteSize, _BufByteSize);
}

void BRStream::Read(void* _RBuf, int _BufByteSize, int _ReadByteSize) 
{
	FileStream::Read(_RBuf, _BufByteSize, _ReadByteSize);
}

void BRStream::Seek(long _Size, int _Mode ) {
	FileStream::Seek(_Size, _Mode);
}