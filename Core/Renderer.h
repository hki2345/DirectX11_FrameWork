#pragma once
#include "HSubTransCom.h"


// #include "Shader_Vertex.h"
#include "KMaterial.h"
#include "KMesh.h"
#include "KBlend.h"
#include "KDevice.h"

class RenderOption
{
public:
	RenderOption() :
		IsLight(1),
		TexCnt(0),
		VTz_PXo(1),
		Defferd_orForward(1), // 0 - ���۵� 1 - ������
		IsBoneAni(0) // 0 - �� �ִ� �ƴ�
	{	}
	// ~RenderOption();


private:
	friend class Renderer;

public:
	int IsLight;
	int Defferd_orForward;
	int VTz_PXo;  // Vert -> 0, Pixel -> 1
	int TexCnt;
	int IsBoneAni; // �̰��� �ִϸ��̼��̳� -> ��Ʈ�� �ؾ��ϴµ� �ϴ� �ھƳ���

	// �̰� �ٷ� ��Ʈ �ϳ� �߰��ϹǷμ� ���� ¥����
	int temp1;
	int temp2;
	int temp3;


private:
	Tex_Data TexArr[8];

};


class Camera;
class KMesh;
class KMaterial;
class Renderer : public HSubTransCom
{
public:
	Renderer();
	~Renderer();


	friend class RenderManager;


protected:
	// �ȼ� ���ؽ����� ���̴�
	// ���� �⺻���� ��Ʈ���� �迭���� �͵��� �ƿ� �������� ������ �ִ°����� �Ѵ�.
	MatrixContainer		m_MD;
	int					m_bLight;
	int					m_Order;
	bool				m_Bill;

	std::vector<KPtr<KMesh>>		m_MeshVec;
	std::vector<KPtr<KMaterial>> m_MtlVec;



	KPtr<KDevice::RState>	m_RsState;

public:
	RenderOption ROpt;




public:
	size_t Count_Mesh() { return m_MeshVec.size(); }
	size_t Count_Material() { return m_MtlVec.size(); }

	// �̹��� ������ ���� �������� ������ ���ش�.
	KPtr<KMaterial> material(const int& _Index = 0);

	void Set_RSState(const wchar_t* _Name);
	int order() { return m_Order; }

	bool Set_Mesh(const wchar_t* _Res, const int& Index = 0);
	bool Set_Mat(const wchar_t* _Res, const int& Index = 0);

private:
	void RenderUpdate();
	void RenderFinalUpdate();

protected:
	virtual void Update_Trans(KPtr<Camera> _Camera);
	virtual void Update_TransCB();
	virtual void Update_MtlCB(const KUINT _Index = 0);

	virtual void Update_TexSmp(const KUINT _Index = 0);
	virtual void Update_Material(const KUINT _Index = 0);
	virtual void Update_Mesh(const KUINT _Index = 0);


public:
	virtual bool Init(int _Order = 0);
	virtual void Render(KPtr<Camera> _Camera) = 0;

};

