
#include "Sequence.h"

Sequence::Sequence()
{
	m_size = 0;
	maxSize = DEFAULT_MAX_ITEMS;
}// Create an empty sequence (i.e., one with no items)

bool Sequence::empty() const
{
	if (m_size == 0)
		return true;
	return false;
}// Return true if the sequence is empty, otherwise false.

int Sequence::size() const
{
	return m_size;
}// Return the number of items in the sequence.


int Sequence::insert(int pos, const ItemType& value)
{
	if (m_size < DEFAULT_MAX_ITEMS && pos >= 0 && pos <= m_size)
	{
		if (pos == m_size) {
			m_theSequence[pos] = value;
			m_size++;
			return pos;
		}
		else {
		for (int i = m_size - 1; i >= pos; i--)
			m_theSequence[i + 1] = m_theSequence[i];
		m_theSequence[pos] = value;
		m_size++;
		return pos;
		}
	}
	return -1;
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return pos if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// e.g., because it's implemented using a fixed-size array.)  Otherwise,
// leave the sequence unchanged and return -1.  Notice that
// if pos is equal to size(), the value is inserted at the end.

int Sequence::insert(const ItemType& value)
{
	int p = 0;
	if (m_size < DEFAULT_MAX_ITEMS && m_size != 0)
	{
		for (; p <= size(); p++)
		{
			if (p != size() && value <= m_theSequence[p])
			{
				for (int i = m_size - 1; i >= p; i--)
					m_theSequence[i + 1] = m_theSequence[i];
				m_theSequence[p] = value;
				m_size++;
				return p;
			}
			else if (p == m_size) {
				m_theSequence[m_size] = value;
				m_size++;
				return m_size;
			}
		}
	}
	else if (m_size < DEFAULT_MAX_ITEMS)
	{
		m_theSequence[0] = value;
		m_size++;
		return 0;
	}
		return -1;
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item in position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos)
{
	if (pos >= 0 && pos < size())
	{
		for (int i = pos; i < size(); i++)
			m_theSequence[i] = m_theSequence[i + 1];
		m_size--;
		return true;
	}
	return false;
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed that item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value)
{
	int i = 0;
	int totalRemoved = 0;
	while (i < size())
	{
		if (m_theSequence[i] == value)
		{
			erase(i);
			totalRemoved++;
		}
		else
			i++;
	}
	return totalRemoved;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos >= 0 && pos < size())
	{
		value = m_theSequence[pos];
		return true;
	}
	return false;
}
// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos >= 0 && pos < size())
	{
		m_theSequence[pos] = value;
		return true;
	}
	return false;
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
	for (int i = 0; i < size(); i++)
		if (value == m_theSequence[i])
			return i;
	return -1;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
	int savedSize = size();
	int otherSavedSize = other.size();
	ItemType tempy;
	ItemType temp[DEFAULT_MAX_ITEMS];
	for (int i = 0; i < size(); i++)
		temp[i] = m_theSequence[i];
	for (int i = 0; i < other.size(); i++)
	{
		m_theSequence[i] = other.m_theSequence[i];
	}
	if (savedSize > other.size())
		for (int i = other.size(); i < savedSize; i++)
			erase(other.size());
	for (int i = 0; i < savedSize; i++)
	{
		other.m_theSequence[i] = temp[i];
	}
	if(savedSize < otherSavedSize)
		for (int i = savedSize; i < otherSavedSize; i++)
			erase(savedSize);
	m_size = otherSavedSize;
	other.m_size = savedSize;
}
// Exchange the contents of this sequence with the other one.
