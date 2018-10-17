#include "HUpdaterBase.h"

HUpdaterBase::HUpdaterBase() : m_bUpdate(true), m_bDeath(false)
{
}

HUpdaterBase::~HUpdaterBase()
{
}

bool HUpdaterBase::Init() { return true; }
void HUpdaterBase::PrevUpdate() {}
void HUpdaterBase::Update() {}
void HUpdaterBase::NextUpdate() {}
void HUpdaterBase::DebugUpdate() {}
void HUpdaterBase::DebugRender() {}