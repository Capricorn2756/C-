#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <vector>
#include <stack>
using namespace std;

class datainput	//базовый класс
{
protected:
	int a, size;	//количество кластеров; количество точек;
	double data[255][2];
public:
	void input(int i, int insize, double **inputdata)
	{
		a = i;	//количество кластеров
		size = insize;	//количество точек
		for (int p = 0; p < size; p++)		//передача массива координат точек через указатели
			for (int t = 0; t < 2; t++)
				data[p][t] = inputdata[p][t];
	}
};

class kmeans: public datainput	//производный класс
{
private:
	double max = INT_MAX;
	int minP, mas[60];			//номер кластера с минимальным R; массив номеров точек, принадлежащих кластеру
protected:
	int clusters№[255];		//массив выбранных кластеров;
	stack <int> groups[127];	
	double Rsqr[127][255], clustData[255][2];
public:
	void firstClust()			//первый выбор кластеров, i - количество кластеров, size - количество элементов в массиве данных, nom[] - массив номеров выбранных элементов
	{
		srand(time(NULL));
		vector<int> myVector(size);
		for (int t = 0; t < size; t++)
			myVector[t] = t;	//вектор из номеров точек
		random_shuffle(myVector.begin(), myVector.end());		//перемешиваем номера координат, и выбираем первые i элементов вектора => получили i случайных кастеров
		for (int p = 0; p < a; p++)
		{
			clusters№[p] = myVector[p];
			cout << clusters№[p] << endl;			//вывод на экран номеров выбранных элементов
		}
		cout << endl;
	}
	void firstDistance()			//считаем расстояние до каждого кластера для каждой точки
	{
		for (int t = 0; t < size; t++)		//пробегаем все точки
		{
			for (int p = 0; p < a; p++)				//пробегаем все кластеры; p - порядковый номер выбранного кластера в массиве	clusters№[]	
			{
				int a = clusters№[p];	// a - номер точки, которую мы взяли за кластер
				Rsqr[p][t] = pow(data[a][0] - data[t][0], 2) + pow(data[a][1] - data[t][1], 2);		//R^2=(Xj-Xi)^2+(Yj-Yi) где j- пробегает все кластеры, а i - все точки
				if (Rsqr[p][t] < max)
				{
					max = Rsqr[p][t];		//ищем минимальное расстояние для каждой точки
					minP = p;				//и запоминаем номер соответствующего кластера
				}
			}
			groups[minP].push(t); //стэк для каждого кластера заполняется соответствующими ему точками
			max = INT_MAX;
		}
	}
	double** newCluster()		
	{
		double **bufdata = new double*[255];	//создаём двумерный динамический массив (буфер)
		for (int t = 0; t < 255; t++)
			bufdata[t] = new double[2];
		int clustSize;
		double sum = 0;
		for (int p = 0; p < a; p++)			//для каждого кластера
		{
			clustSize = groups[p].size();
			for (int t = 0; t < clustSize; t++)		//пока не перебрали все его точки в стэке
			{
				mas[t] = groups[p].top();		//берём номер точки
				groups[p].pop();				//удаляем точку из стека
			}
			for (int q = 0; q < 2; q++)		//X и Y
			{
				for (int t = 0; t < clustSize; t++)	//по всем точкам для этго кластера
					sum += data[mas[t]][q];			//сумма иксов при q=0, игриков при q=1
				clustData[p][q] = sum / clustSize;	//записываем новые средние Х и У  для p-го кластера
				bufdata[p][q] = clustData[p][q];
				//cout << clustData[p][q] << endl;
				sum = 0;		//обнуляем сумму
			}
		}
		return bufdata;		//возвращаем массив получившихся кластеров с их координатами для дальнейшей работы с ними в мэйне
	}
	void distance()			//считаем расстояние до каждого кластера для каждой точки
	{
		for (int t = 0; t < size; t++)		//пробегаем все точки
		{
			for (int p = 0; p < a; p++)				//пробегаем все кластеры; p - порядковый номер выбранного кластера в массиве	clusters№[]	
			{
				Rsqr[p][t] = pow(clustData[p][0] - data[t][0], 2) + pow(clustData[p][1] - data[t][1], 2);		//R^2=(Xj-Xi)^2+(Yj-Yi) где j- пробегает все кластеры, а i - все точки
				if (Rsqr[p][t] < max)
				{
					max = Rsqr[p][t];		//ищем минимальное расстояние для каждой точки
					minP = p;				//и запоминаем номер соответствующего кластера
				}
			}
			groups[minP].push(t); //стэк для каждого кластера заполняется сответствующими ему точками
			max = INT_MAX;
		}
	}
};

int main()
{
	char *bufray = new char[127];		//динамический массив для буффера
	double number = 0;
	int i = 0, j = 0, t, end, start, length, symb, point, shift = 0;
	double **data = new double* [255];		//двумерный динамический массив для координат
	for (t = 0; t < 255; t++)
		data[t] = new double[3];
	double **bufdata;
	ifstream file("test.xyz");					//считвыние из файла
	while (!file.getline(bufray, 127).eof())			//пока не дошли до конца файла
	{
		t = 1;				//t - счётчик символа в строке	
		while (bufray[t] != 9)		//пропускаем номер элемента в строке файла, т.к. в файле идут по порядку, и он соответствует номеру строки
			t++;
		t++;
		start = t;			//start - с какой ячейки строки начинается число (координата)
		while (j < 2)			//2 координаты, j - номер координаты
		{
			while (bufray[t] != 9)		//пока не началась табуляция	
				t++;
			end = t;					//конец числа, на какой ячейке строки оно кончилось		-1
			for (length = 0; length < end - start; length++)		//пока не прошли посимвольно по всему числу
			{
				if (bufray[start + length] == int('.'))		//если считываем точку
				{
					point = length;					//запоминаем на каком месте в числе стоит точка
					shift = 1;					//shift - сдвиг на 1, длина числа на 1 символ короче, т.к. мы считали точку как цифру в числе
				}
				else
				{
					symb = bufray[start + length] - 48;		//buff[] - код символ, который мы считали; чтобы из char превратить его в число, вычитаем 48 из кода символа, получаем код соответсвующего числа, обозначаем его symb
					number = number + symb * pow(10, end - start - 1 - length + shift);		//собираем число, умножая символ на порядок, в котором оно должно оказаться, точка пока не учитывается
				}
			}
			if (shift == 1)		//если число было с точкой
				number = number / pow(10, end - start - 1 - point + shift);		//делим полученное число на 10 в нужной степени, чтобы получить число с точкой
			start = t + 1;			//сдвигаемся на следующее число
			data[i][j] = number;	//записываем координату в массив
			number = 0;				//обнуляем переменные
			point = 0;
			shift = 0;
			t++;					//пропускаем табуляцию
			j++;					//переходим к другой координате
		}
		i++;						//сдвигаем счётчик к следующему элементу (следующая строка)
		j = 0;				//обнуляем счётчик координат
	}						
	file.close();		//закрываем файл
	delete[] bufray;
	//cout << i << endl;	//вывод i - количества элементов в файле
	/*for (t = 0; t < i; t++)	//вывод массива точек
		for (j = 0; j < 2; j++)
		{
			cout << data[t][j] << '	';
			if (j == 1)
				cout << endl;
		}*/
	kmeans test;
	test.input(5, i, data);
	test.firstClust();
	test.firstDistance();
	bufdata = test.newCluster();
	/*for (t = 0; t < 5; t++)	//вывод массива точек
		for (j = 0; j < 2; j++)
		{
			cout << bufdata[t][j] << '	';
			if (j == 1)
				cout << endl;
		}*/
	for (i = 0; i < 255; i++)		//удаление двумерного динамического массива
		delete [] data[i];
	delete[] data;
	for (i = 0; i < 255; i++)		//удаление двумерного динамического массива
		delete[] bufdata[i];
	delete[] bufdata;
	system("pause");
	return 0;
}
