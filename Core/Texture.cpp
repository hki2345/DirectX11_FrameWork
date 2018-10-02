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

//  확장자 마다 다른 함수를 실행 -> 다이렉트에서 그렇게 하니까
// 뭐 당연한 거겠다.
// 단 파일 확장자는 파일에 시작 간 확장자를 구별하기 위한 일종의 코드만 다를뿐
// 바이너리 형태는 같다. (암호화하지 안흔다면) 하면 뭐 복호화 해야하는 거고
bool Texture::Load()
{
	// 확장자명 검색 - DDS -  다이렉트 전용 이미지 파일
	// 자주 쓰이는 것이 앞쪽에 위치 ㅇㅇ
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
	else //png, bmp, jpg확장자는 else로 묶음 ㅇㅇ 
	{
		if (S_OK != DirectX::LoadFromWICFile(path(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image))
		{
			return false;
		}
	}

	// 부ㅠ수정
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

void Texture::Update(const UINT& _Slot)
{
	Core_Class::device_context()->VSSetShaderResources(_Slot, 1, &m_View);
	Core_Class::device_context()->PSSetShaderResources(_Slot, 1, &m_View);
}
