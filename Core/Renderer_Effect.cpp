#include "Renderer_Effect.h"
#include "Texture.h"
#include "Core_Class.h"


Renderer_Effect::Renderer_Effect() : m_UpdateTime(.0f)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_ONE;
}


Renderer_Effect::~Renderer_Effect()
{
}

void Renderer_Effect::Update()
{
	for (size_t i = 0; i < m_EFVec.size(); i++)
	{
	}
}


bool Renderer_Effect::Init(const KUINT& _DotCnt, const int& _Order/* = 0*/)
{
	m_MaxEFCnt = _DotCnt;

	// ��Ʈ�� ���� ����� ����
	m_EFVec.resize(_DotCnt);


	for (size_t i = 0; i < _DotCnt; i++)
	{
		KVector RPos;
		RPos.x = KMath::random_f(-10.0f, 10.0f);
		RPos.y = KMath::random_f(-10.0f, 10.0f);
		RPos.z = KMath::random_f(-10.0f, 10.0f);

		m_EFVec[i].Pos = RPos;
	}


	m_DataBuf = new KStreamBuffer();
	if (false == m_DataBuf->Create(_DotCnt * sizeof(Effect_Begin), &m_EFVec[0], D3D11_BIND_VERTEX_BUFFER))
	{
		BBY;
		return false;
	}

	m_SOBuffer = new KStreamBuffer();
	if (false == m_DataBuf->Create(_DotCnt * sizeof(Effect_Begin), nullptr, D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT))
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
	// ������ �ѱ�� �Ŵ� �ﰢ���� �ƴϰ�
	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_SMtl->Update();


	KUINT TSize = sizeof(Renderer_Effect::Effect_Begin);
	KUINT TOffset = 0;
	ID3D11Buffer* pPosBuf = m_DataBuf->buffer();


	Core_Class::Context()->IASetVertexBuffers(0, 1, &pPosBuf, &TSize, &TOffset);


	m_SOBuffer->Update();

	// ������� ��Ʈ�� ���۸� ��� �ֽ�ȭ �ϴ� ���� ���� ����
	// �� ������� ���۴� �츮�� �ε����� ���ؽ��� �Ϥ����� ������ �־��ִ� ���� �ƴ�
	// �ڵ����� �ֿ��� �ȴ�.
	// ���µ� ������ �ʱ�ȭ�� auto �ȸԵ��
	// Core_Class::Context()->DrawAuto();
	Core_Class::Context()->Draw(m_MaxEFCnt, 0);
	m_SOBuffer->Reset();


	Core_Class::MainDevice().Set_DSS(PrevDs.c_str());
	Core_Class::MainDevice().Set_RS(L"SNONE");
	Core_Class::MainDevice().ResetContext();

	// �̰� ���� �ٽ� �׸��� ����
	m_SOBuffer->Reset();

	ID3D11Buffer* pOutBuf = m_SOBuffer->buffer();
	Core_Class::Context()->IASetVertexBuffers(0, 1, &pOutBuf, &TSize, &TOffset);

	m_RMtl->Update_Tex();
	m_RMtl->Update_Sam();
	m_RMtl->Update();


	Core_Class::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	Core_Class::Context()->DrawAuto();

	Core_Class::MainDevice().Set_RS(L"SBACK");
}


KPtr<KMaterial> Renderer_Effect::render_material()
{
	return m_RMtl;
}