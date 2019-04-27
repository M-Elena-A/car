﻿// Car_01.cpp : Этот файл содержит функцию "main". 
//Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;



struct sPos {
	sPos() { x = 0; y = 0; }
	sPos(int aX, int aY) { x = aX; y = aY; }
	int x;
	int y;
};

struct sSize
{
	sSize() { width = 0; height = 0; }
	sSize(int aW, int aH) { width = aW; height = aW; }
	int width;
	int height;
};

struct sRect
{
	sRect() {};
	sRect(int x, int y, int w, int h) { pos.x = x; pos.y = y; size.width = w; size.height = h; }
	sPos pos;
	sSize size;
	////// atanthion проверка на столкновения
	bool intersects(const sRect& other) {
		return !((other.pos.x + other.size.width <= pos.x) ||
			(other.pos.y + other.size.height <= pos.y) ||
			(other.pos.x >= pos.x + size.width) ||
			(other.pos.y >= pos.y + size.height));
	}
};


enum class eDirection {
	UP,
	LEFT,
	RIGHT,
	DOWN
};
// дополнительно мне надо для вывода
string enum_to_string(eDirection t) {
	switch (t) {
	case eDirection::UP:
		return "UP";
	case eDirection::LEFT:
		return "LEFT"; 
	case eDirection::RIGHT:
		return "RIGHT"; 
	case eDirection::DOWN:
		return "DOWN";
	default:
		return "INVALID ENUM";
	}
}
//

struct sCar {
	sRect rect;
	eDirection dir;
	int speed;
	///////
	void move() {
		switch (dir) {
		case eDirection::UP:
			rect.pos.y += speed;
		case eDirection::DOWN:
			rect.pos.y -= speed;
		case eDirection::RIGHT:
			rect.pos.x += speed;
		case eDirection::LEFT:
			rect.pos.x -= speed;
		}
	}
	// ошибка не при всех значениях возвращает
	sRect getFuturePos() {
		//if (dir != NULL)
		switch (dir) {
		case eDirection::UP:
			return sRect(rect.pos.x, rect.pos.y + speed, rect.size.width, rect.size.height);
		case eDirection::DOWN:
			return sRect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
		case eDirection::RIGHT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.width);
		case eDirection::LEFT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height);
		// Не двигаемся
		default:
			return sRect(rect.pos.x , rect.pos.y, rect.size.width, rect.size.height);
		}
	}

	// надо ли пропускать машину
	bool needPassOtherCar(sCar* otherCar) {
		bool result=false;
		///////////
		auto otherdir = otherCar->dir;
		
		switch (dir) {
		case eDirection::UP:
			//auto otherdir = otherCar->dir;
			if (otherdir == eDirection::LEFT)
				result = true;
			break;
		case eDirection::DOWN:
			//auto otherdir = otherCar->dir;
			if (otherdir == eDirection::RIGHT)
				result = true;
			break; 
		case eDirection::RIGHT:
			//auto otherdir = otherCar->dir;
			if (otherdir == eDirection::UP)
				result = true;
			break; 
		case eDirection::LEFT:
			//auto otherdir = otherCar->dir;
			if (otherdir == eDirection::LEFT)
				result = false;
			//else
			//	result = false;
			break; 
		}
		return result;
	}
	
	virtual int getFuel() = 0;
	virtual void refill(int count) = 0;

	string Print()
	{ 
		
		string inf;
		inf = " x = \t";
		inf += to_string(rect.pos.x);
		inf += "\t y = \t";
		inf += to_string(rect.pos.y);
		inf += "\t height = ";
		inf += to_string(rect.size.height);
		inf += "\t width = ";
		inf += to_string(rect.size.width);
		inf += "\t Direction = " + enum_to_string(dir);
		inf += "\t Fuel = " + to_string( getFuel() );

			return inf;
	}
};


struct sGasEngine : sCar {
	int getFuel() { return fuel; }
	void refill(int count) { fuel += count; }
	void move() { fuel--; sCar::move(); }
	int fuel;
};

struct sElectroCar : sCar {
	int getFuel() { return charge; }
	void refill(int count) { charge += count; }
	void move() { charge--; sCar::move(); }
	int charge;
};

struct sHybrid : sGasEngine, sElectroCar , sCar
{
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		if (rand() % 2 == 0)
			charge--;
		else
			fuel--;
		sCar::move();
	}
};


/////
std::vector<sCar*> asdasd;
const int initialCarsCount = 20;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

/*
анализ по поводу размещения машин:
1) все сгенерированные машины будут размещены в начало перекрестка
2) значение x y это левый верхний угол машины и не важно как она едет, она все равно квадратная 100 на 100
тогда координаты для машин будут:
CarFromTop
x= SCREEN_WIDTH / 2 - 100/2
y= SCREEN_HEIGHT/2 - 100/2 - 100

CarFromBot
x= SCREEN_WIDTH / 2 - 100/2
y= SCREEN_HEIGHT/2+ 100/2 

CarFromLeft()
x= SCREEN_WIDTH / 2 - 100/2-100
y= SCREEN_HEIGHT/2 - 100/2

CarFromRight()
x= SCREEN_WIDTH / 2 + 100/2
y= SCREEN_HEIGHT/2 - 100/2
*/
// шаг 3 машина с верху
void spawnCarFromTop() {
	sCar* car ;
	int carType = rand();
	int x = SCREEN_WIDTH / 2 - 100 / 2;
	int	y = SCREEN_HEIGHT / 2 - 100 / 2 - 100;
	switch (carType % 3) {
	case 0:
		car = new sGasEngine();
		
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		break;

	case 1:
		car = new sElectroCar();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		break;
	case 2:
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		break;
	}
/*
	if (carType % 3 == 0) {
		car = new sGasEngine();
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
	}
	
	//
	 car->rect = sRect(SCREEN_WIDTH / 2, 0, 100, 100);
	car->speed = 1;
	car->dir = eDirection::DOWN;
	*/
	asdasd.push_back(car);
}

// шаг 3 машина с низу
void spawnCarFromBot() {
	int x = SCREEN_WIDTH / 2 - 100 / 2;
	int	y = SCREEN_HEIGHT / 2 + 100 / 2;
	sCar* car;
	int carType = rand();
	if (carType % 3 == 0) {
		car = new sGasEngine();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::UP;
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::UP;
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::UP;
	}
	//
	//car->rect = sRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 100, 100);
	//car->speed = 1;
	asdasd.push_back(car);
}

// шаг 3 машина с лева
void SpawnCarFromLeft() {
	sCar* car;
	int carType = rand();

	int x = SCREEN_WIDTH / 2 - 100 / 2 - 100;
	int	y = SCREEN_HEIGHT / 2 - 100 / 2;
	if (carType % 3 == 0) {
		car = new sGasEngine();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::RIGHT;
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::RIGHT;
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::RIGHT;
	}
	/*
	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->speed = 1;
	*/
	asdasd.push_back(car);
}

// шаг 3 машина с права
void spawnCarFromRight() {
	sCar* car;
	int carType = rand();
	int x = SCREEN_WIDTH / 2 + 100 / 2;
	int	y = SCREEN_HEIGHT / 2 - 100 / 2;
	if (carType % 3 == 0) {
		car = new sGasEngine();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
	}
	if (carType % 3 == 1) {
		car = new sElectroCar();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
	}
	 if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
	}
	
	/*
	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->speed = 1;
	*/
	 asdasd.push_back(car);
}

//2 шаг создание машины выбор от куда едем 
void spawnCar() {
	int carDirection = rand();
	
	if (carDirection % 4 == 1)
		spawnCarFromRight();
	else 
		if (carDirection % 4 == 2)
			spawnCarFromTop();
		else 
			if (carDirection % 4 == 3)
				spawnCarFromBot();
			else 
				if (carDirection % 4 == 0) 
					SpawnCarFromLeft();
}

bool main_loop() {
	for (auto car : asdasd) {
		for (auto car22 : asdasd) {
			if (car->getFuturePos().intersects(car22->getFuturePos())) {
				if (car->needPassOtherCar(car22))
					car->move();
			}
			else {
				car22->move();
			}
		}
		if (car->rect.pos.x <= 0 || car->rect.pos.x >= SCREEN_WIDTH || car->rect.pos.y <= 0 || car->rect.pos.y >= SCREEN_HEIGHT)
			spawnCar();
	}
	return main_loop(); 
		//рекурсия надо что то возвращать
}

int main2(){//int argc, char** argv) {
	// создание машин
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();

	}
	
	for (auto i = 0; i < initialCarsCount; ++i) {
		
		cout <<i<<"\t"<<asdasd[i]->Print()<<"\n";
	}
	
	//main_loop();
	return 0;
}

void main()
{
  cout << "Hello World!\n"; 
  int a;

  a = main2();
  cout << a;

  system("pause");
}

/*
Исправить логику и оптимизировать код. Допускается дополнение и изменение кода.

Программа решает очередность проезда машин на перекрестке:
Если есть помеха справа то машина должна пропустить машину справа.
Если на перекрестке стоят машины со всех 4х сторон, то первой проезжает машина, у которой минимальная координата по X.
Машины не должны наезжать друг на друга.
*/