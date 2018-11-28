#include "Renderer_BonAni.h"
#include "Texture.h"
#include "TimeManager.h"
#include "DXContainer_DE.h"
#include "ResourceManager.h"
#include "DebugManager.h"

#include "Core_Class.h"




Renderer_BonAni::Renderer_BonAni() : 
m_ClipInx(1), 
m_FrameCnt(30), 
m_UpdateSpd(.0f), 
m_UpdateTime(.0f),
m_MeshColor(KColor::White)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_DATA;
	ROpt.IsBoneAni = 1;
}


Renderer_BonAni::~Renderer_BonAni()
{
}


void Renderer_BonAni::Set_Fbx(const wchar_t* _Name)
{
	MCon = ResourceManager<MeshContainer>::Find(_Name);
	Init_Mesh();
}


void Renderer_BonAni::Init_Mesh()
{
	for (size_t MeshInx = 0; MeshInx < MCon->m_Data.MeshVec.size(); MeshInx++)
	{
		KM3Mesh* pMD = MCon->m_Data.MeshVec[MeshInx];

		if (nullptr == pMD)
		{
			BBY;
		}

		KPtr<KMesh> NMesh = new KMesh();
		NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���ؽ� ����
		if (false == NMesh->Create_Vertex(
			pMD->VxCnt
			, pMD->VxSize
			, pMD->Usage
			, pMD->m_VertBD))
		{
			BBY;
		}

		for (size_t SubInx = 0; SubInx < pMD->IxVec.size(); SubInx++)
		{
			KM3Index* pInx = &pMD->IxVec[SubInx];

			NMesh->Create_Index(
				pInx->IxCnt
				, pInx->IxSize
				, pInx->Usage
				, pInx->Fm
				, pInx->m_InxBD
			);
		}


		// �ش� �޽��� ������ �ִ� ���� ����
		Renderer::Set_Mesh(NMesh, (int)MeshInx);



		// ���͸��� -> ���͸��� ������ŭ �� �����־�� �Ѵ�.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->MtlVec.size(); MtlIdx++)
		{
			switch (ROpt.Defferd_orForward)
			{
			case 0:
				Set_Material(L"MESH3DMAT", MtlIdx);
				break;

			case 1:
				Set_Material(L"DEFFERD3DANIMAT", MtlIdx);
				break;

			default:
				break;
			}

			KPtr<KMaterial> CMat = material(MtlIdx);

			Material_FbxData* pData = &pMD->MtlVec[MtlIdx];

			std::wstring TMesh = PathManager::Find_ForderPath(L"Mesh");
			
			// ��ǻ�� �޾ƿ���
			if (pData->Diff[0] != 0)
			{
				std::wstring Tmp = pData->Diff;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// ���� �޾ƿ���
			if (pData->Bump[0] != 0)
			{
				std::wstring Tmp = pData->Bump;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// ���� �޾ƿ���
			if (pData->Spec[0] != 0)
			{
				std::wstring Tmp = pData->Spec;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// �̸� �޾ƿ���
			if (pData->Emiv[0] != 0)
			{
				std::wstring Tmp = pData->Emiv;
				KPtr<Texture> Tex = ResourceManager<Texture>::Load((TMesh + Tmp).c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_EMIS, 3, Tex->FileForder());
			}

			// ������ ������ ���͸��� ������. -> �������� �⺻ �޽��� ���� ����
			Insert_RenderData((KUINT)MeshInx, 0, MtlIdx, MtlIdx);
		}
	}

	if (0 >= MCon->m_Data.AniVec.size())
	{
		return;
	}

	m_pBoneTex = new Texture();
	m_pBoneTex->Create(
		(KUINT)MCon->m_Data.BoneVec.size() * 4, 1
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT
		, D3D11_USAGE_DYNAMIC
	);


	m_BoneData_CurAni.resize(MCon->m_Data.BoneVec.size());
	m_MXData_CurAni.resize(MCon->m_Data.BoneVec.size());

}

// �ϴ�, ���� �ſ� ������ �����.
void Renderer_BonAni::Load_FbxTest(const wchar_t* _Path)
{
	MCon = ResourceManager<MeshContainer>::Load(_Path);
	Init_Mesh();
}

void Renderer_BonAni::PrevUpdate()
{
	if (nullptr != m_CurAni)
	{
		PrevUpdate_Ani();
	}
}

void Renderer_BonAni::PrevUpdate_Ani()
{
	if (0 >= MCon->m_Data.BoneVec.size())
	{
		return;
	}

	// ���� ������
	m_CurTime =
		m_CurAni->Start / m_FrameCnt +
		m_UpdateTime +
		(float)(MCon->m_Data.AniVec[m_ClipInx].Stime.GetSecondDouble());
	m_UpdateTime += DELTATIME;

	if (m_UpdateTime >= MCon->m_Data.AniVec[m_ClipInx].Length_Time)
	{
		m_UpdateTime = .0f;
	}

	// ���� ���� �ð��� �ֽ�ȭ �ð��� ���� �ð��� ���� �������� �ð� - ���� �������� �ǰڴ�.
	m_CurTime = (float)(MCon->m_Data.AniVec[m_ClipInx].Stime.GetSecondDouble() + m_UpdateTime);
	

	int iFrameInx = (int)(m_CurTime * m_FrameCnt);
	int iNextFrameInx = 0;
	
	// ���� �������� �������� ������ ũ�� 0���� �ʱ�ȭ
	if (iFrameInx >= m_CurAni->End - 1)
	{
		m_UpdateTime = .0f; 		
		iFrameInx = m_CurAni->Start;
	}

	// �翬������ ���� ����� + 1 �������� �ǰڴ�.
	iNextFrameInx = iFrameInx + 1;

	for (size_t i = 0; i < MCon->m_Data.BoneVec.size(); i++)
	{
		if (MCon->m_Data.BoneVec[i]->KFrameVec.empty())
		{
			m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->BoneMX;
			continue;
		}

		KeyFrame& CFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iFrameInx];
		KeyFrame& NFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iNextFrameInx];

		float fCFTime = (float)CFrame.dTime;
		float fNFTime = (float)NFrame.dTime;

		// ����ġ�� ���� ����
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);


		// �ش� ������ KM3 Ŭ������ ������ ������ �ּ� ó��
		//// �׶��� ũ�� ��� ���� - ����
		//DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		//DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		//// �׶��� ��ġ ��� ���� - ����
		//DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		//DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		//// �׶��� ȸ�� ��� ���� - ����
		//// �� ȸ���� ���ʹϾ����� �޾ƿ� 
		//// �ϼ��� ���� ��Ŀ��� ȸ������� ���´ٴ� �� �Ұ��ɿ� �����
		//// ���п���, �����(����, ����: quaternion ���ʹϾ�[*]) �Ǵ� �ع��� ��(����: Hamilton number)��
		//// ���Ҽ��� Ȯ���� ���� �� ü���̴�.�� ���� �Ǽ� ������ ������,
		//// ������ ������ ���չ�Ģ �� ������ ��ȯ��Ģ�� ������Ű���� ������ ��ȯ��Ģ�� �������� �ʴ´�.
		//DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		//DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		//// �� �̺κ��� ���� ���ִ� ���� -> ���� ���� �׸��� �� ���� �־��ָ� �ڵ����� �������ش�.
		//// ���� ���̷�Ʈ X�̴� �ȿ��� � ���� �Ͼ�� ������ -> �������� ����
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(CFrame.Scale, NFrame.Scale, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(CFrame.Pos, NFrame.Pos, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(CFrame.Rotate, NFrame.Rotate, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, 1.0f);

		// �⺻ �����¿� ������ ���� ����� ���� -���� ����
		// �� �� ���ϴ� ������ ũ���� ���θ� �����ִ� �༮�� ���̷�Ʈ�� �ִ�.
		// KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_FMX);
		m_BoneData_CurAni[i] = DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
		m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->OffsetMX * m_BoneData_CurAni[i];
	}

	m_pBoneTex->Set_Pixel(&m_MXData_CurAni[0], sizeof(KMatrix) * m_MXData_CurAni.size());


	// ���� �����ϴ� �κ�
	m_MeshColor = KColor::Blue;
	Core_Class::MainDevice().SettingCB<KColor>(L"FORCE_COLOR", m_MeshColor, SHTYPE::ST_PS);
}


void Renderer_BonAni::RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx)
{
	if (nullptr != m_pBoneTex)
	{
		// ������ �ؽ��Ĵ� 10���� �Ҵ��Ѵ�.
		m_pBoneTex->Update(10);
	}
}



KMatrix Renderer_BonAni::Get_BoneMX(const wchar_t* _Name)
{
	KM3Bone* pBone = MCon->Find_Bone(_Name);

	return m_BoneData_CurAni[pBone->Index];
}


KMatrix Renderer_BonAni::Get_WBoneMX(const wchar_t* _Name)
{
	return Get_BoneMX(_Name) * Trans()->worldmat_const();
}



/********************************** ���� �ִϸ��̼�*********************************/
bool Renderer_BonAni::Set_AniChanger(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}

	m_CurAni = FI->second;

	return true;
}

bool Renderer_BonAni::Erase_AniChanger(const wchar_t* _Name)
{
	if (0 == _Name[0])
	{
		BBY;
		return false;
	}

	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);
	if (m_ACMap.end() == FI)
	{
		BBY;
		return false;
	}


	m_ACMap.erase(FI);
	return true;
}

KPtr<Renderer_BonAni::Ani_Changer> Renderer_BonAni::Create_AniChanger(const wchar_t* _Name, const int& _Start, const int& _End)
{
	int TStart = _Start;
	int TEnd = _End;

	// �̰� -> �ִ�ġ �Ѿ�� �׳� ���� - ������ �� ����
	if(TEnd >= MCon->m_Data.AniVec[m_ClipInx].Length_Time)
	{
		TEnd = (int)MCon->m_Data.AniVec[m_ClipInx].Length_Time;
	}
	if (TStart >= MCon->m_Data.AniVec[m_ClipInx].Length_Time - 1)
	{
		TEnd = (int)MCon->m_Data.AniVec[m_ClipInx].Length_Time - 1;
	}

	if (TEnd < TStart)
	{
		int Temp = TStart;
		TStart = TEnd;
		TEnd = Temp;
	}



	if (0 == _Name[0])
	{
		BBY;
		return nullptr;
	}

	Renderer_BonAni::Ani_Changer* NC = new Ani_Changer(_Name, TStart, TEnd);

	m_ACMap.insert(std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::value_type(NC->ws_name(), NC));

	return NC;
}

KPtr<Renderer_BonAni::Ani_Changer> Renderer_BonAni::Find_AniChamnger(const wchar_t* _Name)
{
	std::map<std::wstring, KPtr<Renderer_BonAni::Ani_Changer>>::iterator FI = m_ACMap.find(_Name);

	if (m_ACMap.end() == FI)
	{
		return nullptr;
	}

	return FI->second;
}


void Renderer_BonAni::Set_TexturePath(const TEX_TYPE& _Value, const wchar_t* _Path)
{
	for (size_t MeshInx = 0; MeshInx < MCon->m_Data.MeshVec.size(); MeshInx++)
	{
		KM3Mesh* pMD = MCon->m_Data.MeshVec[MeshInx];
		

		// ���͸��� -> ���͸��� ������ŭ �� �����־�� �Ѵ�.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->MtlVec.size(); MtlIdx++)
		{
			Material_FbxData* pData = &pMD->MtlVec[MtlIdx];

			switch (_Value)
			{
			case TEX_TYPE::TEX_COLOR:
				memcpy_s(pData->Diff, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_BUMP:
				memcpy_s(pData->Bump, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_SPEC:
				memcpy_s(pData->Spec, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			case TEX_TYPE::TEX_EMIS:
				memcpy_s(pData->Emiv, sizeof(wchar_t) * PATHNUM, _Path, sizeof(wchar_t) * PATHNUM);
				break;

			default:
				break;
			}
		}
	}
}