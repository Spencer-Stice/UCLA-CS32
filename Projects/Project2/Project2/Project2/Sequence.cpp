#include "Sequence.h"

#include <iostream>
using namespace std;

Sequence::Sequence()	//construct a new Sequence with initial size 0, head and tail Node pointers are already set to nullptr
{
	m_size = 0;
};

bool Sequence::empty() const		//returns true if Sequence is empty (i.e. m_size == 0)
{
	return(m_size == 0);
}

int Sequence::size() const		//returns the size of the Sequence
{
	return m_size;
}

void Sequence::AddToFront(const ItemType& value)		//creates a new Node at the front with value
{
	Node* newNode = new Node;
	newNode->nodeValue = value;
	newNode->next = head;
	head->previous = newNode;
	head = newNode;
	m_size++;
}

void Sequence::AddToRear(const ItemType& value)		//creates a new Node at the front with value
{
	Node* newNode = new Node;
	newNode->nodeValue = value;
	tail->next = newNode;
	newNode->previous = tail;
	tail = newNode;
	m_size++;
}

int Sequence::insert(const ItemType& value)		//inserts value into the Sequence based on where it should be inserted
{


	int position = 1;
	if (head == nullptr)		//if the sequnce is empty
	{
		Node* newNode = new Node;
		newNode->nodeValue = value;
		head = newNode;
		tail = newNode;
		m_size++;
		return 0;
	}
	if (value <= head->nodeValue)		//if the sequnce is empty
	{
		AddToFront(value);
		return 0;
	}
	Node* p = head;
	while (p->next != nullptr)		//loop through the Sequence to find the node that is one above where the new value should be inserted
	{
		if (p->next->nodeValue >= value)
			break;
		p = p->next;
		position++;
	}
	if (p->next != nullptr)		//if the value right before where the new value should go is not the last part of the Sequence, insert the value as a Node into the sequence
	{
		Node* newNode = new Node;
		newNode->nodeValue = value;
		newNode->next = p->next;
		p->next->previous = newNode;
		p->next = newNode;
		newNode->previous = p;
		m_size++;
		return position;
	}
	if (p->next == nullptr)		//if the value right before where the new value should go is the last part of the sequence, insert the value as a Node at the end
		AddToRear(value);
	return position;
}

int Sequence::insert(int pos, const ItemType& value)	//inserts value as a Node into Sequence at pos, as long as pos is a valid position
{
	if (pos >= 0 && pos <= size())		//if pos is valid
	{
		if (head == nullptr)		//if the sequnce is empty
		{
			Node* newNode = new Node;
			newNode->nodeValue = value;
			head = newNode;
			tail = newNode;
			m_size++;
			return 0;
		}
		if (pos == 0)		//if pos is 0, add value to front
		{
			AddToFront(value);
			return 0;
		}
		if (pos == size())		//if pos is one larger than the current size, add value to rear
		{
			AddToRear(value);
			return pos;
		}
		Node* newNode = new Node;
		newNode->nodeValue = value;
		Node* p = head;		//if pos is somewhere in the middle, loop through the Sequence with a temporary Node* p until p points to the node right before where value should go
		for (int i = 1; i < pos; i++)
			p = p->next;	//insert value as a Node in the middle similar to the other insert function
		newNode->next = p->next;
		p->next->previous = newNode;
		p->next = newNode;
		newNode->previous = p;
		m_size++;
		return pos;
	}
	return -1;
}

bool Sequence::get(int pos, ItemType& value) const		//returns the value stored at position pos
{
	if (pos >= 0 && pos < size())		//if pos is valid
	{
		if (pos == 0)		//if pos is 0, return value of head
		{
			value = head->nodeValue;
			return true;
		}
		if (pos == size()-1)		//if pos is the last value, return it
		{
			value = tail->nodeValue;
			return true;
		}
		Node* p = head;		//if pos is somewhere in the middle, loop through the Sequence with a temporary Node* p until p points to the node at the desired position
		for (int i = 1; i <= pos; i++)
			p = p->next;
		value = p->nodeValue;
		return true;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value)		//sets the value of the Node at position pos to value, if it can
{
	if (pos >= 0 && pos < size())		//is pos is valid
	{
		Node* p = head;		//loop through the Sequence with a temporary Node* p until p points to the node at the desired position
		for (int i = 1; i <= pos; i++)
			p = p->next;
		p->nodeValue = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const	//returns the first position in the sequence where value is found
{
	Node* p = head;
	int position = 0;
	while (p != nullptr)			//loop through the sequence until p == nullptr
	{
		if (p->nodeValue == value)
			return position;		//if value found, return it
		p = p->next;
		position++;					//if value not found, move to next Node
	}
	return -1;
}

bool Sequence::erase(int pos)		//erases the Node at position pos and adjusts the pointers of surrounding Nodes
{
	if (pos >= 0 && pos < size())
	{
		if (size() == 0)
			return false;
		if (size() == 1)		//if the size is one
		{
			delete head;
			head = nullptr;
			tail = nullptr;
			m_size--;
			return true;
		}
		if (pos == 0)			//if the first Node is the one to be erased
		{
			Node* eraseMe = head;
			head = head->next;
			delete eraseMe;
			head->previous = nullptr;
			m_size--;
			return true;
		}
		if (pos == size()-1)		//if the last Node is the one to be erased
		{
			Node* eraseMe = tail;
			tail = tail->previous;
			delete eraseMe;
			tail->next = nullptr;
			m_size--;
			return true;
		}
		Node* p = head;		//loop through the Sequence with a temporary Node* p until p points to the node one before the desired position
		for (int i = 1; i < pos; i++)
			p = p->next;
		Node* eraseMe = p->next;
		Node* willBeNext = eraseMe->next;			//erase desired Node and adjust surrounding Nodes
		delete eraseMe;
		p->next = willBeNext;
		willBeNext->previous = p;
		m_size--;
		return true;
	}
	return false;
}

int Sequence::remove(const ItemType& value)		//removes all instances of value in the sequence
{
	int numDeleted = 0;
	int currentPosition = 0;
	Node* p = head;
	while (p!=nullptr)
	{
		if (p->nodeValue == value)
		{
			if (p->previous != nullptr)
			{
				p = p->previous;
				erase(currentPosition);		//for each node that has nodeValue of value, erase that node if this node isn't head
				currentPosition--;
				numDeleted++;
			}
			else
			{
				if (p->next != nullptr)		//for each node that has nodeValue of value, erase that node if this node isn't the only node
				{
					p = p->next;
					erase(0);
					numDeleted++;
				}
				else
				{
					erase(0);			//erase this node if it is the only one
					return (numDeleted + 1);
				}
			}
		}
		else {
			if (p != nullptr)		//if the current node doesn't need to be erased, move on to the next node
			{
				currentPosition++;
				p = p->next;
			}
			else
				return numDeleted;
		}
	}
	return numDeleted;
}

void Sequence::swap(Sequence& other)		//swaps this sequence with another
{
	Node* originalHead = head;
	Node* originalTail = tail;			//using temporary pointers and variables, swap the head and tail pointers and the sizes of the sequences
	head = other.head;
	tail = other.tail;
	other.head = originalHead;
	other.tail = originalTail;
	int tempSize = m_size;
	m_size = other.m_size;
	other.m_size = tempSize;
}

Sequence::~Sequence()		//destuctor removes all dynamically allocated memory upon program termination
{
	if (size() == 0)
	{
		return;
	}
	else if (size() == 1)		//if there is only one node, delete it
	{
		if(head != nullptr)
			delete head;
		head = nullptr;
		tail = nullptr;
		return;
	}
	else {
		Node* p = head;
		Node* p2;
		while (p != nullptr)		//if there are more than one node, delete all of them with a loop
		{
			p2 = p->next;
			delete p;
			p = p2;
		}
	}
}

Sequence::Sequence(const Sequence& src)		//copy constructor
{
	Node* previous = nullptr;
	ItemType toBeStored;
	for (int i = 0; i < src.size(); i++)		//for each node of the original sequence, create a new node that is a copy of a node in the original sequence
	{
		Node* newNode = new Node;
		if (i == 0)		//if this is the first node to be created, initialize head and tail to point to it
		{
			previous = newNode;
			head = newNode;
			tail = newNode;
			newNode->nodeValue = src.head->nodeValue;
			m_size++;
		}
		else
		{
			previous->next = newNode;		
			newNode->previous = previous;
			tail = newNode;
			src.get(i, toBeStored);
			newNode->nodeValue = toBeStored;
			previous = previous->next;
		}
	}
	m_size = src.size();
}

Sequence& Sequence::operator=(const Sequence& src)		//assignment operator
{
	if (this == &src)
		return *this;
	if (size() == 1)
	{
		delete head;
	}
	else {					//based on the size of the sequence, delete all inital elements in the first sequence
		Node* p = head;
		Node* p2;
		while (p != nullptr)
		{
			p2 = p->next;
			delete p;
			p = p2;
		}
	}
	m_size = src.size();
	Node* previous = nullptr;
	ItemType toBeStored;		
	for (int i = 0; i < src.size(); i++)		//similar to the copy constructor, create new nodes that are a copy of the old nodes
	{				
		Node* newNode = new Node;				//and store them in the new sequence
		if (i == 0)
		{
			previous = newNode;
			head = newNode;
			tail = newNode;
			newNode->nodeValue = src.head->nodeValue;
			m_size++;
		}
		else
		{
			previous->next = newNode;
			newNode->previous = previous;
			tail = newNode;
			src.get(i, toBeStored);
			newNode->nodeValue = toBeStored;
			previous = previous->next;
		}
	}
	m_size = src.size();
	if (size() == 0)
	{
		head = nullptr;
		tail = nullptr;
	}
	return *this;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)		//determines if seq2 is a subsequence of seq1
{
	ItemType seq1Value;
	ItemType seq2Value;
	if (seq1.size() == 0 || seq2.size() == 0)		//do nothing if either sequence has size 0
		return -1;
	seq2.get(0, seq2Value);
	ItemType startValue = seq2Value;			//take the first value of seq2 as starting value
	for (int i = 0; i < seq1.size(); i++)
	{
		seq1.get(i, seq1Value);
		if (seq1Value == startValue)			//if starting value is found in seq1
		{
			for (int j = 0; j < seq2.size(); j++)
			{
				bool isSub = true;
				if (j + i < seq1.size())			//loop through the next few nodes and each sequence to determine if a true subsequence exists
				{
					seq1.get(j+i, seq1Value);
					seq2.get(j, seq2Value);
					if (seq1Value != seq2Value)
						isSub = false;
					if (j == seq2.size() - 1 && isSub)
						return i;				//if a subsequence is found, return the position that starting value was found in seq1
				}
			}
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)	//stores a new sequence in result that is a combination of seq1 and seq2
{
	if (seq2.size() == 0 && seq1.size() == 0)	//if both inputs are empty, do nothing
		return;
	Sequence a;		//create a temporary sequence to store the results in
	if (seq1.size() == seq2.size())		//if the input sizes are the same
	{
		ItemType seq1V;
		ItemType seq2V;
		int nextResIndex = 0;
		int nextSIndex = 0;
		while (nextResIndex < (2 * seq2.size()))	//loop through both inputs and results, getting values from the inputs and inserting them into the result
		{
			seq1.get(nextSIndex, seq1V);
			seq2.get(nextSIndex, seq2V);
			a.insert(nextResIndex, seq1V);
			nextResIndex++;
			a.insert(nextResIndex, seq2V);
			nextResIndex++;
			nextSIndex++;
		}
	}
	else if (seq1.size() > seq2.size())		//if seq1 is larger than seq2
	{
		ItemType seq1V;
		ItemType seq2V;
		int nextResIndex = 0;
		int nextSIndex = 0;
		while (nextResIndex < (seq1.size() + seq2.size()))		//perform the same looping action as above
		{
			if (nextSIndex < seq2.size())		//only insert values from seq2 if there are more values left to insert from it
			{
				seq1.get(nextSIndex, seq1V);
				seq2.get(nextSIndex, seq2V);
				a.insert(nextResIndex, seq1V);
				nextResIndex++;
				a.insert(nextResIndex, seq2V);
				nextResIndex++;
				nextSIndex++;
			}
			else
			{
				seq1.get(nextSIndex, seq1V);
				a.insert(nextResIndex, seq1V);
				nextResIndex++;
				nextSIndex++;
			}
		}
	}
	else if (seq1.size() < seq2.size())//if seq2 is larger than seq1
	{
		ItemType seq1V;
		ItemType seq2V;
		int nextResIndex = 0;
		int nextSIndex = 0;
		while (nextResIndex < (seq1.size() + seq2.size()))//perform the same looping action as above
		{
			if (nextSIndex < seq1.size())//only insert values from seq2 if there are more values left to insert from it
			{
				seq1.get(nextSIndex, seq1V);
				seq2.get(nextSIndex, seq2V);
				a.insert(nextResIndex, seq1V);
				nextResIndex++;
				a.insert(nextResIndex, seq2V);
				nextResIndex++;
				nextSIndex++;
			}
			else
			{
				seq2.get(nextSIndex, seq2V);
				a.insert(nextResIndex, seq2V);
				nextResIndex++;
				nextSIndex++;
			}
		}
	}
	else if (seq2.size() == 0 && seq1.size() != 0)		//if one sequence is 0, only loop through that sequenec
	{
		ItemType seq1V;
		int nextResIndex = 0;
		int nextSIndex = 0;
		while (nextResIndex < seq1.size())
		{
			seq1.get(nextSIndex, seq1V);
			a.insert(nextResIndex, seq1V);
			nextResIndex++;
			nextSIndex++;
		}
	}
	else if (seq1.size() == 0 && seq2.size() != 0)//if one sequence is 0, only loop through that sequenece
	{
		ItemType seq2V;
		int nextResIndex = 0;
		int nextSIndex = 0;
		while (nextResIndex < seq2.size())
		{
			seq1.get(nextSIndex, seq2V);
			a.insert(nextResIndex, seq2V);
			nextResIndex++;
			nextSIndex++;
		}
	}
	result = a;		//store the results in the result sequence
}