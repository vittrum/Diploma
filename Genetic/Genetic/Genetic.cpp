// Genetic.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <chrono> 

using namespace std;
using namespace std::chrono;

// Use auto keyword to avoid typing long 
// type definitions to get the timepoint 
// at this instant use function now() 
auto start = high_resolution_clock::now();


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

int main()
{
	auto start = high_resolution_clock::now();

	vector<vector<int>> hold;
	int a = 5;
	int b = 3;
	hold = initialize_hold(hold, a, b);

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
			cout << hold[i][j] << ' ';
		cout << endl;
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << duration.count() << " ms" << endl;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

