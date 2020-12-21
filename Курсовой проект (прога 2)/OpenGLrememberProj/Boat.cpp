#pragma once
//переменные для работы с движением 
#include "MyVector3d.h"
#include "BoatSystem.h"
#include "MyOGL.h"
#include <ctime>
#include "ObjLoader.h"
#include "Texture.h"
extern BoatСontrolSystem Boat;
//время последнего кадра
extern double TimeLastFrame;

extern ObjFile boatObj;
extern Texture boatTex;

//рассчет новой точки
void NewPositionBoat() {
	if (Boat.KeyDuration_W > 3) {
		Boat.KeyDuration_W = 3;
	}
	Boat.BoatPosition.setCoord_X(Boat.BoatPosition.X() + Boat.Speed * cos(Boat.AngleZ * PI / 180) * (Boat.KeyDuration_W / 5));
	Boat.BoatPosition.setCoord_Y(Boat.BoatPosition.Y() + Boat.Speed * sin(Boat.AngleZ * PI / 180) * (Boat.KeyDuration_W / 5));

}

//обработчик нажания кнопок 
void UpdateParameters() {
	//получаем текущее время
	double end = clock();

	//время кадра
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	if (TimeFrame > 2) {
		return;
	}

	//замедление объекта при движении вперед, если не нажаты клавиши W и S
	if (!OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W > 0) {
		//проверяем, чтоб время нажетия W не ушло в минус
		if (Boat.KeyDuration_W - TimeFrame < 0) {
			Boat.KeyDuration_W = 0;
		}
		else
		{
			Boat.KeyDuration_W -= TimeFrame;
			NewPositionBoat();
		}
		//устанавливаем флаг на разгон после торможения
		Boat.FlagAccAfterDec = true;
	}

	//плавное замедление при движении назад
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

	//торможение при движении вперед, нажата только S
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

	//торможение при движении назад
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

	//движение вперед, нажата только W
	if (OpenGL::isKeyPressed('W') && !OpenGL::isKeyPressed('S') && Boat.KeyDuration_W >= 0)
	{
		//проверяем, есть ли сейчас разгон
		if (!Boat.FlagAcceleration)
		{
			Boat.FlagAcceleration = true;
		}
		else
		{
			//ускоряемся
			Boat.KeyDuration_W += TimeFrame;
			NewPositionBoat();
		}
	}

	//движение назад, нажата только S
	if (OpenGL::isKeyPressed('S') && !OpenGL::isKeyPressed('W') && Boat.KeyDuration_W <= 0)
	{
		if (!Boat.FlagDeceleration)
		{
			Boat.FlagDeceleration = true;
		}
		else
		{
			//замедляемся
			Boat.KeyDuration_W -= TimeFrame;
			NewPositionBoat();
		}
	}

	//поворачиваем вправо
	if (OpenGL::isKeyPressed('D')) {
		
		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Boat.AngleZ -= Boat.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Boat.AngleZ += Boat.SpeedWheel / 10;
		}
	}

	//поворачиваем влево
	if (OpenGL::isKeyPressed('A')) {
		//увеличиваем градус поворота
		if (OpenGL::isKeyPressed('W')) {
			Boat.AngleZ += Boat.SpeedWheel / 10;
		}
		if (OpenGL::isKeyPressed('S')) {
			Boat.AngleZ -= Boat.SpeedWheel / 10;
		}
	}

	
	//P - вернуть в исходное положение
	if (OpenGL::isKeyPressed('P')) {
		Boat = BoatСontrolSystem();
	}

	//пробел стоп
	if (OpenGL::isKeyPressed(32)) {
		Boat.KeyDuration_W = 0;
	}
}

void BoatSystem() {
	//обновление параметров 
	UpdateParameters();

	//обновление времени последнего кадра
	TimeLastFrame = clock();

	glPushMatrix();
	glTranslated(Boat.BoatPosition.X(), Boat.BoatPosition.Y(), Boat.BoatPosition.Z() - 0.2);
	glRotated(90, 0, 0, 1);
	glRotated(Boat.AngleZ, 0, 0, 1);

	boatTex.bindTexture();
	boatObj.DrawObj();
	glPopMatrix();
}