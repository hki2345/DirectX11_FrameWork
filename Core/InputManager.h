#pragma once
#include <vector>
#include <unordered_map>
#include "KMacro.h"
#include "DirectXHeader.h"

#include "Stl_Assistor.h"
#include "SmartPtr.h"


#define KEY_PRESS(ACTION) InputManager::Press(L##ACTION)
#define KEY_UP(ACTION) InputManager::Up(L##ACTION)
#define KEY_DOWN(ACTION) InputManager::Down(L##ACTION)
#define KEY_UNPRESS(ACTION) InputManager::UnPress(L##ACTION)
#define KEY_OVER(ACTION) InputManager::Over(L##ACTION)
#define KEY_OVER_RESET(ACTION) InputManager::Over_Reset(L##ACTION)


enum INPUT_ERROR
{
	INPUT_OK,
	INPUT_CREATE_ERROR,
	INPUT_FIND_ERROR,
	INPUT_MAX,
};


class InputManager
{
public:
	friend class KCore;

private:
	static POINT	m_WIN_MOUSEPOINT;
	static KVector2 m_SCREEN_MOUSEPOS;
	static KVector2 m_SCREEN_MOUSEDIR;

public:
	static KVector2 mouse_pos()
	{
		return m_SCREEN_MOUSEPOS;
	}
	static KVector2 mouse_dir()
	{
		return m_SCREEN_MOUSEDIR;
	}
	static const char m_ErrorMsg[INPUT_MAX][256];

private:
	// �̹����� Ű�� char ���� �ϳ��� ��� �����ϵ��� �Ѵ�.
	// �������� ���������� �޸𸮸� �Ƴ� �� �ִ�. -> �Ʋ����� ������ ���̴�.
	// �̷����� �޸� �Ƴ��� �������ƶ� -> �ϴ��� ������ �����غ��� ��
	class Command : public SmartPtr
	{
	private:
		static const char g_IsUp;
		static const char g_IsUnPress;
		static const char g_IsDown;
		static const char g_IsPress;
		static const char g_IsRevUp;
		static const char g_IsRevUnPress;
		static const char g_IsRevDown;
		static const char g_IsRevPress;

	public:
		friend InputManager;

	private:
		std::vector<int> m_vec_Command;
		char m_Data;

		// �߰��� �� ->> ������ �ִ� �ð��� �����Ͽ� Over�� ������ �� ���� -> 
		// �����ð� ������ �ִ°�
		float m_PressTime;

	public:
		// �������ڸ� �ϳ��� ���� ���� -> �ϳ��� �� ������ �Լ��� ���� �����ȴٴ� ����
		// �����ϵ��� ����.
		template<typename T, typename... Rest>
		void Insert_Command(const T& _Key, Rest... _Arg)
		{
			m_vec_Command.push_back(_Key);
			Insert_Command(_Arg...);
		}

		// ���÷� ������ ����
		void Insert_Command() {		}


	private:
		bool Command_Check();
		void Update();

		inline bool Up();
		inline bool UnPress();
		inline bool Down();
		inline bool Press();
		inline bool Over(float _Time);
		inline bool Over_Reset(float _Time);

	public:
		Command(const size_t _RSize);
		~Command();
	};

public:
	static const char* MSG_ERROR(INPUT_ERROR _Msg);

private:
	static std::unordered_map<std::wstring, KPtr<Command>>::iterator m_Iter_ComStart;
	static std::unordered_map<std::wstring, KPtr<Command>>::iterator m_Iter_ComEnd;
	static std::unordered_map<std::wstring, KPtr<Command>> m_Map_Command;

public:
	template<typename... Rest>
	static INPUT_ERROR Create_Command(const wchar_t* _Name, Rest... _Arg)
	{
		KPtr<Command> pKEY = Map_Find<KPtr<Command>>(m_Map_Command, _Name);

		if (nullptr != pKEY)
		{
			return INPUT_CREATE_ERROR;
		}

		Command* New_Command = new Command(sizeof...(_Arg));
		New_Command->Insert_Command(_Arg...);
		m_Map_Command.insert(std::unordered_map<std::wstring, KPtr<Command>>::value_type(_Name, New_Command));
		return INPUT_OK;
	}

	static KPtr<Command> Find_Command(const wchar_t* _Name);

public:
	static bool Is_Key(const wchar_t* _Name);

	static bool Up(const wchar_t* _Name);
	static bool UnPress(const wchar_t* _Name);
	static bool Down(const wchar_t* _Name);
	static bool Press(const wchar_t* _Name);
	static bool Over(const wchar_t* _Name, float _Time);
	static bool Over_Reset(const wchar_t* _Name, float _Time);

private:
	static  void Update();

private:
	InputManager();
	~InputManager();
};

