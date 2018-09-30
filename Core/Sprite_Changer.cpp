#include "Sprite_Changer.h"
#include "KMacro.h"
#include "Stl_Assistor.h"
#include "TimeManager.h"
#include "Renderer_Sprite.h"

Sprite_Changer::Sprite_Changer()
{
}


Sprite_Changer::~Sprite_Changer()
{
}

void Sprite_Changer::set_spirte(KPtr<Renderer_Sprite> _Value)
{
	m_ThisRenderer = _Value;
}

bool Sprite_Changer::create_piece_time(const wchar_t* _PName, const wchar_t* _IName,
	const size_t& _Start, const size_t& _End, const float& _speed, const bool& _Loop /*= true*/)
{
	create_piece_loop(_PName, _IName, _Start, _End, _speed, _Loop);
	return true;
}

bool Sprite_Changer::create_piece_loop(const wchar_t* _PName, const wchar_t* _IName,
	const size_t& _Start, const size_t& _End, const float& _speed, const bool& _Loop /*= true*/)
{
	Piece* NewPiece = new Piece();

	NewPiece->m_PieceName = _PName;
	NewPiece->m_SpriteName = _IName;
	NewPiece->m_Start = _Start;
	NewPiece->m_End = _End;
	NewPiece->m_loop = _Loop;
	NewPiece->m_FrameSpeed = _speed;
	NewPiece->m_NumPiece = NewPiece->m_End - NewPiece->m_Start + 1;
	m_PieceMap.insert(std::unordered_map<std::wstring, KPtr<Piece>>::value_type(_PName, NewPiece));
	return true;
}

bool Sprite_Changer::change_piece(const wchar_t* _Name, const size_t& _Start/* = 0*/)
{
	// ������ ������ �翬�� ������ ��������
	KASSERT(nullptr == m_ThisRenderer);
	KPtr<Piece> NewPiece = Map_Find<KPtr<Piece>>(m_PieceMap, _Name);
	KASSERT(nullptr == NewPiece);
	if (nullptr == NewPiece)
	{
		return false;
	}

	m_CurPiece = NewPiece;
	m_CurFrame = m_CurPiece->m_Start + _Start;
	m_CurFrameTime = .0f;
	m_ThisRenderer->image(m_CurPiece->m_SpriteName.c_str());

	return true;
}

void Sprite_Changer::Update()
{
	if (nullptr != m_CurPiece)
	{
		m_CurFrameTime += DELTATIME;

		// ó���ӵ��� ���� ���� ���� ��� �س���.
		// ���� ���̶� �� ���� �Ѿ�� �ϴ���
		size_t Count = (size_t)(m_CurFrameTime / m_CurPiece->m_FrameSpeed);

		// �� ���̶� �Ѿ�� �ӷ���� �ð� �ʱ�ȭ
		if (0 < Count)
		{
			// ���� �ʱ�ȭ�� �ϴ� �� �ƴ϶� ���� ��Ų��. -> �ܿ� �ð����� �Ű� ���ٴ� ��
			m_CurFrameTime -= m_CurPiece->m_FrameSpeed * Count;
		}

		// ������ �ƴ� ��� ������ �ִ�� �����ȴ�.
		if (false == m_CurPiece->m_loop && m_CurPiece->m_End <= m_CurFrame + Count)
		{
			m_CurFrame = m_CurPiece->m_End;
			m_ThisRenderer->index(m_CurFrame);
			return;
		}

		size_t ActingFrame = m_CurFrame - m_CurPiece->m_Start;
		Count += ActingFrame;

		if (Count > m_CurPiece->m_NumPiece)
		{
			Count %= m_CurPiece->m_NumPiece;
		}

		// �̰� 0�����Ӻ��� �� ��
		m_CurFrame = Count + m_CurPiece->m_Start;

		// ũ�� �� ��� ������ �ʿ���
		if (m_CurPiece->m_End + 1 <= m_CurFrame)
		{
			m_CurFrame = m_CurPiece->m_Start;
		}

		m_ThisRenderer->index(m_CurFrame);
	}
}