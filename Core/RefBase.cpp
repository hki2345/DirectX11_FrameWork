#include "RefBase.h"

HRefBase::HRefBase(const HRefBase& _Otehr) : m_RefCount(0)
{

}

HRefBase::HRefBase() : m_RefCount(0)
{
}


HRefBase::~HRefBase()
{
}

void HRefBase::ADDRef() {
	++m_RefCount;
}

void HRefBase::DECRef() {
	--m_RefCount;

	if (0 >= m_RefCount)
	{
		delete this;
	}
}