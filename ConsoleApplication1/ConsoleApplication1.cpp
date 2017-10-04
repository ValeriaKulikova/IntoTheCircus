// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <condition_variable>
#include <vector>
#include <thread>

using namespace std;

const int N = 5;//сколько будет львов

condition_variable condition;//условная переменная для синхронизации 

class animal {
	mutex mutex; // 
	bool previous_tiger;// было ли тигров животное в пред выводе
public: //конструктор класса
	animal() {
		previous_tiger = true; //т к первый лев задаем что предыдущ тигр
	}
	
	void show(int i) { 
		if (i%2 == 0) { //если число четное лев
			unique_lock<std::mutex> lock(mutex);// блок мютекса
			if (!previous_tiger) { //если предыд не был тигром то идет лев
				condition.wait(lock);// этот поток ожидает пока выйдет тигр
				cout << "Thread#" << this_thread::get_id() << " Выходит лев" << endl;
				previous_tiger = false;

			}
			else {
				cout << "Thread#" << this_thread::get_id() << " Выходит лев" << endl;
				previous_tiger = false;
				condition.notify_all(); //уведомляем все потоки что лев вышел
				lock.unlock();
			}
		}
		if (i % 2 != 0) {
			unique_lock<std::mutex> lock2(mutex);
			if (previous_tiger) {
				condition.wait(lock2);
				cout << "Thread#" << this_thread::get_id() << " Выходит тигр" << endl;
				previous_tiger = true;
			}
			cout << "Thread#" << this_thread::get_id() << " Выходит тигр" << endl;
			previous_tiger = true;
			condition.notify_all();
			lock2.unlock();
		}
	}
};

int main()
{
	setlocale(0,"Rus");
	animal ob; //созд объект класса
	vector<std::thread> animals; //вектор с потоками
	for (int i = 0; i < 2 * N - 1; i++) {
		animals.push_back(thread(&animal::show, &ob, i)); //заполняем вектор потоками
	}
	for (int i = 0; i < 2 * N - 1; i++) { //объединение с главным потоком
		animals[i].join();
	}
	system("pause");
    return 0;
}

