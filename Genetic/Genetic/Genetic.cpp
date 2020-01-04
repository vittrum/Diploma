// Genetic.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <chrono> 
#include <tuple>
#include <array>
using namespace std;
using namespace std::chrono; // Clock


// TO DO:
// Дебажить заполнение координат
// Определить размещение контейнеров
// Прикрутить библиотеку генетических алгоритмов


auto start = high_resolution_clock::now();	// Clock

// Заполнить трюм пустыми значениями
auto initialize_hold(vector<vector<int>> hold, int a_side, int b_side)->vector<vector<int>>;
// Заполнить список контейнеров значениями
auto fill_containers()->vector<tuple<int, int>>;
// Создать список возможных координат для каждого контейнера
auto create_container_coordinates(vector<tuple<int, int>> containers, vector<vector<int>> hold)->vector<array<int, 4>>;
// Разместить контейнеры
auto settle_containers_in_hold();

int main()
{
	auto start = high_resolution_clock::now();	// Clock

	vector<vector<int>> hold;
	int a = 5; // Vertical
	int b = 3; // Horizontal
	hold = initialize_hold(hold, a, b);
	auto containers = fill_containers();

	// Распечатать трюм
	/*
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
			cout << hold[i][j] << ' ';
		cout << endl;
	}*/

	auto coordinates = create_container_coordinates(containers, hold);
	// Распечатать координаты
	for (int i = 0; i < coordinates.size(); i++)
	{
		cout << endl;
		for (int j = 0; j < coordinates[0].size(); j++)
			cout << coordinates[i][j] << ' ';
	}

	auto stop = high_resolution_clock::now();					// Clock
	auto duration = duration_cast<microseconds>(stop - start);	// Clock

	cout << endl << duration.count() << " ms" << endl;			// Clock
}

vector<vector<int>> initialize_hold(vector<vector<int>> hold, int a_side, int b_side)
{
	hold.clear();
	vector<int> temp;
	for (int j = 0; j < b_side; j++)
		temp.push_back(0);
	for (int i = 0; i < a_side; i++)
		hold.push_back(temp);
	return hold;
}

vector<tuple<int, int>> fill_containers()
{
	vector<tuple<int, int>> containers;
	auto small = make_tuple(2, 2); 
	auto med = make_tuple(2, 3);
	auto big = make_tuple(2, 4);
	containers.push_back(small);
	containers.push_back(med);
	containers.push_back(big);
	return containers;
}

vector<array<int, 4>> create_container_coordinates(vector<tuple<int, int>> containers, vector<vector<int>> hold)
{
	vector<array<int, 4>> coordinates;
	int a = hold.size();
	int b = hold[0].size();
	array <int, 4> coord;
	for (auto cont : containers) 
	{
		for (int i = 0; i < a - get<1>(cont) + 1; i++)
			for (int j = 0; j < b - get<0>(cont) + 1; j++)
			{
				coord = { i, j, i + a, j + b };
				coordinates.push_back(coord);
			}
	}
	return coordinates;
}




