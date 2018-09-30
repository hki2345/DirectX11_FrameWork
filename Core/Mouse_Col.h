#pragma once
#include "SmartPtr.h"

#define MOUSE_DOWN Mouse_Col::Mouse_Down
#define MOUSE_UP Mouse_Col::Mouse_Up
#define MOUSE_PRESS Mouse_Col::Mouse_Press
 
class TheOne;
class Collider2D_DE;
class Mouse_Col : public SmartPtr
{
public:
	enum MOUSE_BTN {
		MBTN_LEFT = 0,
		MBTN_RIGHT,
		MBTN_MIDDLE,
	};

private:
	static KPtr<TheOne>			m_Mouse;
	static KPtr<Collider2D_DE>	m_Col2D;

	static KPtr<Collider2D_DE> Get_Coll2D();

public:
	static bool Init();
	static void Update();

	static KPtr<Collider2D_DE>	Mouse_Down(const MOUSE_BTN& _Value);
	static KPtr<Collider2D_DE>	Mouse_Up(const MOUSE_BTN& _Value);
	static KPtr<Collider2D_DE>	Mouse_Press(const MOUSE_BTN& _Value);
	// static KPtr<Collider2D_DE> Mouse_UnPress();

public:
	Mouse_Col();
	~Mouse_Col();
};

