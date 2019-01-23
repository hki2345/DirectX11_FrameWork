#include "Controll_User.h"
#include "Force_Unit.h"


#include <Core_Class.h>
#include <Renderer_UI.h>


void Controll_User::Init_UI()
{
	m_uMedic = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uMedic->one()->Trans()->scale_local(KVector(100.0f, 100.0f, 10.0f, .0f));
	m_uMedic->one()->Trans()->pos_local(KVector(-200.0f, -200.0f, 1.1f, .0f));
	m_uMedic->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"btn-ability-tychus-odin.dds");


	m_uBackHP = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uBackHP->one()->Trans()->scale_local(KVector(300.0f, 30.0f, 10.0f, .0f));
	m_uBackHP->one()->Trans()->pos_local(KVector(0.0f, 200.0f, 1.1f, .0f));
	m_uBackHP->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"storm_ui_glues_loading_progressbarbg.dds");

}

void Controll_User::Update_UI()
{

}