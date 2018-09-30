#include "stdafx.h"
#include "EditUpdater.h"

#include <KWindow.h>
#include <InputManager.h>
#include <DebugManager.h>
#include <Mouse_Col.h>
#include <KThread.h>
#include <TimeManager.h>
#include <Collider2D_DE.h>
#include <GameData.h>

#include "Dlg_TheOneTab.h"
#include "Edit_Class.h"

EditUpdater::EditUpdater()
{
}


EditUpdater::~EditUpdater()
{
}

void EditUpdater::Update_State()
{
	// �̰� ��ȯ���� �Ͼ�� -> �̰� �˾Ƴ��� �� ���� ����
	// ������ �������Ϳ��� ������Ʈ�� ��� �������� �ϴϹ������� ��������
	// FindState = Edit_Class::Cur_State();

	Update_Camera();
	Update_Trans();
	Update_Mouse();
	Update_OneTree();
	
	
	// ObjToMouseCheck();
}

void EditUpdater::Start_State()
{
	Number = 0;
	m_bTestThread = true;

	CamSpeed = 100.0f;
	debug_On();

	m_OriginScreen = window()->size();
	// KThread::Start_Thread<EditUpdater>(L"TestThread", &EditUpdater::Func, this);
}


void EditUpdater::Update_Mouse()
{
	if (true == Mouse_InScreen())
	{
		return;
	}

	float ZoomSize = 50.0f;
	KVector2 Temp = state()->camera()->screen_size();
	float ratio = Temp.y / Temp.x;

	if (KEY_PRESS("MOUSE_MB"))
	{
		if (KEY_DOWN("MOUSE_LB"))
		{
			state()->camera()->screen_size({ Temp.x - (50.0f * 1.0f), Temp.y - (50.0f * ratio) });
		}
		else if (KEY_DOWN("MOUSE_RB"))
		{
			state()->camera()->screen_size({ Temp.x + (50.0f * 1.0f), Temp.y + (50.0f * ratio) });
		}
	}

	// �ּ� �ִ� �� ����
	if (state()->camera()->screen_size().x < m_OriginScreen.x * .5f)
	{
		state()->camera()->screen_size(m_OriginScreen * .5f);
	}
	else if (state()->camera()->screen_size().x > m_OriginScreen.x)
	{
		state()->camera()->screen_size(m_OriginScreen);
	}
}

void EditUpdater::Update_Camera()
{
	if (KEY_PRESS("Up"))
	{
		state()->camera()->one()->transform()->Moving(KVector::Up * CamSpeed);
	}
	if (KEY_PRESS("Down"))
	{
		state()->camera()->one()->transform()->Moving(KVector::Down * CamSpeed);
	}
	if (KEY_PRESS("Left"))
	{
		state()->camera()->one()->transform()->Moving(KVector::Left * CamSpeed);
	}
	if (KEY_PRESS("Right"))
	{
		state()->camera()->one()->transform()->Moving(KVector::Right * CamSpeed);
	}
}


// ���콺 �� ����ٴϴ� �浹ü - �浹 üũ
void EditUpdater::ObjToMouseCheck()
{
	Edit_One();
}



void EditUpdater::Edit_One()
{
	// ���� �� ���� �� -> ��ü �̵�
	if (KEY_PRESS("MOUSE_LB"))
	{
		// �� ��Ƽ�� �˻� ������ ��Ŀ�� ��ü�� ���� ���� �����ϱ� ����
		if (true == Mouse_InScreen() && nullptr == Edit_Class::FocusOne)
		{
			// �ٽ� �ѹ� üũ -> �ֽ�ȭ
			KPtr<Collider2D_DE> m_Col = MOUSE_DOWN(Mouse_Col::MOUSE_BTN::MBTN_LEFT);
			KPtr<TheOne> TempOne;
			if (nullptr != m_Col && true == m_Col->is_Active())
			{
				TempOne = m_Col->one();

				// ���� ���� �� ������ Ż��
				if (nullptr == TempOne) { return; }

				Edit_Class::FocusOne = TempOne;
				Edit_Class::SelectOne = TempOne;
			}
		}
		else if (false == Mouse_InScreen())
		{
			Edit_Class::FocusOne = nullptr;
		}

		// ���� ��ü -> ���� �����ִ� �͸� ������ �� ����
		// + ȸ�� ��������
		if (nullptr != Edit_Class::FocusOne)
		{			
			Edit_Class::FocusOne->Local_Pos({ Core_Class::main_state()->camera()->screen_to_world(
				InputManager::mouse_pos()) , Edit_Class::FocusOne->Local_Pos().z });
		}
	}

	if (KEY_UP("MOUSE_LB"))
	{
		Edit_Class::FocusOne = nullptr;
/*
		if (nullptr != Edit_Class::SelectOne && true == Mouse_InScreen())
		{
			if (nullptr != Edit_Class::SelectOne->Get_Component<Panza>() ||
				nullptr != Edit_Class::SelectOne->Get_Component<Bird_Launcher>())
			{
				Edit_Class::m_View_Component->Dlg_Release();
				Edit_Class::m_View_Component->set_one(Edit_Class::SelectOne);
			}
		}*/
	}
}


void EditUpdater::Update_Trans()
{
	if (nullptr == Edit_Class::TransTab)
	{
		return;
	}

	if (nullptr != Edit_Class::TransTab->m_Trans && true == KEY_UP("MOUSE_LB"))
	{
		Edit_Class::TransTab->Update_EditValue();
	}
}

void EditUpdater::Update_OneTree()
{
	if (nullptr != Edit_Class::TheOneTab)
	{
	}
}

// ���콺�� ���� �۾� â���� ������� üũ��
bool EditUpdater::Mouse_InScreen()
{
	KVector CPos = Core_Class::main_state()->camera()->one()->Local_Pos();
	KVector2 MPos = state()->camera()->screen_to_world(InputManager::mouse_pos());

	if (window()->size().x_part() < fabsf(MPos.x - CPos.x) ||
		window()->size().y_part() < fabsf(MPos.y - CPos.y))
	{
		return false;
	}
	return true;
}

void EditUpdater::TestThread()
{
	if (KEY_DOWN("Quit"))
	{
		KThread::Start_Thread<EditUpdater>(L"TestThread", &EditUpdater::Func, this);

		if (true == m_bTestThread)
		{
			KThread::Pause_Thread(L"TestThread");
			CheckNumber = Number;
			m_bTestThread = false;
		}
		else {
			KThread::Recov_Thread(L"TestThread");
			m_bTestThread = true;
		}
	}
}

unsigned int EditUpdater::Func(void* _Test)
{
	Number = 0;
	float Time = 0.0f;

	KPtr<TimeManager::Timer> ptr = TimeManager::Create_Timer(L"TestTimer");

	while (true)
	{
		Time += ptr->Update();

		if (Time > 0.5f)
		{
			++Number;
			Time = 0.0f;
		}
	}

	return 0;
}

