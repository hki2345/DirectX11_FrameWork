// SpriteDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AR14TOOL.h"
#include "SpriteDlg.h"
#include "afxdialogex.h"
#include <HResMgr.h>
#include "TVAR.h"
#include <HVAR.h>
#include <HImage.h>
#include <HTexture.h>
#include <HCamera.h>
#include "HSpriteCut.h"
#include <HThread.h>

#include <BWStream.h>
#include <BRStream.h>


// SpriteDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(SpriteDlg, TabDlg)

SpriteDlg::SpriteDlg(CWnd* pParent /*=NULL*/)
	: TabDlg(IDD_SPRITEDLG, pParent)
	, m_SelectSpriteName(_T(""))
	, m_CurX(0)
	, m_CurY(0)
{
	TVAR::SpDlg = this;
}

SpriteDlg::~SpriteDlg()
{

}

void SpriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPRITETREE, m_SpriteTree);
	DDX_Text(pDX, IDC_SELECTSPRITE, m_SelectSpriteName);
	DDX_Text(pDX, IDC_CUTX, m_CurX);
	DDX_Text(pDX, IDC_CUTY, m_CurY);
}


BEGIN_MESSAGE_MAP(SpriteDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SpriteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SpriteDlg::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_SPRITETREE, &SpriteDlg::OnTvnSelchangedSpritetree)
	ON_EN_CHANGE(IDC_CUTX, &SpriteDlg::OnEnChangeCutx)
	ON_EN_CHANGE(IDC_CUTY, &SpriteDlg::OnEnChangeCuty)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SAVEDATA, &SpriteDlg::OnBnClickedSavedata)
	ON_BN_CLICKED(IDC_LOADDATA, &SpriteDlg::OnBnClickedLoaddata)
END_MESSAGE_MAP()


// SpriteDlg 메시지 처리기입니다.


void SpriteDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();
}


void SpriteDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnCancel();
}

BOOL SpriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void SpriteDlg::SpriteInit() 
{
	// SpriteCheck(GamePath::FindPath(L"Texture"), L"Texture", m_SpriteTree.InsertItem(L"Texture"));

	HPTR<HScene> m_Scene = HVAR::MainSceneMgr().FindScene(L"SpriteDlg");
	HPTR<HActor> pActor = m_Scene->CreateActor();
	HPTR<HSpriteCut> SpriteCut = pActor->AddCom<HSpriteCut>();
	m_SpriteRender = SpriteCut->SpriteRender;
	pActor->Trans()->LPos({ 0, 0, 5 });

	OnBnClickedLoaddata();

	// OnBnClickedLoaddata();

	// HThread::StartThread<SpriteDlg>(L"PlayerLoader", &SpriteDlg::TestPlayerLoad, this);
	// HThread::StartThread<SpriteDlg>(L"Player2Loader", &SpriteDlg::TestPlayerLoad2, this);

	//std::wstring Path1 = GamePath::FindPath(L"Texture");
	//Path1 += L"\\Player\\";
	//SpriteCheck(Path1.c_str(), L"Player", m_SpriteTree.InsertItem(L"Player"));
	//std::wstring Path2 = GamePath::FindPath(L"Texture");
	//Path2 += L"\\Player2\\";
	//SpriteCheck(Path2.c_str(), L"Player2", m_SpriteTree.InsertItem(L"Player2"));
}

unsigned int SpriteDlg::TestPlayerLoad(void* _Arg) 
{
	OnBnClickedLoaddata();

	return 0;
	//std::wstring Path1 = GamePath::FindPath(L"Texture");
	//Path1 += L"\\Player\\";
	//SpriteCheck(Path1.c_str(), L"Player", m_SpriteTree.InsertItem(L"Player"));
}
unsigned int SpriteDlg::TestPlayerLoad2(void* _Arg) {
	std::wstring Path2 = GamePath::FindPath(L"Texture");
	Path2 += L"\\Player2\\";
	SpriteCheck(Path2.c_str(), L"Player2", m_SpriteTree.InsertItem(L"Player2"));

	return 0;
}

void SpriteDlg::SpriteCheck(const wchar_t* _Path, const wchar_t* _Folder, HTREEITEM _ParentItem)
{
	
	CFileFind FileFind;
	CString FindPath = _Path;
	BOOL bFile = FileFind.FindFile(FindPath + L"*.*");
	// 리스트를 찾은거라.
	while (bFile)
	{
		bFile = FileFind.FindNextFileW();

		if (FileFind.IsDots())
		{
			continue;
		}
		else if (TRUE == FileFind.IsDirectory()) 
		{
			CString Path = FileFind.GetFilePath();
			Path += L"\\";
			if (nullptr == GamePath::FindPath(FileFind.GetFileName()))
			{
				GamePath::CreatePath(FileFind.GetFileName(), Path.GetString());
			}

			HTREEITEM FolderItem = m_SpriteTree.InsertItem(FileFind.GetFileName(), _ParentItem);
			m_SpriteTree.SetItemData(FolderItem, 0);
			SpriteCheck(Path.GetString(), FileFind.GetFileName(), FolderItem);
		}
		else 
		{
			std::map<std::wstring, SpriteData*>::iterator FindIter = m_pDataList.find(FileFind.GetFileName().GetString());

			if (FindIter != m_pDataList.end())
			{
				if (nullptr == HResMgr<HTexture>::Find(FileFind.GetFileName()))
				{
					HResMgr<HTexture>::Load(FileFind.GetFilePath());
					HResMgr<HImage>::Load(FileFind.GetFilePath());
				}

				HPTR<HImage> m_Img = HResMgr<HImage>::Find(FileFind.GetFileName());
				m_Img->Cut(FindIter->second->CutX, FindIter->second->CutY);
				size_t Index = m_Img->Cut(HVEC(0.0f, 0.0f, 1.0f, 1.0f));
				m_SpriteRender->ImageIndex(Index);
				
				HTREEITEM FileItem = m_SpriteTree.InsertItem(FileFind.GetFileName(), _ParentItem);
				m_SpriteTree.SetItemData(FileItem, (DWORD_PTR)FindIter->second);
			}
			else {

				CString Name = FileFind.GetFilePath();

				HPTR<HTexture> Tex = HResMgr<HTexture>::Load(Name);
				HPTR<HImage> Img = HResMgr<HImage>::Load(Name);

				if (nullptr != Img)
				{
					SpriteData* pNewData = new SpriteData();
					lstrcpyW(pNewData->ImagePath, FileFind.GetFilePath());
					lstrcpyW(pNewData->FileName, Img->FullFileName());

					m_pDataList.insert(std::map<std::wstring, SpriteData*>::value_type(pNewData->FileName, pNewData));

					HTREEITEM FileItem = m_SpriteTree.InsertItem(FileFind.GetFileName(), _ParentItem);
					m_SpriteTree.SetItemData(FileItem, (DWORD_PTR)pNewData);
				}
			}
		}
	}

	FileFind.Close();
}

void SpriteDlg::OnTvnSelchangedSpritetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// 순회는 이걸 사용하면 쉽게 할수 있을 것이다.
	//m_SpriteTree.GetRootItem();
	//m_SpriteTree.GetNextItem()

	CString FileName = m_SpriteTree.GetItemText(pNMTreeView->itemNew.hItem);
	SpriteData* PTR = (SpriteData*)m_SpriteTree.GetItemData(pNMTreeView->itemNew.hItem);

	if (0 != PTR)
	{
		m_CurSelectItem = pNMTreeView->itemNew.hItem;

		m_CurX = (UINT)PTR->CutX;
		m_CurY = (UINT)PTR->CutY;

		m_SelectSpriteName = FileName;
		m_SpriteRender->Image(FileName.GetString());
		m_SpriteRender->ImageScaleSetting();
		if (nullptr != m_SpriteRender->Texture()
			&& nullptr != m_SpriteRender->Scene()->Camera())
		{
			HVEC2 ImgSize = m_SpriteRender->Texture()->ImageSize();
			HVEC2 CameraSize = m_SpriteRender->Scene()->Camera()->ScreenSize();
			HVEC2 WindowSize = HVAR::MainWindow().Size();

			m_SpriteRender->ImageIndex(m_SpriteRender->Image()->CutCount() - 1);

			if (ImgSize.RatioX() > CameraSize.RatioX())
			{
				// 너비로 맞춘다.
				m_SpriteRender->Scene()->Camera()->ScreenSize({ ImgSize.x, ImgSize.x * WindowSize.RatioY() });
			}
			else if (ImgSize.RatioX() < CameraSize.RatioX())
			{
				// 길이로 맞춘다.
				m_SpriteRender->Scene()->Camera()->ScreenSize({ ImgSize.y * WindowSize.RatioX(), ImgSize.y });
			}
			else 
			{
				m_SpriteRender->Scene()->Camera()->ScreenSize({ ImgSize.y, ImgSize.y * WindowSize.RatioY() });
				// 별 상관 없다.
			}
		}
	}
	else {
		m_CurSelectItem = nullptr;
		m_SpriteRender->Image(nullptr);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(FALSE);
}


void SpriteDlg::OnEnChangeCutx()
{
	UpdateData(TRUE);

	if (nullptr == m_CurSelectItem)
	{
		UpdateData(FALSE);
		return;
	}

	SpriteData* PTR = (SpriteData*)m_SpriteTree.GetItemData(m_CurSelectItem);
	PTR->CutX = m_CurX;

	if (1 > PTR->CutX)
	{
		m_CurX = 1;
	}

	m_SpriteRender->Image()->Cut((size_t)PTR->CutX, (size_t)PTR->CutY);
	size_t Index = m_SpriteRender->Image()->Cut(HVEC(0.0f, 0.0f, 1.0f, 1.0f));
	m_SpriteRender->ImageIndex(Index);

	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
}


void SpriteDlg::OnEnChangeCuty()
{
	UpdateData(TRUE);

	if (nullptr == m_CurSelectItem)
	{
		UpdateData(FALSE);
		return;
	}

	if (1 > m_CurY)
	{
		m_CurY = 1;
	}

	SpriteData* PTR = (SpriteData*)m_SpriteTree.GetItemData(m_CurSelectItem);
	PTR->CutY = m_CurY;

	m_SpriteRender->Image()->Cut((size_t)PTR->CutX, (size_t)PTR->CutY);
	size_t Index = m_SpriteRender->Image()->Cut(HVEC(0.0f, 0.0f, 1.0f, 1.0f));
	m_SpriteRender->ImageIndex(Index);


	UpdateData(FALSE);

}


void SpriteDlg::OnDestroy()
{
	TabDlg::OnDestroy();
	Release();
}

void SpriteDlg::Release() {

	std::map<std::wstring, SpriteData*>::iterator StartIter = m_pDataList.begin();
	std::map<std::wstring, SpriteData*>::iterator EndIter = m_pDataList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		delete StartIter->second;
	}

	m_pDataList.clear();
}


void SpriteDlg::OnBnClickedSavedata()
{
	CString SaveFolder = GamePath::FindPath(L"Data");
	SaveFolder += L"SpriteData.dat";

	CString BackFolder = GamePath::FindPath(L"SpriteBack");

	wchar_t BackNumber[256];

	_itow_s((int)time(nullptr), BackNumber, 10);

	BackFolder += L"SpriteData_";
	BackFolder += BackNumber;
	BackFolder += L".dat";

	BWStream SaveStream = BWStream(SaveFolder);
	BWStream SaveBackStream = BWStream(BackFolder);

	int Size = (int)m_pDataList.size();
	SaveStream.Write(&Size, sizeof(int));
	SaveBackStream.Write(&Size, sizeof(int));

	std::map<std::wstring, SpriteData*>::iterator StartIter = m_pDataList.begin();
	std::map<std::wstring, SpriteData*>::iterator EndIter = m_pDataList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		SpriteData* pSaveData = StartIter->second;

		SaveStream.Write(pSaveData, sizeof(SpriteData));
		SaveBackStream.Write(pSaveData, sizeof(SpriteData));
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void SpriteDlg::OnBnClickedLoaddata()
{
	Release();
	m_SpriteTree.DeleteAllItems();

	CString SaveFolder = GamePath::FindPath(L"Data");
	SaveFolder += L"SpriteData.dat";

	BRStream ReadStream = BRStream(SaveFolder);

	if (false == ReadStream.IsFile())
	{
		SpriteCheck(GamePath::FindPath(L"Texture"), L"Texture", m_SpriteTree.InsertItem(L"Texture"));
		return;
	}

	int Count = 0;
	ReadStream.Read(&Count, sizeof(int), sizeof(int));

	for (int i = 0; i < Count; i++)
	{
		SpriteData* NewData = new SpriteData();

		ReadStream.Read(NewData, sizeof(SpriteData), sizeof(SpriteData));
		m_pDataList.insert(std::map<std::wstring, SpriteData*>::value_type(NewData->FileName, NewData));
	}

	m_CurSelectItem = nullptr;
	m_SpriteRender->Image(nullptr);

	SpriteCheck(GamePath::FindPath(L"Texture"), L"Texture", m_SpriteTree.InsertItem(L"Texture"));
}
