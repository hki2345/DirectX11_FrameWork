#include "H3DMeshRenderer.h"
#include "HMACRO.h"
#include "HWindow.h"
#include "HTrans.h"
#include "HResMgr.h"


H3DMeshRenderer::H3DMeshRenderer()
{
}

H3DMeshRenderer::~H3DMeshRenderer()
{
}
void H3DMeshRenderer::Render(HPTR<HCamera> _Camera)
{
}

bool H3DMeshRenderer::Init(int _Order /*= 0*/)
{
	HRenderer::Init(_Order);

	SetRSState(L"SBACK");
	return true;
}

