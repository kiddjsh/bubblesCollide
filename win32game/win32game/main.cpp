// lab7: win32game
// Joshua Kidder
// read main.cpp, and follow the instructions at the bottom of main.cpp

#include <windows.h>
#include "application.h"
#include <stdlib.h>
#include <time.h>

#include "win32console.h" // createWin32Console

// game object. Global, but accessible only in this compilation unit
static Application app;

// handle double-buffering
#include "screenbuffer.h"
ScreenBuffer screen;

// pass Windows events to the application
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
// conveniently converts lParam into an X/Y COORD struct
#define CRD		(*((COORD*)&lParam))
	switch(msg)
	{
	case WM_CREATE:
		createWin32Console(); // create a console, for helpful debug messages
		screen.init(hwnd);
		//screen.setUseDoubleBuffer(false); // test non-double-buffering
		app.init(hwnd);
		break;
	case WM_DESTROY:
		app.release();
		screen.release();
		FreeConsole(); // cleanup win32 console
		PostQuitMessage(0);			
		break;
	case WM_PAINT:
		screen.drawBegin();
		app.draw(screen.getHdc());
		screen.drawEnd();
		break;
	case WM_SIZE:
		screen.resize();
		app.resize(CRD.X, CRD.Y);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:	app.setMousePress(CRD.X,CRD.Y,wParam,msg);	break;
	case WM_MOUSEMOVE:	app.setMouseState(CRD.X,CRD.Y,wParam);		break;
	case WM_KEYDOWN:	app.setUserInput(wParam);		break;
	case WM_CLOSE:		DestroyWindow(hwnd);		break;
	default:	return DefWindowProc(hwnd, msg, wParam, lParam);
	}
#undef CRD
	return 0;
}

// entry point of a Win32 application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// boiler-plate Win32 application code-------------------------------------
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	const char g_szClassName[] = "myWindowClass";
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Application Name",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);
	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// end boiler-plate -------------------------------------------------------

	// seed random number
	srand((unsigned int)time(0));

	// variables keep track of real-time in game-loop
	float seconds;
	unsigned long now = GetTickCount(), then, passed, preferredDelay = 20;
	do
	{
		// calculate timing
		then = now;           // remember when the last loop started
		now = GetTickCount(); // get when this loop started
		passed = now - then;  // calculate how long the last loop took
		seconds = passed / 1000.0f; // calculate it in seconds
		// queue to refresh the screen each game-loop iteration
		InvalidateRect (hwnd, NULL, FALSE);
		// handle messages (will eventually call WndProc)
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		// update game based on passage of real time
		app.update(seconds);
		// "throttle code", slows processing, interrupted by any windows event
		unsigned long soon = now + preferredDelay;
		while(GetTickCount() < soon && !PeekMessage(&Msg,0,0,0,0)){Sleep(1);}
	}
	while(WM_QUIT != Msg.message);

	return Msg.wParam;
}

// Note: If while attempting to compile, you receive an error that looks like:
//     cannot convert from 'const char [14]' to 'LPCWSTR'
// This is a compile settings problem.
//     Change your project configuration to use multibyte strings. Press ALT+F7
//     to open the properties, and navigate to 
//         Configuration Properties > General. 
//     Switch "Character Set" to "Use Multi-Byte Character Set". 
//     http://social.msdn.microsoft.com/Forums/vstudio/en-US/c1b08c0a-a803-41c3-ac8c-84eba3be1ddb/faq-cannot-convert-from-const-char-to-lpctstr?forum=vclanguage
//  TL;DR http://img5.uploadhouse.com/fileuploads/2286/22866038f4d569af84a8ed576c98c2692642251.png
//
// 1) Replace Vector2 and Coordinate
//   a) Create a new Vec2 class as a replacement for both Vector2 and
//      Coordinate. Vec2 must be a templated class, so that it stores and
//      manipulates X and Y values of a templated type. It may be helpful to
//      start with the Vector2 class, add "template<typename TYPE>" to the
//      header, and replace (Ctrl+H) each instance of "float" with "TYPE", and
//      "Vector2" with "Vec2".
//   b) Remember, a templated class must have all method definitions in the
//      same file as the templated class declaration. Also remember that method
//      definitions outside of a templated class' declaration need a 
//      "template<typename TYPE>" header, and a "<TYPE>" appended to the class
//      name with the scope resolution operator (e.g.: 
//      "void Vec2<TYPE>::limitMagnitude(TYPE max) {/* method body */}").
//   c) Replace the use of Vector2 with Vec2<float>.
//   d) Replace the use of Coordinate with Vec2<int>.
// 2) More Game Goals
//   a) Instead of having a single goal for the player, create at least 3
//      randomly placed goals for the player. Use a "std::vector<Entity>" 
//      object (from the Standard Template Library) named "goals" to store the
//      goals.
//   c) Whenever the user clicks in the game window, the game should create
//      another goal object for the player to get, and add it to the
//      "std::vector<Entity> goals" object.
//   d) Each retrieved goal should be removed from the game after being
//      retrieved by the player. A "You Win!" message should display when the
//      player retrieves all goals.
//
// [Autonomous Agent] The player controlled Entity can move to collect each
//      goal without any user input!
// [He's Coming, RUN!] Create a MovingTarget class that extends Entity, and use
//      that for the game goals. Each MovingTarget should wander randomly and
//      slowly, running away from the player if the player comes withing a
//      certain radius. Be sure to prevent goal entities from escaping the game
//      boundaries!
// [I'll Make My Own List] Create your own templated list type and use it
//      instead of the std::vector class.
// [Blocked!] Implement collision detection, so that Entity objects can't
//      occupy the same area.
// [We've Had Enough!] Create a Hunter class that extends Entity, and slowly
//      follows the player's Entity, ending the game in defeat if the player
//      should be caught by a Hunter.
// [Let's Fight!] Create a Projectile class that acts as a projectile attack
//      for the player's Entity. Keep track of all projectiles in the game with
//      a "vector<Projectile>" object. If there are Hunters in the game,
//      give Hunters a projectile attack as well! Don't forget to remove
//      projectiles from the game once they hit something or leave the game
//      boundaries!
//
// When finished:
// 1) Make sure your name is at the top of this source file
// 2) Submit this project online
//   a) Right-click on the .cpp file's name within visual studio, and select
//      "Open Containing Folder"
//   b) Close Visual Studio (you may re-open this .cpp file by right-clicking
//      on it in the file system, and slecting edit)
//   c) Make sure the following files are DELETED from the project's file
//      structure:
//      * Any file with the extension: ".ncb", ".sdf"
//      * Any folder named: "Debug" or "ipch"
//      * If Visual Studio is open, you will not be able to delete some files
//      * If you do not see file extensions, press Alt in the file explorer,
//        select "Tools"->"Folder options..."->"View", and uncheck
//        "Hide extensions for known file types".
//   d) zip the file structure (the project), which is now missing the
//      temporary files
//      * Select all of the files in the project folder
//      * If the resulting zip file is more than 1mb, you have included
//        temporary files mentioned above. Delete temporary files (that don't
//        have a .cpp or .h extension) and try again.
//   e) upload the project to the appropriate week's iLab at the GSP125
//      website, linked from http://www.devryu.net
