#pragma once

#include <iostream>
#include <iomanip>
using namespace std;

namespace LinkHashTable
{
enum Result
{
	OK,
	FOUND,
	NOT_FOUND,
	OUT_OF_BOUNDARY,
	BAD_HASH_FUNCTION,
};

template <class DataType>
class HashTable;

template <class DataType>
class HashNode
{
private:
	friend class HashTable<DataType>;
	DataType data;
	HashNode<DataType> *next = nullptr;
	HashNode(void);
	HashNode(const DataType &Data, HashNode<DataType> *Next);
	~HashNode();
};

template <class DataType>
HashNode<DataType>::HashNode(void) : data(0), next(nullptr) {}

template <class DataType>
HashNode<DataType>::HashNode(const DataType &Data, HashNode<DataType> *Next)
{
	this->data = Data;
	this->next = Next;
}

template <class DataType>
HashNode<DataType>::~HashNode()
{
	// cout << "free " << this->data << endl;
}

template <class DataType>
class HashTable
{
private:
	HashNode<DataType> **table = nullptr;
	int max_size = 0;
	int size = 0;
	int (*hash_func)(DataType &Data) = nullptr;
	void push_back(const int &idx, const DataType &Data);
	void remove_all(const int &idx, const DataType &Data);

public:
	HashTable(const int Max, int (*HashFunction)(DataType &Data), const DataType Default);
	~HashTable();

	Result insert(DataType Data);
	Result search(DataType Data) const;
	Result remove(DataType Data);

	void print_Table() const;
	void print_Table_with_Target(DataType Target) const;
};

template <class DataType>
HashTable<DataType>::HashTable(const int Max, int (*HashFunction)(DataType &Data), const DataType Default)
{
	this->hash_func = HashFunction;
	this->max_size = Max;
	this->table = new HashNode<DataType> *[this->max_size];
	if (this->table == nullptr)
	{
		this->max_size = 0;
	}
	else
	{
		for (int i = 0; i < this->max_size; i++)
		{
			this->table[i] = new HashNode<DataType>(Default, nullptr);
		}
	}
}

template <class DataType>
HashTable<DataType>::~HashTable()
{
	if (this->table != nullptr)
	{
		for (int i = 0; i < this->max_size; i++)
		{
			for (HashNode<DataType> *p = this->table[i]; p != nullptr;)
			{
				HashNode<DataType> *n = p;
				p = p->next;
				delete n;
			}
		}
		delete[] this->table;
	}
}

template <class DataType>
Result HashTable<DataType>::insert(DataType Data)
{
	int idx = this->hash_func(Data);
	if (idx < 0 || idx >= this->max_size)
	{
		return BAD_HASH_FUNCTION;
	}
	else
	{
		this->push_back(idx, Data);
		return OK;
	}
}

template <class DataType>
void HashTable<DataType>::push_back(const int &idx, const DataType &Data)
{
	this->table[idx]->next = new HashNode<DataType>(Data, this->table[idx]->next);
}

template <class DataType>
Result HashTable<DataType>::search(DataType Data) const
{
	int idx = this->hash_func(Data);
	if (idx < 0 || idx >= this->max_size)
	{
		return BAD_HASH_FUNCTION;
	}
	else
	{
		for (HashNode<DataType> *p = this->table[idx]->next; p != nullptr; p = p->next)
		{
			if (p->data == Data)
			{
				return FOUND;
			}
		}
		return NOT_FOUND;
	}
}

template <class DataType>
Result HashTable<DataType>::remove(DataType Data)
{
	int idx = this->hash_func(Data);
	if (idx < 0 || idx >= this->max_size)
	{
		return BAD_HASH_FUNCTION;
	}
	else
	{
		this->remove_all(idx, Data);
		return OK;
	}
}

template <class DataType>
void HashTable<DataType>::remove_all(const int &idx, const DataType &Data)
{
	HashNode<DataType> *front = this->table[idx];
	HashNode<DataType> *current = this->table[idx]->next;
	while (current != nullptr)
	{
		HashNode<DataType> *next = current->next;
		if (current->data == Data)
		{
			front->next = next;
			delete current;
		}
		else
		{
			front = current;
		}
		current = next;
	}
}

template <class DataType>
void HashTable<DataType>::print_Table() const
{
	for (int i = 0; i < this->max_size; i++)
	{
		cout << "[" << setw(3) << i << "]";
		if (this->table[i]->next == nullptr)
		{
			cout << " -> *" << endl;
		}
		else
		{
			for (HashNode<DataType> *p = this->table[i]->next; p != nullptr; p = p->next)
			{
				cout << " -> " << p->data;
			}
			cout << endl;
		}
	}
}

template <class DataType>
void HashTable<DataType>::print_Table_with_Target(DataType Target) const
{
	for (int i = 0; i < this->max_size; i++)
	{
		cout << "[" << setw(3) << i << "]";
		if (this->table[i]->next == nullptr)
		{
			cout << " -> *" << endl;
		}
		else
		{
			for (HashNode<DataType> *p = this->table[i]->next; p != nullptr; p = p->next)
			{
				if (p->data == Target)
				{
					cout << " -> ( ";
					cout << p->data << " )";
				}
				else
				{
					cout << " -> " << p->data;
				}
			}
			cout << endl;
		}
	}
}

}; // namespace LinkHashTable
