#include "Camera.h"
#include "InputCommands.h"

void Camera::Update(InputCommands & inputCommands)
{
	DirectX::SimpleMath::Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	m_camOrientation.y -= inputCommands.mouseHori;

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

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

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}

void Camera::Init(float movespeed, float camRotRate)
{
	DirectX::SimpleMath::Vector3 zero = DirectX::SimpleMath::Vector3(0,0,0);
	m_camPosition = zero;
	m_camOrientation = zero;
	m_camLookAt = zero;
	m_camLookDirection = zero;
	m_camRight = zero;

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

void Camera::SetLookAt(float x, float y, float z)
{
	m_camLookAt.x = x; m_camLookAt.y = y; m_camLookAt.z = z;
}

void Camera::SetLookAt(DirectX::SimpleMath::Vector3 location)
{
	SetLookAt(location.x, location.y, location.z);
}

DirectX::SimpleMath::Vector3 Camera::GetLookat()
{
	return m_camLookAt;
}
