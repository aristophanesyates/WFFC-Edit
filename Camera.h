#pragma once
#include "pch.h"
//using namespace DirectX;
//using namespace SimpleMath;

class InputCommands;

class Camera
{
protected:
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;

public:

	void Update(InputCommands & inputCommands);
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float								m_movespeed;
	float								m_camRotRate;
	void Init(float movespeed, float camRotRate);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::SimpleMath::Vector3 location);
	DirectX::SimpleMath::Vector3 GetPosition();

	void SetLookAt(float x, float y, float z);
	void SetLookAt(DirectX::SimpleMath::Vector3 location);
	DirectX::SimpleMath::Vector3 GetLookat();
};

