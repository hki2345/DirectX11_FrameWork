#pragma once
#include "KShader.h"

// �ȼ��� ���ؽ��� ���� �и��Ǿ� �ִµ�
// ���߿� ���� �ҋ� ������ �϶��
class Pixel_Shader :	public KShader
{
private:
	ID3D11PixelShader* m_Shader;

public:
	// �Լ� �̸� -> char�� �޴´�. GPU���� \
	// �ڿ� �ΰ��� UINT�� ���� ����
	bool Load(const char* _Func, UINT _VH = 5, UINT _VL = 0);
	void Update() override;


private:
	void UpdateCB(KPtr<Const_Buffer> _Buf) override;

public:
	Pixel_Shader();
	~Pixel_Shader();
};

