#include "BaseStream.h"


BaseStream::BaseStream(const wchar_t* _FilePath, const wchar_t* _Mode) : m_pFile(nullptr)
{
	_wfopen_s(&m_pFile, _FilePath, _Mode);
}
BaseStream::~BaseStream()
{	
	if (nullptr != m_pFile)
	{
		fclose(m_pFile);
	}
}

void BaseStream::seek(long _Size, int _Mode)
{
	if (nullptr != m_pFile)
	{
		fseek(m_pFile, _Size, _Mode);
	}
}

void BaseStream::write(void* _WBuf, int _ByteSize) {

	if (nullptr != m_pFile)
	{
		fwrite(_WBuf, _ByteSize, 1, m_pFile);
	}
}

void BaseStream::read(void* _RBuf, int _BufByteSize, int _ReadByteSize)
{
	if (nullptr != m_pFile)
	{
		if (_BufByteSize < _ReadByteSize)
		{
			_ReadByteSize = _BufByteSize;
		}

		fread_s(_RBuf, _BufByteSize, _ReadByteSize, 1, m_pFile);
	}
}