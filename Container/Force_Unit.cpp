#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <KSphere_Col.h>


Force_Unit::Force_Unit(const wchar_t* _Name, KPtr<SC2_Force> _Force) : m_Force(_Force)
{
	Begin::name(_Name);
}


Force_Unit::~Force_Unit()
{
}

bool Force_Unit::Init()
{


	return true;
}

void Force_Unit::Update()
{

}