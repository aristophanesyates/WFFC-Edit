#pragma once

struct InputCommands
{
	bool forward = false;		//key w
	bool back = false;			//key s
	bool right = false;			//key d
	bool left = false;			//key a
	bool up = false;			//key e
	bool down = false;			//key q
	bool arrowForward = false;	//key up-arrow
	bool arrowBack = false;		//key down-arrow
	bool arrowRight = false;	//key right-arrow
	bool arrowLeft = false;		//key left-arrow
	bool pageUp = false;		//key page-up
	bool pageDown = false;		//key page-down
	bool freeMouse = false;		//key shift
	bool focus = false;			//key ctrl
	float mouseHori = 0;		//delta-mouse-x
	float mouseVert = 0;		//delta-mouse-y
	int mouseX = 0;				//mouse-x
	int mouseY = 0;				//mouse-y
	bool leftClick = false;		//left-mouse-button
	bool rightClick = false;	//right-mouse-button
};
