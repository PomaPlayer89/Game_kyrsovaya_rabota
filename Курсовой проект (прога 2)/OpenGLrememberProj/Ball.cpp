#include "DrawBezierAndHermit.h"
#include "ObjLoader.h"
#include <random>
#include <iostream>
#include "BoatSystem.h"

using namespace std;

extern bool StopBall;
extern ObjFile ballObj, boatObj;

extern int Numbar_ball;
extern double Rboat;
extern int Max_Ball;
extern double NextTime;

extern BoatСontrolSystem Boat;
//время последнего кадра
extern double TimeLastFrame;

void Color_4d(float color[4]) {
	color[0] = (float)((rand() % 999) + 1.0) / 1000;
	color[1] = (float)((rand() % 999) + 1.0) / 1000;
	color[2] = (float)((rand() % 999) + 1.0) / 1000;
	color[3] = (float)((rand() % 999) + 1.0) / 1000;
}

void Color_3d(float color[3]) {
	color[0] = (float)((rand() % 999) + 1.0) / 1000;
	color[1] = (float)((rand() % 999) + 1.0) / 1000;
	color[2] = (float)((rand() % 999) + 1.0) / 1000;
}

vector<float> Color_4d() {
	vector<float> color = {
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000
	};
	return color;
}

vector<float> Color_3d() {
	vector<float> color = {
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000,
		(float)((rand() % 999) + 1.0) / 1000
	};
	return color;
}

double random(int a, int b)
{
	std::random_device random_device; // Источник энтропии.
	std::mt19937 generator(random_device()); // Генератор случайных чисел.
	// (Здесь берется одно инициализирующее значение, можно брать больше)

	std::uniform_int_distribution<> distribution(a, b); // Равномерное распределение [a, b]

	return distribution(generator); // Случайное число.
}

void BallSystem() {
	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	static vector<int> color = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	static float color4d[4];
	static float color3d[3];

	static vector<vector<Point>> Position;
	static bool pos = true;
	if (pos) {
		for (double i = -5; i <= 5; i = i + 1) {
			for (double j = -5; j <= 5; j = j + 1) {
				vector<Point> points = { Point(i, j, -3), Point(i, j, -3), Point(i, j, 10), Point(i, j, 10) };
				Position.push_back(points);
			}
		}
		pos = false;
	}

	static double time = 0;

	//получаем текущее время
	double end = clock();
	//время кадра
	double TimeFrame = (end - TimeLastFrame) / CLOCKS_PER_SEC;

	time += TimeFrame;
	static vector<vector<Point>> Position2;
	static vector<bool> Position2_flag;
	static int Position2_flag_N = 0;
	static vector<vector<float>> pos2_color3d;
	static vector<vector<float>> pos2_color4d;
	static int pos2_max = 10;
	static bool pos2 = true;

	Max_Ball = pos2_max;

	static double Boat_R = 0.5;
	static double ball_R = 0.2;
	static double Boat_Rnow = Boat_R;

	static vector<vector<float>> BallON_color3d = vector<vector<float>>();
	static vector<vector<float>> BallON_color4d = vector<vector<float>>();

	static double Time_max = 15;

	NextTime = Time_max - time;

	if (time >= Time_max) {
		time = 0;
		pos2 = true;
	}

	if (pos2) {
		int a = -5.0;
		int b = 5.0;
		Position2.clear();
		Position2_flag.clear();

		Position2_flag_N = 0;
		BallON_color3d.clear();
		BallON_color4d.clear();
		Boat_Rnow = Boat_R;

		for (int i = 0; i < pos2_max; i++) {
			vector<Point> points = {
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b)),
				Point(random(a, b), random(a, b), random(a, b))
			};
			Position2.push_back(points);
			Position2_flag.push_back(true);
		}
		pos2 = false;
	}
	//настройка времени
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max становится = 1 за 5 секунд
	//t_max сама по себе изменяется от 0 до 1 постепенно от кадра к кадру
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max -= go;
		t_max_next = t_max - go;
		if (t_max < 0) {
			t_max = 0;
			flag_tmax = true;
		}
		if (t_max_next < 0) {
			t_max_next = 0;
		}
	}

	//отключаем свет
	glDisable(GL_LIGHTING);
	//отключаем текстуры
	glDisable(GL_TEXTURE_2D);
	//включаем режим смешивания
	glEnable(GL_BLEND);
	////задаем опцию для коэффициентов источника и приемника
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (StopBall) {
		if (t_max >= -0.1 && t_max < 0.1 && color[0] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[0] = 1;

			color[1] = 0;
			color[9] = 0;
		}
		if (t_max >= 0.1 && t_max < 0.2 && color[1] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[1] = 1;

			color[0] = 0;
			color[2] = 0;
		}
		if (t_max >= 0.2 && t_max < 0.3 && color[2] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[2] = 1;

			color[1] = 0;
			color[3] = 0;
		}
		if (t_max >= 0.3 && t_max < 0.4 && color[3] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[3] = 1;

			color[2] = 0;
			color[4] = 0;
		}
		if (t_max >= 0.4 && t_max < 0.5 && color[4] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[4] = 1;

			color[3] = 0;
			color[5] = 0;
		}
		if (t_max >= 0.5 && t_max < 0.6 && color[5] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[5] = 1;

			color[4] = 0;
			color[6] = 0;
		}
		if (t_max >= 0.6 && t_max < 0.7 && color[6] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[6] = 1;

			color[5] = 0;
			color[7] = 0;
		}
		if (t_max >= 0.7 && t_max < 0.8 && color[7] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[7] = 1;

			color[6] = 0;
			color[8] = 0;
		}
		if (t_max >= 0.8 && t_max < 0.9 && color[8] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[8] = 1;

			color[7] = 0;
			color[9] = 0;
		}
		if (t_max >= 0.9 && t_max < 1.0 && color[9] == 0) {
			Color_3d(color3d);
			Color_4d(color4d);

			pos2_color3d.clear();
			pos2_color4d.clear();
			for (int i = 0; i < pos2_max; i++) {
				pos2_color3d.push_back(Color_3d());
				pos2_color4d.push_back(Color_4d());
			}

			color[9] = 1;

			color[8] = 0;
			color[0] = 0;
		}
	}

	glColor3d(color3d[0], color3d[1], color3d[2]);
	glColor4d(color4d[0], color4d[1], color4d[2], color4d[3]);


	//for (int i = 0; i < (int)Position.size(); i++) {
	//	glPushMatrix();
	//
	//	Point point = BezierCurve3(Position[i][0], Position[i][1], Position[i][2], Position[i][3], t_max);
	//	Point next_point = BezierCurve3(Position[i][0], Position[i][1], Position[i][2], Position[i][3], t_max_next);
	//	point.Translated();
	//	ballObj.DrawObj();
	//
	//	glPopMatrix();
	//}

	for (int i = 0; i < (int)Position2.size(); i++) {
		if (Position2_flag[i]) {
			glPushMatrix();

			glColor3d(pos2_color3d[i][0], pos2_color3d[i][1], pos2_color3d[i][2]);
			glColor4d(pos2_color4d[i][0], pos2_color4d[i][1], pos2_color4d[i][2], pos2_color4d[i][3]);

			Point point = BezierCurve3(Position2[i][0], Position2[i][1], Position2[i][2], Position2[i][3], t_max);
			Point next_point = BezierCurve3(Position2[i][0], Position2[i][1], Position2[i][2], Position2[i][3], t_max_next);
			point.Translated();
			ballObj.DrawObj();

			glPopMatrix();

			double CD = sqrt(pow(Boat.BoatPosition.X() * 0.5 - point.x, 2) + pow(Boat.BoatPosition.Y() * 0.5 - point.y, 2)) - Boat_Rnow - ball_R;
			if (CD <= 0) {
				vector<float> color3d = vector<float>();
				color3d.push_back(pos2_color3d[i][0]);
				color3d.push_back(pos2_color3d[i][1]);
				color3d.push_back(pos2_color3d[i][2]);
				BallON_color3d.push_back(color3d);

				vector<float> color4d = vector<float>();
				color4d.push_back(pos2_color4d[i][0]);
				color4d.push_back(pos2_color4d[i][1]);
				color4d.push_back(pos2_color4d[i][2]);
				color4d.push_back(pos2_color4d[i][3]);
				BallON_color4d.push_back(color4d);

				Position2_flag[i] = false;
				Position2_flag_N++;

				Boat_Rnow += 0.1;
			}
		}
	}

	if (Position2_flag_N != 0) {
		double h = 0.3;
		for (int i = 0; i < (int)BallON_color3d.size(); i++) {
			glPushMatrix();
			glTranslated(Boat.BoatPosition.X(), Boat.BoatPosition.Y(), Boat.BoatPosition.Z() + h);
			glColor3d(BallON_color3d[i][0], BallON_color3d[i][1], BallON_color3d[i][2]);
			glColor4d(BallON_color4d[i][0], BallON_color4d[i][1], BallON_color4d[i][2], BallON_color4d[i][3]);
			ballObj.DrawObj();
			glPopMatrix();

			h += 0.2;
		}
	}

	if (Position2_flag_N == pos2_max) {
		glPushMatrix();
		glScaled(10, 10, 10);
		vector<float> color3d = Color_3d();
		vector<float> color4d = Color_4d();

		glColor3d(color3d[0], color3d[1], color3d[2]);
		glColor4d(color4d[0], color4d[1], color4d[2], color4d[3]);

		Point point = BezierCurve3(Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 5), Point(0, 0, 5), t_max);
		Point next_point = BezierCurve3(Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 5), Point(0, 0, 5), t_max_next);
		point.Translated();
		ballObj.DrawObj();
		glPopMatrix();
	}
	//выключаем режим смешивания
	glDisable(GL_BLEND);
	//включаем свет
	glEnable(GL_LIGHTING);
	//включаем текстуры
	glEnable(GL_TEXTURE_2D);

	Numbar_ball = Position2_flag_N;
	Rboat = Boat_Rnow;
}