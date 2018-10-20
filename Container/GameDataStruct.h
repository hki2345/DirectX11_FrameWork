#pragma once

class SpriteData
{
public:
	wchar_t ImagePath[256];
	wchar_t FileName[256];
	size_t CutX;
	size_t CutY;

public:
	SpriteData() : CutX(1), CutY(1)
	{

	}
};


// class MonsterData
// {
// public:
// 	KVector4 Pos;
// 	KVector4 Scale;
// 	wchar_t AniSpriteName[256];
// 	HVEC2 AttAniIndex;
// 	HVEC2 DeathAniIndex;
// };