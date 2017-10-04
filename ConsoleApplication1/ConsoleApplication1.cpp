// ConsoleApplication1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <condition_variable>
#include <vector>
#include <thread>

using namespace std;

const int N = 5;//������� ����� �����

condition_variable condition;//�������� ���������� ��� ������������� 

class animal {
	mutex mutex; // 
	bool previous_tiger;// ���� �� ������ �������� � ���� ������
public: //����������� ������
	animal() {
		previous_tiger = true; //� � ������ ��� ������ ��� �������� ����
	}
	
	void show(int i) { 
		if (i%2 == 0) { //���� ����� ������ ���
			unique_lock<std::mutex> lock(mutex);// ���� �������
			if (!previous_tiger) { //���� ������ �� ��� ������ �� ���� ���
				condition.wait(lock);// ���� ����� ������� ���� ������ ����
				cout << "Thread#" << this_thread::get_id() << " ������� ���" << endl;
				previous_tiger = false;

			}
			else {
				cout << "Thread#" << this_thread::get_id() << " ������� ���" << endl;
				previous_tiger = false;
				condition.notify_all(); //���������� ��� ������ ��� ��� �����
				lock.unlock();
			}
		}
		if (i % 2 != 0) {
			unique_lock<std::mutex> lock2(mutex);
			if (previous_tiger) {
				condition.wait(lock2);
				cout << "Thread#" << this_thread::get_id() << " ������� ����" << endl;
				previous_tiger = true;
			}
			cout << "Thread#" << this_thread::get_id() << " ������� ����" << endl;
			previous_tiger = true;
			condition.notify_all();
			lock2.unlock();
		}
	}
};

int main()
{
	setlocale(0,"Rus");
	animal ob; //���� ������ ������
	vector<std::thread> animals; //������ � ��������
	for (int i = 0; i < 2 * N - 1; i++) {
		animals.push_back(thread(&animal::show, &ob, i)); //��������� ������ ��������
	}
	for (int i = 0; i < 2 * N - 1; i++) { //����������� � ������� �������
		animals[i].join();
	}
	system("pause");
    return 0;
}

