#pragma once
#include "KFBX.h"
#include "Resource.h"



enum km3LOAD_MODE
{
	KL_FBX,
	KL_km3,
};

class km3Bone
{
public:
	// 저장 불러오기가 쉽게 하려고 이렇게 크기를 지정한다.
	wchar_t Name[512] = { 0, }; // 리터럴 초기화
	KUINT Depth;
	KUINT Index;
	KMatrix OffsetMX;
	std::vector<KeyFrame> KFrameVec;
};

class km3Index
{
public:
	// 크기를 불러오고 잘 저장하기 위함
	KUINT IxSize;
	KUINT IxCnt;
	D3D11_USAGE Usage;
	DXGI_FORMAT Fm;
	char* m_Index;

public:
	km3Index()
	{
		delete m_Index;
	}
};

class km3Mesh
{
public:
	KUINT VxSize;
	KUINT VxCnt;

	D3D11_USAGE Usage;
	char* m_Vertex;

	std::vector<km3Index> IxVec;

};


class km3
{
public:
	km3();
	~km3();
};

