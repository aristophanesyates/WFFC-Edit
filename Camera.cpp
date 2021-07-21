#include "Camera.h"
#include "InputCommands.h"
#include "SceneObject.h"

void Camera::Update(InputCommands & inputCommands)
{
	using namespace DirectX;
	using namespace SimpleMath;
	using namespace std;

	float rotRate = m_camRotRate / 100; // scaler for pitch and yaw
	Vector3 upVector = SimpleMath::Vector3::UnitY;
	//add mouse rotation and clamp vertical rotation to one half rotation
	m_camOrientation.y -= inputCommands.mouseHori;
	m_camOrientation.x += inputCommands.mouseVert;
	m_camOrientation.x = max(min(m_camOrientation.x, 314.f), -314.f);
	if (!inputCommands.focus)
	{
		//create look direction from angles in m_camOrientation
		m_camLookDirection = XMVECTOR{ 0.f,0.f,1.f };
		m_camLookDirection = XMVector3Transform(m_camLookDirection, XMMatrixRotationRollPitchYaw(m_camOrientation.x * rotRate, m_camOrientation.y * rotRate, 0.0f));
		m_camLookDirection.Cross(upVector, m_camRight);
	}
	else
	{
		//look direction created by Focus(), compute right direction and up direction
		m_camLookDirection.Cross(upVector, m_camRight);
		m_camRight.Cross(m_camLookDirection, upVector);
	}
	

	//process input and update stuff
	if (inputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (inputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (inputCommands.right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (inputCommands.left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}
	if (inputCommands.up)
	{
		m_camPosition += upVector * m_movespeed;
	}
	if (inputCommands.down)
	{
		m_camPosition -= upVector * m_movespeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}

void Camera::Init(float movespeed, float camRotRate)
{
	m_camPosition = DirectX::SimpleMath::Vector3{ 0, 0, 0 };
	m_camOrientation = DirectX::SimpleMath::Vector2{ 0, 0};
	m_camLookAt = DirectX::SimpleMath::Vector3{ 0, 0, 0 };
	m_camLookDirection = DirectX::SimpleMath::Vector3{ 0, 0, 1 };
	m_camRight = DirectX::SimpleMath::Vector3{ 0, 0, 0 };

	m_movespeed = movespeed;
	m_camRotRate = camRotRate;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_camPosition.x = x; m_camPosition.y = y; m_camPosition.z = z;
}

void Camera::SetPosition(DirectX::SimpleMath::Vector3 location)
{
	SetPosition(location.x, location.y, location.z);
}

DirectX::SimpleMath::Vector3 Camera::GetPosition()
{
	return m_camPosition;
}

DirectX::SimpleMath::Vector3 Camera::GetLookat()
{
	return m_camLookAt;
}

DirectX::SimpleMath::Vector3 Camera::Focus(std::vector<SceneObject *> & selectedObjects)
{
	using namespace DirectX;
	using namespace SimpleMath;

	int selectionQuantity = selectedObjects.size();
	Vector3 lookAt = Vector3{ 0,0,0 };
	for (int i = 0; i < selectionQuantity; i++)
	{
		SceneObject & o = *selectedObjects.at(i);
		lookAt += Vector3{ o.posX, o.posY, o.posZ };
	}
	//average; denominator must not be zero
	m_camLookDirection = lookAt / std::max(selectionQuantity, 1);
	m_camLookDirection -= m_camPosition;
	m_camLookDirection.Normalize();
	return m_camLookAt;
}
