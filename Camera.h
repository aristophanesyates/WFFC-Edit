#pragma once
#include "pch.h"
//using namespace DirectX;
//using namespace SimpleMath;

class InputCommands;
class SceneObject;

class Camera
{
protected:
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector2		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;

public:

	void Update(InputCommands & inputCommands);
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float								m_movespeed;
	float								m_camRotRate;
	//initialise camera
	void Init(float movespeed, float camRotRate);
	//set camera position
	void SetPosition(float x, float y, float z);
	//set camera position
	void SetPosition(DirectX::SimpleMath::Vector3 location);
	//get camera position
	DirectX::SimpleMath::Vector3 GetPosition();

	//return the location infront of camera.
	DirectX::SimpleMath::Vector3 GetLookat();
	//return the location that is the average of the selected object positions.
	DirectX::SimpleMath::Vector3 Focus(std::vector<SceneObject *> & selectedObjects);
};

