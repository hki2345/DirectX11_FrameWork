#include "Component_DE.h"



Component_DE::Component_DE()
{
}


Component_DE::~Component_DE()
{
}

void Component_DE::Init_Component()
{
	m_Trans = Get_Component <TransPosition>();
}