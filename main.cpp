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

class datainput	//������� �����
{
protected:
	int a, size;	//���������� ���������; ���������� �����;
	double data[255][2];
public:
	void input(int i, int insize, double **inputdata)
	{
		a = i;	//���������� ���������
		size = insize;	//���������� �����
		for (int p = 0; p < size; p++)		//�������� ������� ��������� ����� ����� ���������
			for (int t = 0; t < 2; t++)
				data[p][t] = inputdata[p][t];
	}
};

class kmeans: public datainput	//����������� �����
{
private:
	double max = INT_MAX;
	int minP, mas[60];			//����� �������� � ����������� R; ������ ������� �����, ������������� ��������
protected:
	int clusters�[255];		//������ ��������� ���������;
	stack <int> groups[127];	
	double Rsqr[127][255], clustData[255][2];
public:
	void firstClust()			//������ ����� ���������, i - ���������� ���������, size - ���������� ��������� � ������� ������, nom[] - ������ ������� ��������� ���������
	{
		srand(time(NULL));
		vector<int> myVector(size);
		for (int t = 0; t < size; t++)
			myVector[t] = t;	//������ �� ������� �����
		random_shuffle(myVector.begin(), myVector.end());		//������������ ������ ���������, � �������� ������ i ��������� ������� => �������� i ��������� ��������
		for (int p = 0; p < a; p++)
		{
			clusters�[p] = myVector[p];
			cout << clusters�[p] << endl;			//����� �� ����� ������� ��������� ���������
		}
		cout << endl;
	}
	void firstDistance()			//������� ���������� �� ������� �������� ��� ������ �����
	{
		for (int t = 0; t < size; t++)		//��������� ��� �����
		{
			for (int p = 0; p < a; p++)				//��������� ��� ��������; p - ���������� ����� ���������� �������� � �������	clusters�[]	
			{
				int a = clusters�[p];	// a - ����� �����, ������� �� ����� �� �������
				Rsqr[p][t] = pow(data[a][0] - data[t][0], 2) + pow(data[a][1] - data[t][1], 2);		//R^2=(Xj-Xi)^2+(Yj-Yi) ��� j- ��������� ��� ��������, � i - ��� �����
				if (Rsqr[p][t] < max)
				{
					max = Rsqr[p][t];		//���� ����������� ���������� ��� ������ �����
					minP = p;				//� ���������� ����� ���������������� ��������
				}
			}
			groups[minP].push(t); //���� ��� ������� �������� ����������� ���������������� ��� �������
			max = INT_MAX;
		}
	}
	double** newCluster()		
	{
		double **bufdata = new double*[255];	//������ ��������� ������������ ������ (�����)
		for (int t = 0; t < 255; t++)
			bufdata[t] = new double[2];
		int clustSize;
		double sum = 0;
		for (int p = 0; p < a; p++)			//��� ������� ��������
		{
			clustSize = groups[p].size();
			for (int t = 0; t < clustSize; t++)		//���� �� ��������� ��� ��� ����� � �����
			{
				mas[t] = groups[p].top();		//���� ����� �����
				groups[p].pop();				//������� ����� �� �����
			}
			for (int q = 0; q < 2; q++)		//X � Y
			{
				for (int t = 0; t < clustSize; t++)	//�� ���� ������ ��� ���� ��������
					sum += data[mas[t]][q];			//����� ����� ��� q=0, ������� ��� q=1
				clustData[p][q] = sum / clustSize;	//���������� ����� ������� � � �  ��� p-�� ��������
				bufdata[p][q] = clustData[p][q];
				//cout << clustData[p][q] << endl;
				sum = 0;		//�������� �����
			}
		}
		return bufdata;		//���������� ������ ������������ ��������� � �� ������������ ��� ���������� ������ � ���� � �����
	}
	void distance()			//������� ���������� �� ������� �������� ��� ������ �����
	{
		for (int t = 0; t < size; t++)		//��������� ��� �����
		{
			for (int p = 0; p < a; p++)				//��������� ��� ��������; p - ���������� ����� ���������� �������� � �������	clusters�[]	
			{
				Rsqr[p][t] = pow(clustData[p][0] - data[t][0], 2) + pow(clustData[p][1] - data[t][1], 2);		//R^2=(Xj-Xi)^2+(Yj-Yi) ��� j- ��������� ��� ��������, � i - ��� �����
				if (Rsqr[p][t] < max)
				{
					max = Rsqr[p][t];		//���� ����������� ���������� ��� ������ �����
					minP = p;				//� ���������� ����� ���������������� ��������
				}
			}
			groups[minP].push(t); //���� ��� ������� �������� ����������� ��������������� ��� �������
			max = INT_MAX;
		}
	}
};

int main()
{
	char *bufray = new char[127];		//������������ ������ ��� �������
	double number = 0;
	int i = 0, j = 0, t, end, start, length, symb, point, shift = 0;
	double **data = new double* [255];		//��������� ������������ ������ ��� ���������
	for (t = 0; t < 255; t++)
		data[t] = new double[3];
	double **bufdata;
	ifstream file("test.xyz");					//��������� �� �����
	while (!file.getline(bufray, 127).eof())			//���� �� ����� �� ����� �����
	{
		t = 1;				//t - ������� ������� � ������	
		while (bufray[t] != 9)		//���������� ����� �������� � ������ �����, �.�. � ����� ���� �� �������, � �� ������������� ������ ������
			t++;
		t++;
		start = t;			//start - � ����� ������ ������ ���������� ����� (����������)
		while (j < 2)			//2 ����������, j - ����� ����������
		{
			while (bufray[t] != 9)		//���� �� �������� ���������	
				t++;
			end = t;					//����� �����, �� ����� ������ ������ ��� ���������		-1
			for (length = 0; length < end - start; length++)		//���� �� ������ ����������� �� ����� �����
			{
				if (bufray[start + length] == int('.'))		//���� ��������� �����
				{
					point = length;					//���������� �� ����� ����� � ����� ����� �����
					shift = 1;					//shift - ����� �� 1, ����� ����� �� 1 ������ ������, �.�. �� ������� ����� ��� ����� � �����
				}
				else
				{
					symb = bufray[start + length] - 48;		//buff[] - ��� ������, ������� �� �������; ����� �� char ���������� ��� � �����, �������� 48 �� ���� �������, �������� ��� ��������������� �����, ���������� ��� symb
					number = number + symb * pow(10, end - start - 1 - length + shift);		//�������� �����, ������� ������ �� �������, � ������� ��� ������ ���������, ����� ���� �� �����������
				}
			}
			if (shift == 1)		//���� ����� ���� � ������
				number = number / pow(10, end - start - 1 - point + shift);		//����� ���������� ����� �� 10 � ������ �������, ����� �������� ����� � ������
			start = t + 1;			//���������� �� ��������� �����
			data[i][j] = number;	//���������� ���������� � ������
			number = 0;				//�������� ����������
			point = 0;
			shift = 0;
			t++;					//���������� ���������
			j++;					//��������� � ������ ����������
		}
		i++;						//�������� ������� � ���������� �������� (��������� ������)
		j = 0;				//�������� ������� ���������
	}						
	file.close();		//��������� ����
	delete[] bufray;
	//cout << i << endl;	//����� i - ���������� ��������� � �����
	/*for (t = 0; t < i; t++)	//����� ������� �����
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
	/*for (t = 0; t < 5; t++)	//����� ������� �����
		for (j = 0; j < 2; j++)
		{
			cout << bufdata[t][j] << '	';
			if (j == 1)
				cout << endl;
		}*/
	for (i = 0; i < 255; i++)		//�������� ���������� ������������� �������
		delete [] data[i];
	delete[] data;
	for (i = 0; i < 255; i++)		//�������� ���������� ������������� �������
		delete[] bufdata[i];
	delete[] bufdata;
	system("pause");
	return 0;
}