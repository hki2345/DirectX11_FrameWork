#include "Begin_Updater.h"



Begin_Updater::Begin_Updater() : m_b_Active(true), m_b_Death(false)
{
}


Begin_Updater::~Begin_Updater()
{
}

bool Begin_Updater::Init() { return true; }
void Begin_Updater::PrevUpdate(){}
void Begin_Updater::Update(){}
void Begin_Updater::NextUpdate(){}
void Begin_Updater::UIRender(){}
void Begin_Updater::DebugUpdate(){}
void Begin_Updater::DebugRender() {}