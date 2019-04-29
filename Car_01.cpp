// Car_01.cpp : Этот файл содержит функцию "main". 
//Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768


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
	int Id;
	/*движение 
	 учитывая, что левый верхний угол машины нулевой, 
	 то и левый верхний угол экрана будет считаться нулевым,
	следовательно, все, что будет двигать вверх оно будет двигаться противоположно оси ОУ -,
	а в них - по направлению ОУ +
	*/
	void move() {
		switch (dir) {
		case eDirection::UP:
			rect.pos.y -= speed;
		case eDirection::DOWN:
			rect.pos.y += speed;
		case eDirection::RIGHT:
			rect.pos.x += speed;
		case eDirection::LEFT:
			rect.pos.x -= speed;
		}
	}
	// перемещение машины если они стоят друг на друге
	void moveOder(sCar* otherCar) {
		
		switch (dir) {
		case eDirection::UP:
			rect.pos.y += otherCar->rect.size.height;
			break;

		case eDirection::DOWN:
			rect.pos.y -= otherCar->rect.size.height;
			break;
		case eDirection::RIGHT:
			rect.pos.x -= otherCar->rect.size.width;
			break;
		case eDirection::LEFT:
			rect.pos.x += otherCar->rect.size.width;
			break;



		}
	}
	
	// проверки находятся ли машины на перекрестке
	bool atСrossroads() {

		switch (dir) {
		case eDirection::UP:
			if ((rect.pos.x == SCREEN_WIDTH / 2) && (rect.pos.y == SCREEN_HEIGHT / 2 + 100)) return true;
			else return false;
			break;

		case eDirection::DOWN:
			if ((rect.pos.x == SCREEN_WIDTH / 2 - 100) && (rect.pos.y == SCREEN_HEIGHT / 2 - 2 * 100)) return true;
			else return false;
			break;
		case eDirection::RIGHT:
			if ((rect.pos.x == SCREEN_WIDTH / 2 - 2 * 100) && (rect.pos.y == SCREEN_HEIGHT / 2 )) return true;
			else return false;
			break;
		case eDirection::LEFT:
			if ((rect.pos.x == SCREEN_WIDTH / 2+100) && (rect.pos.y == SCREEN_HEIGHT / 2 - 100)) return true;
			else return false;
			break;



		}
	}

	// ошибка не при всех значениях возвращает исправлена?
	sRect getFuturePos() {
		//if (dir != NULL)
		switch (dir) {
		case eDirection::UP:
			return sRect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
		case eDirection::DOWN:
			return sRect(rect.pos.x, rect.pos.y + speed, rect.size.width, rect.size.height);
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

	// вывод машин для проверки
	string Print()
	{ 
		
		string inf;
		inf= "id = " + to_string(Id);
		inf += "\t x = \t" + to_string(rect.pos.x);
		inf += "\t y = \t" + to_string(rect.pos.y);
		//inf += "\t height = + to_string(rect.size.height);
		//inf += "\t width = + to_string(rect.size.width);
		inf += "\t Direction = " + enum_to_string(dir);
		//inf += "  \t Fuel = " + to_string( getFuel() );

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
std::vector<sCar*> out;
const int initialCarsCount = 20;
int idCount = 1;

//#define SCREEN_WIDTH 1024
//#define SCREEN_HEIGHT 768

/*
анализ по поводу размещения машин:
0) перекресток - это пересечение двух двух полосных дорог
1) все сгенерированные машины будут размещены в начало перекрестка
2) значение x y это левый верхний угол машины и не важно как она едет, она все равно квадратная 100 на 100
3) расунок дорог прилогается, наверное))
тогда координаты для машин будут:
CarFromTop
x= SCREEN_WIDTH / 2 - 100;
y= SCREEN_HEIGHT/2 - 2*100;

CarFromBot
x= SCREEN_WIDTH / 2;
y= SCREEN_HEIGHT/2+ 100; 

CarFromLeft()
x= SCREEN_WIDTH / 2 -  2*100;
y= SCREEN_HEIGHT/2 ;

CarFromRight()
x= SCREEN_WIDTH / 2 + 100;
y= SCREEN_HEIGHT/2 - 100;
*/
// шаг 3 машина с верху улучшенная
void spawnCarFromTop() {
	sCar* car ;
	int carType = rand();

	switch (carType % 3) {
	case 0:
		car = new sGasEngine();
		/*
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		*/
		break;
	case 1:
		car = new sElectroCar();
		/*
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		*/
		break;
		/*case 2:
		car = new sHybrid();
	
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::DOWN;
		
		break;
		*/
	default: car = new sHybrid();

	}

	int x = SCREEN_WIDTH / 2 - 100;
	int	y = SCREEN_HEIGHT / 2 - 2 * 100;
	car->rect = sRect(x, y, 100, 100);
	car->speed = 1;
	car->dir = eDirection::DOWN;
	car->Id = idCount;
	idCount++;

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

	int x = SCREEN_WIDTH / 2 ;
	int	y = SCREEN_HEIGHT / 2 + 100 ;

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
	else { //if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::UP;
	}
	//
	//car->rect = sRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 100, 100);
	//car->speed = 1;
	car->Id = idCount;
	idCount++;
	asdasd.push_back(car);

}

// шаг 3 машина с лева
void SpawnCarFromLeft() {
	sCar* car;
	int carType = rand();

	int x = SCREEN_WIDTH / 2 -  2 * 100;
	int	y = SCREEN_HEIGHT / 2 ;

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
	else {//if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::RIGHT;
	}
	/*
	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->speed = 1;
	*/
	car->Id = idCount;
	idCount++;
	asdasd.push_back(car);
}

// шаг 3 машина с права
void spawnCarFromRight() {
	sCar* car;
	int carType = rand();
	int x = SCREEN_WIDTH / 2 + 100 ;
	int	y = SCREEN_HEIGHT / 2 - 100;
	if (carType % 3 == 0) {
		car = new sGasEngine();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
		car->Id = idCount;
		idCount++;
	}
	if (carType % 3 == 1) {
		car = new sElectroCar();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
		car->Id = idCount;
		idCount++;
	}
	 if (carType % 3 == 2) {
		car = new sHybrid();
		car->rect = sRect(x, y, 100, 100);
		car->speed = 1;
		car->dir = eDirection::LEFT;
		car->Id = idCount;
		idCount++;
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
	
	if (carDirection % 4 == 0)//1
		spawnCarFromRight();
	else 
		if (carDirection % 4 == 3)//2
			spawnCarFromTop();
		else 
			if (carDirection % 4 == 2)//3
				spawnCarFromBot();
			else 
				if (carDirection % 4 == 1) //0
					SpawnCarFromLeft();
}


/*логика как проезжают светофор
Программа решает очередность проезда машин на перекрестке:
Если есть помеха справа, то машина должна пропустить машину справа.
Если на перекрестке стоят машины со всех 4х сторон, то первой проезжает машина, у которой минимальная координата по X.
Машины не должны наезжать друг на друга.

1)результатом выполнения будет список, в котором записан порядок очередности машин
2)Если есть помеха справа, то машина должна пропустить машину справа.
за это отвечает needPassOtherCar()
3)Если на перекрестке стоят машины со всех 4х сторон, то первой проезжает машина, у которой минимальная координата по X.
на перекрестке машина с Right имеет наименьшие координаты, следовательно, стартует первой
следующие машины будут DOWN, потом Left и UP, учитывая пункт 2

*/
void main_loop2() {
	sCar*  carAtCross[4] = { NULL, NULL, NULL, NULL };;
	
	for (auto car : asdasd)
	{
		if (car->dir == eDirection::RIGHT)
		{
			if (carAtCross[0]==NULL)
			{
				carAtCross[0] = car;
				
			}
			else if (car->rect.pos.x > carAtCross[0]->rect.pos.x)
				carAtCross[0] = car;
		}
		if (car->dir == eDirection::DOWN)
		{
			if (carAtCross[1] == NULL)
			{
				carAtCross[1] = car;
				
			}
			else if (car->rect.pos.y > carAtCross[1]->rect.pos.y)
				carAtCross[1] = car;
		}
		if (car->dir == eDirection::LEFT)
		{
			if (carAtCross[2] == NULL)
			{
				carAtCross[2] = car;
				
			}
			else if (car->rect.pos.x < carAtCross[2]->rect.pos.x)
				carAtCross[2] = car;
		}
		if (car->dir == eDirection::UP) 
		{
			if (carAtCross[3] == NULL)
			{
				carAtCross[3] = car;
				
			}
			else if (car->rect.pos.y < carAtCross[3]->rect.pos.y)
				carAtCross[3] = car;
		}
	}

	
	cout << "\n";
	for (int i = 0; i < 4; ++i) 
	{
		if (carAtCross[i] != NULL) {
			int j = 0;

			cout << i + 1 << "\t" << carAtCross[i]->Print() << "\n";
			out.push_back(carAtCross[i]);

			for (auto car : asdasd)
			{
				if (car->Id != carAtCross[i]->Id)
					j++;
				else break;
			}

			asdasd.erase(asdasd.begin() + j);
		}
	}
	

	



/*
	for (auto car : asdasd) 
		for (auto car22 : asdasd) 
	if (car->atСrossroads()&& car22->atСrossroads()) // на перекрестке
	{
		
		if (car->getFuturePos().intersects(car22->getFuturePos()))
		// кого пропускать?
		if (car->needPassOtherCar(car22))
			car->move();
		   cout <<"1 \t"<< car22->Print() << "\n";
		// движение
	}
	else // не на перекрестке, движение к перекрестку
	{
		if (car->getFuturePos().intersects(car22->getFuturePos())) // есть в переди место? да движемся
		{
			car->move();
			cout <<"2 \t" << car22->Print() << "\n";
		}
		//else {}// нет места не движемся
	}
*/
	
/*
	for (auto car : asdasd) {
		for (auto car22 : asdasd) {
			if (car->getFuturePos().intersects(car22->getFuturePos())) {
				if (car->needPassOtherCar(car22))
					car->move();
			}
			else {
				car22->move();
				cout << car22->Print() << "\n";
			}
		}
		// создание новой машины
		if (car->rect.pos.x <= 0 || car->rect.pos.x >= SCREEN_WIDTH || car->rect.pos.y <= 0 || car->rect.pos.y >= SCREEN_HEIGHT)
			spawnCar();
	}
	return main_loop2();
	//рекурсия надо что-то возвращать
	*/

}
// старая
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

/* убираем наслоение машин
путем попарного сравнения и сдвига если совпадает положение
*/
void order()
{
	for (int i = 0; i < initialCarsCount; ++i) {
		for (int j = i+1; j < initialCarsCount; ++j) {
			
			if (asdasd[i]->dir==asdasd[j]->dir) {

				asdasd[j]->moveOder(asdasd[i]);
				
			}
			
		}
		
	}
}


void main2(){//int argc, char** argv) {
	/* создание машин 
	машин создается 20, тогда все варианты направлений рассматриваются.
	вначале они все находятся в одной точке (каждый для своего движения)*/
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();

	}
	
	// вывод машин для проверки 
	/*for (auto i = 0; i < initialCarsCount; ++i) {
		
		cout <<i+1<<"\t"<<asdasd[i]->Print()<<"\n";
	}
	*/
	/* убираем наслоение машин
	передвигаем наслоённые  машины назад от перекрестка, иногда за поле, но они потом "приедут"	*/
	order();
	
	// вывод машин для проверки
	cout << "\n";
	for (auto i = 0; i < asdasd.size(); ++i) {

		cout << i+1 << "\t" << asdasd[i]->Print() << "\n";
	}
	cout << "\n";
	
	for (int i =1 ; 0!=asdasd.size(); i++)
	{// в процессе
		main_loop2();
/*
		// вывод машин для проверки
		cout <<i << " -- \n";

		for (auto i = 0; i < asdasd.size(); ++i) {

			cout << i + 1 << "\t" << asdasd[i]->Print() << "\n";
		}
*/
	
	}
	/*cout << "\n";
	for (auto i = 0; i < out.size(); ++i) {

		cout  << out[i]->Id << "\t";
	}
	cout << "\n";
	*/
	
}



void main()
{
  cout << "Hello Cars!\n"; 
  int a;

	main2();
	cout << "\n";
	for (auto i = 0; i < out.size(); ++i) {

		cout << out[i]->Id << "\t";
	}
	cout << "\n";

  system("pause");
}

/*
Исправить логику и оптимизировать код. Допускается дополнение и изменение кода.

Программа решает очередность проезда машин на перекрестке:
Если есть помеха справа, то машина должна пропустить машину справа.
Если на перекрестке стоят машины со всех 4х сторон, то первой проезжает машина, у которой минимальная координата по X.
Машины не должны наезжать друг на друга.
*/