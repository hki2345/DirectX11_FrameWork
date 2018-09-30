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

	// 픽셀 버텍스에서 쓰이는
	// 아주 기본적인 메트릭스 배열같은 것들은 아예 랜더러가 가지고 있는것으로 한다.
	DATA_3D m_MD;

	int			m_Light;
	int			m_Order;

	KPtr<Mesh>		m_Mesh;
	KPtr<Material>	m_Material;
	KPtr<KDevice::RasterState> m_RasterState;

	// 디바이스의 업데이트를 돌리기 위함ㅇㅇ -> 렌더러에서 각자가 도렬주는 것으로
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

