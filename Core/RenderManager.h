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
	friend Camera;
	friend KLight;
	friend Renderer;

private:
	// Camera
	std::set<KPtr<Camera>> m_CameraMap;
	std::set<KPtr<Camera>>::iterator m_Camera_StartIter;
	std::set<KPtr<Camera>>::iterator m_Camera_EndIter;

	// 이 레이어 이터는 카메라가 들고 있음 ㅇㅇ - ㅇ 벡터로 바꿈

	// Renderer
	std::map<int, std::list<KPtr<Renderer>>> m_RendererMap;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_Renderer_FindIter;
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
	void Light_Check(const int& _Layer, const std::set<KPtr<Camera>>::iterator& _Iter);
	void Reset_SamplerList();

public:
	void Render();
	void Release();

	bool Is_Name(const wchar_t* _Name);

public:
	RenderManager();
	~RenderManager();
};

