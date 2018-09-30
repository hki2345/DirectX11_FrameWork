// Dlg_Sprite.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_SpriteTab.h"
#include "afxdialogex.h"
#include <ResourceManager.h>
#include "Edit_Class.h"
#include "Split_Debug.h"

#include <Core_Class.h>
#include <Camera.h>
#include <KImage.h>
#include <Texture.h>

#include <ReadStream.h>
#include <WriteStream.h>

// Dlg_Sprite 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_SpriteTab, TabState)

Dlg_SpriteTab::Dlg_SpriteTab(CWnd* pParent /*=NULL*/)
	: TabState(IDD_SPRITE, pParent)
	, m_SpriteName(_T(""))
{
	Edit_Class::SpriteTab = this;
}

Dlg_SpriteTab::~Dlg_SpriteTab()
{
}

void Dlg_SpriteTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Sprite_Tree);
	DDX_Text(pDX, IDC_SPRITENAME, m_SpriteName);
	DDX_Text(pDX, IDC_SPRITEU, m_SplitX);
	DDX_Text(pDX, IDC_SPRITEV, m_SplitY);
}


BEGIN_MESSAGE_MAP(Dlg_SpriteTab, CDialogEx)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &Dlg_SpriteTab::OnTvnSelchangedTree1)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_SPRITEU, &Dlg_SpriteTab::OnEnChangeSpriteu)
	ON_EN_CHANGE(IDC_SPRITEV, &Dlg_SpriteTab::OnEnChangeSpritev)
	ON_BN_CLICKED(IDC_LOAD, &Dlg_SpriteTab::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &Dlg_SpriteTab::OnBnClickedSave)
END_MESSAGE_MAP()


// Dlg_Sprite 메시지 처리기입니다.

void Dlg_SpriteTab::Init()
{
	KPtr<State> FindState = Core_Class::Main_StateManager().find_state(L"Sprite");

	KPtr<TheOne> NewOne = FindState->Create_TheOne();
	KPtr<Split_Debug> NewDeg = NewOne->Add_Component<Split_Debug>();
	m_SpRenderer = NewDeg->m_Renderer;
	NewOne->Local_Pos({ 0, 0, 10.0f });

	OnBnClickedLoad();
}

void Dlg_SpriteTab::check(const wchar_t* _Path, const wchar_t* _Folder, HTREEITEM _ParentItem)
{
	CFileFind finder;
	CString findPath = _Path;

	BOOL isFile = finder.FindFile(findPath + L"*.*");

	while (isFile)
	{
		isFile = finder.FindNextFileW();

		if (finder.IsDots())
		{
			continue;
		}

		// 폴더일 경우
		else if (TRUE == finder.IsDirectory())
		{
			// 전체 팼,
			CString thisPath = finder.GetFilePath();
			thisPath += L"\\";
			if (nullptr == PathManager::Find_Path(finder.GetFileName()))
			{
				PathManager::Create_FullPath(finder.GetFileName(), thisPath.GetString());
			}

			// 구조체로 따로 저장해야 할 수도 ->  UV 설정으로 인해
			HTREEITEM newItem = m_Sprite_Tree.InsertItem(finder.GetFileName(), _ParentItem);
			m_Sprite_Tree.SetItemData(newItem, 0);
			check(thisPath.GetString(), finder.GetFileName(), newItem);
		}

		// 파일일 경우
		else
		{
			CString arr;
			arr = finder.GetFilePath();

			wchar_t ArrDrive[128] = { 0, };
			wchar_t ArrFolder[128] = { 0, };
			wchar_t ArrFile[128] = { 0, };
			wchar_t ArrExist[128] = { 0, };

			// 파일 경로를 뜯는다.
			_wsplitpath_s(arr, ArrDrive, ArrFolder, ArrFile, ArrExist);

			std::wstring Drive = ArrDrive;
			std::wstring Exi = ArrExist;
			std::wstring Name = ArrFile;
			std::wstring Folder = ArrFolder;

			Name += Exi;

			size_t a = Folder.find(_Folder);
			Folder.clear();

			for (size_t i = a; i < 128; i++)
			{
				Folder += ArrFolder[i];
			}

			Folder.replace(Folder.find_last_of('\\', Folder.size()),
				Folder.size(), L"");

			if (Exi == L".png" || Exi == L".PNG" || Exi == L".Png" || Exi == L".jpg" || Exi == L".JPG" || Exi == L".Jpg")
			{
				std::map<std::wstring, Split_Data*>::iterator FindIter = m_SplitData_Map.find(finder.GetFileName().GetString());

				if (FindIter != m_SplitData_Map.end())
				{
					if (nullptr == ResourceManager<Texture>::Find(finder.GetFileName()))
					{
						ResourceManager<Texture>::Load_FullPath(finder.GetFilePath());
						ResourceManager<KImage>::Load_FullPath(finder.GetFilePath());
					}
					KPtr<KImage> m_Img = ResourceManager<KImage>::Find(finder.GetFileName());
					m_Img->Split(FindIter->second->X, FindIter->second->Y);
					size_t Index = m_Img->Split(KVector(0.0f, 0.0f, 1.0f, 1.0f));
					m_SpRenderer->index(Index);

					HTREEITEM FileItem = m_Sprite_Tree.InsertItem(finder.GetFileName(), _ParentItem);
					m_Sprite_Tree.SetItemData(FileItem, (DWORD_PTR)FindIter->second);
				}
				else {
					ResourceManager<Texture>::Load_FullPath(finder.GetFilePath());
					KPtr<KImage> Img = ResourceManager<KImage>::Load_FullPath(finder.GetFilePath());

					if (nullptr != Img)
					{
						Split_Data* NewData = new Split_Data();
						lstrcpyW(NewData->path, finder.GetFilePath());
						lstrcpyW(NewData->name, Img->name_exist());
						m_SplitData_Map.insert(std::map<std::wstring, Split_Data*>::value_type(NewData->name, NewData));

						HTREEITEM newItem = m_Sprite_Tree.InsertItem(finder.GetFileName(), _ParentItem);
						m_Sprite_Tree.SetItemData(newItem, (DWORD_PTR)NewData);
					}
				}
			}
		}
	}

	finder.Close();
}

void Dlg_SpriteTab::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//UINT A;
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString mName = m_Sprite_Tree.GetItemText(pNMTreeView->itemNew.hItem);
	Split_Data* ptr = (Split_Data*)m_Sprite_Tree.GetItemData(pNMTreeView->itemNew.hItem);

	// 파일일 때
	if (0 != ptr)
	{
		m_CurItem = pNMTreeView->itemNew.hItem;
		m_SplitX = (UINT)ptr->X;
		m_SplitY = (UINT)ptr->Y;

		m_SpriteName = mName;
		m_SpRenderer->image(mName.GetString());
		m_SpRenderer->set_image_size();


		if (nullptr != m_SpRenderer->texture() && 
			nullptr != m_SpRenderer->state()->camera())
		{
			KVector2 imgSize = m_SpRenderer->texture()->size();
			KVector2 CamSize = m_SpRenderer->state()->camera()->screen_size();
			KVector2 WinSize = Core_Class::Main_Window().size();

			m_SpRenderer->index(m_SpRenderer->image()->split_cnt() - 1);

			// 가로가 더 클 때
			if (imgSize.x_ratio() > CamSize.x_ratio())
			{
				m_SpRenderer->state()->camera()->screen_size({
					imgSize.x, imgSize.x * WinSize.y_ratio() });
			}
			// 세로가 더 클 떄
			else if (imgSize.x_ratio() < CamSize.x_ratio())
			{
				m_SpRenderer->state()->camera()->screen_size({
					imgSize.y * WinSize.x_ratio(), imgSize.y  });
			}

			// 정사각형
			else
			{
				m_SpRenderer->state()->camera()->screen_size(imgSize);
			}
		}
	}
	else
	{
		m_SpRenderer->image(nullptr);
		m_CurItem = nullptr;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(FALSE);
}
void Dlg_SpriteTab::OnDestroy()
{
	TabState::OnDestroy();
	Release();
}

void Dlg_SpriteTab::Release()
{
	std::map<std::wstring, Split_Data*>::iterator StartIter = m_SplitData_Map.begin();
	std::map<std::wstring, Split_Data*>::iterator EndIter = m_SplitData_Map.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		delete StartIter->second;
	}

	m_SplitData_Map.clear();
}

void Dlg_SpriteTab::OnEnChangeSpriteu()
{
	UpdateData(TRUE);

	if (nullptr == m_CurItem)
	{
		UpdateData(FALSE);
		return;
	}


	Split_Data* PTR = (Split_Data*)m_Sprite_Tree.GetItemData(m_CurItem);
	PTR->X = m_SplitX;

	if (1 > PTR->X)
	{
		m_SplitX = 1;
	}

	m_SpRenderer->image()->Split((size_t)PTR->X, (size_t)PTR->Y);
	size_t newIndex = m_SpRenderer->image()->Split(KVector(0.0f, 0.0f, 1.0f, 1.0f));
	m_SpRenderer->index(newIndex);

	UpdateData(FALSE);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabState::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_SpriteTab::OnEnChangeSpritev()
{
	UpdateData(TRUE);


	if (nullptr == m_CurItem)
	{
		UpdateData(FALSE);
		return;
	}


	Split_Data* PTR = (Split_Data*)m_Sprite_Tree.GetItemData(m_CurItem);
	PTR->Y = m_SplitY;

	if (1 > PTR->Y)
	{
		PTR->Y = 1;
		m_SplitY = 1;
	}

	m_SpRenderer->image()->Split((size_t)PTR->X, (size_t)PTR->Y);
	size_t newIndex = m_SpRenderer->image()->Split(KVector(0.0f, 0.0f, 1.0f, 1.0f));
	m_SpRenderer->index(newIndex);

	UpdateData(FALSE);
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// TabState::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void Dlg_SpriteTab::OnBnClickedLoad()
{
	Release();
	m_Sprite_Tree.DeleteAllItems();

	CString LoadFolder = PathManager::Find_Path(L"Sprite_Data");
	LoadFolder += L"SpriteData.dat";

	ReadStream Stream = ReadStream(LoadFolder);

	if (false == Stream.is_file())
	{
		return;
	}

	int Count = 0;
	Stream.read(&Count, sizeof(int), sizeof(int));

	for (size_t i = 0; i < (size_t)Count - 1; i++)
	{
		Split_Data* NewData = new Split_Data();

		Stream.read(NewData, sizeof(Split_Data), sizeof(Split_Data));
		m_SplitData_Map.insert(std::map<std::wstring, Split_Data*>::value_type(NewData->name, NewData));
	}

	m_CurItem = nullptr;
	m_SpRenderer->image(nullptr);

	check(PathManager::Find_Path(L"Texture"), L"Texture", m_Sprite_Tree.InsertItem(L"Texture"));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Dlg_SpriteTab::OnBnClickedSave()
{
	CString SaveFolder = PathManager::Find_Path(L"Sprite_Data");
	SaveFolder += L"SpriteData.dat";

	CString BackFolder = PathManager::Find_Path(L"Sprite_Back");

	wchar_t BackNumber[256];

	_itow_s((int)time(nullptr), BackNumber, 10);

	BackFolder += L"SpriteData_";
	BackFolder += BackNumber;
	BackFolder += L".dat";

	WriteStream SaveStream = WriteStream(SaveFolder);
	WriteStream SaveBackStream = WriteStream(BackFolder);

	int Size = (int)m_SplitData_Map.size();
	SaveStream.write(&Size, sizeof(int));
	SaveBackStream.write(&Size, sizeof(int));

	std::map<std::wstring, Split_Data*>::iterator StartIter = m_SplitData_Map.begin();
	std::map<std::wstring, Split_Data*>::iterator EndIter = m_SplitData_Map.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		Split_Data* pSaveData = StartIter->second;

		SaveStream.write(pSaveData, sizeof(Split_Data));
		SaveBackStream.write(pSaveData, sizeof(Split_Data));
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
