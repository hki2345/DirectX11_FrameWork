#pragma once
#include <vector>
#include <unordered_map>
#include "KMacro.h"
#include "DHeader.h"

#include "Stl_AID.h"
#include "SmartPtr.h"

enum KEYMGRERROR
{
	KM_OK,
	KM_CREATE_ERROR,
	KM_CREATE_FIND_ERROR,
	KM_MAX,
};

class KCore;
class InputMgr
{
public:
	friend KCore;

public:
	static const char KM_ERRORMSG[KM_MAX][256];

private:
	class KEYDATA : public SmartPtr
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
		friend InputMgr;

	private:
		std::vector<int> m_KeyDataList;
		char m_Data;
		float m_PushTime;

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
		
		inline bool IsUp();
		inline bool IsUpStay();
		inline bool IsDown();
		inline bool IsDownStay();
		inline bool IsOver(float _Time);
		inline bool IsOverReset(float _Time);

	public:
		KEYDATA(const size_t _RSize);
		~KEYDATA();
	};

private:
	static POINT m_iPoint;
	static HVEC2 m_MousePos;
	static HVEC2 m_OriMousePos;
	static HVEC2 m_MouseDir;

public:
	static HVEC2 MousePos() {
		return m_MousePos;
	}

	static HVEC2 MouseDir() {
		return m_MouseDir;
	}

public:
	static const char* ErrorMsg(KEYMGRERROR _Msg);

private:
	static std::unordered_map<std::wstring, KPtr<KEYDATA>>::iterator m_KeyStartIter;
	static std::unordered_map<std::wstring, KPtr<KEYDATA>>::iterator m_KeyEndIter;
	static std::unordered_map<std::wstring, KPtr<KEYDATA>> m_KeyMap;

public:
	template<typename... Rest>
	static KEYMGRERROR CreateKey(const wchar_t* _Name, Rest... _Arg) 
	{
		KPtr<KEYDATA> pKEY = Map_Find<KPtr<KEYDATA>>(m_KeyMap, _Name);

		if (nullptr != pKEY)
		{
			return KM_CREATE_ERROR;
		}

		// 템플릿 가변인자.
		// 구분 -> 이게 뭐지?
		// sizeof...(_Arg) 가변인자의 개수를 리턴해줍니다.
		KEYDATA* NewKeyData = new KEYDATA(sizeof...(_Arg));
		NewKeyData->InsertKeyData(_Arg...);
		m_KeyMap.insert(std::unordered_map<std::wstring, KPtr<KEYDATA>>::value_type(_Name, NewKeyData));
		return KM_OK;
	}

public:
	static bool IsKey(const wchar_t* _Name);
	static bool IsUp(const wchar_t* _Name);
	static bool IsUpStay(const wchar_t* _Name);
	static bool IsDown(const wchar_t* _Name);
	static bool IsDownStay(const wchar_t* _Name);
	static bool IsOver(const wchar_t* _Name, float _Time);
	static bool IsOverReset(const wchar_t* _Name, float _Time);

private:
	static  void Update();

private:
	InputMgr();
	~InputMgr();

};

