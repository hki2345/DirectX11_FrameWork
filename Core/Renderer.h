#pragma once
#include "Independent_Trans.h"

#include "Mesh.h"
#include "Material.h"
#include "Blend.h"
#include "KDevice.h"

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

	KPtr<Mesh>		m_Mesh;
	KPtr<Material>	m_Material;
	KPtr<KDevice::RasterState> m_RasterState;

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

protected:
	virtual void Update_Trans(KPtr<Camera> _Cam);
	virtual void Update_CB();
	virtual void Update_MeshMat();

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(KPtr<Camera> _Camera) = 0;

public:
	Renderer();
	~Renderer();
};

