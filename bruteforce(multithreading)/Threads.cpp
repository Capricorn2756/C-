#include "allHeaders.h"
#include "Structure.cpp"

#define NUM_THREADS 4

void encrypt(uint32_t* v, uint32_t* key) {
	long r = 10;
	const int kw = 4;
	uint32_t c;
	for (r = 0; r < kw * 4 + 32; r++) c = v[1], v[1] += v[0] + ((v[1] << 6) ^ (v[1] >> 8)) + key[r%kw] + r, v[0] = c;
}
void decrypt(uint32_t* v, uint32_t* key) {
	long r = 10;
	int kw = 4;
	uint32_t c;
	for (r = kw * 4 + 31; r != -1; r--) c = v[0], v[0] = v[1] -= v[0] + ((v[0] << 6) ^ (v[0] >> 8)) + key[r%kw] + r, v[1] = c;
}

DWORD WINAPI Brute_force(void *data) {
	Thread_data *td = (Thread_data *)data;
	MSG    msg;
	uint32_t keyTest[8];
	int n0, n1, n2, n3, start, end;
	char crypText[10];	//���� ���������� ������������� �����
	printf("%d thread has started\n", td->partNum);
	start = td->partNum * 100 / NUM_THREADS;
	end = (td->partNum + 1) * 100 / NUM_THREADS;
	for (n0 = start; n0 < end; n0++) {	//�������
		keyTest[0] = n0;
		for (n1 = 0; n1 < 100; n1++) {
			keyTest[1] = n1;
			for (n2 = 0; n2 < 100; n2++) {
				keyTest[2] = n2;
				for (n3 = 0; n3 < 100; n3++) {
					if (PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE)) {	//����� ����� �������� ��� ����� � ���� ���������
						printf("%d thread has ended\n", td->partNum);
						return 0;
					}
					keyTest[3] = n3;
					decrypt((uint32_t*)crypText, keyTest);
					if(!strncmp(crypText, td->textCopy,8)){	//������������ �������������� ����������� ������ ����� � ����� �� �������� ������
						for (int ii = 0; ii < 4; ii++)	//��������� ���������� ����
							td->GotKey[ii] = keyTest[ii];
						printf("thread %d has got the correct result! Key=%d%d%d%d\n", td->partNum, keyTest[0], keyTest[1], keyTest[2], keyTest[3]);
						return 0;
					}
					else
						for (int ii = 0; ii < 8; ii++) 
							crypText[ii] = td->encText[ii];
				}
			}
		}
	}
	printf("%d thread has ended\n", td->partNum);
	return 0;
}

int main(void) {
	HANDLE thread[NUM_THREADS];
	DWORD thrId[NUM_THREADS];
	Thread_data td[NUM_THREADS];
	char charText[10] = "Test123";
	char encrypText[10] = "Test123";
	uint32_t key[4] = { 25, 47, 12, 56};	//������� ����
	int i, thread_answer;
	cout << "Original text: ";
	for (i = 0; i < 8; i++) {	//����� ��� ����������
		printf("%c", encrypText[i]);
	}
	printf("\n");
	encrypt((uint32_t*)encrypText, key);	//���������� ������ charText
	cout << "Result after using cipher: ";
	for (i = 0; i < 8; i++) {	//��������� ����������
		printf("%c", encrypText[i]);
	}
	printf("\n");
	for (i = 0; i < NUM_THREADS; i++) {
		for (int ii = 0; ii < 8; ii++) {
			td[i].encText[ii] = encrypText[ii];	//�������� �������������� ������
			td[i].textCopy[ii] = charText[ii];	//�������� ����� ������������ ������ ��� ��������
		}
		td[i].partNum = i;	//������������� ������� ��� ��������
		thread[i] = CreateThread(NULL, 0, &Brute_force, &td[i], 0, &thrId[i]);	//�������� ������
	}
	WaitForMultipleObjects(NUM_THREADS, thread, FALSE, INFINITE);	//���, ���� �� ������� ������, ��� �����-�� ����� ����� ������ ����
	for (i = 0; i < NUM_THREADS; i++) {	//����� ����� ������, ������� ����� �����
		DWORD exitcode;
		GetExitCodeThread(thread[i], &exitcode);
		if (!exitcode) {
			thread_answer = i;
			break;
		}
	}
	for (i = 0; i < NUM_THREADS; i++) {
		PostThreadMessage(thrId[i], WM_QUIT, 0, 0);	//��������� ������
	}
	WaitForMultipleObjects(NUM_THREADS, thread, TRUE, INFINITE);	//����������, ���� ��� ������ ��������� � ���������� 
	for (i = 0; i < NUM_THREADS; i++)
		CloseHandle(thread[i]);	//������� ����� �������
	cout << "Key: ";
	for (i = 0; i < 4; i++)
		printf("%d", td[thread_answer].GotKey[i]);	//����� ������
	decrypt((uint32_t*)encrypText, td[thread_answer].GotKey);	//����������� ������������ ������ ���������� ������
	printf("\n"); 
	cout << "Decrypted text: ";
	for (i = 0; i<8; i++) {
		printf("%c", encrypText[i]);	
	}
	printf("\n");
	getchar();
	return 0;
}