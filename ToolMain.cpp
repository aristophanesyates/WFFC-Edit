#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);
	m_toolHandle = handle;
	//initialise centre position
	m_centrePos.x = m_width / 2;
	m_centrePos.y = m_height / 2;
	pos.x = m_width / 2;
	pos.y = m_height / 2;
	ClientToScreen(m_toolHandle, &m_centrePos);
	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg)
{
	//mouse determing camera angle:
	if (!m_toolInputCommands.freeMouse)
	{
		SetCursorPos(m_centrePos.x, m_centrePos.y);
	}
	//mouse determining camera angle and camera is unfocused:
	if (!m_toolInputCommands.freeMouse && !m_toolInputCommands.focus)
	{
		//clears selection
		DeselectAll();
	}
	//mouse freed
	else
	{
		//selection and picking
		if (m_toolInputCommands.leftClick)
		{
			int clickedObject = m_d3dRenderer.Pick();
			//cursor has hit an object?
			if (clickedObject != -1)
			{
				ToggleSelectObject(clickedObject);
			}
		}
		//transform selected objects according to mode and keys pressed
		Transform();
	}
	m_d3dRenderer.Tick(&m_toolInputCommands, &m_selectedObjects);
	//reset leftclick
	m_toolInputCommands.leftClick = false;
	UpdateDisplayList();
}

void ToolMain::UpdateInput(MSG * msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		//get mouse coordinates
		m_toolInputCommands.mouseX = GET_X_LPARAM(msg->lParam);
		m_toolInputCommands.mouseY = GET_Y_LPARAM(msg->lParam);
		GetCursorPos(&pos);
		//reset delta mouse
		m_toolInputCommands.mouseHori = 0;
		m_toolInputCommands.mouseVert = 0;
		//mouse determing camera angle:
		if (!m_toolInputCommands.freeMouse && !m_toolInputCommands.focus)
		{
			//compute delta mouse
			m_toolInputCommands.mouseHori = pos.x - m_centrePos.x;
			m_toolInputCommands.mouseVert = pos.y - m_centrePos.y;
		}
		break;

	case WM_LBUTTONDOWN:
		m_toolInputCommands.leftClick = true;
		break;

	case WM_LBUTTONUP:
		m_toolInputCommands.leftClick = false;
		break;

	}
	//free mouse - shift

	if (m_keyArray[16])
	{
		m_toolInputCommands.freeMouse = true;
	}
	else
	{
		m_toolInputCommands.freeMouse = false;
	}
	
	//focus camera on selected objects - ctrl

	if (m_keyArray[17])
	{
		m_toolInputCommands.focus = true;
	}
	else
	{
		m_toolInputCommands.focus = false;
	}
	//WASD movement
	// 16 = shift
	// 17 = control

	//WASD QE

	if (m_keyArray['W'] && CameraCanMove())
	{
		m_toolInputCommands.forward = true;
	}
	else m_toolInputCommands.forward = false;

	if (m_keyArray['S'] && CameraCanMove())
	{
		m_toolInputCommands.back = true;
	}
	else m_toolInputCommands.back = false;

	if (m_keyArray['A'] && CameraCanMove())
	{
		m_toolInputCommands.left = true;
	}
	else m_toolInputCommands.left = false;

	if (m_keyArray['D'] && CameraCanMove())
	{
		m_toolInputCommands.right = true;
	}
	else m_toolInputCommands.right = false;

	if (m_keyArray['Q'] && CameraCanMove())
	{
		m_toolInputCommands.down = true;
	}
	else m_toolInputCommands.down = false;

	if (m_keyArray['E'] && CameraCanMove())
	{
		m_toolInputCommands.up = true;
	}
	else m_toolInputCommands.up = false;

	//rotation - arrow keys + page up & page down
	if (m_keyArray[38])
	{
		m_toolInputCommands.arrowForward = true;
	}
	else m_toolInputCommands.arrowForward = false;

	if (m_keyArray[40])
	{
		m_toolInputCommands.arrowBack = true;
	}
	else m_toolInputCommands.arrowBack = false;

	if (m_keyArray[37])
	{
		m_toolInputCommands.arrowLeft = true;
	}
	else m_toolInputCommands.arrowLeft = false;

	if (m_keyArray[39])
	{
		m_toolInputCommands.arrowRight = true;
	}
	else m_toolInputCommands.arrowRight = false;

	if (m_keyArray[33])
	{
		m_toolInputCommands.pageUp = true;
	}
	else m_toolInputCommands.pageUp = false;

	if (m_keyArray[34])
	{
		m_toolInputCommands.pageDown = true;
	}
	else m_toolInputCommands.pageDown = false;
}

void ToolMain::ToggleSelectObject(int objectId)
{
	int numberOfObjects = m_objectSelected.size();
	if (!m_objectSelected[objectId])
	{
		//object unselected, select object
		m_d3dRenderer.Highlight(objectId, true);
		m_objectSelected[objectId] = true;
		if (m_objectSelected.size() > numberOfObjects)
		{
			//new element, add to vector
			m_selectedObjects.push_back(&m_sceneGraph.at(objectId));
		}
	}
	else
	{
		//object selected, deselect object
		m_d3dRenderer.Highlight(objectId, false);
		m_objectSelected[objectId] = false;
	}
}

void ToolMain::Deselect(int objectId)
{
	m_d3dRenderer.Highlight(objectId, false);
	m_objectSelected[objectId] = false;
}

void ToolMain::DeselectAll()
{
	m_selectedObjects.clear();
	m_objectSelected.clear();
	m_d3dRenderer.DeselectAll();
}

void ToolMain::Transform()
{
	// iterate through selected objects map, skipping unselected objects
	SceneObject * object = nullptr;
	for (auto selectedObject : m_objectSelected)
	{
		bool notSelected = !selectedObject.second;
		int objectID = selectedObject.first;
		if (notSelected)
		{
			continue;
		}
		object = &m_sceneGraph.at(objectID);
		
		//do transforms
		if (m_toolInputCommands.arrowForward)
		{
			DirectX::SimpleMath::Vector3 vector = m_d3dRenderer.CamXZForward();
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posX += vector.x;
				object->posY += vector.y;
				object->posZ += vector.z;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaZ += 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotZ += 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
		if (m_toolInputCommands.arrowRight)
		{
			DirectX::SimpleMath::Vector3 vector = m_d3dRenderer.CamRight();
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posX += vector.x;
				object->posY += vector.y;
				object->posZ += vector.z;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaX += 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotX += 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
		if (m_toolInputCommands.arrowBack)
		{
			DirectX::SimpleMath::Vector3 vector = m_d3dRenderer.CamXZForward();
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posX -= vector.x;
				object->posY -= vector.y;
				object->posZ -= vector.z;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaZ -= 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotZ -= 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
		if (m_toolInputCommands.arrowLeft)
		{
			DirectX::SimpleMath::Vector3 vector = m_d3dRenderer.CamRight();
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posX -= vector.x;
				object->posY -= vector.y;
				object->posZ -= vector.z;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaX -= 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotX -= 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
		if (m_toolInputCommands.pageUp)
		{
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posY += 1;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaY += 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotY += 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
		if (m_toolInputCommands.pageDown)
		{
			switch (m_transform_mode)
			{
			case TRANSFORM_MODE::TRANSLATE:
				object->posY -= 1;
				break;
			case TRANSFORM_MODE::SCALE:
				object->scaY -= 1;
				break;
			case TRANSFORM_MODE::ROTATE:
				object->rotY -= 1;
				break;
			case TRANSFORM_MODE::NONE:
				break;
			}
		}
	}
}

void ToolMain::UpdateDisplayList()
{
	// iterate through selected objects map, skipping unselected objects
	for (auto selectedObject : m_objectSelected)
	{
		bool notSelected = !selectedObject.second;
		int objectID = selectedObject.first;
		if (notSelected)
		{
			continue;
		}
		else
		{
			//update object in display list
			m_d3dRenderer.UpdateDisplayList(objectID, &m_sceneGraph);
		}
	}
}

void ToolMain::SetTranslateMode()
{
	m_transform_mode = TRANSFORM_MODE::TRANSLATE;
}

void ToolMain::SetScaleMode()
{
	m_transform_mode = TRANSFORM_MODE::SCALE;
}

void ToolMain::SetRotateMode()
{
	m_transform_mode = TRANSFORM_MODE::ROTATE;
}
