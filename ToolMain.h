#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>
#include <map>


class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	void SetTranslateMode();
	void SetScaleMode();
	void SetRotateMode();									//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;		//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;			//our landscape chunk
	int timer;

private:	//methods
	//!freeMouse || focus
	bool inline CameraCanMove() { return !m_toolInputCommands.freeMouse || m_toolInputCommands.focus; }
	//highlights the object with a matching id, sets it as selected, and adds it to selected objects if it wasn't already there.
	void	ToggleSelectObject(int objectId);
	//dehighlights the object with a matching id and sets it to be deselected
	void	Deselect(int objectId);
	//dehighlights all objects and clears the selection
	void	DeselectAll();		
	//perform transformations based on key inputs
	void	Transform();
	//iterate through map m_objectSelected, and updates the display list using the selected object's id
	void UpdateDisplayList();
private:	//variables
	enum class TRANSFORM_MODE {TRANSLATE, SCALE, ROTATE, NONE};
	TRANSFORM_MODE m_transform_mode = TRANSFORM_MODE::NONE;
	std::map<int, bool>					m_objectSelected;	//is object [id] selected?
	std::vector<SceneObject *>			m_selectedObjects;	//current selection
	InputCommands						m_toolInputCommands;//input commands that we want to use and possibly pass over to the renderer

	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	POINT m_centrePos;
	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	//float m_mouseLastX;
	//float m_mouseLastY;

	
};
