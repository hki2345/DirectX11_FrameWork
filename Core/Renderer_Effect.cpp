#include "Renderer_Effect.h"
#include "Core_Class.h"


Renderer_Effect::Renderer_Effect()
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_ONE;
}


Renderer_Effect::~Renderer_Effect()
{
}

void Renderer_Effect::Update()
{

}


bool Renderer_Effect::Init(const KUINT& _DotCnt, const int& _Order/* = 0*/)
{
	// 스트림 버퍼 만드는 구간
	m_EFVec.resize(_DotCnt);
	m_EFPVec.resize(_DotCnt);


	for (size_t i = 0; i < _DotCnt; i++)
	{
		KVector RPos;
		RPos.x = KMath::random_f(-10.0f, 10.0f);
		RPos.y = KMath::random_f(-10.0f, 10.0f);
		RPos.z = KMath::random_f(-10.0f, 10.0f);

		m_EFPVec[i] = RPos;
		m_EFVec[i].Pos = RPos;
		m_EFVec[i].Switch = 1;
	}

	m_Tex = new Texture();
	if (false == m_Tex->Create(_DotCnt, &m_EFPVec[0], _DotCnt * sizeof(KVector)
		, D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_IMMUTABLE))
	{
		BBY;
	}

	m_InitBuf = new KStreamBuffer();
	if (false == m_InitBuf->Create(_DotCnt * sizeof(Effect_Begin), D3D11_BIND_VERTEX_BUFFER))
	{
		BBY;
		return false;
	}

	m_SOBuffer = new KStreamBuffer();
	if (false == m_InitBuf->Create(_DotCnt * sizeof(Effect_Begin), D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
	{
		BBY;
		return false;
	}

	m_SMtl = ResourceManager<KMaterial>::Find(L"EFFECTFILTERMTL");
	m_RMtl = ResourceManager<KMaterial>::Find(L"EFFECTRENDERMTL");
	Renderer::Init();

	return true;
}

// 해당 효과 랜더는 일일이 직접 구현이 들어가는 부분이다.
void Renderer_Effect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	Core_Class::MainDevice().ResetContext();
	std::wstring PrevDs = Core_Class::MainDevice().ds_name();

	Core_Class::MainDevice().Set_DSS(L"EFFECT");
	Core_Class::Context()->RSSetState(nullptr);

	m_SMtl->Update();

	// 점들을 넘기는 거다 삼각형이 아니고
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	KUINT TSize = 20;
	KUINT TOffset = 0;


	ID3D11Buffer* pPosBuf = m_InitBuf->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pPosBuf, &TSize, &TOffset);


	m_SOBuffer->Update();

	// 만들어진 스트림 버퍼를 어떻게 최신화 하는 지에 대한 구간
	// 단 만들어진 버퍼는 우리가 인덱스와 버텍스를 일ㅇ일이 지정해 넣어주는 것이 아닌
	// 자동으로 넣우제 된다.
	Core_Class::Context()->DrawAuto();
	Core_Class::MainDevice().Set_DSS(PrevDs.c_str());


	// 이건 이제 다시 그리는 과정
	m_SOBuffer->Reset();

	ID3D11Buffer* pOutBuf = m_SOBuffer->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pOutBuf, &TSize, &TOffset);

	m_RMtl->Update();
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Core_Class::Context()->DrawAuto();
}
