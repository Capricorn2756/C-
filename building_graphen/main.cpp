#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main()
{
	ofstream myFile;
	myFile.open("test.txt");	//создание файла
	int N = 205;	//количество точек по заданию
	int i;
	int DotNum = 1;	//количество точек 
	double maxLinks = N*(N - 1) / 2;	//количество связей, если соединить все со всеми
	double level = 0;	//размер нашего большого шестиугольника
	double Vdots = 0;	//количество точек в объёме
	while (Vdots < N)	//считаем, какого размера у нас будет большой шестиугольник
	{
		level++;	//level+1=количеству точек на стороне шестиугольника
		DotNum = DotNum + 6 * level;
		Vdots = DotNum * 2 * level;
	}
	double myLinks = 12;	//сколько получится связей у моего варианта, 12 связей у элементарной конструкции из 7ми точек, отсчитываем от неё
	for (i = 1; i < level - 1; i++)	//для начала считаем связи в полных шестиугольниках
		myLinks = myLinks + 12 * i + 6 * (i - 1);
	myLinks = myLinks + 18 * (level - 2);
	myLinks = myLinks*(level - 1);
	myLinks = myLinks + (DotNum * 3 - level * 6)*(level - 1);
	double hight = level * 2;
	double h = sqrt(3) / 2;	//высота
	int k = 0;	//ширина
	double H = h;
	double Dots[1000][2];	//массив координат узлов; номер точки, 0=х, 1=у
	i = 0;	//счётчик узлов
	for (double d = 0; d < level; d++)
	{
		k = 0;	//по сути отклонение от середины без единицы
		while (k < 2 * level - d)
		{
			Dots[i][0] = -level + (d + 1) / 2 + k;	//записываем координаты в массив 
			Dots[i][1] = H;
			i++;
			Dots[i][0] = -level + (d + 1) / 2 + k;	//симметричные узлы относительно середины
			Dots[i][1] = -H;
			i++;
			k++;
		}
		H = H + h;	//поднимаемся на следующий уровень по высоте
	}
	for (k = 0; k < 2 * level + 1; k++)	//ось узлов относительно которой симетричен шестиугольник, заполняем его узлами
	{
		Dots[i][0] = -level + k;
		Dots[i][1] = 0;
		i++;
	}
	int oneLayer = i, p = 0;
	double layer = 0;
	double Alldots[1000][3];
	double flagZero = 0;	//сдвиг делаем или не делаем
	for (int s = 0; s < level * 2 - 1; s++)
	{
		for (i = 0; i < oneLayer; i++)
		{
			Alldots[p][0] = Dots[i][0] + 0.5*flagZero;
			Alldots[p][1] = Dots[i][1] + h*flagZero;
			Alldots[p][2] = layer;
			p++;
		}
		if (flagZero == 0)
			flagZero = 1;
		else
			flagZero = 0;
		layer = layer + h;
	}
	
	
	
	//обсчитываем последний слой



	int NN = N - p;	
	DotNum = 1;
	level = 0;
	H = h;
	while (DotNum < NN)	//считаем, какого размера у нас будет большой шестиугольник
	{
		level++;	//level+1=количеству точек на стороне шестиугольника
		DotNum = DotNum + 6 * level;
	}
	if (DotNum != NN)	//если последний слой заполнен неполностью
	{
	DotNum = DotNum - 6 * level;	//внешний слой точек - удаляем сколько их там
	level--;	//уменьшили размер на 1, недостающие точки потом достроим
	}
	double myNewLinks = 12;	//сколько получится связей у моего варианта
	for (i = 1; i < level - 1; i++)	//для начала считаем связи в полных шестиугольниках
		myNewLinks = myNewLinks + 12 * i + 6 * (i - 1);
	myNewLinks = myNewLinks + 18 * (level - 2);
	myNewLinks = myNewLinks + 3 * DotNum;
	double NewDots[400][2];	//массив координат узлов; номер точки, 0=х, 1=у
	i = 0;	//счётчик узлов
	for (double d = 0; d < level; d++)
	{
		k = 0;	//по сути отклонение от середины без единицы
		while (k < 2 * level - d)
		{
			NewDots[i][0] = -level + (d + 1) / 2 + k;	//записываем координаты в массив 
			NewDots[i][1] = H;
			i++;
			NewDots[i][0] = -level + (d + 1) / 2 + k;	//симметричные узлы относительно середины
			NewDots[i][1] = -H;
			i++;
			k++;
		}
		H = H + h;	//поднимаемся на следующий уровень по высоте
	}
	for (k = 0; k < 2 * level + 1; k++)	//ось узлов относительно которой симетричен шестиугольник, заполняем его узлами
	{
		NewDots[i][0] = -level + k;
		NewDots[i][1] = 0;
		i++;
	}
	level++;	//переходим на слой, который надо достраивать
	int difference = NN - DotNum;	//на сколько точек надо ещё построить
	if (difference < level)		//в зависимости от того, сколько точек достраивается на последнем слое, уменьшаем количество связей, в самом цикле +3 связи, но иногда +2, надо учесть
		myNewLinks--;
	if (difference > level)
		myNewLinks--;
	if (difference > level * 2)
		myNewLinks--;
	if (difference > level * 3)
		myNewLinks--;
	if (difference > level * 4)
		myNewLinks = myNewLinks - 2;
	int flag1 = 0;
	int flag2 = 0;	//счётчик сколько точек мы уже добавили
	double x = -level;
	double y = 0;
	if (difference > 0)
	{
		if (3 * level < difference)	//6*level/2	если недостающих точек больше половины слоя
			flag1 = 1;
		while (difference > 0)
		{
			if (flag2 < level)	//поднимаемся вверх так /
			{
				x = x + 0.5;
				y = y + h;
			}
			else if (flag2 >= level && flag2 < 2 * level)	//идём вбок --
			{
				x = x + 1;
			}
			else	//идём вниз так \ 
			{
				x = x + 0.5;
				y = y - h;
			}
			NewDots[i][0] = x;	//добавляем координаты в массив
			NewDots[i][1] = y;
			i++;
			flag2++;
			myNewLinks = myNewLinks + 3 + 2;	//количество появившихся связей
			difference--;	//на 1 точку меньше осталось построить
			if (flag1 = 1 && difference > 0)	//добавляем симетричную точку, если недостающих точек больше половины
			{
				NewDots[i][0] = x;
				NewDots[i][1] = -y;	//симметричная точка
				i++;
				myNewLinks = myNewLinks + 3;
				difference--;	//на 1 точку меньше осталось построить
			}
		}
	}
	for (k = 0; k < i; k++)
	{
		Alldots[p][0] = NewDots[k][0] + 0.5*flagZero;
		Alldots[p][1] = NewDots[k][1] + h*flagZero;
		Alldots[p][2] = layer;
		p++;
	}
	for (i = 0; i < N; i++)							//запись в файл
	{
		for (int j = 0; j < 3; j++)
		{
			myFile << Alldots[i][j] << "\t";
			if (j == 2)
				myFile << "\t" << endl;
		}
	}
	myFile.close();	//закрываем файл
	double result = 100 * (myLinks*hight + myNewLinks) / maxLinks;	//процент от возможного количества связей
	cout << result << '%' << endl;	//вывод на экран процента
	system("pause");
	return 0;
}
