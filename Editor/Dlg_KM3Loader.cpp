// Dlg_KM3Loader.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_KM3Loader.h"
#include "afxdialogex.h"


#include <Core_Class.h>
#include <Renderer_BonAni.h>

// Dlg_KM3Loader 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_KM3Loader, TabDlg)

Dlg_KM3Loader::Dlg_KM3Loader(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_KM3DLG, pParent)
{
}

Dlg_KM3Loader::~Dlg_KM3Loader()
{
}

void Dlg_KM3Loader::DoDataExchange(CDataExchange* pDX)
{
	TabDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KM3LIST, m_KM3List);
}


BEGIN_MESSAGE_MAP(Dlg_KM3Loader, TabDlg)
	ON_BN_CLICKED(IDC_KM3LOAD, &Dlg_KM3Loader::OnBnClickedKm3load)
	ON_LBN_SELCHANGE(IDC_KM3LIST, &Dlg_KM3Loader::OnLbnSelchangeKm3list)
END_MESSAGE_MAP()


void Dlg_KM3Loader::Hide_Dlg()
{
	if (nullptr != m_One)
	{
		m_One->Set_Death();
	}
}

// Dlg_KM3Loader 메시지 처리기입니다.


void Dlg_KM3Loader::OnBnClickedKm3load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ResourceManager<MeshContainer>::All_Load(L"\\Data\\KM3");

	Update_RscTree();
}


void Dlg_KM3Loader::OnLbnSelchangeKm3list()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Tint = m_KM3List.GetCurSel();
	if (-1 == Tint)
	{
		return;
	}

	if (nullptr != m_One)
	{
		m_One->Set_Death();
	}

	CString TempStr;
	m_KM3List.GetText(Tint, TempStr);

	m_One = Core_Class::MainScene()->Create_One(L"FBX_LOAD");
	m_One->Trans()->pos_local(KVector(.0f));
	m_One->Trans()->scale_local(KVector(1.f, 1.f, 1.f));
	KPtr<Renderer_BonAni> TRender = m_One->Add_Component<Renderer_BonAni>();

	TRender->Set_Fbx(TempStr);
	TRender->Create_Clip(L"ALLAni", 0, 100000);
	TRender->Set_Clip(L"ALLAni");
}



void Dlg_KM3Loader::Update_RscTree()
{
	UpdateData(TRUE);
	
	// 리스트 초기화
	m_KM3List.ResetContent();

	std::vector<KPtr<MeshContainer>> TVec = ResourceManager<MeshContainer>::All_SingleVec();

	for (size_t i = 0; i < TVec.size(); i++)
	{
		std::wstring Temp = TVec[i]->FileExt();
		if (L".KM3" == Temp)
		{
			m_KM3List.AddString(TVec[i]->FileNameExt());
		}
	}

	UpdateData(FALSE);
}
