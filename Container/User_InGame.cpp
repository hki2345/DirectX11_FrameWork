#include "Controll_User.h"

#include "Con_Class.h"
#include "SC2Manager.h"
#include "SC2_Force.h"

#include "Force_Unit.h"

#include <State.h>
#include <KBox_Col.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <KWindow.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <Renderer_UI.h>

#include <KFont.h>
#include <SoundPlayer.h>


void Controll_User::Init_Game()
{
	m_pFont = ResourceManager<KFont>::Find(L"Kostar");

	m_MType = MOVE_TYPE::MT_IDLE;
	m_AType = ACT_TYPE::AT_IDLE;
	m_OType = OPTI_TYPE::OT_ODIN;
	m_pUnit->Get_Component<KBox_Col>()->StayFunc(L"Game User", this, &Controll_User::Update_GameCol);
	m_pUnit->Get_Component<KBox_Col>()->ExitFunc(L"Game User", this, &Controll_User::Update_GExitCol);
	m_uCover->one()->Active_Off();
	m_uConsole->one()->Active_Off();
	m_uFKey->one()->Active_Off();

	m_uConsole->alpha_value(.8f);

	m_InGame = true;
	m_OutGame = false;

	m_GameTime = .0f;
	m_iStory = 0;
	m_OpTime = 1000000.0f;
}

void Controll_User::Update_Game()
{
	Update_Story();
	if (nullptr != m_pStoryUnit)
	{
		KLOG(m_pStoryUnit->name());
		KLOG(L"Story Num: %d", m_iStory);

		if (L"QUEENCHAMBER" == m_pStoryUnit->ws_name())
		{
			m_OutGame = true;
		}

		if (true == KEY_DOWN(L"F"))
		{
			if (m_AType == Controll_User::AT_STORY)
			{
				Update_Story();
				return;
			}

			SoundPlayer SCount = SoundPlayer();
			SCount.Play(L"UI_BnetSelect01_1.wav");

			if (L"TYCHUS" == m_pStoryUnit->ws_name())
			{
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_TYCHUS;
				m_iStory = 0;
			}
			if (L"NOVA" == m_pStoryUnit->ws_name())
			{
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_NOVA;
				m_iStory = 0;
			}
			if (L"COMMANDCENTER" == m_pStoryUnit->ws_name())
			{
				m_MType = Controll_User::MT_NONE;
				m_AType = Controll_User::AT_STORY;
				m_SType = Controll_User::ST_COMMAND;
				m_iStory = 0;
			}
		}
	}
}


void Controll_User::Update_Story()
{
	if (true == m_InGame)
	{
		m_GameTime += DELTATIME;
		m_uCover->one()->Active_On();

		if (m_GameTime < 3.0f)
		{
			m_uCover->alpha_value(1.0f - m_GameTime * .33f);
		}
		else
		{
			m_GameTime = .0f;
			m_InGame = false;
			m_uCover->one()->Active_Off();
		}
	}

	if (true == m_OutGame)
	{
		m_GameTime += DELTATIME;
		m_uCover->one()->Active_On();

		if (m_GameTime < 3.0f)
		{
			m_uCover->alpha_value(m_GameTime * .33f);
		}
		else
		{
			Con_Class::s2_manager()->m_GameSet = true;
		}
	}
}


void Controll_User::Update_GameCol(KCollision* _Left, KCollision* _Right)
{
	m_pStoryUnit = _Left->Get_Component<Force_Unit>();
	if (m_pStoryUnit == m_pUnit)
	{
		m_pStoryUnit = _Right->Get_Component<Force_Unit>();

		if (m_pStoryUnit == m_pUnit)
		{
			m_pStoryUnit = nullptr;
			return;
		}

		return;
	}
	m_pStoryUnit = nullptr;
	return;
}


void Controll_User::Update_GExitCol(KCollision* _Left, KCollision* _Right)
{
	m_pStoryUnit = nullptr;
}

void Controll_User::UIRender()
{
	float FontSize = 25.0f;
	switch (m_SType)
	{
	case Controll_User::ST_TYCHUS:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"����, ��ٸ����� ������ٰ�\n��� �� �������� �ʰھ�?",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"�� �� �� ģ���� ���� �ʹٸ�\n �����϶��.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (3 == m_iStory)
		{
			m_pFont->Draw_Font(L"���׸� ��� ���� ���ڰ� ģ��", KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_ODIN;
				m_OpTime = 50.0f;

				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}

		break;
	case Controll_User::ST_NOVA:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"�츮 ��ġ�ɿ���\n���ѷ� �߷����� �潺ũ��\n��Ű�� ���� ���´µ�...\n \n���� ��� ����\n����� ������ �߽��̱���.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize * 3.0f)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"�� �������� ����� ���͵帮��.\n�� �༮�� ȭ�� �� ���� �̴ϴ�.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");

				Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->Set_Active(true);
				Con_Class::s2_manager()->Find_Force(L"GLASSBEAD")->playable_type(PLAYABLE_TYPE::PBT_ENEMY);
				++m_iStory;
			}
		}
		else if (3 == m_iStory)
		{
			m_pFont->Draw_Font(L"���� ���� ������.\n�� ���̳� ��.", KVector2(kwindow()->size().x * .5f,
				kwindow()->size().y * .5f/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_NOVA;
				m_OpTime = 20.0f;
				
				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}
		break;
	case Controll_User::ST_COMMAND:
		m_uConsole->one()->Active_On();
		if (0 == m_iStory)
		{
			if (true == KEY_DOWN(L"F"))
			{
				++m_iStory;
			}
		}
		else if (1 == m_iStory)
		{
			m_pFont->Draw_Font(L"�����Ͻ� ���� ���� ������\n���������� �����߽��ϴ�.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f
					/* - m_Start->one()->Trans()->pos_local().y*/ - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				Con_Class::s2_manager()->Find_Force(L"POP+STARS")->Set_Active(true);
				Con_Class::s2_manager()->Find_Force(L"POP+STARS")->playable_type(PLAYABLE_TYPE::PBT_ENEMY);

				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				++m_iStory;
			}
		}
		else if (2 == m_iStory)
		{
			m_pFont->Draw_Font(L"���� �����ð��Դϴ�.\n������ ��ٸ��ڽ��ϴ�. �����.",
				KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - FontSize * .5f - FontSize)
				, FontSize, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

			if (true == KEY_DOWN(L"F"))
			{
				SoundPlayer SCount = SoundPlayer();
				SCount.Play(L"UI_BnetSelect01_1.wav");
				m_OType = OPTI_TYPE::OT_HYPERION;
				m_OpTime = 120.0f;

				m_SType = Controll_User::ST_NONE;
				m_AType = Controll_User::AT_IDLE;
				m_MType = Controll_User::MT_IDLE;
				m_uConsole->one()->Active_Off();
				m_iStory = 0;
			}
		}
		break;
	default:
		break;
	}
}