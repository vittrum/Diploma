// Genetic.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <chrono> 
#include <tuple>
#include <array>


// TO DO:
// Почему-то не заходит в повороты контейнеров
// 
// POTOM: 
// Прикрутить библиотеку генетических алгоритмов
//

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
	/*Container_Type*/
	int _type;
	int length;
	int width;
	int amount;
};

using Containers  = std::vector<Container>;
using Coordinate  = std::array <int, 5>;				// 1 и 2 - (x1,y1), 3 и 4 - (x2,y2), 5 - номер контейнера
using Coordinates = std::vector<Coordinate>;
using Hold        = std::vector<std::vector<int>>;		

// Это число служит для того, чтобы знать, расположены контейнеры заранее, или 
// их уже перебирал bruteforce
static const int BASE = 9;

auto start = high_resolution_clock::now();	// Clock
// Заполнить трюм пустыми значениями
auto initialize_hold(Hold hold, int a_side, int b_side)->Hold;
// Создать контейнеры
auto create_containers()->Containers;
// Сделать поворот контейнера
auto rotate_containers(Containers c_s)->Containers;
// Координаты контейнера
auto container_coordinates(Container c, Hold h)->Coordinates;
// Разместить контейнеры
auto settle_containers(Containers original, Containers rotated, Hold &hold)->Coordinates;
// Подсчет пустого места в трюме
auto calculate_space(Hold hold)->int;
// Удаление контейнера, возвращает тип контейнера
auto delete_container(Hold& h, Coordinate c)->int;
// Размещение контейнера, true если удалось разместить, иначе false
auto place_container(Hold& hold, Container c, Coordinates& cs)->bool;
// Брутфорс, возвращает список отпечатанных значений
auto brute_force(Containers original, Containers rotated, Hold& hold)->std::vector<std::tuple<Coordinates, Hold, int>>;
// Сортировка брутфорса
auto get_best_case(std::vector<std::tuple<Coordinates, Hold, int>> archive)->std::tuple<Coordinates, Hold, int>;


int main()
{
	auto start = high_resolution_clock::now();	// Clock

	// Создание трюма
	Hold hold;
	int a = 11; // Vertical
	int b = 10; // Horizontal
	hold = initialize_hold(hold, a, b);
	// Конец создания трюма

	// Создание контейнера
	Containers original_containers = create_containers();
	Containers rotated_containers = rotate_containers(original_containers);
	// Конец создания контейнера

	// Создание координат контейнера
	std::vector<Coordinates> original, rotated;
	for (Container c : original_containers)
		original.push_back(container_coordinates(c, hold));
	for (Container c : rotated_containers)
		rotated.push_back(container_coordinates(c, hold));
	// Конец создания координат контейнера

	// Симуляция размещения
	auto coord = settle_containers(original_containers, rotated_containers, hold);
	// Конец симуляции размещения

	// Начало брутфорса
	auto archive = brute_force(original_containers, rotated_containers, hold);
	// Конец брутфорса
	hold = std::get<1>(get_best_case(archive));

	for (auto i : hold)
	{
		std::cout << '\n';
		for (auto j : i)
			std::cout << j << ' ';
	}

	auto stop     = high_resolution_clock::now();				       // Clock
	auto duration = duration_cast<microseconds>(stop - start);	       // Clock

	std::cout << std::endl << duration.count() << " ms" << std::endl;  // Clock
}

Hold initialize_hold(Hold hold, int vertical, int horizontal)
{
	hold.clear();
	std::vector<int> temp;
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
	Container c1 = { 1, 2, 2, 5000 };
	Container c2 = { 2, 3, 2, 4000 };
	Container c3 = { 3, 4, 3, 3000 };
	Container c4 = { 4, 5, 4, 2000 };
	containers.push_back(c1);
	containers.push_back(c4);
	containers.push_back(c3);
	containers.push_back(c2);
	
	return containers;
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

Coordinates settle_containers(Containers original, Containers rotated, Hold &hold)
{
	int hold_length = hold.size();
	int hold_width = hold[0].size();
	Coordinates coordinates;
	Coordinate coord;
	
	for (int i = 0; i < hold_length; i++)
		for (int j = 0; j < hold_width; j++)
			for (int k = 0; k < rotated.size(); k++)
			{
				if 
				(
					hold[i][j] == 0                         && // Если ячейка контейнера, с которой начинают движение, свободна
					rotated[k].amount > 0					&& // Если контейнеры данного типа еще остались
					i + rotated[k].length - 1 < hold_length && // Если помещается по длине
					j + rotated[k].width - 1 < hold_width 	   // По ширине					
				)
				{
					bool free = true;
					for (int m = 0; m < rotated[k].length; m++)
						for (int n = 0; n < rotated[k].width; n++)
							if (hold[i + m][j + n] > 0)
							{
								free = false;
								break;
							}
					if (free)
					{
						coord = { i, j, i + rotated[k].length - 1, j + rotated[k].width - 1 };
						coordinates.push_back(coord);
						for (int m = 0; m < rotated[k].length; m++)
							for (int n = 0; n < rotated[k].width; n++)
								hold[i + m][j + n] = rotated[k]._type;
						rotated[k].amount--;
					}
				}
				else 
				if 
				(
					hold[i][j] == 0							 &&
					rotated[k].amount > 0					 &&
					i + original[k].length - 1 < hold_length && 
					j + original[k].width - 1 < hold_width	  					
				)
				{
					bool free = true;
					for (int m = 0; m < original[k].length; m++)
						for (int n = 0; n < original[k].width; n++)
							if (hold[i + m][j + n] > 0)
							{
								free = false;
								break;
							}
					if (free)
					{
						coord = { i, j, i + original[k].length, j + original[k].width };
						coordinates.push_back(coord);
						for (int m = 0; m < original[k].length; m++)
							for (int n = 0; n < original[k].width; n++)
								hold[i + m][j + n] = rotated[k]._type + 10;
						rotated[k].amount--;
					}
				}
			}	
	return coordinates;
}

std::vector<std::tuple<Coordinates, Hold, int>> brute_force(Containers original, Containers rotated, Hold &hold)
{
	// Это та структура, в которую будут записываться все результаты
	// Т.е. в конце итерации, когда весь контейнер заполнен, в нее будут
	// занесены все координаты контейнеров, отпечаток трюма и его свободное место
	std::vector<std::tuple<Coordinates, Hold, int>> archive;
	std::tuple<Coordinates, Hold, int> record; 
	// Это предел, при котором будет ясно, когда перебрали все контейнеры на шаге
	int LIMIT = original.size(); 

	int previous;
	bool done_bruteforcing = false;
	
	// Делаем простое размещение контейнеров, чтобы понять, на каких координатах какой контейнер размещен
	// После этого координаты будем рассматривать снизу вверх, т.к. последняя координата - последний контейнер
	Coordinates coords = settle_containers(original, rotated, hold);

	Coordinate  last_block = coords[coords.size() - 1];	
	auto it = coords.end();
	coords.erase(it);
	previous = delete_container(hold, last_block);
	// Если удалённый контейнер не трогали, мы удаляем его и начинаем подставлять туда контейнеры из 
	// списка с самого начала, до тех пор, пока какой-то не встанет на место или не окажется, что его 
	// больше некуда ставить. Тогда делается отпечаток результатов и начинается копание дальше
	while (!done_bruteforcing)
	{
		if (previous > BASE)
		{
			for (auto c : original) // Перебор всех контейнеров
			{
				if (place_container(hold, c, coords)) // Если контейнер удалось поместить, то прерываем перебор
				{
					break;
				}
				else
				{
					// Если ни один из контейнеров не поместился, значит, трюм полон и нужно 
					// сохранить результат
					archive.push_back(record = { coords, hold, calculate_space(hold) });
				}
			}
		}
		else if (previous == LIMIT)
		{
			last_block = coords[coords.size() - 1];
			auto it = coords.end();
			coords.erase(it);
			if (!coords.empty())
			{
				previous = delete_container(hold, last_block);
			}
			else
			{
				done_bruteforcing = true;
			}
		}
		else if (previous < LIMIT)
		{
			for (int i = previous+1; i < LIMIT; i++) // Перебор начинаем с того контейнера, который был до этого
			{
				if (place_container(hold, original[i], coords))
				{
					break;
				}
				else
				{
					// Если ни один из контейнеров не поместился, значит, трюм полон и нужно 
					// сохранить результат
					archive.push_back(record = { coords, hold, calculate_space(hold) });
				}
			}
		}
	}
	return archive;
}

int calculate_space(Hold hold)
{
	int res = 0;
	for (int i = 0; i < hold.size(); i++)
		for (int j = 0; j < hold[0].size(); j++)
			if (i == 0)
				res++;
	return res;
}

int delete_container(Hold &h, Coordinate c) 
{
	for (int m = 0; m < c[2]; m++)
		for (int n = 0; n < c[3]; n++)
			h[c[0] + m][c[1] + n] = 0;
	return c[4];
}

bool place_container(Hold& hold, Container c, Coordinates &cs)
{
	Coordinate coord;
	for (int i = 0; i < hold.size(); i++)
		for (int j = 0; j < hold[0].size(); j++)
		{
			if
				(
					hold[i][j] == 0						&& 					
					i + c.length - 1 < hold.size()		&& 
					j + c.width - 1 < hold[0].size() 	  				
				)
			{
				bool free = true;
				for (int m = 0; m < c.length; m++)
					for (int n = 0; n < c.width; n++)
						if (hold[i + m][j + n] > 0)
						{
							free = false;
							return false;
						}				
				if (free)
				{
					coord = { i, j, i + c.length - 1, j + c.width - 1 };
					cs.push_back(coord);
					for (int m = 0; m < c.length; m++)
						for (int n = 0; n < c.width; n++)
							hold[i + m][j + n] = c._type;
					return true;
				}				
			}
		}
	return false;
}

std::tuple<Coordinates, Hold, int> get_best_case(std::vector<std::tuple<Coordinates, Hold, int>> archive)
{
	auto best_solution = archive[0];
	for (auto i : archive)
	{
		if (std::get<2>(i) < std::get<2>(best_solution))
			best_solution = i;
	}
	return best_solution;
}

