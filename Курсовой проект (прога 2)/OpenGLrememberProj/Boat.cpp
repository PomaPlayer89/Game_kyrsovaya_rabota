#pragma once
//���������� ��� ������ � ��������� 
#include "MyVector3d.h"
#include "BoatSystem.h"
#include "MyOGL.h"
#include <ctime>
#include "ObjLoader.h"
#include "Texture.h"
extern Boat�ontrolSystem Boat;
//����� ���������� �����
extern double TimeLastFrame;

extern ObjFile boatObj;
extern Texture boatTex;

//������� ����� �����
void NewPositionBoat() {
	if (Boat.KeyDuration_W > 3) {
		Boat.KeyDuration_W = 3;
	}
	Boat.BoatPosition.setCoord_X(Boat.BoatPosition.X() + Boat.Speed * cos(Boat.AngleZ * PI / 180) * (Boat.KeyDuration_W / 5));
	Boat.BoatPosition.setCoord_Y(Boat.BoatPosition.Y() + Boat.Speed * sin(Boat.AngleZ * PI / 180) * (Boat.KeyDuration_W / 5));

}

//���������� ������� ������ 
void UpdateParameters() {
	//�������� ������� �����
	double end = clock();

	//����� �����
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//���������� ������� ��� �������� ������, ���� �� ������ ������� W � S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W > 0) {
		//���������, ���� ����� ������� W �� ���� � �����
		if (Boat.KeyDuration_W - TimeFrame < 0) {
			Boat.KeyDuration_W = 0;
		}
		else
		{
			Boat.KeyDuration_W -= TimeFrame;
			NewPositionBoat();
		}
		//������������� ���� �� ������ ����� ����������
		Boat.FlagAccAfterDec = true;
	}

	//������� ���������� ��� �������� �����
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W < 0)
	{
		if (Boat.KeyDuration_W + TimeFrame > 0)
			Boat.KeyDuration_W = 0;
		else
		{
			Boat.KeyDuration_W += TimeFrame;
			NewPositionBoat();
		}
	}

	//���������� ��� �������� ������, ������ ������ S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Boat.KeyDuration_W > 0)
	{
		if (Boat.KeyDuration_W - Boat.SpeedSlow < 0)
			Boat.KeyDuration_W = 0;
		else
		{
			Boat.KeyDuration_W -= Boat.SpeedSlow;
			NewPositionBoat();
		}

	}

	//���������� ��� �������� �����
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W < 0)
	{
		if (Boat.KeyDuration_W + Boat.SpeedSlow < 0)
			Boat.KeyDuration_W = 0;
		else
		{
			Boat.KeyDuration_W += Boat.SpeedSlow;
			NewPositionBoat();
		}
	}

	//�������� ������, ������ ������ W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W >= 0)
	{
		//���������, ���� �� ������ ������
		if (!Boat.FlagAcceleration)
		{
			Boat.FlagAcceleration = true;
		}
		else
		{
			//����������
			Boat.KeyDuration_W += TimeFrame;
			NewPositionBoat();
		}
	}

	//�������� �����, ������ ������ S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Boat.KeyDuration_W <= 0)
	{
		if (!Boat.FlagDeceleration)
		{
			Boat.FlagDeceleration = true;
		}
		else
		{
			//�����������
			Boat.KeyDuration_W -= TimeFrame;
			NewPositionBoat();
		}
	}

	//������������ ������
	if (OpenGL::isKeyPressed('D')) {
		
		//����������� ������ ��������
		if (OpenGL::isKeyPressed('W')) {
			Boat.AngleZ -= Boat.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Boat.AngleZ += Boat.SpeedWheel / 10;
		}
	}

	//������������ �����
	if (OpenGL::isKeyPressed('A')) {
		//����������� ������ ��������
		if (OpenGL::isKeyPressed('W')) {
			Boat.AngleZ += Boat.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Boat.AngleZ -= Boat.SpeedWheel / 10;
		}
	}

	
	//P - ������� � �������� ���������
	if (OpenGL::isKeyPressed('P')) {
		Boat = Boat�ontrolSystem();
	}

	//������ ����
	if (OpenGL::isKeyPressed(32)) {
		Boat.KeyDuration_W = 0;
	}
}

void BoatSystem() {
	//���������� ���������� 
	UpdateParameters();

	//���������� ������� ���������� �����
	TimeLastFrame = clock();

	glPushMatrix();
	glTranslated(Boat.BoatPosition.X(), Boat.BoatPosition.Y(), Boat.BoatPosition.Z() - 0.2);
	glRotated(90, 0, 0, 1);
	glRotated(Boat.AngleZ, 0, 0, 1);

	boatTex.bindTexture();
	boatObj.DrawObj();
	glPopMatrix();
}