#include "DebugManager.h"
#include "Core_Class.h"
#include "KWindow.h"
#include "Mesh.h"
#include "Material.h"
#include "ResourceManager.h"
#include "DirectXHeader_DE.h"
#include "KFont.h"

#include "TransPosition.h"
#include "Renderer_Sprite.h"


bool DebugManager::m_Debug = true;
KColor DebugManager::m_LogColor = KColor::White;
float DebugManager::m_LogSize = 15.0f;
std::wstring	DebugManager::Messege;

std::list<std::wstring>::iterator DebugManager::m_S_Log;
std::list<std::wstring>::iterator DebugManager::m_E_Log;
std::list<std::wstring> DebugManager::m_LogList;



DebugManager::DebugManager()
{
}


DebugManager::~DebugManager()
{
}

void DebugManager::draw_rect(const KVector2& _Pos, const KVector2& _Size, const KColor& _Color, const float& _Border)
{
	draw_rect(KRect(_Pos, _Size), _Color, _Border);
}

void DebugManager:: draw_rect(const KRect& _Rect, const KColor& _Color, const float& _Border)
{
	if (false == m_Debug)
	{
		return;
	}

	if ( nullptr == Core_Class::Main_Window().this_StateManager.state()->camera())
	{
		return;
	}

	KVector2 m_Size = Core_Class::Main_Window().this_StateManager.state()->camera()->screen_size();

	KMatrix m_Scale;
	KMatrix m_Pos;
	KMatrix m_VP = Core_Class::Main_Window().this_StateManager.state()->camera()->View_Proj();

	m_Scale.Scale(KVector(_Rect.m_Size.x, _Rect.m_Size.y, 0.1f));
	m_Pos.Translation(KVector(
		_Rect.m_Pos.x - Core_Class::main_state()->camera()->one()->Local_Pos().x,
		_Rect.m_Pos.y - Core_Class::main_state()->camera()->one()->Local_Pos().y, 10.1f));

	
	KPtr<Material> m_Mat = ResourceManager<Material>::Find(L"LINEMAT");
	KPtr<Mesh> m_Mesh = ResourceManager<Mesh>::Find(L"LINEMESH");
	KBorder NewBorder;

	NewBorder.m_Color = _Color;
	NewBorder.m_UvPerPixel = KVector2(1 / _Rect.m_Size.x, 1 / _Rect.m_Size.y);
	NewBorder.m_Border = _Border;

	m_Mat->VShader()->Set_ConstBuffer<KMatrix>(L"TRANS", (m_Scale * m_Pos * m_VP).TransPose_Referance());
	m_Mat->PShader()->Set_ConstBuffer<KBorder>(L"BORDER", NewBorder);

	m_Mat->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}


void DebugManager::draw_font(const wchar_t* _pStr, const KVector2& _Pos, const float& _fSize, const KColor& _COLOR /*= KColor::Black*/, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/)
{
	if (false == m_Debug)
	{
		return;
	}

	KPtr<KFont> m_Font = ResourceManager<KFont>().Find(L"돋움");
	m_Font->draw_font(_pStr, _Pos, _fSize, _COLOR.color_to_reverse255(), _Flag);
}


void DebugManager::WLog(const wchar_t* const _Str, ...)
{
	// 가변인자 받는 매크로 맨 밑에 end와 같이 쓰임
	
	// Ap -> 가변인자 들어가는 리스트
	// va_start -> 가변인자 시작지점
	// va_arg -> 뒤에 인자값 만큼 포인터를 이동시켜줌 -> 리턴값 그 포인터의 값을 반환
	// [단위 참고] char, short 의 경우에는 int로 대신 쓰고,
	// flaot의 경우에는 double로 대신 쓴 이후 형 변환을 해주어야 한다고 합니다. 
	// (예. char ch = (char) va_arg(ap, int); )
	// va_end -> 맨 뒤에 end를 넣어 가변인자의 사용을 종료한다.ㄴ
	va_list Ap;
	va_start(Ap, _Str);
	const wchar_t* TextPar = _Str;
	std::wstring TempString;

	while (*TextPar != 0)
	{
		if ('%' == *TextPar)
		{
			TextPar += 1;

			switch (*TextPar)
			{
			case 'd':
			{
				std::wstring NumTemp;
				NumTemp = std::to_wstring((int)(va_arg(Ap, int)));
				TempString += NumTemp;

				break;
			}
			case 'c':
			{
				TempString += (char)(va_arg(Ap, int));
				break;
			}
			case 'f':
			{
				std::wstring NumTemp;
				NumTemp = std::to_wstring((float)(va_arg(Ap, double)));
				TempString += NumTemp;

				break;
			}
			case 'b':
			{
				std::wstring NumTemp;

				if (0 == (va_arg(Ap, int)))
				{
					NumTemp = L"false";
				}
				else
				{
					NumTemp = L"true";
				}
				TempString += NumTemp;

				break;
			}
			default:
				break;
			}

			TextPar += 1;
		}
		else {
			TempString += *TextPar;
			TextPar += 1;
		}
	}

	m_LogList.push_back( TempString.c_str());

	va_end(Ap);
}

void DebugManager::Logging()
{
	m_S_Log = m_LogList.begin();
	m_E_Log = m_LogList.end();

	KVector2 TempPos = { 10.0f, 10.0f };

	for (; m_S_Log != m_E_Log; ++m_S_Log)
	{
		draw_font((wchar_t*)(*m_S_Log).c_str(), TempPos, m_LogSize, m_LogColor);
		TempPos.y += m_LogSize;
	}

	m_LogList.clear();
}