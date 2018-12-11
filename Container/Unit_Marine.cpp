#include "Unit_Marine.h"



#include <Renderer_BonAni.h>
#include <KSphere_Col.h>


Unit_Marine::Unit_Marine() 
{
	name(L"Marine");
}


Unit_Marine::~Unit_Marine()
{
}

bool Unit_Marine::Init()
{
	m_Ren->Set_Fbx(L"Marine.FBX");
	m_Ren->Create_Clip(L"TestAni", 120, 5000);
	m_Ren->Set_Clip(L"TestAni");


	return true;
}
void Unit_Marine::Update()
{
}