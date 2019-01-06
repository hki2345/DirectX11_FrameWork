#include "Controll_User.h"
#include "Force_Unit.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>
#include <KWindow.h>

#include "SC2_Camera.h"


Controll_User::Controll_User()
{
}


Controll_User::~Controll_User()
{
}


void Controll_User::Set_Render()
{
	m_RList = one()->Get_Component_List<Renderer_BonAni>();
}

void Controll_User::Add_Render(KPtr<Renderer_BonAni> _Other)
{
	m_RList.push_back(_Other);
}

void Controll_User::Del_Render(KPtr<Renderer_BonAni> _Other)
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			m_RList.erase(m_SRI);
			return;
		}
	}
}

bool Controll_User::Init(KPtr<Renderer_Terrain> _Terrain, KPtr<Force_Unit> _Unit, KPtr<SC2_Camera> _Cam)
{
	if (false == IS_KEY(L"MFOR"))
	{
		InputManager::Create_Command(L"MFOR", 'W');
	}
	if (false == IS_KEY(L"MBACK"))
	{
		InputManager::Create_Command(L"MBACK", 'S');
	}
	if (false == IS_KEY(L"MLEFT"))
	{
		InputManager::Create_Command(L"MLEFT", 'A');
	}
	if (false == IS_KEY(L"MRIGHT"))
	{
		InputManager::Create_Command(L"MRIGHT", 'D');
	}

	if (false == IS_KEY(L"SHOT"))
	{
		InputManager::Create_Command(L"SHOT", VK_LBUTTON);
	}
	if (false == IS_KEY(L"BOMB"))
	{
		InputManager::Create_Command(L"BOMB", VK_RBUTTON);
	}


	if (false == IS_KEY(L"HEAL"))
	{
		InputManager::Create_Command(L"HEAL", 'E');
	}
	if (false == IS_KEY(L"RUN"))
	{
		InputManager::Create_Command(L"RUN", VK_LSHIFT);
	}
	if (false == IS_KEY(L"OPTI"))
	{
		InputManager::Create_Command(L"OPTI", 'Q');
	}


	if (nullptr == _Terrain)
	{
		BBY;
	}


	m_pTer = _Terrain;
	m_pUnit = _Unit;
	m_pCam = _Cam;
	m_pCam->Set_User(this);

	m_MType = MOVE_TYPE::MT_IDLE;
	m_AType = ACT_TYPE::AT_IDLE;

	m_MirrorY = false;

	return true;
}


void Controll_User::Update()
{
	if (SC2_Camera::SC2_CAMMODE::S2M_EDIT == m_pCam->cam_mode())
	{
		InputManager::Set_MUnLock();
		return;
	}

	Update_Move();
	Update_Act();
	Update_Terrain();
	Update_Mouse();
	Update_RenCol();
}

void Controll_User::Update_RenCol()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->rot_pivot(m_RenderRot + KVector(.0f, KPI, .0f));
	}
	
	m_pUnit->Rot_Unit(m_RenderRot + KVector(.0f, KPI, .0f));
}


void Controll_User::Update_Move()
{
	switch (m_MType)
	{
	case Controll_User::MT_IDLE:
		Update_MIDLE();
		KLOG(L"Unit Move: IDLE");
		break;
	case Controll_User::MT_MOVE:
		Update_MOVE();
		KLOG(L"Unit Move: MOVE");
		break;
	case Controll_User::MT_RUN:
		Update_RUN();
		KLOG(L"Unit Move: RUN");
		break;
	default:
		break;
	}	
}


void Controll_User::Update_Act()
{
	switch (m_AType)
	{
	case Controll_User::AT_IDLE:
		Update_AIDLE();
		KLOG(L"Unit Act: IDLE");
		break;
	case Controll_User::AT_ATTACK:
		Update_ATTACK();
		KLOG(L"Unit Act: ATTACK");
		break;
	case Controll_User::AT_BOMB:
		Update_BOMB();
		KLOG(L"Unit Act: BOMB");
		break;
	case Controll_User::AT_HEAL:
		Update_HEAL();
		KLOG(L"Unit Act: HEAL");
		break;
	case Controll_User::AT_OPTI:
		Update_OPTI();
		KLOG(L"Unit Act: OPTI");
		break;
	default:
		break;
	}
}

void Controll_User::Update_Mouse()
{
	if (true == InputManager::Is_MouseMove())
	{
		if (true == KEY_PRESS(L"MUNLOCK"))
		{
			InputManager::Set_MUnLock();
		}
		else
		{
			m_PlayRot.x += InputManager::MouseDir().y * .5f *  DELTATIME;
			m_PlayRot.y += InputManager::MouseDir().x * .5f *  DELTATIME;

			KMatrix RMatX;
			KMatrix RMatY;
			KMatrix RMatZ;

			RMatX.RotX(m_PlayRot.x); // X
			RMatY.RotY(m_PlayRot.y); // Y
			RMatZ.RotZ(m_PlayRot.z); // Z
								
			KMatrix RMat = RMatX * RMatY * RMatZ;

			
			m_PlayPos = KVector4::Forword;
			m_PlayPos = RMat.MulVecZero(m_PlayPos);
			m_PlayPos.NormalizeVec3();

			InputManager::Set_MLock();
		}
	}
}


void Controll_User::Update_Terrain()
{
	KVector4 TT = one()->Trans()->pos_local();


	if (m_pTer->TER_Left() > TT.x)
	{
		TT.x = m_pTer->TER_Left();
	}
	if (m_pTer->TER_Right() < TT.x)
	{
		TT.x = m_pTer->TER_Right();
	}
	if (m_pTer->TER_Down() > TT.z)
	{
		TT.z = m_pTer->TER_Down();
	}
	if (m_pTer->TER_Up() < TT.z)
	{
		TT.z = m_pTer->TER_Up();
	}
	one()->Trans()->pos_local(TT);
}