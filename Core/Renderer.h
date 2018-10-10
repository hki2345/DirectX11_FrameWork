#pragma once
#include "Independent_Trans.h"

#include "Mesh.h"
#include "Material.h"
#include "Blend.h"
#include "KDevice.h"


class RenderOption
{
private:
	friend class Renderer;

public:
	int CheckLight;
	int Deffert_orFoward;
	int VzPo; // Vert -> 0, Pixel -> 1
	int TexCnt;

private:
	Texture_Data Texes[8];

public:
	RenderOption() : CheckLight(1), TexCnt(0),
		VzPo(1), Deffert_orFoward(1) {}
};

class Camera;
class Renderer : public Independent_Trans
{
protected:
	friend class RenderManager;

	// �ȼ� ���ؽ����� ���̴�
	// ���� �⺻���� ��Ʈ���� �迭���� �͵��� �ƿ� �������� ������ �ִ°����� �Ѵ�.
	DATA_3D m_MD;

	int			m_Light;
	int			m_Order;
	bool		m_Bill;

	KPtr<Mesh>		m_Mesh;
	KPtr<Material>	m_Material;
	KPtr<KDevice::RasterState> m_RasterState;
	RenderOption	m_ROption;

	// ����̽��� ������Ʈ�� ������ ���Ԥ��� -> ���������� ���ڰ� �����ִ� ������
private:
	void RenderUpdate();
	void RenderFinalUpdate();

public:
	void Create_RasterState(const wchar_t* _Name);

	int& order() { return m_Order; }
	bool Set_Mesh(const wchar_t* _Name);
	bool Set_Mesh();
	bool Set_Material(const wchar_t* _Name);

	// �̹��� ������ ���� �������� ������ ���ش�.
	KPtr<Material> material();

protected:
	virtual void Update_Trans(KPtr<Camera> _Cam);
	virtual void Update_CB();
	virtual void Update_MeshMat();

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(KPtr<Camera> _Camera) = 0;

	void billboard(const bool& _Value)
	{
		m_Bill = _Value;
	}

public:
	Renderer();
	~Renderer();
};

