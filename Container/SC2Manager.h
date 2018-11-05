#pragma once
#include <Begin_Updater.h>
#include <map>
#include <stireg.h>



class SC2_Force;
class SC2Manager : public Begin_Updater
{
private:
	enum GAMERUN_TYPE
	{
		GRT_PAUSE,
		GRT_PLAY,
		GRT_ERROR,
	};


private:
	GAMERUN_TYPE m_bGR;


	std::map<std::wstring, KPtr<SC2_Force>> m_FMap;
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_SFI;
	std::map<std::wstring, KPtr<SC2_Force>>::iterator m_EFI;



public:
	bool Init() override;
	void Update() override;

public:
	SC2Manager();
	~SC2Manager();
};

