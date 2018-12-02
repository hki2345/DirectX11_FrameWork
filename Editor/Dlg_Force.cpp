// Dlg_Force.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Dlg_Force.h"
#include "afxdialogex.h"


#include <Renderer_Grid.h>
#include <Light.h>
#include <SC2_Camera.h>

#include <Renderer_BonAni.h>
#include <ResourceManager.h>
// Dlg_Force ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dlg_Force, TabDlg)

Dlg_Force::Dlg_Force(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_FORCEDLG, pParent)
{

}

Dlg_Force::~Dlg_Force()
{
}



BOOL Dlg_Force::OnInitDialog()
{
	TabDlg::OnInitDialog();

	KPtr<State> TabScene = Core_Class::MainSceneMgr().Find_State(StateName.GetString());

	if (nullptr == TabScene)
	{
		KASSERT(true);
	}

	TabScene->Camera()->Add_Component<SC2_Camera>();
	TabScene->Camera()->Far(10000.0f);
	TabScene->Camera()->one()->Trans()->pos_local(KVector4(0.0f, 10.0f, -20.0f));


	KPtr<TheOne> Light2 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight2 = Light2->Add_Component<Light>();
	pLight2->Trans()->rotate_world(KVector4(45.0F, 0.0F, 0.0f));
	pLight2->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight2->PushLightLayer(0);


	KPtr<TheOne> Light3 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight3 = Light3->Add_Component<Light>();
	pLight3->Trans()->rotate_world(KVector4(-45.0F, 0.0F, 0.0f));
	pLight3->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight3->PushLightLayer(1);


	KPtr<TheOne> Light4 = TabScene->Create_One();
	// �������� dir�� �ƴ� ���� ũ�⸦ ��Ÿ����.
	// Light->Trans()->scale_local(KVector4(1000.0f, 1000.0f, 1000.0f));
	// Foward�� ����ٰ� �����Ѵ�.
	KPtr<Light> pLight4 = Light4->Add_Component<Light>();
	pLight4->Trans()->rotate_world(KVector4(.0F, 45.0F, 0.0f));
	pLight4->Trans()->scale_world(KVector4(30.0f, 30.0f, 30.0f));
	pLight4->PushLightLayer(0);



	KPtr<TheOne> GRIDACTOR = TabScene->Create_One();
	GRIDACTOR->Trans()->rotate_world(KVector4(90.0f, 0.0f, 0.0f));
	GRIDACTOR->Trans()->scale_world(KVector4(10000.0f, 10000.0f, 10000.0f));
	GRIDACTOR->Add_Component<Renderer_Grid>();

	return TRUE;
}


void Dlg_Force::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FORKM3LIST, m_KM3List);
	DDX_Control(pDX, IDC_FORCELIST, m_ForceList);
	DDX_Control(pDX, IDC_FORCENAME, m_ForceName);
	DDX_Control(pDX, IDC_FORCECOLOR, m_ForceColor);
}


BEGIN_MESSAGE_MAP(Dlg_Force, CDialogEx)
	ON_BN_CLICKED(IDC_FORCECOLOR, &Dlg_Force::OnBnClickedForcecolor)
	ON_LBN_SELCHANGE(IDC_FORCELIST, &Dlg_Force::OnLbnSelchangeForcelist)
	ON_EN_CHANGE(IDC_FORCENAME, &Dlg_Force::OnEnChangeForcename)
	ON_LBN_SELCHANGE(IDC_FORKM3LIST, &Dlg_Force::OnLbnSelchangeForkm3list)
	ON_BN_CLICKED(IDC_KM3RESETBTN, &Dlg_Force::OnBnClickedKm3resetbtn)
END_MESSAGE_MAP()





void Dlg_Force::Init_KM3List()
{

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ��ü �޽��� �ִϸ��̼��� �ٷ� �����Ű����
	// �ִϸ��̼��� ���� �ҷ��� �� ��޹޴� ���̴�.
	ResourceManager<MeshContainer>::Clear();
	ResourceManager<Changer_Animation>::Clear();
	ResourceManager<Changer_Animation>::All_Load();
	ResourceManager<MeshContainer>::All_Load();


	// ����Ʈ �ʱ�ȭ
	m_KM3List.ResetContent();


	m_SO = m_OList.begin();
	m_EO = m_OList.end();

	for (; m_SO != m_EO; ++m_SO)
	{
		(*m_SO)->Set_Death();
	}

	m_OList.clear();



	UpdateData(TRUE);


	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = TVec[i]->FileExt();
		if (L".KM3" == Temp)
		{
			m_CurOne = Core_Class::MainScene()->Create_One(TVec[i]->FileNameExt());
			m_CurOne->Trans()->pos_local(KVector(.0f));
			m_CurOne->Trans()->scale_local(KVector(1.f, 1.f, 1.f));

			KPtr<Renderer_BonAni> TRender = m_CurOne->Add_Component<Renderer_BonAni>();
			TRender->Set_Fbx(TVec[i]->FileNameExt());

			if (nullptr == TRender->changer_animation())
			{
				TRender->Create_Animation();
			}

			m_OList.push_back(m_CurOne);
			m_CurOne->Active_Off();
			m_KM3List.AddString(TVec[i]->FileNameExt());
		}
	}

	m_CurOne = nullptr;

	UpdateData(FALSE);
}


// Dlg_Force �޽��� ó�����Դϴ�.

void Dlg_Force::OnLbnSelchangeForcelist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void Dlg_Force::OnEnChangeForcename()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// TabDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void Dlg_Force::OnBnClickedForcecolor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void Dlg_Force::OnLbnSelchangeForkm3list()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void Dlg_Force::OnBnClickedKm3resetbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Init_KM3List();
}
