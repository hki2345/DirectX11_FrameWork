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
	// ��Ʈ�� ���� ����� ����
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

// �ش� ȿ�� ������ ������ ���� ������ ���� �κ��̴�.
void Renderer_Effect::Render(KPtr<Camera> _Camera, const KUINT& _MeshIdx, const KUINT& _MtlIdx, Render_Data* _Data)
{
	Core_Class::MainDevice().ResetContext();
	std::wstring PrevDs = Core_Class::MainDevice().ds_name();

	Core_Class::MainDevice().Set_DSS(L"EFFECT");
	Core_Class::Context()->RSSetState(nullptr);

	m_SMtl->Update();

	// ������ �ѱ�� �Ŵ� �ﰢ���� �ƴϰ�
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	KUINT TSize = 20;
	KUINT TOffset = 0;


	ID3D11Buffer* pPosBuf = m_InitBuf->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pPosBuf, &TSize, &TOffset);


	m_SOBuffer->Update();

	// ������� ��Ʈ�� ���۸� ��� �ֽ�ȭ �ϴ� ���� ���� ����
	// �� ������� ���۴� �츮�� �ε����� ���ؽ��� �Ϥ����� ������ �־��ִ� ���� �ƴ�
	// �ڵ����� �ֿ��� �ȴ�.
	Core_Class::Context()->DrawAuto();
	Core_Class::MainDevice().Set_DSS(PrevDs.c_str());


	// �̰� ���� �ٽ� �׸��� ����
	m_SOBuffer->Reset();

	ID3D11Buffer* pOutBuf = m_SOBuffer->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pOutBuf, &TSize, &TOffset);

	m_RMtl->Update();
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Core_Class::Context()->DrawAuto();
}
