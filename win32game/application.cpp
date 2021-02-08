#include "application.h"
#include <stdio.h> // printf (debug messages, thanks createWin32Console()!)
#include <Windows.h>

void Application::init(void* hwnd)
{
	printf("init\n");
	if(state == Application::STATE_INIT)
	{
		this->hwnd = hwnd;
		// setup the game entities
		RECT r;
		GetClientRect((HWND)hwnd, &r);
		height = r.bottom-r.top;
		width = r.right-r.left;
		player.init(Vec2<float>(width/2.0f, height/2.0f), 20, RGB(32,255,32));
		player.setMaxSpeed(100);
		// make 3 random goals
		for (int i = 0; i < 5; i++)
		{
			float wx = (float)(rand() % (width-15));
			float hy = (float)(rand() % (height-20));
			goals.push_back(Entity(Vec2<float>( wx, hy), 10, RGB(255, 192, 32)));
		}

		mouseClickEntity.init(Vec2<float>(0,0), 2, RGB(255,255,255));
		outputMessage= "Use the Arrow keys to move, and collect Goal Objects!";

		// finished initialization
		state = Application::STATE_RUNNING;
	}
}

void Application::release()
{
	printf("release\n");
	if(state != STATE_RELEASED)
	{
		state = STATE_RELEASED;
	}
}

void Application::resize(int a_width, int a_height)
{
	width = a_width;
	height = a_height;
}

void Application::draw(void* a_hdc)
{
	HDC hdc = (HDC)a_hdc;

	// dark background
	RECT r;
	GetClientRect((HWND)hwnd, &r);
	FillRect(hdc, &r, HBRUSH(GetStockObject(DKGRAY_BRUSH)));

	// draw the game
	// for all goals
	for (unsigned int i = 0; i < goals.size(); i++)
	{
	goals[i].draw(hdc);
    }

	// draw player
	player.draw(hdc);
	mouseClickEntity.draw(hdc);

	// draw the in-game message
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc, player.getColor());
	DrawText(hdc, outputMessage, -1, &r, DT_CENTER );
}

void applyMove(MOB * e, float seconds, bool left, bool up, bool right, bool down)
{
	const float maxAccel = 100, accel = maxAccel * seconds;
	if(left || up || right || down) {
		Vec2<float> direction;
		if(left) { direction += Vec2<float>(-maxAccel,0); }
		if(up)   { direction += Vec2<float>(0,-maxAccel); }
		if(right){ direction += Vec2<float>(maxAccel,0);  }
		if(down) { direction += Vec2<float>(0,maxAccel);  }
		direction.limitMagnitude(accel);
		e->addVelocity(direction);
	} else {
		e->applyStoppingForce(accel);
	}
}

void Application::setUserInput(int input)
{
	switch(input){
	case VK_ESCAPE:
		state = Application::STATE_QUIT;
		DestroyWindow((HWND)hwnd);
		return;
	}
}

void Application::setMouseState(int x, int y, int mouseButtonState)
{
	mouseLoc.init(x, y);
	mouseButtonsPressed = mouseButtonState;
}

void Application::setMousePress(int x, int y, int mouseButtonState, int msg)
{
	mouseButtonsPressed = mouseButtonState;
	switch(msg) {
	case WM_LBUTTONDOWN:
		mouseClickEntity.setPosition(Vec2<float>((float)x,(float)y));

		// make goal
		goals.push_back(Entity(Vec2<float>((float)x, (float)y), 10, RGB(255, 192, 32)));
		break;
	}
}

bool Application::isLeftMousePressed()
{
	return (mouseButtonsPressed & MK_LBUTTON) != 0;
}

void Application::update(float secondsPassed)
{
	// another way to test for key presses
	unsigned short PRESSED = (1 << 15);
	if ((GetAsyncKeyState(VK_ESCAPE) & PRESSED) != 0) {
		state = Application::STATE_QUIT;
		DestroyWindow((HWND)hwnd);
	}
	// move direction of player according to key presses
	applyMove(&player, secondsPassed,
		(GetAsyncKeyState(VK_LEFT) & PRESSED) != 0,
		(GetAsyncKeyState(VK_UP) & PRESSED) != 0,
		(GetAsyncKeyState(VK_RIGHT) & PRESSED) != 0,
		(GetAsyncKeyState(VK_DOWN) & PRESSED) != 0);
	// update the game
	player.update(secondsPassed);

	// check for collissions
	// for each goal
	for (unsigned int i = 0; i < goals.size(); i++)
	{
		if (goals[i].collidesWith(&player)) {
			goals[i].setColor(RGB(64, 64, 64));
			goals.erase(goals.begin() + i);
			break;
		}
	}

	// no more goals
	if(goals.size() == 0)
	{
		outputMessage = "You Win!";
	}
}
