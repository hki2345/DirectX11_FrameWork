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
		Defferd_orForward(1), // 0 - 디퍼드 1 - 포워드
		IsBoneAni(0) // 0 - 본 애니 아님
	{	}
	// ~RenderOption();


private:
	friend class Renderer;

public:
	int IsLight;
	int Defferd_orForward;
	int VTz_PXo;  // Vert -> 0, Pixel -> 1
	int TexCnt;
	int IsBoneAni; // 이것이 애니메이션이냐 -> 인트로 해야하는데 일단 박아넣음

	// 이게 바로 인트 하나 추가하므로서 생긴 짜투리
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
	// 픽셀 버텍스에서 쓰이는
	// 아주 기본적인 메트릭스 배열같은 것들은 아예 랜더러가 가지고 있는것으로 한다.
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

	// 이미지 설정을 이제 렌더러의 재질이 해준다.
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

