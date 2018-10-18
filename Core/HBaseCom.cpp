#include "HBaseCom.h"
#include "HTrans.h"
#include "KMacro.h"


HBaseCom::HBaseCom()
{
}


HBaseCom::~HBaseCom()
{
}

void HBaseCom::ComInit()
{
	m_Trans = GetCom<HTrans>();
}