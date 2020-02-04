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
using Hold        = std::vector<std::vector<int>>;		//  

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

Hold brute_force(Containers original, Containers rotated, Hold &hold)
{
	// Это та структура, в которую будут записываться все результаты
	// Т.е. в конце итерации, когда весь контейнер заполнен, в нее будут
	// занесены все координаты контейнеров, отпечаток трюма и его свободное место
	std::vector<std::tuple<Coordinates, Hold, int>> poops;
	std::tuple<Coordinates, Hold, int> poop; 
	// Это список контейнеров, которые в данный момент перебираются итератором. 
	// Как только какое-то из значений в depth равняется с LIMIT, это значит, что  
	// все контейнеры уже пробовались и стоит подниматься на уровень выше
	std::vector<int> depth;
	// Это предел, при котором будет ясно, когда перебрали все контейнеры на шаге
	int LIMIT = original.size(); 

	// Делаем простое размещение контейнеров, чтобы понять, на каких координатах какой контейнер размещен
	// После этого координаты будем рассматривать снизу вверх, т.к. последняя координата - последний контейнер
	Coordinates coords = settle_containers(original, rotated, hold);
	Coordinate last_block = coords[coords.size() - 1];
}
Hold loop(Coordinate last, Hold hold, Containers conts, Coordinates &coords)
{
	// Удаление контейнера
	delete_container(hold, last);
	// Попытка разместить контейнеры
	for (int i = 0; i < conts.size(); i++)
	{
		if (place_container(hold, conts[i], coords))
		{

		}
	}
	return hold;

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

void delete_container(Hold &h, Coordinate c) 
{
	for (int m = 0; m < c[2]; m++)
		for (int n = 0; n < c[3]; n++)
			h[c[0] + m][c[1] + n] = 0;
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
