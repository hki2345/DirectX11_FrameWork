#include "DebugManager.h"
#include "Core_Class.h"
#include "KWindow.h"
#include "KMesh.h"
#include "KMaterial.h"
#include "ResourceManager.h"
#include "Sampler.h"
#include "KFont.h"
#include "RenderTarget_Multi.h"


bool DebugManager::m_bDebug = true;
KColor DebugManager::m_LogColor = KColor::White;
float DebugManager::m_LogSize = 15.0f;
std::wstring	DebugManager::Messege;
//
//std::list<std::wstring>::iterator DebugManager::m_S_Log;
//std::list<std::wstring>::iterator DebugManager::m_E_Log;
//std::list<std::wstring> DebugManager::m_LogList;

std::list<DebugManager::LogMsg>::iterator DebugManager::m_S_Log;
std::list<DebugManager::LogMsg>::iterator DebugManager::m_E_Log;
std::list<DebugManager::LogMsg> DebugManager::m_LogList;


DebugManager::DebugManager()
{
}


DebugManager::~DebugManager()
{
}

void DebugManager::Draw_Rect(KVector2 _Pos, KVector2 _Size, float _Border/* = 1*/)
{
	Draw_Rect(KRect( _Pos , _Size ), _Border);
}
// ī�޶� ����� �������� ����.
void DebugManager::Draw_Rect(const KRect& _Rect, float _Border)
{
	if (nullptr == Core_Class::MainWindow().statemanager.cur_state()->Camera())
	{
		return;
	}

	KVector2 m_Size = Core_Class::MainWindow().statemanager.cur_state()->Camera()->screen_size();

	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewToLH(KVector4::Zero, KVector4::Forword, KVector4::Up);
	m_Proj.OrthLH(m_Size.x, m_Size.y, 0.1f, 1000.0f);

	KMatrix m1 = Core_Class::MainWindow().statemanager.cur_state()->Camera()->ViewProj();
	KMatrix m2 = m_View * m_Proj;

	KMatrix m_Scale;
	m_Scale.Scale(KVector4(_Rect.m_Size.x, _Rect.m_Size.y, 1.0f));
	KMatrix m_Pos;
	m_Pos.Trans(KVector4(_Rect.m_Pos.x, _Rect.m_Pos.y, 1.1f));

	KPtr<KMesh> Mesh = ResourceManager<KMesh>::Find(L"LINERECTMESH");
	KPtr<KMaterial> Mat = ResourceManager<KMaterial>::Find(L"DEBUGRECTMAT");
	Mat->VShader()->SettingCB<KMatrix>(L"TRANS", (m_Scale * m_Pos * m1).RTranspose());

	Mat->Update();
	Mesh->Update();
	Mesh->Render();
}

void DebugManager::Draw_Font(wchar_t* _pStr, KVector2 _Pos, float _fSize, KVector4 _COLOR, FW1_TEXT_FLAG _Flag /*= FW1_TEXT_FLAG::FW1_TOP*/) {

	KPtr<KFont> FindFont = ResourceManager<KFont>::Find(L"�³���");

	if (nullptr != FindFont)
	{
		FindFont->Draw_Font(_pStr, _Pos, _fSize, _COLOR.color_to_reverse255(), _Flag);

		// FindFont->Draw_Font(_pStr, _Pos, _fSize, 0xffffffff, _Flag);
	}
	Core_Class::MainDevice().ResetContext();
}

void DebugManager::Draw_Log(const wchar_t* const _Str, ...)
{
	// �������� �޴� ��ũ�� �� �ؿ� end�� ���� ����

	// Ap -> �������� ���� ����Ʈ
	// va_start -> �������� ��������
	// va_arg -> �ڿ� ���ڰ� ��ŭ �����͸� �̵������� -> ���ϰ� �� �������� ���� ��ȯ
	// [���� ����] char, short �� ��쿡�� int�� ��� ����,
	// flaot�� ��쿡�� double�� ��� �� ���� �� ��ȯ�� ���־�� �Ѵٰ� �մϴ�. 
	// (��. char ch = (char) va_arg(ap, int); )
	// va_end -> �� �ڿ� end�� �־� ���������� ����� �����Ѵ�.��
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

	m_LogList.push_back({ TempString.c_str(), KColor::White});

	va_end(Ap);
}

void DebugManager::Targetting()
{
	KPtr<Sampler> Smp = ResourceManager<Sampler>::Find(L"DefaultSmp");

	if (nullptr == Smp)
	{
		BBY;
	}
	std::vector<KPtr<RenderTarget_Multi>> Vec = ResourceManager<RenderTarget_Multi>::All_SingleVec();

	KPtr<KMesh> Mesh = ResourceManager<KMesh>::Find(L"RECT");
	KPtr<KMaterial> Mat = ResourceManager<KMaterial>::Find(L"TAGETDEBUGMAT");

	KMatrix m_View;
	KMatrix m_Proj;
	m_View.ViewToLH(KVector4::Zero, KVector4::Forword, KVector4::Up);
	m_Proj.OrthLH(Core_Class::MainWindow().width_f(), Core_Class::MainWindow().height_f(), 0.1f, 1000.0f);

	MatrixContainer tMatData;

	int CountX = 0;
	int CountY = 0;
	int WCount = 5;

	float SizeX = Core_Class::MainWindow().width_f() / WCount;
	float SizeY = Core_Class::MainWindow().height_f() / WCount;

	for (size_t i = 0; i < Vec.size(); i++)
	{
		std::vector<KPtr<RenderTarget>> TagetVec = Vec[i]->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Identity();
			m_Scale.Scale(KVector4(SizeX, SizeY, 1.0F));
			m_Pos.Identity();
			m_Pos.Trans(
				KVector4( (-Core_Class::MainWindow().width_f() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::MainWindow().height_f() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->target_tex()->SRV())
			{
				BBY;
			}
			TagetVec[j]->target_tex()->Update(0);

			Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", tMatData, SHTYPE::ST_VS);

			Mat->Update();
			Mesh->Update();
			Mesh->Render();

			TagetVec[j]->target_tex()->Reset(0);

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

	std::map<int, KPtr<Camera>>::iterator m_CSI = Core_Class::MainScene()->RenderMgr.m_CamMap.begin();
	std::map<int, KPtr<Camera>>::iterator m_CEI = Core_Class::MainScene()->RenderMgr.m_CamMap.end();;

	for (; m_CSI != m_CEI; ++m_CSI)
	{
		std::vector<KPtr<RenderTarget>> TagetVec = m_CSI->second->m_CameraTaget->TagetTexList();

		for (size_t j = 0; j < TagetVec.size(); j++)
		{
			KMatrix m_Scale;
			KMatrix m_Pos;

			m_Scale.Identity();
			m_Scale.Scale(KVector4(SizeX, SizeY, 1.0F));
			m_Pos.Identity();
			m_Pos.Trans(
				KVector4((-Core_Class::MainWindow().width_f() * 0.5F) + (CountX * SizeX) + (SizeX * 0.5F)
					, (Core_Class::MainWindow().height_f() * 0.5F) + (-CountY * SizeY) - (SizeY * 0.5F)
					, 1.1f));

			KMatrix m_W = m_Scale * m_Pos;

			tMatData.m_V = m_View;
			tMatData.m_P = m_Proj;
			tMatData.m_W = m_W;
			tMatData.m_WV = m_W * m_View;
			tMatData.m_WVP = tMatData.m_WV * m_Proj;
			tMatData.RTrans();

			Smp->Update(0);
			if (nullptr == TagetVec[j]->target_tex()->SRV())
			{
				BBY;
			}
			TagetVec[j]->target_tex()->Update(0);

			Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", tMatData, SHTYPE::ST_VS);

			Mat->Update();
			Mesh->Update();
			Mesh->Render();

			TagetVec[j]->target_tex()->Reset(0);

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

	KVector2 Pos = { 10.0f, 10.0f };

	for (; m_S_Log != m_E_Log; ++m_S_Log)
	{
		Draw_Font((wchar_t*)(*m_S_Log).Msg.c_str(), Pos, m_LogSize, m_S_Log->m_COLOR);
		Pos.y += m_LogSize;
	}

	// �ϴ� dir�� �����ϸ� �Ǵϱ�.
	// �ؽ�ó Ȯ���� �ߴ�.

	m_LogList.clear();
}