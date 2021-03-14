#pragma once
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <intrin.h>
#include <string>

using namespace std;

enum STATUS {
	SUCCESS = 0,
	FAIL = 0xffffffff,
};

enum EMER_STATUS {
	OFF = 0,
	TRIGGER = 1,
	ON = 2
};

typedef struct {
	int size;
	char data[1];
} str;


EMER_STATUS emer_mode = OFF;

class FixedBufBase 
{
public:
	FixedBufBase(string& d) :desc(d) {}
	FixedBufBase(const char* d) :desc(d){}
	virtual ~FixedBufBase() {};
	virtual void select() = 0;
	virtual void resize(int new_size) = 0;
	friend ostream& operator<<(ostream& out, FixedBufBase& arr)
	{
		out << "Node(" << arr.desc << ")";
		return out;
	}
	string& getDesc() { return this->desc; }
private:
	string desc;
};

#define FIXED_SIZE 16
template <class DataType>
class FixedBufGrow : public FixedBufBase
{
public:
	FixedBufGrow(int data_size,string &d) 
		: m_pData(NULL), size(data_size), FixedBufBase(d)
	{
		if (data_size > FIXED_SIZE) {
			m_pData = (DataType*)calloc(sizeof(DataType), data_size);
		}
		else
			memset(m_Data, 0, sizeof(m_Data));
	}
	FixedBufGrow(int data_size, const char *d)
		: m_pData(NULL), size(data_size), FixedBufBase(d)
	{
		if (data_size > FIXED_SIZE) {
			m_pData = (DataType*)calloc(sizeof(DataType), data_size);
		}
		else
			memset(m_Data, 0, sizeof(m_Data));
	}
	~FixedBufGrow()
	{
		if (m_pData) {
			delete[] m_pData;
		}
	}
	void resize(int new_size) 
	{
		if (m_pData) {
			delete[] m_pData;
		}
		if (new_size > FIXED_SIZE) {
			m_pData = (DataType*)calloc(sizeof(DataType), new_size);
		}
		else
			memset(m_Data, 0, sizeof(m_Data));
		size = new_size;
	}

	DataType& operator[](int index) 
	{
		if (m_pData) return m_pData[index];
		else return m_Data[index];
	}

	void select();
	void add(int index) { cout << "Not support!" << endl; }
	void dele(int index);
	void edit(int index) { cout << "Not support!" << endl; }
	void show(int index) { cout << (*this)[index] << endl; }
	int get_size() { return size; };
private:
	DataType m_Data[FIXED_SIZE];
	DataType* m_pData;
	int size;
};

template <class DataType>
void FixedBufGrow<DataType>::select()
{
	FixedBufGrow<DataType>& obj = *this;
	while (1) 
	{
		cout << "Current: " << this->getDesc() << endl;
		cout << "Index(-1 for back to upper): ";
		int index;
		cin >> index;
		if (index == -1) return;
		if (index < 0 || index >= size)
		{
			cout << "Index out of range!" << endl;
			return;
		}
		if (typeid(DataType) == typeid(FixedBufBase*) && *(FixedBufBase**)(&obj[index]) == NULL)
		{
			if (emer_mode == ON) return;
			else if (emer_mode == TRIGGER) emer_mode = ON;
			cout << "No data here,create new one?(y/else)" << endl;
			char ch;
			cin >> ch;
			if (ch != 'y') return;
			obj.add(index);
			//(*(FixedBufBase**)(&obj[index]))->select();
		}
		else
		{
			bool cont = true;
			while (cont)
			{
				cout << "1. Delete" << endl;
				cout << "2. Edit" << endl;
				cout << "3. Show" << endl;
				cout << "4. Select" << endl;
				cout << "5. Cancel" << endl;
				cout << "What do you want to do with it: ";
				int choice;
				cin >> choice;
				switch (choice)
				{
				case 1:
					obj.dele(index);
					cont = false;
					break;
				case 2:
					obj.edit(index);
					break;
				case 3:
					obj.show(index);
					break;
				case 4:
					if (typeid(DataType) != typeid(FixedBufBase*)) cout << "Only Object array could be selected." << endl;
					else 
					{
						(*(FixedBufBase**)(&obj[index]))->select();
						cont = false;
					}
					break;
				case 5:
					cont = false;
					break;
				default:
					cout << "Index out of range!" << endl;
				}
			}
		}
	}
}

template <class DataType>
void FixedBufGrow<DataType>::dele(int index)
{
	FixedBufGrow<DataType>& obj = *this;
	DataType data = 0;
	obj[index] = data;
	cout << "Delete success!" << endl;
}

template <>
void FixedBufGrow<int>::edit(int index)
{
	FixedBufGrow<int>& obj = *this;
	cout << "Support operator(+,-,*,/) e.g. +123" << endl;
	cout << "Input expression: ";
	char op;
	int op2;
	cin >> op >> op2;
	switch (op)
	{
	case '+':
		obj[index] += op2;
		break;
	case '-':
		obj[index] -= op2;
		break;
	case '*':
		obj[index] *= op2;
		break;
	case '/':
		if (op2 != 0)
			obj[index] /= op2;
		else 
		{
			cout << "div 0 error!" << endl;
			return;
		}
		break;
	default:
		cout << "Invaild operator!" << endl;
		return;
	}
	cout << "Edit success!" << endl;
}

template <>
void FixedBufGrow<float>::edit(int index)
{
	FixedBufGrow<float>& obj = *this;
	cout << "Support operator(+,-,*,/) e.g. +123" << endl;
	cout << "Input expression: ";
	char op;
	float op2;
	cin >> op >> op2;
	switch (op)
	{
	case '+':
		obj[index] += op2;
		break;
	case '-':
		obj[index] -= op2;
		break;
	case '*':
		obj[index] *= op2;
		break;
	case '/':
		if (op2 != 0.0)
			obj[index] /= op2;
		else
		{
			cout << "div 0 error!" << endl;
			return;
		}
		break;
	default:
		cout << "Invaild operator!" << endl;
		return;
	}
	cout << "Edit success!" << endl;
}

template <>
void FixedBufGrow<FixedBufBase*>::edit(int index)
{
	FixedBufGrow<FixedBufBase*>& obj = *this;
	cout << "WARNING:Modifying the arraysize will cause data loss,continue?(y/else)" << endl;
	char ch;
	cin >> ch;
	if (ch != 'y') return;
	cout << "New size(beta feature,new_size max value: "<< FIXED_SIZE <<"): ";
	int size;
	cin >> size;
	if (size <= 0 || size > FIXED_SIZE)
	{
		cout << "Index out of range!" << endl;
		return;
	}
	obj[index]->resize(size);
	cout << "Edit success!" << endl;
	if (emer_mode == OFF)
	{
		emer_mode = TRIGGER;
		cout << "Error detected,the system will enter emergency mode shortly!!!" << endl;
	}
}

template <>
void FixedBufGrow<str*>::edit(int index)
{
	FixedBufGrow<str*>& obj = *this;
	cout << "Input size: ";
	int size;
	cin >> size;
	if ( size < 0 || size > 16380)
	{
		cout << "Size max value is 16380!" << endl;
		return;
	}
	str* s = obj[index];
	if (s != NULL && s->size < size)
	{
		free(s);
		obj[index] = NULL;
		s = (str*)calloc(1, size + sizeof(int));
	}
	else if (s == NULL)
	{
		s = (str*)calloc(1, size + sizeof(int));
	}
	s->size = size;
	cout << "Contents: ";
	cin.ignore(0x1000,'\n');
	cin.getline(s->data, s->size <= 16380 ? s->size : 16380);
	obj[index] = s;
	cout << "Edit success!" << endl;
}

template<>
void FixedBufGrow<FixedBufBase*>::add(int index)
{
	FixedBufGrow<FixedBufBase*>& obj = *this;
	cout << "1. Object" << endl;
	cout << "2. Integer" << endl;
	cout << "3. Float Number" << endl;
	cout << "4. String" << endl;
	cout << "Select the type of new array: ";
	int choice;
	cin >> choice;
	if (choice < 1 || choice > 4)
	{
		cout << "Index out of range!" << endl;
		return;
	}
	int size;
	cout << "Array size: ";
	cin >> size;
	if (size <= 0 || size > 0x1000)
	{
		cout << "Index out of range!" << endl;
		return;
	}
	cout << "Description: ";
	string des;
	cin >> des;
	cout << "Repeat until non-empty one?(y/else)" << endl;
	bool repeat = false;
	char ch;
	cin >> ch;
	if (ch == 'y') repeat = true;
	for (;index < obj.get_size() && *(FixedBufBase**)(&obj[index]) == NULL;index++)
	{
		switch (choice)
		{
		case 1:
			obj[index] = new FixedBufGrow<FixedBufBase*>(size, des);
			break;
		case 2:
			obj[index] = new FixedBufGrow<int>(size, des);
			break;
		case 3:
			obj[index] = new FixedBufGrow<float>(size, des);
			break;
		case 4:
			obj[index] = new FixedBufGrow<str*>(size, des);
			break;
		}
		if (repeat == false) break;
	}
	cout << "Add success!" << endl;
}

template<>
void FixedBufGrow<FixedBufBase*>::dele(int index)
{
	FixedBufGrow<FixedBufBase*>& obj = *this;
	delete obj[index];
	obj[index] = NULL;
	cout << "Delete success!" << endl;
}

template <>
void FixedBufGrow<str*>::dele(int index)
{
	FixedBufGrow<str*>& obj = *this;
	free(obj[index]);
	obj[index] = NULL;
	cout << "Delete success!" << endl;
}


template<>
void FixedBufGrow<FixedBufBase*>::show(int index) {	cout << *(*this)[index] << endl;}

template<>
void FixedBufGrow<str*>::show(int index) { cout << (*this)[index]->data << endl; }