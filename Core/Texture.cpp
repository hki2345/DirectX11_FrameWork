#include "Texture.h"
#include "Core_Class.h"
#include "KMacro.h"


#if X64
#if _DEBUG
#pragma comment(lib, "DirectXTex_X64_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X64_RELEASE")
#endif
#else if WIN32
#if _DEBUG
#pragma comment(lib, "DirectXTex_X86_DEBUG")
#else if NDEBUG
#pragma comment(lib, "DirectXTex_X86_RELEASE")
#endif
#endif // DEBUG


Texture::Texture()
{
}


Texture::~Texture()
{
	if (nullptr != m_Texture2D) { m_Texture2D->Release(); }
	if (nullptr != m_View) { m_View->Release(); }
}

//  Ȯ���� ���� �ٸ� �Լ��� ���� -> ���̷�Ʈ���� �׷��� �ϴϱ�
// �� �翬�� �Űڴ�.
// �� ���� Ȯ���ڴ� ���Ͽ� ���� �� Ȯ���ڸ� �����ϱ� ���� ������ �ڵ常 �ٸ���
// ���̳ʸ� ���´� ����. (��ȣȭ���� ����ٸ�) �ϸ� �� ��ȣȭ �ؾ��ϴ� �Ű�
bool Texture::Load()
{
	// Ȯ���ڸ� �˻� - DDS -  ���̷�Ʈ ���� �̹��� ����
	// ���� ���̴� ���� ���ʿ� ��ġ ����
	if (true == is_Exist(L".dds") || true == is_Exist(L".DDS") || true == is_Exist(L".DDs"))
	{
		if (S_OK != DirectX::LoadFromDDSFile(path(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}
	else if (true == is_Exist(L".tga") || true == is_Exist(L".TGA"))
	{
		if (S_OK != DirectX::LoadFromTGAFile(path(), nullptr, m_Image))
		{
			return false;
		}
	}
	else //png, bmp, jpgȮ���ڴ� else�� ���� ���� 
	{
		if (S_OK != DirectX::LoadFromWICFile(path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	// �Τм���
	if (S_OK != DirectX::CreateShaderResourceView(Core_Class::device(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		&m_View))
	{
		return false;
	}
	
	KASSERT(nullptr == m_View);

	m_View->GetResource((ID3D11Resource**)(&m_Texture2D));

	KASSERT(nullptr == m_Texture2D);
	return true;
}

void Texture::Update()
{
	Core_Class::device_context()->PSSetShaderResources(0, 1, &m_View);
}
