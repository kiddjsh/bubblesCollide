#include "mob.h"
#include <Windows.h>
#include "utilitymacro.h"

void MOB::draw(void* a_hdc)
{
	Entity::draw(a_hdc);
	HDC hdc = (HDC)a_hdc;
	HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(DC_PEN));
	COLORREF oldDCPenColor = SetDCPenColor(hdc, DARKEN(color) );

	MoveToEx(hdc,(int)area.center.x,(int)area.center.y,NULL);
	Vec2<float> direction = area.center + velocity;
	LineTo(hdc,(int)direction.x,(int)direction.y);

	SetDCPenColor(hdc, oldDCPenColor);
	SelectObject(hdc, oldPen);
}

void MOB::applyStoppingForce(float force)
{
	Vec2<float> stoppingForce = velocity;
	stoppingForce.limitMagnitude(force);
	velocity -= stoppingForce;
}
