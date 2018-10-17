#include "HFreeCamera.h"
#include <InputMgr.h>
#include <TimeMgr.h>
#include <GameDebug.h>


HFreeCamera::HFreeCamera() : m_Speed(10.0f)
{
}


HFreeCamera::~HFreeCamera()
{
}

bool HFreeCamera::Init() 
{
	m_Camera = GetCom<HCamera>();
	m_Camera->ProjectionMode(HCamera::PROJMODE::PM_PERS);

#pragma region KEYCREATE


	if (false == InputMgr::IsKey(L"FREELEFT"))
	{
		InputMgr::CreateKey(L"FREELEFT", 'A');
	}

	if (false == InputMgr::IsKey(L"FREERIGHT"))
	{
		InputMgr::CreateKey(L"FREERIGHT", 'D');
	}

	if (false == InputMgr::IsKey(L"FREEUP"))
	{
		InputMgr::CreateKey(L"FREEUP", 'Q');
	}

	if (false == InputMgr::IsKey(L"FREEDOWN"))
	{
		InputMgr::CreateKey(L"FREEDOWN", 'E');
	}

	if (false == InputMgr::IsKey(L"FREEFORWARD"))
	{
		InputMgr::CreateKey(L"FREEFORWARD", 'W');
	}

	if (false == InputMgr::IsKey(L"FREEBACK"))
	{
		InputMgr::CreateKey(L"FREEBACK", 'S');
	}

	if (false == InputMgr::IsKey(L"ROTLOCK"))
	{
		InputMgr::CreateKey(L"ROTLOCK", VK_RBUTTON);
	}

	if (false == InputMgr::IsKey(L"Boost"))
	{
		InputMgr::CreateKey(L"Boost", VK_SHIFT);
	}

	if (false == InputMgr::IsKey(L"MODECHANAGE"))
	{
		InputMgr::CreateKey(L"MODECHANAGE", 'R');
	}

	if (false == InputMgr::IsKey(L"Z"))
	{
		InputMgr::CreateKey(L"Z", 'Z');
	}

	if (false == InputMgr::IsKey(L"X"))
	{
		InputMgr::CreateKey(L"X", 'X');
	}

	if (false == InputMgr::IsKey(L"C"))
	{
		InputMgr::CreateKey(L"C", 'C');
	}

	if (false == InputMgr::IsKey(L"F"))
	{
		InputMgr::CreateKey(L"F", 'F');
	}

#pragma endregion
	return true;
}
void HFreeCamera::Update() 
{


	if (true == InputMgr::IsDownStay(L"Boost"))
	{
		m_RotSpeed = 360.0f;
		m_Speed = 100.0f;
	}
	else {
		m_RotSpeed = 90.0f;
		m_Speed = 10.0f;
	}

	if (true == InputMgr::IsDown(L"MODECHANAGE"))
	{
		m_Camera->ModeChange();
	}


	if (true == InputMgr::IsDown(L"Z"))
	{
		HVEC Rot = m_Trans->LRot();
		Rot.z = 0.0f;
		m_Trans->LRot(Rot);
	}

	if (true == InputMgr::IsDown(L"Z"))
	{
		HVEC Rot = m_Trans->LRot();
		Rot.z = 0.0f;
		m_Trans->LRot(Rot);
	}

	if (true == InputMgr::IsDown(L"X"))
	{
		HVEC Rot = m_Trans->LRot();
		Rot.x = 0.0f;
		m_Trans->LRot(Rot);
	}

	if (true == InputMgr::IsDown(L"F"))
	{
		m_Trans->Reset();
		m_Trans->LMove(HVEC(0.0f, 0.0f, -10.0f));
	}

	if (true == InputMgr::IsDownStay(L"FREELEFT"))
	{
		m_Trans->LMove(m_Trans->LLeft() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"FREERIGHT"))
	{
		m_Trans->LMove(m_Trans->LRight() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"FREEUP"))
	{
		m_Trans->LMove(m_Trans->LUp() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"FREEDOWN"))
	{
		m_Trans->LMove(m_Trans->LDown() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"FREEFORWARD"))
	{
		m_Trans->LMove(m_Trans->LForward() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"FREEBACK"))
	{
		m_Trans->LMove(m_Trans->LBack() * TimeMgr::DeltaTime() * m_Speed);
	}

	if (true == InputMgr::IsDownStay(L"ROTLOCK"))
	{
		m_Trans->LAccRotDeg(HVEC(InputMgr::MouseDir().y * m_RotSpeed * TimeMgr::DeltaTime(), InputMgr::MouseDir().x * m_RotSpeed * TimeMgr::DeltaTime()));
	}

	wchar_t Arr[256];

	swprintf_s(Arr, L"CameraPos : %f, %f, %f", Trans()->LPos().x, Trans()->LPos().y, Trans()->LPos().z);
	GameDebug::DrawLog(Arr);
}
void HFreeCamera::DebugRender() 
{
	//wchar_t Arr[256];

	//swprintf_s(Arr, L"RotScale : %f, %f", RotPos.x, RotPos.y);
	//GameDebug::DrawFont(Arr, { 10.0f, 20.0f }, 20.0f);

	//HVEC2 CurMouse = InputMgr::MousePos();

	//swprintf_s(Arr, L"MousePos : %f, %f", CurMouse.x, CurMouse.y);
	//GameDebug::DrawFont(Arr, { 10.0f, 40.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraRot : %f, %f", m_Trans->LRot().x, m_Trans->LRot().y);
	//GameDebug::DrawFont(Arr, { 10.0f, 60.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraF : %f, %f, %f", m_Trans->LForward().x, m_Trans->LForward().y, m_Trans->LForward().z);
	//GameDebug::DrawFont(Arr, { 10.0f, 80.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraU : %f, %f, %f", m_Trans->LUp().x, m_Trans->LUp().y, m_Trans->LUp().z);
	//GameDebug::DrawFont(Arr, { 10.0f, 100.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraPos : %f, %f, %f", m_Trans->LPos().x, m_Trans->LPos().y, m_Trans->LPos().z);
	//GameDebug::DrawFont(Arr, { 10.0f, 120.0f }, 20.0f);
}