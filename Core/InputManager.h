#pragma once
#include <vector>
#include <unordered_map>
#include "KMacro.h"
#include "DXContainer.h"

#include "Stl_AID.h"
#include "SmartPtr.h"



#define KEY_PRESS(ACTION) InputManager::Press(L##ACTION)
#define KEY_UP(ACTION) InputManager::Up(L##ACTION)
#define KEY_DOWN(ACTION) InputManager::Down(L##ACTION)
#define KEY_UNPRESS(ACTION) InputManager::UnPress(L##ACTION)
#define KEY_OVER(ACTION) InputManager::Over(L##ACTION)
#define KEY_OVER_RESET(ACTION) InputManager::Over_Reset(L##ACTION)


// 많은 라이브러리에서 쓰듯이 이렇게 에러를 미리 지정해놓고 불러오는 식이다.
enum INPMGR_ERR
{
	IERR_OK,
	IERR_CREATE_ERROR,
	IERR_CREATE_FIND_ERROR,
	IERR_MAX,
};

class KCore;
class InputManager
{
private:
	InputManager();
	~InputManager();
	
public:
	friend KCore;

public:
	static const char KM_ERRORMSG[IERR_MAX][256];

private:
	// 비트단위 연산
	// 이번에는 키를 char 정보 하나에 담아 저장하도록 한다.
	// 가독성은 떨어지지만 메모리를 아낄 수 있다. -> 아껴봤자 새발의 피이다.
	// 이런데서 메모리 아끼려 하지말아라 -> 일단은 시험삼아 경험해보는 것
	class Input_Command : public SmartPtr
	{
	private:
		static const char g_bUp;
		static const char g_bUpStay;
		static const char g_bDown;
		static const char g_bDownStay;
		static const char g_brUp;
		static const char g_brUpStay;
		static const char g_brDown;
		static const char g_brDownStay;

	public:
		friend InputManager;

	private:
		std::vector<int> m_KeyDataList;
		char m_Data;


		// 추가된 점 ->> 누르고 있는 시간을 측정하여 Over를 측정할 수 있음 -> 
		// 일정시간 누르고 있는가
		float m_PressTime;

	public:
		template<typename T, typename... Rest>
		void InsertKeyData(const T& _Key, Rest... _Arg)
		{
			m_KeyDataList.push_back(_Key);
			InsertKeyData(_Arg...);
		}

		void InsertKeyData()		{		}


	private:
		bool AllKeyCheck();
		void Update();
		
		inline bool Up();
		inline bool UpPress();
		inline bool Down();
		inline bool Press();
		inline bool Over(float _Time);
		inline bool Over_Reset(float _Time);

	public:
		Input_Command(const size_t _RSize);
		~Input_Command();
	};

	/******************* MOUSE *******************/

private:
	static POINT m_iPoint;
	static KVector2 m_MousePos;
	static KVector2 m_OriMousePos;
	static KVector2 m_MouseDir;

public:
	static KVector2 MousePos() {
		return m_MousePos;
	}

	static KVector2 MouseDir() {
		return m_MouseDir;
	}

public:
	static const char* ErrorMsg(INPMGR_ERR _Msg);

private:
	static std::unordered_map<std::wstring, KPtr<Input_Command>>::iterator m_KeyStartIter;
	static std::unordered_map<std::wstring, KPtr<Input_Command>>::iterator m_KeyEndIter;
	static std::unordered_map<std::wstring, KPtr<Input_Command>> m_KeyMap;

public:
	template<typename... Rest>
	static INPMGR_ERR CreateKey(const wchar_t* _Name, Rest... _Arg) 
	{
		KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);

		if (nullptr != pKEY)
		{
			return IERR_CREATE_ERROR;
		}

		// 템플릿 가변인자.
		// 구분 -> 이게 뭐지?
		// sizeof...(_Arg) 가변인자의 개수를 리턴해줍니다.
		Input_Command* NewKeyData = new Input_Command(sizeof...(_Arg));
		NewKeyData->InsertKeyData(_Arg...);
		m_KeyMap.insert(std::unordered_map<std::wstring, KPtr<Input_Command>>::value_type(_Name, NewKeyData));
		return IERR_OK;
	}

public:
	static bool IsKey(const wchar_t* _Name);

	static bool Up(const wchar_t* _Name);
	static bool UpPress(const wchar_t* _Name);
	static bool Down(const wchar_t* _Name);
	static bool Press(const wchar_t* _Name);
	static bool Over(const wchar_t* _Name, float _Time);
	static bool Over_Reset(const wchar_t* _Name, float _Time);

private:
	static  void Update();


};

