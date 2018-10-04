#include "Begin.h"



Begin::Begin()
{
}

Begin::Begin(const wchar_t* _Name) : m_Name(_Name)
{

}

Begin::Begin(const Begin& _Other) : TypeBegin(_Other), m_Name(_Other.m_Name)
{

}


Begin::~Begin()
{
}
