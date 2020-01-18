// Genetic.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <chrono> 
#include <tuple>
#include <array>


// TO DO:
// Функция создания координат контейнера
// Функция поворота контейнера
// Функция перебора координат контейнера для размещения в трюме
// 
// 
// POTOM: 
// Прикрутить библиотеку генетических алгоритмов
// Создать структуру контейнеров, чтобы можно было создавать 
// контейнеры вручную

//using namespace std;
using namespace std::chrono; // Clock

enum Container_Type
{
	SMALL,
	MED,
	BIG
};

struct Container
{
	Container_Type _type;
	int length;
	int width;
	int amount;
};

using Containers  = std::vector<Container>;
using Coordinate  = std::array <int, 5>;           // 1 и 2 - (x1,y1), 3 и 4 - (x2,y2), 5 - номер контейнера
using Coordinates = std::vector<Coordinate>;
using Hold        = std::vector<std::vector<int>>;		//  


auto start = high_resolution_clock::now();	// Clock

// Заполнить трюм пустыми значениями
auto initialize_hold(Hold hold, int a_side, int b_side)->Hold;
// Создать контейнеры
auto create_containers()->Containers;
// Заполнить список контейнеров значениями
//auto fill_containers(Container c)->Containers;
// Сделать поворот контейнера
auto rotate_containers(Containers c_s)->Containers;

// Заполнить координаты
//auto make_coordinates(Coordinates& original, Coordinates& duplicate)->void;

// Подготовить контейнеры
//auto prepare_containers (vector<Containers> types_list)-> 

// Создать список возможных координат для каждого контейнера
//auto container_coordinates(Container container, Hold hold)->vector<Coordinate>;

// Разместить контейнеры
//auto settle_containers_in_hold(Hold hold, vector<Coordinate> coordinates)-> Hold;


int main()
{
	auto start = high_resolution_clock::now();	// Clock

	// Создание трюма
	Hold hold;
	int a = 5; // Vertical
	int b = 4; // Horizontal
	hold = initialize_hold(hold, a, b);
	// Конец создания трюма

	// Создание контейнера
	Containers original_containers = create_containers();
	Containers rotated_containers = rotate_containers(original_containers);
	// Конец создания контейнера

	// Создание координат контейнера
	
	// Конец создания координат контейнера
	
	auto stop     = high_resolution_clock::now();				       // Clock
	auto duration = duration_cast<microseconds>(stop - start);	       // Clock

	std::cout << std::endl << duration.count() << " ms" << std::endl;  // Clock
}

Hold initialize_hold(Hold hold, int vertical, int horizontal)
{
	hold.clear();
	vector<int> temp;
	for (int j = 0; j < horizontal; j++)
		temp.push_back(0);
	for (int i = 0; i < vertical; i++)
		hold.push_back(temp);
	return hold;
}

Containers create_containers()
{
	// В будущем здесь можно провести ввод контейнеров 
	// с клавиатуры или с интерфейса
	Containers containers;
	Container c1 = { SMALL, 2, 2, 100 };
	Container c2 = { MED, 3, 2, 100 };
	Container c3 = { BIG, 4, 2, 100 };
	containers.push_back(c1);
	containers.push_back(c2);
	containers.push_back(c3);
}

Containers rotate_containers(Containers containers)
{
	Container r_container;
	Containers r_containers;
	for (Container c : containers)
	{
		r_container = { c._type, c.width, c.length, c.amount };
		r_containers.push_back(r_container);
	}	
	return containers;
}

Coordinates container_coordinates(Container container, Hold hold)
{
	int vertical = static_cast<int>(hold.size());
	int horizontal = static_cast<int>(hold[0].size());

	Coordinate coordinate;
	Coordinates coordinates;

	for (int i = 0; i < vertical - container.length; i++)
		for (int j = 0; j < horizontal - container.width; j++)
		{
			coordinate = { i, j, i + container.length - 1, j + container.width, container._type };
			coordinates.push_back(coordinate);
		}
	return coordinates;
}




//
//vector<Coordinate> create_container_coordinates(vector<Container> containers, Hold hold)
//{
//	vector<Coordinate> coordinates;
//	int a = hold.size();
//	int b = hold[0].size();
//	Coordinate coord;
//	int counter = 1;
//	// Перебираем все контейнеры
//	// get<1>(cont) - длина контейнера по вертикали, 0 - по горизонтали
//	// Для каждого контейнера сохраняем 4 значения: 2 для координаты левого верхнего
//	// и 2 - для нижнего правого углов
//	for (Container cont : containers) 
//	{
//		for (int i = 0; i < a - get<1>(cont) + 1; i++)
//			for (int j = 0; j < b - get<0>(cont) + 1; j++)
//			{
//				coord = { i, j, i + get<1>(cont) - 1, j + get<0>(cont) - 1, get<2>(cont) }; 
//				coordinates.push_back(coord);
//			}
//	}
//	return coordinates;
//}
//
//// Переполнение стека. Почему - непонятно. Ломается на последнем элементе, т.е. на самом большом
//Hold settle_containers_in_hold(Hold hold, vector<Coordinate> coordinates/*, int asml, int amed, int abig*/)
//{
//	//int free_space = hold[0].size() * hold.size();
//	for (Coordinate c : coordinates)
//	{
//		int v = c[2] - c[0] + 1; // vertical
//		int h = c[3] - c[1] + 1; // horizontal
//		cout << v << ' ' << h << endl;
//		for (int i = 0; i < v; i++)
//		{
//			for (int j = 0; j < h; j++)
//			{
//				hold[v + i - 2][h + j - 2]++;
//			}
//		}
//	}
//	return hold;
//}
//
//
