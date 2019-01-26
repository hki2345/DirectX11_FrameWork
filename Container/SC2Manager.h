#pragma once
#include <DXContainer.h>
#include <Begin_Updater.h>

#include <list>
#include <map>
#include <string.h>



// ���µ�� �����Ȳ�� �����Ѵ�.
class SC2_Force;
class SC2_Camera;
class Force_Unit;
class Renderer_Terrain;
class SC2Manager : public Begin_Updater
{
private:
	enum GAMERUN_TYPE
	{
		GRT_PAUSE,
		GRT_PLAY,
		GRT_ERROR,
	};


public:
	bool m_GameSet;

private:
	bool m_bInit;
	GAMERUN_TYPE m_bGR;


	std::map<std::wstring, KPtr<SC2_Force>> m_FMap;
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_SFI;
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_EFI;

	std::map<std::wstring, KPtr<SC2_Camera>> m_CMap;
	std::map<std::wstring, KPtr<SC2_Camera>>::iterator m_SCI;
	std::map<std::wstring, KPtr<SC2_Camera>>::iterator m_ECI;



public:

	std::map<std::wstring, KPtr<SC2_Force>>* force_map()
	{
		return &m_FMap;
	}

	KPtr<SC2_Force> Create_Camera(const wchar_t* _Name, const KVector2& _Size, const KVector2& _Pos);
	KPtr<SC2_Force> Find_Camera(const wchar_t* _Name);


	KPtr<SC2_Force> Create_Force(const wchar_t* _Name, const KColor& _Color);
	KPtr<SC2_Force> Find_Force(const wchar_t* _Name);
	KPtr<SC2_Force> Find_Force(const KColor& _Color);

	void Save(const wchar_t* _Name);
	void Load(const wchar_t* _Name, KPtr<Renderer_Terrain> _Ter);
	void Clear_Force();


	std::list<KPtr<Force_Unit>>* force_unit_list(const wchar_t* _Name);



	void game_run(const GAMERUN_TYPE& _Value)
	{
		m_bGR = _Value;
	}
	GAMERUN_TYPE game_run()
	{
		return m_bGR;
	}

	bool Init() override;
	void Update() override;

public:
	SC2Manager();
	~SC2Manager();
};

