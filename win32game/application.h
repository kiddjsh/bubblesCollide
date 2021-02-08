#pragma once


#include "entity.h"
#include "mob.h"
#include <vector>
#include "vec2.h"
#include <stdlib.h>

class Application
{
private:
	int state;
	Vec2<int> mouseLoc;
	int mouseButtonsPressed;
	int width, height;

	MOB player;

	// random goals
	vector<Entity> goals;
	
	Entity mouseClickEntity;

	void* hwnd; // HWND, void* type so remove windows.h dependency from header

	char * outputMessage;

public:
	static const int 
		STATE_INIT = 0,    // an application not yet fully initialized
		STATE_RUNNING = 1, // running
		STATE_QUIT = 2,    // user requested quit
		STATE_RELEASED = -1;// system has been de-allocated

	/** @return Application::STATE_* */
	int getState(){ return state; }

	Application():state(STATE_INIT){}

	void init(void* hwnd);
	void release();

	int getWidth() { return width; }
	int getHeight() { return height; }
	void resize(int width, int height); // call when window is resized
	void draw(void* hdc);               // call to draw
	void setUserInput(int input);
	void setMouseState(int x, int y, int mouseButtonState); // when mouse moves
	void setMousePress(int x, int y, int mouseButtonState, int msg);
	bool isLeftMousePressed();
	void update(float secondsPassed);
};
