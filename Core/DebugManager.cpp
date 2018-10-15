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
#include "RenderTarget_Multi.h"

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

void DebugManager::Targetting()
{
	KPtr<Sampler> Smp = ResourceManager<Sampler>::Find(L"DefaultSam");

	if (nullptr == Smp)
	{
		KASSERT(true);
	}
	std::vector<KPtr<RenderTarget_Multi>> Vec = ResourceManager<RenderTarget_Multi>::All_SingleResVec();


	// 렉트 맷은 노말값까지 가짐
	// 그냥 디버그 렉트는 순수 값들임 - 아마 Rect맷으로 될 가능성 농후
	KPtr<Mesh> TMesh = ResourceManager<Mesh>::Find(L"RECT3D");
	KPtr<Material> Mat = ResourceManager<Material>::Find(L"MT_MAT");


	// 간이 카메라
	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewTo_LH(KVector::Zero, KVector::Forword, KVector::Up);
	m_Proj.Proj_Orthographic(Core_Class::Main_Window().widthf(), Core_Class::Main_Window().heigthf(), 0.1f, 1000.0f);

	DATA_3D tMatData;

	int CountX = 0;
	int CountY = 0;
	int WCount = 5;

	float SizeX = Core_Class::Main_Window().widthf() / WCount;
	float SizeY = Core_Class::Main_Window().heigthf() / WCount;

	for (size_t i = 0; i < Vec.size(); i++)
	{
		std::vector<KPtr<RenderTarget>> TagetVec = Vec[i]->RenderTargetList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Identity();
			m_Scale.Scale(KVector(SizeX, SizeY, 1.0F));
			m_Pos.Identity();
			m_Pos.Translation(
				KVector((-Core_Class::Main_Window().widthf() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::Main_Window().heigthf() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.Transpose_Ref();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->texture()->Shader_RescourceView())
			{
				KASSERT(true);
			}
			TagetVec[j]->texture()->Update(0);

			Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", tMatData, SHADER_TYPE::ST_VS);

			Mat->Update();
			TMesh->Update();
			TMesh->Render();

			TagetVec[j]->texture()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % 5))
			{
				++CountY;
				CountX = 0;
			}
		}
	}

	CountX = 0;
	CountY += 1;


	std::map<int, KPtr<Camera>>::iterator m_SC = Core_Class::main_state()->this_RenderManager.m_CameraMap.begin();
	std::map<int, KPtr<Camera>>::iterator m_EC = Core_Class::main_state()->this_RenderManager.m_CameraMap.end();

	for (; m_SC != m_EC; ++m_SC)
	{
		std::vector<KPtr<RenderTarget>> TempVec = m_SC->second->m_MTarget->RenderTargetList();

		for (size_t j = 0; j < TempVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Identity();

			m_Scale.Scale(KVector(SizeX, SizeY, 1.0F));
			m_Pos.Identity();
			m_Pos.Translation(
				KVector((-Core_Class::Main_Window().widthf() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::Main_Window().heigthf() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			// m_Scale.Scale(KVector(Core_Class::Main_Window().widthf(), Core_Class::Main_Window().heigthf(), 1.0F));
			// m_Pos.Identity();
			// m_Pos.Translation(KVector(.0f, .0f, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.Transpose_Ref();

			Smp->Update(0);
			if (nullptr == TempVec[j]->texture()->Shader_RescourceView())
			{
				KASSERT(true);
			}
			TempVec[j]->texture()->Update(0);

			Core_Class::MainDevice().Set_DeviceCB<DATA_3D>(L"DATA3D", tMatData, SHADER_TYPE::ST_VS);

			Mat->Update();
			TMesh->Update();
			TMesh->Render();

			TempVec[j]->texture()->Reset(0);

			++CountX;

			if (0 != CountX && 0 == (CountX % 5))
			{
				++CountY;
				CountX = 0;
			}
		}
	}
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