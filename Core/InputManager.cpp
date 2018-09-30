#include "InputManager.h"

#include <Windows.h>
#include "TimeManager.h"
#include "Core_Class.h"

KVector2 InputManager::m_SCREEN_MOUSEPOS;
KVector2 InputManager::m_SCREEN_MOUSEDIR;
POINT InputManager::m_WIN_MOUSEPOINT;
std::unordered_map<std::wstring, KPtr<InputManager::Command>>::iterator InputManager::m_Iter_ComStart;
std::unordered_map<std::wstring, KPtr<InputManager::Command>>::iterator InputManager::m_Iter_ComEnd;
std::unordered_map<std::wstring, KPtr<InputManager::Command>> InputManager::m_Map_Command;

const char InputManager::Command::g_IsUp			= (char)0b00000001;
const char InputManager::Command::g_IsUnPress		= (char)0b00000010;
const char InputManager::Command::g_IsDown			= (char)0b00000100;
const char InputManager::Command::g_IsPress			= (char)0b00001000;
const char InputManager::Command::g_IsRevUp			= (char)0b11111110;
const char InputManager::Command::g_IsRevUnPress	= (char)0b11111101;
const char InputManager::Command::g_IsRevDown		= (char)0b11111011;
const char InputManager::Command::g_IsRevPress		= (char)0b11110111;

const char InputManager::m_ErrorMsg[INPUT_MAX][256] =
{
	{ "OK" },
	{ "키를 제작중 오류가 났습니다." },
	{ "넣어준 이름에 해당하는 키데이터가 없습니다." }
};

//////////////////////////////////////////////////

const char* InputManager::MSG_ERROR(INPUT_ERROR _Msg)
{
	return m_ErrorMsg[_Msg];
}

InputManager::Command::Command(const size_t _RSize) : m_Data(g_IsUnPress)
{
	m_vec_Command.reserve(_RSize);
}

InputManager::Command::~Command()
{
	
}

bool InputManager::Command::Command_Check()
{
	for (size_t i = 0; i < m_vec_Command.size(); i++)
	{
		if (0 == GetAsyncKeyState(m_vec_Command[i]))
		{
			return false;
		}
	}

	return true;
}

void InputManager::Command::Update()
{
	bool Check = Command_Check();

	// 키가 눌렸을때
	if (true == Check)
	{
		m_PressTime += TimeManager::DeltaTime();

		if (0 != (m_Data & g_IsUnPress))
		{
			m_Data &= g_IsRevUp;
			m_Data &= g_IsRevUnPress;
			m_Data |= g_IsDown;
			m_Data |= g_IsPress;
		}
		else if (0 == (m_Data & g_IsUnPress))
		{
			m_Data &= g_IsRevUp;
			m_Data &= g_IsRevUnPress;
			m_Data &= g_IsRevDown;
			m_Data |= g_IsPress;
		}
	}
	// 키가 눌리지 않았을때;
	else
	{
		if (0 != (m_Data & g_IsPress))
		{
			m_PressTime = 0.0f;

			m_Data |= g_IsUp;
			m_Data |= g_IsUnPress;
			m_Data &= g_IsRevDown;
			m_Data &= g_IsRevPress;
		}
		else if (0 == (m_Data & g_IsPress))
		{
			m_Data &= g_IsRevUp;
			m_Data |= g_IsUnPress;
			m_Data &= g_IsRevDown;
			m_Data &= g_IsRevPress;
		}
	}
}

bool InputManager::Is_Key(const wchar_t* _Name)
{
	KPtr<Command> pCom = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == pCom)
	{
		return false;
	}

	return true;
}

bool InputManager::Command::Up()
{
	return 0 != (m_Data & g_IsUp);
}
bool InputManager::Command::UnPress()
{
	return 0 != (m_Data & g_IsUnPress);
}

bool InputManager::Command::Down()
{
	return 0 != (m_Data & g_IsDown);
}
bool InputManager::Command::Press()
{
	return 0 != (m_Data & g_IsPress);
}
bool InputManager::Command::Over(float _Time)
{
	return m_PressTime >= _Time;
}
bool InputManager::Command::Over_Reset(float _Time)
{
	// 자동발사 시스템 -> 누르면 일정시간 후 계속 초기화 됌
	bool Check = m_PressTime >= _Time;
	m_PressTime = 0.0f;
	return Check;
}

/////////////////////////////////// InputMgr

InputManager::InputManager()
{

}

InputManager::~InputManager()
{ }

void InputManager::Update()
{
	GetCursorPos(&m_WIN_MOUSEPOINT);
	ScreenToClient(Core_Class::Main_Window().KWnd(), &m_WIN_MOUSEPOINT);

	KVector2 TempPos = m_SCREEN_MOUSEPOS;

	m_SCREEN_MOUSEPOS.m_Vector.x = (float)m_WIN_MOUSEPOINT.x;
	m_SCREEN_MOUSEPOS.m_Vector.y = (float)m_WIN_MOUSEPOINT.y;

	m_SCREEN_MOUSEDIR = m_SCREEN_MOUSEPOS - TempPos;

	m_Iter_ComStart = m_Map_Command.begin();
	m_Iter_ComEnd = m_Map_Command.end();

	for (; m_Iter_ComStart != m_Iter_ComEnd; m_Iter_ComStart++)
	{
		if (nullptr == m_Iter_ComStart->second)
		{
			continue;
		}

		m_Iter_ComStart->second->Update();
	}

}


bool InputManager::Up(const wchar_t* _Name) {
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->Up();
}
bool InputManager::UnPress(const wchar_t* _Name) {
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->UnPress();
}
bool InputManager::Down(const wchar_t* _Name) {
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->Down();
}
bool InputManager::Press(const wchar_t* _Name) {
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->Press();
}

bool InputManager::Over(const wchar_t* _Name, float _Time) {
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->Over(_Time);
}

bool InputManager::Over_Reset(const wchar_t* _Name, float _Time) {

	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);
	if (nullptr == p_Com)
	{
		return false;
	}
	return p_Com->Over_Reset(_Time);
}

KPtr<InputManager::Command> InputManager::Find_Command(const wchar_t* _Name)
{
	KPtr<Command> p_Com = Map_Find<KPtr<Command>>(m_Map_Command, _Name);

	if (nullptr == p_Com)
	{
		return nullptr;
	}

	return p_Com;
}