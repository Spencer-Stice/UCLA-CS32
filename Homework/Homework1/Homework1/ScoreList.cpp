
#include "Sequence.h"
#include "ScoreList.h"
ScoreList::ScoreList()
{}

bool ScoreList::add(unsigned long score)
{
	if (score >= 0 && score <= 100)
	{
		if(m_sequence.insert(score))
			return true;
	}
	return false;
}
// If the score is valid (a value from 0 to 100) and the score list
// has room for it, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.

bool ScoreList::remove(unsigned long score)
{
	int pos = m_sequence.find(score);
	if (pos != -1 && m_sequence.erase(pos))
		return true;
	return false;
}
// Remove one instance of the specified score from the score list.
// Return true if a score was removed; otherwise false.

int ScoreList::size() const
{
	return m_sequence.size();
}// Return the number of scores in the list.

unsigned long ScoreList::minimum() const
{
	unsigned long currentLow;
	unsigned long next;
	if (!m_sequence.get(0, currentLow))
		return NO_SCORE;
	for (int i = 1; i < m_sequence.size(); i++)
	{
		m_sequence.get(i, next);
		if (next < currentLow)
			currentLow = next;
	}
	return currentLow;
}
// Return the lowest-valued score in the score list.  If the list is
// empty, return NO_SCORE.

unsigned long ScoreList::maximum() const
{
	unsigned long currentHigh;
	unsigned long next;
	if (!m_sequence.get(0, currentHigh))
		return NO_SCORE;
	for (int i = 1; i < m_sequence.size(); i++)
	{
		m_sequence.get(i, next);
		if (next > currentHigh)
			currentHigh = next;
	}
	return currentHigh;
}
// Return the highest-valued score in the score list.  If the list is
// empty, return NO_SCORE.
