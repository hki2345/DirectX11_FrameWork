#include "TheOne.h"
#include "Component.h"
#include "TransPosition.h"



KVector4 TheOne::World_Scale()
{
	return one_Trans->World_Scale();
}
KVector4 TheOne::World_Pos()
{
	return one_Trans->World_Pos();
}
KVector4 TheOne::World_Rotate()
{
	return one_Trans->World_Rotate();
}

KVector4 TheOne::World_Forward()
{
	return one_Trans->World_Forward();
}
KVector4 TheOne::World_Up()
{
	return one_Trans->World_Up();
}
KVector4 TheOne::World_Right()
{
	return one_Trans->World_Right();
}
KVector4 TheOne::World_Back()
{
	return one_Trans->World_Back();
}
KVector4 TheOne::World_Down()
{
	return one_Trans->World_Down();
}
KVector4 TheOne::World_Left()
{
	return one_Trans->World_Left();
}

KVector4 TheOne::Local_Scale()
{
	return one_Trans->Local_Scale();
}
KVector4 TheOne::Local_Pos()
{
	return one_Trans->Local_Pos();
}
KVector4 TheOne::Local_Rotate()
{
	return one_Trans->Local_Rotate();
}

KVector4 TheOne::Local_Forward()
{
	return one_Trans->Local_Forward();
}
KVector4 TheOne::Local_Up()
{
	return one_Trans->Local_Up();
}
KVector4 TheOne::Local_Right()
{
	return one_Trans->Local_Right();
}
KVector4 TheOne::Local_Back()
{
	return one_Trans->Local_Back();
}
KVector4 TheOne::Local_Down()
{
	return one_Trans->Local_Down();
}
KVector4 TheOne::Local_Left()
{
	return one_Trans->Local_Left();
}

void TheOne::World_Pos(const KVector4& _Value)
{
	one_Trans->World_Pos(_Value);
}
void TheOne::World_Scale(const KVector4& _Value)
{
	one_Trans->World_Scale(_Value);
}
void TheOne::World_Rotate(const KVector4& _Value)
{
	one_Trans->World_Rotate(_Value);
}

void TheOne::World_Scale(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->World_Scale(_X, _Y, _Z);
}
void TheOne::World_Pos(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->World_Pos(_X, _Y, _Z);
}
void TheOne::World_Rotate(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->World_Rotate(_X, _Y, _Z);
}
	 
void TheOne::World_Scale(const KVector2& _Vec, const float& _Z)
{
	one_Trans->World_Scale(_Vec, _Z);
}
void TheOne::World_Pos(const KVector2& _Vec, const float& _Z)
{
	one_Trans->World_Pos(_Vec, _Z);
}
void TheOne::World_Rotate(const KVector2& _Vec, const float& _Z)
{
	one_Trans->World_Rotate(_Vec, _Z);
}






void TheOne::Local_Scale(const KVector4& _Value)
{
	one_Trans->Local_Scale(_Value);
}
void TheOne::Local_Pos(const KVector4& _Value)
{
	one_Trans->Local_Pos(_Value);
}
void TheOne::Local_Rotate(const KVector4& _Value)
{
	one_Trans->Local_Rotate(_Value);
}

void TheOne::Local_Scale(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->Local_Scale(_X, _Y, _Z);
}
void TheOne::Local_Pos(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->Local_Pos(_X, _Y, _Z);
}
void TheOne::Local_Rotate(const float& _X, const float& _Y, const float& _Z)
{
	one_Trans->Local_Rotate(_X, _Y, _Z);
}

void TheOne::Local_Scale(const KVector2& _Vec, const float& _Z)
{
	one_Trans->Local_Scale(_Vec, _Z);
}
void TheOne::Local_Pos(const KVector2& _Vec, const float& _Z)
{
	one_Trans->Local_Pos(_Vec, _Z);
}
void TheOne::Local_Rotate(const KVector2& _Vec, const float& _Z)
{
	one_Trans->Local_Rotate(_Vec, _Z);
}

bool TheOne::BWorld() const
{
	return one_Trans->BWorld();
}