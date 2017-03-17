#pragma once
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <math.h>
#include <D3DX10Math.h>

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	//void GetRotation(float&);
	void GetRotation(D3DXVECTOR3&);
	void GetPosition(D3DXVECTOR3&);

	void TurnLeft(bool, float);
	void TurnRight(bool, float);
	void LookUp(bool, float);
	void LookDown(bool, float);
	void MoveLeft(bool, float);
	void MoveRight(bool, float);
	void MoveForward(bool, float);
	void MoveBackward(bool, float);

private:
	float m_frameTime;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_positionX, m_positionY, m_positionZ;
	float m_leftTurnSpeed, m_rightTurnSpeed, m_upTurnSpeed, m_downTurnSpeed;
	float m_moveLeftSpeed, m_moveRightSpeed, m_moveForwardSpeed, m_moveBackwardSpeed;
};

#endif