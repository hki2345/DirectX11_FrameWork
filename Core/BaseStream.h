#pragma once
#include <iostream>

class BaseStream
{
protected:
	FILE* m_pFile;

public:
	bool is_file()
	{
		if ( nullptr  == m_pFile)
		{
			return false;
		}

		return true;
	}
	void seek(long _Size, int _Mode = SEEK_SET);
	void write(void* _WBuf, int _ByteSize);
	void read(void* _RBuf, int _BufByteSize, int _ReadByteSize);

public:
	BaseStream(const wchar_t* _FilePath, const wchar_t* _Mode);
	~BaseStream();
};

