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
	// 렌더러 없으면 당연히 터져야 인지상정
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

		// 처리속도에 따른 다음 장을 계산 해낸다.
		// 다음 장이란 몇 장이 넘어가야 하는지
		size_t Count = (size_t)(m_CurFrameTime / m_CurPiece->m_FrameSpeed);

		// 한 장이라도 넘어가면 속력재는 시간 초기화
		if (0 < Count)
		{
			// 마냥 초기화를 하는 게 아니라 차감 시킨디ㅏ. -> 잔여 시간들을 신경 쓴다는 것
			m_CurFrameTime -= m_CurPiece->m_FrameSpeed * Count;
		}

		// 루프가 아닐 경우 프레임 최대로 지정된다.
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

		// 이건 0프레임부터 할 때
		m_CurFrame = Count + m_CurPiece->m_Start;

		// 크게 될 경우 보정이 필요함
		if (m_CurPiece->m_End + 1 <= m_CurFrame)
		{
			m_CurFrame = m_CurPiece->m_Start;
		}

		m_ThisRenderer->index(m_CurFrame);
	}
}