#include "InputManager.h"

#include "TimeManager.h"
#include "Core_Class.h"


KVector2	InputManager::m_MousePos;
KVector2	InputManager::m_OriMousePos;
KVector2	InputManager::m_MouseDir;
POINT		InputManager::m_iPoint;
int			InputManager::m_WheelMove;
bool		InputManager::m_bLock = false;
std::unordered_map<std::wstring, KPtr<InputManager::Input_Command>>::iterator InputManager::m_KeyStartIter;
std::unordered_map<std::wstring, KPtr<InputManager::Input_Command>>::iterator InputManager::m_KeyEndIter;
std::unordered_map<std::wstring, KPtr<InputManager::Input_Command>> InputManager::m_KeyMap;

const char InputManager::Input_Command::g_bUp       = (char)0b00000001;
const char InputManager::Input_Command::g_bUpStay   = (char)0b00000010;
const char InputManager::Input_Command::g_bDown     = (char)0b00000100;
const char InputManager::Input_Command::g_bDownStay = (char)0b00001000;
const char InputManager::Input_Command::g_brUp	  = (char)0b11111110;
const char InputManager::Input_Command::g_brUpStay  = (char)0b11111101;
const char InputManager::Input_Command::g_brDown    = (char)0b11111011;
const char InputManager::Input_Command::g_brDownStay= (char)0b11110111;

const char InputManager::KM_ERRORMSG[IERR_MAX][256] =
{
	{"OK"},
	{"키를 제작중 오류가 났습니다."},
	{ "넣어준 이름에 해당하는 키데이터가 없습니다." }
};

//////////////////////////////////////////////////

const char* InputManager::ErrorMsg(INPMGR_ERR _Msg) 
{
	return KM_ERRORMSG[_Msg];
}

InputManager::Input_Command::Input_Command(const size_t _RSize) : m_Data(g_bUpStay)
{
	m_KeyDataList.reserve(_RSize);
}

InputManager::Input_Command::~Input_Command() 
{}

bool InputManager::Input_Command::AllKeyCheck() 
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

void InputManager::Input_Command::Update() 
{
	bool Check = AllKeyCheck();

	// 키가 눌렸을때
	if (true == Check)
	{
		m_PressTime += TimeManager::DeltaTime();

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
			m_PressTime = 0.0f;

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

bool InputManager::Input_Command::Up()
{
	return 0 != (m_Data & g_bUp);
}
bool InputManager::Input_Command::UnPress() 
{
	return 0 != (m_Data & g_bUpStay);
}

bool InputManager::Input_Command::Down() 
{
	return 0 != (m_Data & g_bDown);
}
bool InputManager::Input_Command::Press() 
{
	return 0 != (m_Data & g_bDownStay);
}
bool InputManager::Input_Command::Over(float _Time)
{
	return m_PressTime >= _Time;
}
bool InputManager::Input_Command::Over_Reset(float _Time)
{
	bool Check = m_PressTime >= _Time;
	m_PressTime = 0.0f;
	return Check;
}

/////////////////////////////////// InputManager

InputManager::InputManager()
{

}

InputManager::~InputManager()
{
}

void InputManager::Update() 
{
	Update_Mouse();
	Update_Key();
}

void InputManager::Update_Mouse()
{
	GetCursorPos(&m_iPoint);
	ScreenToClient(Core_Class::MainWindow().KHwnd(), &m_iPoint);
	m_OriMousePos = m_MousePos;

	m_MousePos.m_XMVec2.x = (float)m_iPoint.x;
	m_MousePos.m_XMVec2.y = (float)m_iPoint.y;

	m_MouseDir = m_MousePos - m_OriMousePos;
	

	if (true == m_bLock)
	{
		m_MousePos = Core_Class::MainWindow().size() * .5f;
		m_iPoint = { (long)m_MousePos.x, (long)m_MousePos.y };
		ScreenToClient(Core_Class::MainWindow().KHwnd(), &m_iPoint);
		SetCursorPos(m_iPoint.x, m_iPoint.y);
		KLOG(L"Mouse Pos: %d %d", m_iPoint.x, m_iPoint.y);
		ShowCursor(false);
	}
	else
	{
		ShowCursor(true);
	}
}

void InputManager::Update_Key()
{

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

bool InputManager::IsKey(const wchar_t* _Name) {

	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return true;
}
bool InputManager::Up(const wchar_t* _Name) {
	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->Up();
}
bool InputManager::UnPress(const wchar_t* _Name) {
	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->UnPress();
}
bool InputManager::Down(const wchar_t* _Name) {
	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->Down();
}
bool InputManager::Press(const wchar_t* _Name) {
	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->Press();
}

bool InputManager::Over(const wchar_t* _Name, float _Time) {
	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->Over(_Time);
}

bool InputManager::Over_Reset(const wchar_t* _Name, float _Time) {

	KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);
	if (nullptr == pKEY)
	{
		return false;
	}
	return pKEY->Over_Reset(_Time);
}


bool InputManager::Check_InScr()
{
	KVector2 MPos = MousePos();
	bool Check;

	if (Core_Class::MainWindow().size() > MPos && KVector2::Zero < MPos)
	{
		Check = true;
	}
	else
	{
		Check = false;
	}

	return Check;
}

void InputManager::Set_MLock()
{
	m_bLock = true;
}
void InputManager::Set_MUnLock()
{
	m_bLock = false;
}


// 변수 넣어주시면 바꿔드립니다. - 휠 움직인 값으로 - 짜피 메모리 더써야되니까
// 그 쓰는 현장에서 변수 선언 해서 가져가는 식으로 하세요
int& InputManager::WheelValue(int* _Ref)
{
	*_Ref = m_WheelMove;

	return *_Ref;
}


void InputManager::EndUpdate()
{
	m_WheelMove = 0;
}