#pragma once
#include <map>
#include <set>
#include <list>
#include "SmartPtr.h"
#include "HCamera.h"

class HScene;
class HRenderer;
class HCamera;
class HActor;
class HLight;
class HRenderMgr
{
public:
	friend HActor;
	friend HScene;
	friend HCamera;
	friend HRenderer;
	friend class GameDebug;

private:
	static bool ZOrderSort(KPtr<HRenderer> _Left, KPtr<HRenderer> _Right);

private:
	// 1 2 3 4 
	std::map<int, KPtr<HCamera>>::iterator m_CamStartIter;
	std::map<int, KPtr<HCamera>>::iterator m_CamEndIter;
	std::map<int, KPtr<HCamera>> m_CameraMap;
	//std::vector<int>::iterator m_GroupStartIter;
	//std::vector<int>::iterator m_GroupEndIter;

	// List Int
	std::map<int, std::list<KPtr<HRenderer>>>::iterator m_GFindIter;
	std::map<int, std::list<KPtr<HRenderer>>>::iterator m_RAStartIter;
	std::map<int, std::list<KPtr<HRenderer>>>::iterator m_RAEndIter;

	std::list<KPtr<HRenderer>>::iterator m_RStartIter;
	std::list<KPtr<HRenderer>>::iterator m_REndIter;
	std::map<int, std::list<KPtr<HRenderer>>> m_RenderMap;


private:
	void PushCamera(HCamera* _Camera);
	void PushRenderer(HRenderer* _Renderer);
	void PushOverRenderer(KPtr<HRenderer> _Renderer);

public: //////////////////////// Light;
	std::set<KPtr<HLight>> m_LightSet;
	std::set<KPtr<HLight>>::iterator m_LightStartIter;
	std::set<KPtr<HLight>>::iterator m_LightEndIter;

	void PushLight(HLight* _Camera);
	void LightCheck(KPtr<HCamera> _Camera, int _Group);
	void Render_Defferd_Light(KPtr<HCamera> _Camera, int _Group);

private:
	void ResetSR();

public:
	void Render();
	void Render_Defferd(KPtr<HCamera> _Camera, std::map<int, std::list<KPtr<HRenderer>>>::iterator _Iter, size_t _Index);
	void Render_Forward(KPtr<HCamera> _Camera, std::map<int, std::list<KPtr<HRenderer>>>::iterator _Iter, size_t _Index);
	void ScreenMerge();
	void Release();

public:
	HRenderMgr();
	~HRenderMgr();
};

