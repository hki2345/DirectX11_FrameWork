#pragma once
#include <map>
#include <set>
#include <list>

#include "SmartPtr.h"
#include "Camera.h"

class Renderer;
class TheOne;
class KLight;
class RenderManager
{
public:
	friend TheOne;
	friend KLight;
	friend Renderer;
	friend class Camera;
	friend class DebugManager;
	

public:
	RenderManager();
	~RenderManager();

private:
	// Camera
	std::map<int, KPtr<Camera>> m_CameraMap;
	std::map<int, KPtr<Camera>>::iterator m_CSI;
	std::map<int, KPtr<Camera>>::iterator m_CEI;

	// 이 레이어 이터는 카메라가 들고 있음 ㅇㅇ - ㅇ 벡터로 바꿈

	// Renderer
	std::map<int, std::list<KPtr<Renderer>>> m_RendererMap;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_RFI;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_AllRenderer_StartIter;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_AllRenderer_EndIter;
	std::list<KPtr<Renderer>>::iterator m_Renderer_StartIter;
	std::list<KPtr<Renderer>>::iterator m_Renderer_EndIter;



private:
	static bool Sort_Z(KPtr<Renderer> _Left, KPtr<Renderer> _Right);

	void Insert_Camera(Camera* _Camera);
	void Insert_Renderer(Renderer* _Renderer);
	void insert_AbsRenderer(KPtr<Renderer> _Renderer);

private:
	std::set<KPtr<KLight>> m_LightSet;
	std::set<KPtr<KLight>>::iterator m_LS;
	std::set<KPtr<KLight>>::iterator m_LE;

	void Insert_Light(KLight* _Light);
	void Reset_SamplerList();

public:
	void Render();
	void Release();

	void Render_Defferd(KPtr<Camera> _Cam, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index);
	void Render_Forward(KPtr<Camera> _Cam, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index);

	bool Is_Name(const wchar_t* _Name);

	void Light_Check(const int& _Layer, KPtr<Camera> _Iter);
	void Render_LightDef(const int& _Layer);
	void Render_Screen();
};

