#include "InputMgr.h"
#include <Windows.h>
#include "TimeMgr.h"
#include "Core_Class.h"

HVEC2 InputMgr::m_MousePos;
HVEC2 InputMgr::m_OriMousePos;
HVEC2 InputMgr::m_MouseDir;
POINT InputMgr::m_iPoint;
std::unordered_map<std::wstring, KPtr<InputMgr::KEYDATA>>::iterator InputMgr::m_KeyStartIter;
std::unordered_map<std::wstring, KPtr<InputMgr::KEYDATA>>::iterator InputMgr::m_KeyEndIter;
std::unordered_map<std::wstring, KPtr<InputMgr::KEYDATA>> InputMgr::m_KeyMap;

const char InputMgr::KEYDATA::g_bUp       = (char)0b00000001;
const char InputMgr::KEYDATA::g_bUpStay   = (char)0b00000010;
const char InputMgr::KEYDATA::g_bDown     = (char)0b00000100;
const char InputMgr::KEYDATA::g_bDownStay = (char)0b00001000;
const char InputMgr::KEYDATA::g_brUp	  = (char)0b11111110;
const char InputMgr::KEYDATA::g_brUpStay  = (char)0b11111101;
const char InputMgr::KEYDATA::g_brDown    = (char)0b11111011;
const char InputMgr::KEYDATA::g_brDownStay= (char)0b11110111;

const char InputMgr::KM_ERRORMSG[KM_MAX][256] =
{
	{"OK"},
	{"키를 제작중 오류가 났습니다."},
	{ "넣어준 이름에 해당하는 키데이터가 없습니다." }
};

//////////////////////////////////////////////////

const char* InputMgr::ErrorMsg(KEYMGRERROR _Msg) 
{
	return KM_ERRORMSG[_Msg];
}

InputMgr::KEYDATA::KEYDATA(const size_t _RSize) : m_Data(g_bUpStay)
{
	m_KeyDataList.reserve(_RSize);
}

InputMgr::KEYDATA::~KEYDATA() 
{}

bool InputMgr::KEYDATA::AllKeyCheck() 
{
	for (size_t i = 0; i < m_KeyDataList.size(); i++)
	{
		if (0 == GetAsyncKeyState(m_KeyDataList[i]))
		{
			return false;
		}
	}

	return true;
}

void InputMgr::KEYDATA::Update() 
{
	bool Check = AllKeyCheck();

	// 키가 눌렸을때
	if (true == Check)
	{
		m_PushTime += TimeMgr::DeltaTime();

		if (0 != (m_Data & g_bUpStay))
		{
			m_Data &= g_brUp;
			m_Data &= g_brUpStay;
			m_Data |= g_bDown;
			m_Data |= g_bDownStay;
		}
		else if(0 == (m_Data & g_bUpStay))
		{
			m_Data &= g_brUp;
			m_Data &= g_brUpStay;
			m_Data &= g_brDown;
			m_Data |= g_bDownStay;
		}
	}
	// 키가 눌리지 않았을때;
	else 
	{
		if (0 != (m_Data & g_bDownStay))
		{
			m_PushTime = 0.0f;

			m_Data |= g_bUp;
			m_Data |= g_bUpStay;
			m_Data &= g_brDown;
			m_Data &= g_brDownStay;
		}
		else if (0 == (m_Data & g_bDownStay))
		{
			m_Data &= g_brUp;
			m_Data |= g_bUpStay;
			m_Data &= g_brDown;
			m_Data &= g_brDownStay;
		}
	}
}

bool InputMgr::KEYDATA::IsUp()
{
	return 0 != (m_Data & g_bUp);
}
bool InputMgr::KEYDATA::IsUpStay() 
{
	return 0 != (m_Data & g_bUpStay);
}

bool InputMgr::KEYDATA::IsDown() 
{
	return 0 != (m_Data & g_bDown);
}
bool InputMgr::KEYDATA::IsDownStay() 
{
	return 0 != (m_Data & g_bDownStay);
}
bool InputMgr::KEYDATA::IsOver(float _Time)
{
	if (m_PushTime >= 5.0f)
	{
		int a = 0;
	}

	return m_PushTime >= _Time;
}
bool InputMgr::KEYDATA::IsOverReset(float _Time)
{
	bool Check = m_PushTime >= _Time;
	m_PushTime = 0.0f;
	return Check;
}

/////////////////////////////////// InputMgr

InputMgr::InputMgr()
{

}

InputMgr::~InputMgr()
{
}

void InputMgr::Update() 
{
	GetCursorPos(&m_iPoint);
	ScreenToClient(Core_Class::MainWindow().KHwnd(), &m_iPoint);

	m_OriMousePos = m_MousePos;
	m_MousePos.m_Vec.x = (float)m_iPoint.x;
	m_MousePos.m_Vec.y = (float)m_iPoint.y;

	m_MouseDir = m_MousePos - m_OriMousePos;


	m_KeyStartIter = m_KeyMap.begin();
	m_KeyEndIter = m_KeyMap.end();

	for (; m_KeyStartIter != m_KeyEndIter; ++m_KeyStartIter)
	{
		if (nullptr == m_KeyStartIter->second)
		{
			continue;
		}

		m_KeyStartIter->second->Update();
	}

}

bool InputMgr::IsKey(const wchar_t* _Name) {

	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return true;
}
bool InputMgr::IsUp(const wchar_t* _Name) {
	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsUp();
}
bool InputMgr::IsUpStay(const wchar_t* _Name) {
	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsUpStay();
}
bool InputMgr::IsDown(const wchar_t* _Name) {
	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsDown();
}
bool InputMgr::IsDownStay(const wchar_t* _Name) {
	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsDownStay();
}

bool InputMgr::IsOver(const wchar_t* _Name, float _Time) {
	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsOver(_Time);
}

bool InputMgr::IsOverReset(const wchar_t* _Name, float _Time) {

	KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->IsOverReset(_Time);
}