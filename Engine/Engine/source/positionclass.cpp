#include "headers\positionclass.h"

PositionClass::PositionClass()
{
	m_frameTime = 0.0f;
	m_rotationY = 50.0f;
	m_rotationX = 0.0f;
	m_rotationZ = 0.0f;
	m_positionX = 5.0f;
	m_positionY = 6.0f;
	m_positionZ = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_upTurnSpeed = 0.0f;
	m_downTurnSpeed = 0.0f;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::GetRotation(D3DXVECTOR3& camRot)
{
	camRot.x = m_rotationX;
	camRot.y = m_rotationY;
	camRot.z = m_rotationZ;
	return;
}

void PositionClass::GetPosition(D3DXVECTOR3& camPos)
{
	camPos.x = m_positionX;
	camPos.y = m_positionY;
	camPos.z = m_positionZ;
	return;
}

void PositionClass::TurnLeft(bool keydown, float dt)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * dt;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY -= m_leftTurnSpeed;
	/*if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}*/
	if (m_rotationY < -90.0f)
	{
		m_rotationY = -90.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown, float dt)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * dt;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotationY += m_rightTurnSpeed;
	/*if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}*/
	if (m_rotationY > 90.0f)
		m_rotationY = 90.0f;
	return;
}

void PositionClass::LookUp(bool keydown, float dt)
{
	if (keydown)
	{
		m_upTurnSpeed += m_frameTime * dt;
		if (m_upTurnSpeed > m_frameTime * 0.15f)
		{
			m_upTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_upTurnSpeed -= m_frameTime * 0.005f;
		if (m_upTurnSpeed < 0)
			m_upTurnSpeed = 0;
	}

	m_rotationX -= m_upTurnSpeed;
	if (m_rotationX > 45.0f)
	{
		m_rotationX = 45.0f;
	}

	return;
}

void PositionClass::LookDown(bool keydown, float dt)
{
	if (keydown)
	{
		m_downTurnSpeed += m_frameTime * dt;
		if (m_downTurnSpeed > m_frameTime * 0.15f)
		{
			m_downTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_downTurnSpeed -= m_frameTime * 0.005f;
		if (m_downTurnSpeed < 0)
			m_downTurnSpeed = 0;
	}

	m_rotationX += m_downTurnSpeed;

	if (m_rotationX < -45.0f)
	{
		m_rotationX = -45.0f;
	}

	return;
}

void PositionClass::MoveLeft(bool keydown, float dt)
{
	float radians;

	if (keydown)
	{
		m_moveLeftSpeed += m_frameTime * dt;
		if (m_moveLeftSpeed > m_frameTime * 0.015f)
		{
			m_moveLeftSpeed = m_frameTime * 0.015f;
		}
	}
	else
	{
		m_moveLeftSpeed -= m_frameTime * 0.005f;
		if (m_moveLeftSpeed < 0)
			m_moveLeftSpeed = 0;
	}
	
	radians = m_rotationY * 0.0174532925f;
	m_positionX -= cosf(radians) * m_moveLeftSpeed;
	m_positionZ += sinf(radians) * m_moveLeftSpeed;

	return;
}

void PositionClass::MoveRight(bool keydown, float dt)
{
	float radians;
	if (keydown)
	{
		m_moveRightSpeed += m_frameTime * dt;
		if (m_moveRightSpeed > m_frameTime * 0.015f)
		{
			m_moveRightSpeed = m_frameTime * 0.015f;
		}
	}
	else
	{
		m_moveRightSpeed -= m_frameTime * 0.005f;
		if (m_moveRightSpeed < 0)
			m_moveRightSpeed = 0;
	}

	radians = m_rotationY * 0.0174532925f;
	m_positionX += cosf(radians) * m_moveRightSpeed;
	m_positionZ -= sinf(radians) * m_moveRightSpeed;

	return;
}

void PositionClass::MoveForward(bool keydown, float dt)
{
	float radians;

	if (keydown)
	{
		m_moveForwardSpeed += m_frameTime * dt;
		if (m_moveForwardSpeed > m_frameTime * 0.015f)
			m_moveForwardSpeed = m_frameTime * 0.015f;
	}
	else 
	{
		m_moveForwardSpeed -= m_frameTime * 0.005f;
		if (m_moveForwardSpeed < 0)
			m_moveForwardSpeed = 0;
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_moveForwardSpeed;
	m_positionZ += cosf(radians) * m_moveForwardSpeed;

	return;
}

void PositionClass::MoveBackward(bool keydown, float dt)
{
	float radians;

	if (keydown)
	{
		m_moveBackwardSpeed += m_frameTime * dt;
		if (m_moveBackwardSpeed > m_frameTime * 0.015f)
			m_moveBackwardSpeed = m_frameTime * 0.015f;
	}
	else
	{
		m_moveBackwardSpeed -= m_frameTime * 0.005f;
		if (m_moveBackwardSpeed < 0)
			m_moveBackwardSpeed = 0;
	}

	//convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	//update the position.
	m_positionX -= sinf(radians) * m_moveBackwardSpeed;
	m_positionZ -= cosf(radians) * m_moveBackwardSpeed;

	return;
}
