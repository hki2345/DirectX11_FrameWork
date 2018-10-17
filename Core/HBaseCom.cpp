#include "HBaseCom.h"
#include "HTrans.h"
#include "HMACRO.h"


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