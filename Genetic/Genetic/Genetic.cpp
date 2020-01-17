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

using namespace std;
using namespace std::chrono; // Clock

enum Container_Types
{
	SMALL,
	MEDIUM,
	BIG
};


using Container = tuple <int, int, int>; // 1 - высота, 2 - ширина, 3 - код контейнера
using Containers = vector<Container>;
using Coordinate = array <int, 5>;       // 1 и 2 - (x1,y1), 3 и 4 - (x2,y2), 5 - номер контейнера
using Hold = vector<vector<int>>;		 //  


auto start = high_resolution_clock::now();	// Clock

// Заполнить трюм пустыми значениями
auto initialize_hold(Hold hold, int a_side, int b_side)->Hold;

// Заполнить список контейнеров значениями
auto fill_container(Container c)->Containers;

// Сделать поворот контейнера
auto rotate_container(Containers c_s)->Containers;

// Подготовить контейнеры
auto prepare_containers (vector<Containers> types_list)-> 

// Создать список возможных координат для каждого контейнера
auto create_container_coordinates(Container container, Hold hold)->vector<Coordinate>;

// Разместить контейнеры
//auto settle_containers_in_hold(Hold hold, vector<Coordinate> coordinates)-> Hold;


int main()
{
	auto start = high_resolution_clock::now();	// Clock

	Hold hold;
	int a = 5; // Vertical
	int b = 4; // Horizontal
	hold = initialize_hold(hold, a, b);
	//auto containers = fill_containers();
	int small = 100;
	int med = 100;
	int big = 100;

	// Распечатать трюм
	/*
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
			cout << hold[i][j] << ' ';
		cout << endl;
	}*/

	auto coordinates = create_container_coordinates(container, hold);

	auto coord2 = coordinates;
	coord2.clear();
	coord2.push_back(coordinates[coordinates.size()-1]);
	coord2.push_back(coordinates[0]);
	// Распечатать координаты
	/*for (int i = 0; i < coordinates.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < coordinates[0].size(); j++)
			cout << coordinates[i][j] << ' ';
	}*/
	
	hold = settle_containers_in_hold(hold, coord2);

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
			cout << hold[i][j] << ' ';
		cout << endl;
	}

	auto stop     = high_resolution_clock::now();				// Clock
	auto duration = duration_cast<microseconds>(stop - start);	// Clock

	cout << endl << duration.count() << " ms" << endl;			// Clock
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

vector<Coordinate> create_container_coordinates(Container container, Hold hold)
{
	int vertical   = static_cast<int>(hold.size());
	int horizontal = static_cast<int>(hold[0].size());
	int heigth     = get<0>(container);
	int width	   = get<1>(container);
	int type	   = get<2>(container);

	Coordinate coordinate;
	vector<Coordinate> coordinates;
	
	for (int i = 0; i < vertical - heigth; i++)
		for (int j = 0; j < horizontal - width; j++)
		{
			coordinate = { i, j, i + heigth - 1, j + width, type };
			coordinates.push_back(coordinate);
		}
}

vector<Container> rotate_container(Containers containers)
{
	Container r_container;
	Containers r_containers;
	for (Container c : containers)
	{
		r_container = { get<1>(c), get<0>(c), get<2>(c) };
		r_containers.push_back(r_container);
	}	
	return containers;
}

vector<Container> fill_containers()
{
	vector<Container> containers;
	auto small = make_tuple(2, 2, 1); 
	auto med = make_tuple(3, 2, 2);
	auto big = make_tuple(3, 3, 3);
	containers.push_back(small);
	containers.push_back(med);	
	containers.push_back(big);
	return containers;
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
