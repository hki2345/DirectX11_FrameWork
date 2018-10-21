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
		Defferd_orForward(1) // 0 - 디퍼드 1 - 포워드
	{	}
	// ~RenderOption();


private:
	friend class Renderer;

public:
	int IsLight;
	int Defferd_orForward;
	int VTz_PXo;  // Vert -> 0, Pixel -> 1
	int TexCnt;

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

	KPtr<KMesh>				m_Mesh;
	KPtr<KMaterial>			m_Mat;
	KPtr<KDevice::RState>	m_RsState;

public:
	RenderOption ROpt;




public:
	// 이미지 설정을 이제 렌더러의 재질이 해준다.
	KPtr<KMaterial> material();

	void SetRSState(const wchar_t* _Name);
	int order() { return m_Order; }

	bool SetMesh(const wchar_t* _Res);
	bool SetMat(const wchar_t* _Res);

private:
	void RenderUpdate();
	void RenderFinalUpdate();

protected:
	virtual void Update_Trans(KPtr<Camera> _Camera);
	virtual void Update_TransCB();

	virtual void Update_Material();
	virtual void Update_Mesh();

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(KPtr<Camera> _Camera) = 0;

};

