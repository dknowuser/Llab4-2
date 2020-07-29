/*
 * main.cpp
 *
 *  Created on: 6 янв. 2019 г.
 *      Author: wetfloor
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/resource.h>

using namespace std;

const unsigned int maxCharacters = 256;
FILE* inputFile;
FILE* oddFile;
FILE* evenFile;


void* oddFunction(void* inputString) {
	if(fputs((char*)inputString, oddFile) == EOF) {
		cout << "Ошибка при записи в файл." << endl;
	};
	pthread_exit(0);
};

void* evenFunction(void* inputString) {
	if(fputs((char*)inputString, evenFile) == EOF) {
		cout << "Ошибка при записи в файл." << endl;
	};
	pthread_exit(0);
};

int main(void) {
	inputFile = fopen("/home/wetfloor/Документы/etc/inputFile", "r");
	oddFile = fopen("/home/wetfloor/Документы/etc/oddFile", "w");
	evenFile = fopen("/home/wetfloor/Документы/etc/evenFile", "w");

	//Буфер для считывания строк
	char oddStr[maxCharacters - 1];
	char evenStr[maxCharacters - 1];
	bool continueRead = true;

	//Основной цикл
	while(continueRead) {
		//Считываем нечётную строку
		continueRead = fgets(oddStr, maxCharacters, inputFile);

		if(!continueRead) {	break; };

		pthread_t oddThread = 0;
		if(!pthread_create(&oddThread, NULL, oddFunction, oddStr)) {
			cout << "Поток для нечётных строк успешно создан." << endl;
		}
		else {
			cout << "Ошибка при создании потока для нечётных строк." << endl;
			break;
		};

		//Считываем чётную строку
		continueRead = fgets(evenStr, maxCharacters, inputFile);

		if(!continueRead) {	break; };

		pthread_t evenThread = 0;
		if(!pthread_create(&evenThread, NULL, evenFunction, evenStr)) {
			cout << "Поток для чётных строк успешно создан." << endl;
		}
		else {
			cout << "Ошибка при создании потока для чётных строк." << endl;
			break;
		};

		//Ждём, пока оба потока завершат работу
		if(!pthread_join(oddThread, 0)) {
			if(!pthread_join(evenThread, 0)) {
				cout << "Оба потока успешно завершили работу." << endl;
			}
			else {
				cout << "Ошибка при ожидании завершения потока для чётных строк." << endl;
			};
		}
		else {
			cout << "Ошибка при ожидании завершения потока для нечётных строк." << endl;
		};
	};

	cout << "Достигнут конец входного файла." << endl;
	fclose(inputFile);
	fclose(oddFile);
	fclose(evenFile);

	return 0;
};


