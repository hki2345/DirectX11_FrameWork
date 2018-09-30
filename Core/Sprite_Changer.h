#pragma once
#include "Component.h"
#include <unordered_map>


// 2D �ִϸ��̼��� ���̻� �������� �ƴѴ� -> ���� ������ ������ �ʴ´�.
// �ٸ� ��Ƽ �̹������� ������ �ϴµ� �ű⼭ ���̴� �ε����� �ٲپ��ִ� ������ �� ��
class Renderer_Sprite;
class Sprite_Changer : public Component
{
private:
	class Piece : public SmartPtr
	{
	public:
		friend Sprite_Changer;

	private:
		std::wstring m_PieceName;
		std::wstring m_SpriteName;
		size_t m_Start;
		size_t m_End;
		size_t m_NumPiece;
		float m_FrameSpeed;
		bool m_loop;
	};

private:
	KPtr<Renderer_Sprite> m_ThisRenderer;
	KPtr<Piece> m_CurPiece;
	size_t m_CurFrame;
	float m_CurFrameTime;
	std::unordered_map<std::wstring, KPtr<Piece>> m_PieceMap;

public:
	void Update();

	void set_spirte(KPtr<Renderer_Sprite> _Renderer);
	bool create_piece_time(const wchar_t* _PName, const wchar_t* _IName, 
		const size_t& _Start, const size_t& _End, const float& _speed, const bool& _Loop = true);
	bool create_piece_loop(const wchar_t* _PName, const wchar_t* _IName,
		const size_t& _Start, const size_t& _End, const float& _speed, const bool& _Loop = true);

	bool change_piece(const wchar_t* _Name, const size_t& _Start = 0);

public:
	Sprite_Changer();
	~Sprite_Changer();
};

