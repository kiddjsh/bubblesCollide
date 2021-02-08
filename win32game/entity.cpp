#include "entity.h"
#include <Windows.h>
#include "utilitymacro.h"

void Entity::init(Vec2<float> center, float radius, int color)
{
	area.center = center;
	area.radius = radius;
	this->color = color;
}

void Entity::draw(void* a_hdc)
{
	HDC hdc = (HDC)a_hdc;
	// remember what old draw settings were
	HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(DC_PEN));
	COLORREF oldDCPenColor = SetDCPenColor(hdc, DARKEN(color) );
	HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(DC_BRUSH));
	COLORREF oldDCBrushColor = SetDCBrushColor(hdc, color);
	// draw with new settings
	area.draw(a_hdc);
	// revert to old settings
	SetDCBrushColor(hdc, oldDCBrushColor);
	SelectObject(hdc, oldBrush);
	SetDCPenColor(hdc, oldDCPenColor);
	SelectObject(hdc, oldPen);
}

bool Entity::collidesWith(GameObject * go)
{
	Entity * e = dynamic_cast<Entity*>(go);
	if(e != NULL) {
		return area.overlaps(e->area);
	}
	return false;
}
