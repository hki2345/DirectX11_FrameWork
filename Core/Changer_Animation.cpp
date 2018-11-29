#include "Changer_Animation.h"



Changer_Animation::Changer_Animation()
{
}


Changer_Animation::~Changer_Animation()
{
}



KPtr<Changer_Animation::Ani_Clip> Changer_Animation::Create_AniClip(const wchar_t* _Name, const int& _Start, const int& _End)
{
	if (0 == _Name[0])
	{
		BBY;
		return nullptr;
	}

	Changer_Animation::Ani_Clip* NC = new Ani_Clip(_Name, _Start, _End);

	m_ACMap.insert(std::map<std::wstring, KPtr<Changer_Animation::Ani_Clip>>::value_type(NC->ws_name(), NC));

	return NC;
}

bool Changer_Animation::Erase_AniClip(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Changer_Animation::Ani_Clip>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}


	m_ACMap.erase(FI);
	return true;
}



bool Changer_Animation::Set_AniClip(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Changer_Animation::Ani_Clip>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}

	m_CurClip = FI->second;

	return true;
}
