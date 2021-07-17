#pragma once

struct InputCommands
{
	bool forward = false;
	bool back = false;
	bool right = false;
	bool left = false;
	bool arrowForward = false;
	bool arrowBack = false;
	bool arrowRight = false;
	bool arrowLeft = false;
	bool pageUp = false;
	bool pageDown = false;
	bool freeMouse = false;
	float mouseHori = 0;
	float mouseVert = 0;
	int mouseX = 0;
	int mouseY = 0;
	bool leftClick = false;
	bool rightClick = false;
};
