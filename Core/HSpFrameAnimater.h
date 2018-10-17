#pragma once
#include "HComponent.h"
#include <unordered_map>
#include "RefBase.h"

class HSpRenderer;
class HSpFrameAnimater : public HComponent
{
private:
	class SpriteAni : public HRefBase
	{
	public:
		friend HSpFrameAnimater;

	private:
		std::wstring m_AniName;
		std::wstring m_ImageName;
		size_t		 m_Start;
		size_t		 m_End;
		size_t		 m_Inter;
		float	     m_FrameSpeed;
		bool         m_bLoop;
	};

private:
	HPTR<HSpRenderer> m_SpRender;
	HPTR<SpriteAni> m_CurAni;
	size_t		 m_CurFrame;
	float	     m_CurFrameTime;
	std::unordered_map<std::wstring, HPTR<SpriteAni>> m_AniMap;

public:
	void Update();

	void SettingSpRender(HPTR<HSpRenderer> _SpRender);
	bool CreateAniT(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, float FrameSpeed = 0.1f, bool _bLoop = true);
	bool CreateAniL(const wchar_t* _AniName, const wchar_t* _ImageName, size_t _Start, size_t _End, bool _bLoop = true, float FrameSpeed = 0.1f);

	bool ChangeAni(const wchar_t* _AniName, size_t _StartIndex = 0);


public:
	HSpFrameAnimater();
	~HSpFrameAnimater();
};

