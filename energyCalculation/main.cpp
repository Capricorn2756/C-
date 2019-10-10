#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
using namespace std;

int main()
{
	char buff[255];
	double data[511][2], a = 0, d = 1.42, y, r, E1 = 0, E2 = 0, x;
	int i = 0, imax, j = 0, t, k, k1, n, b, q, shift, mas1[3];
	ifstream file("test.xyz");
	file.getline(buff, sizeof(buff));
	while (!file.getline(buff, sizeof(buff)).eof())					//считывание
	{
		t = 2;		//пропускаем С
		k1 = 2;
		j = 0;
		while (j < 3) //3 координаты
		{
			while (buff[t] != 9)
				t++;
			k = t;
			for (n = 0; n < k - k1; n++)
			{
				if (buff[k1 + n] == int('.'))
				{
					q = n;							//точка
					shift = 1;
				}
				else
				{
					b = buff[k1 + n] - 48;
					a = a + b*pow(10, k - k1 - 1 - n + shift);
				}
			}
			a = a / pow(10, k - k1 - 1 - q + shift);
			k1 = t + 1;
			data[i][j] = a;
			a = 0;
			q = 0;
			shift = 0;
			t++;
			j++;
		}
		i++;
		imax = i;
	}							
	file.close();
	//энергия растяжения 
	for (j = 0; j < imax; j++)
	{
		x = data[j][0];
		y = data[j][1];
		for (i = 0; i < imax; i++)
			if (i == j)
				continue;
			else if (data[i][0] <= x && data[i][0] > (x - d * 1.4))
				if (data[i][1] <= y && data[i][1] > (y - d * 1.3))		//1.3=sin60 + 0.5
				{
					r = sqrt(pow(x - data[i][0], 2) + pow(y - data[i][1], 2));
					E1 = E1 + pow(r - d, 2);
				}
	}
	int A, B, C;
	// энергия деформации
	for (j = 0; j < imax; j++)
	{
		x = data[j][0];
		y = data[j][1];
		k = 0;
		mas1[2] = -1;
		for (i = 0; i < imax; i++)
		{
			if (i == j)
				continue;
			else if (data[i][0] < (x + d * 1.4) && data[i][0] > (x - d * 1.4))
			{
				if (data[i][1] < (y + d * 1.3) && (data[i][1]) > (y - d * 1.3))		//1.3=sin60 + 0.5  	
				{
					mas1[k] = i;
					k++;
				}
			}
		}
		A = mas1[0], B = mas1[1];
		a = acos(((data[A][0] - x) * (data[B][0] - x) + (data[A][1] - y) * (data[B][1] - y)) / sqrt((pow(data[A][0] - x, 2) + pow(data[A][1] - y, 2)) * (pow(data[B][0] - x, 2) + pow(data[B][1] - y, 2))));		//из скалярнго произведения
		E2 = E2 + pow(a - 2*acos(-1.0)/3, 2);
		if (mas1[2] != -1)
		{
			C = mas1[2];
			a = acos(((data[A][0] - x) * (data[C][0] - x) + (data[A][1] - y) * (data[C][1] - y)) / sqrt((pow(data[A][0] - x, 2) + pow(data[A][1] - y, 2)) * (pow(data[C][0] - x, 2) + pow(data[C][1] - y, 2))));
			E2 = E2 + pow(a - 2 * acos(-1.0) / 3, 2);
			a = acos(((data[B][0] - x) * (data[C][0] - x) + (data[B][1] - y) * (data[C][1] - y)) / sqrt((pow(data[B][0] - x, 2) + pow(data[B][1] - y, 2)) * (pow(data[C][0] - x, 2) + pow(data[C][1] - y, 2))));
			E2 = E2 + pow(a - 2 * acos(-1.0) / 3, 2);
		}
	}
	cout << E1 << endl; //сдвиг
	cout << E2 << endl; //углы
	cout << E2 + E1 << endl << endl;
	system("pause");
	return 0;
}
