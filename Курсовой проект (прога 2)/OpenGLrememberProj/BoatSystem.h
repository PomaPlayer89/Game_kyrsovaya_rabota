#pragma once
#include "MyVector3d.h"
class Boat�ontrolSystem {
public:
	//������������ ���� �������� 
	double MaxAngleWheel = 27;

	//���� �������� ������� ������������ ��� Z
	double AngleZ = 0;

	//���� �������� 
	double AngleWheel = 0;

	//�������� 
	double Speed = 0.1;

	//�������� �������� 
	double SpeedWheel = 4;

	//�������� ����������w
	double SpeedSlow = 0.1;

	//������ ������� W
	double KeyDuration_W = 0;

	//���������� �� ������ ����������
	bool FlagDeceleration = false;

	//���������� �� ������ ������
	bool FlagAcceleration = false;

	//������ ����� ����������
	bool FlagAccAfterDec = false;

	//������� �������
	Vector3 BoatPosition;

	bool Stop = false;

	Boat�ontrolSystem() {
		BoatPosition = Vector3(0, 0, 0);
	}
};