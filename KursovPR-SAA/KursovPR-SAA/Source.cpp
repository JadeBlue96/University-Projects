#include <iostream>
#include <fstream>       //Библиотека за файлов входно-изходен поток
#include <ctime>         //Библиотека за манипулация с дата и време(за генериране на случайни числа)
#define EOQ -82589230    //Константа указваща краят на опашката
using namespace std;
int SIZE = 0;            //Променлива показваща броят на елементите в опашката

struct elem {            //Динамична структура за елемент на опашка
	int key;
	elem *next;
};

struct queue {         //Динамична структура - опашка от елементи
	elem *first;
	elem *last;
};

void push(queue& q, int n);   //Функция, която добавя елемент в опашката
int pop(queue& q);            //Функция, която извежда елемент от опашката
void generate();              //Функция, която генерира въведеният брой стойности като случайни в определен интервал, и ги записва във файл
void showqueue(queue& q);     //Функция, която извежда на екрана съдържанието на опашката
void loadqueue(queue& q);     //Функция, която запълва опашката със данните записани във външния файл
void savequeue(queue& q1);    //Функция, която записва резултатите във външен файл
void quicksort(queue& q, int pos);   //Функция, която изпълнява бързо сортиране на опашката по възходящ или низходящ ред
int calc_mvalue(queue& q);    //Функция, която изчислява средноаритметичната стойност на елементите в опашката
int search(queue& q);         //Функция, която намира най-малкият елемент в опашката, който е по-голям от средноаритметичната й стойност
int deleteq(queue& q);        //Функция, която изтрива намереният елемент

void push(queue& q,int n)
{
	elem *p = new elem;
	if (q.last == NULL) q.first = p;
	else q.last->next = p;
	q.last = p;
	p->key = n;
	p->next = NULL;
}
int pop(queue& q) {
	int n;
	elem *p = q.first;
	if (p == NULL) return EOQ;
	n = p->key;
	q.first = p->next;
	delete p;
	if (q.first == NULL) q.last = NULL;
	return n;
}
void showqueue(queue& q) {
	if (q.first == NULL) {
		cout << "Queue is empty!" << endl;
		return;
	}
	cout << endl;
	int t;
	push(q, EOQ);
	do {
		t = pop(q);
		if (t != EOQ) {
			cout << t << " ";
			push(q, t);
		}
	} while (t != EOQ);
	cout << endl << endl;
}
void generate()
{
	int n, t;
	srand(time(NULL));                 //Инициализиране на функцията за генериране на случайни числа

	cout << "Enter the number of elements for the queue:";
	cin >> n;
	SIZE = n;
	cout << endl;
	ofstream file;
	file.open("content.txt");        //Отваряне на файл за запис
	if (file.is_open())
	{
		for (int i = 0; i<n; i++)
		{
			t = rand() % 1000;
			file << t << endl;       //Генериране на данни във файла
		}
		file.close();                //Затваряне на файл
	}
	else cout << "Error:File could not be opened.";
}



void loadqueue(queue& q)
{
	int t;
	ifstream file;
	file.open("content.txt", ios::in);     //Отваряне на файл за четене
	
	if (file.is_open())
	{
		while (q.first) pop(q);
		while (file >> t)
		{
           push(q, t);                   //Запис на данни от файл във опашката
		}
		file.close();
		cout << endl << " File successfully read." << endl << endl;
	}
	else cout << "Error: File could not be read." << endl;
}

void savequeue(queue& q1)
{
	elem *k = q1.first;
	ofstream outf2;
	if (q1.first == NULL) {
		cout << "Queue is empty!" << endl;
		return;
	}
	if (q1.first)
	{
		outf2.open("results.txt", ios::out);         //Отваряне на файл за запис
		if (outf2)
		{
			while (k->next)
			{
				outf2 << k->key << endl;           //Запис на данни от опашката във файл
				k = k->next;
			}
			outf2 << k->key << endl;
			outf2.close();
			cout << "Results saved in file." << endl << endl;
		}
		else cout << "File could not be saved!" << endl << endl;
	}
	else cout << "The queue is empty!" << endl;
}

void quicksort(queue& q, int pos) {

	if (q.first == NULL)
	{
		return;
	}
	queue ql = { NULL, NULL };          //Създаване на две подопашки като лява и дясна част
	queue qr = { NULL, NULL };
	int  t, pv, count = 1;
	// Разделяне и разпределение на елементите на опашката
	pv = pop(q);
	while (q.first) {
		t = pop(q);
		if (t == pv) count++;
		else if (t<pv) push(ql, t);
		else  push(qr, t);
	}
	quicksort(ql, pos);
	quicksort(qr, pos);
	// Обединяване на опашката и окончателно сортиране (pos=1 (Възходящ ред) ; pos=0 (Низходящ ред))
	if (pos) {
		while (ql.first) push(q, pop(ql));
		for (int i = 0; i<count; i++) push(q, pv);
		while (qr.first) push(q, pop(qr));
	}
	else {
		while (qr.first) push(q, pop(qr));
		for (int i = 0; i<count; i++) push(q, pv);
		while (ql.first) push(q, pop(ql));
	}
}

int calc_mvalue(queue& q)
{
	if (q.first == NULL) {
		cout << "Queue is empty!" << endl;
		return 0;
	}
	int L(0),count(0);
	int t;
	push(q, EOQ);
	do {
		t = pop(q);
		if (t != EOQ)
		{
			push(q, t);
			L += t;
		}
	} while (t != EOQ);
	L=L / SIZE;                 //Изчисляване на средноаритметичната стойност от елементите
	cout << endl << "L=" << L << endl << endl;;
	return L;
}
int search(queue& q) {
	if (q.first == NULL) {
		cout << "Queue is empty!" << endl;
		return 0;
	}
	int t, min, flag = 0;
	int k = calc_mvalue(q);
	if (q.first == NULL) {
		cout << "Queue is empty!\n";
		return 0;
	}
	push(q, EOQ);
	do {                      //Търсене на минималния елемент на опашката(по-голям от L)
		t = pop(q);
		if (t != EOQ)
		{
			push(q, t);
			if (t > k)
			{
				if (!flag) { min = t; flag = 1; }
				if (t < min) min = t;
			}
		}

	} while (t != EOQ);
	cout << endl;
	if (flag) cout << "Min. element:" << min << endl << endl;
	return min;
}
int deleteq(queue& q)
{
	if (q.first == NULL) {
		cout << "Queue is empty!" << endl;
		return 0;
	}
	int del(0);
	int k = search(q);
	int t(0);
	if (q.first == NULL) {
		cout << "Queue is empty!\n";
		return 0;
	}
	push(q, EOQ);
	do {                        //Откриване на елемента със стойност min и неговото изтриване
		t = pop(q);
		if (t != EOQ)
		{
			if (t == k)
			{
				cout << "Deleting element:" << t << endl << endl;
				del = pop(q);
				push(q, del);
			}
			else push(q, t);
		}
	} while (t != EOQ);
	return 0;
}
void main()
{
	int choice(0);
	queue q={ NULL, NULL };
	clock_t timer = clock();                  //Променлива за отчитане на времето за изпълнение на сортировката
	do {                          //Меню за работа
		cout << "1.Create a file with random integers." << endl;
		cout << "2.Load the data into the queue." << endl;
		cout << "3.Print the queue." << endl;
		cout << "4.Sort the queue(quicksort-Ascending)" << endl;
		cout << "5.Sort the queue(quicksort-Descending)" << endl;
		cout << "6.Calculate the middle value of the queue." << endl;
		cout << "7.Search for the min. element bigger than the middle value." << endl;
		cout << "8.Delete" << endl;
		cout << "9.Save the results onto a file." << endl;
		cout << "10.Exit the program." << endl;
		cout << "Enter choice:";
		cin >> choice;
		switch (choice)
		{
		case 1: generate(); break;
		case 2: loadqueue(q); break;
		case 3: showqueue(q); break;
		case 4: quicksort(q,1);
			timer = clock() - timer; 
			cout << "Sorting time: " << (float(timer) / CLOCKS_PER_SEC) << "s" << endl;   //Представяне на таймера в секунди с неговото разделяне на броя тактове в секунда
			break;
		case 5: quicksort(q,0);
			timer = clock() - timer;
			cout << "Sorting time: " << (float(timer) / CLOCKS_PER_SEC) << "s" << endl;
			break;
		case 6: calc_mvalue(q); break;
		case 7: search(q); break;
		case 8: deleteq(q); break;
		case 9: savequeue(q); break;
		}
	} while (choice != 10);
}