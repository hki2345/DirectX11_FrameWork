#include "HObjectBase.h"



HObjectBase::HObjectBase()
{
}

HObjectBase::HObjectBase(const HObjectBase& _Other) : HTypeBase(_Other), m_Name(_Other.m_Name)
{

}

HObjectBase::HObjectBase(const wchar_t* _Name) : m_Name(_Name)
{

}


HObjectBase::~HObjectBase()
{
}
