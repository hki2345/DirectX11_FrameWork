#pragma once
#include <map>
#include "Resource.h"

class Changer_Animation : public Resource
{
public:
	class Ani_Clip : public Begin
	{
	public:
		// 이름은 Begin -> string 함수 쓰기 귀찮음
		// 짜피 걔가 다 가지고 있는뎅
		int Start;
		int End;
		float Speed;


	public:
		Ani_Clip(const wchar_t* _Name, const int& _Start, const int& _End)
			: Start(_Start), End(_End)
		{
			name(_Name);
		}

	private:
		Ani_Clip() {};
		Ani_Clip(const Ani_Clip& _Value) {};
	};

public:
	Changer_Animation();
	~Changer_Animation();


private:
	std::map<std::wstring, KPtr<Ani_Clip>> m_ACMap;
	KPtr<Ani_Clip> m_CurClip;

public:
	KPtr<Ani_Clip> Create_AniClip(const wchar_t* _Name, const int& _Start, const int& _End);
	bool Erase_AniClip(const wchar_t* _Name);
	bool Set_AniClip(const wchar_t* _Name);

	KPtr<Ani_Clip> cur_clip()
	{
		return m_CurClip;
	}
};

