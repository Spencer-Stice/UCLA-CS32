#ifndef History_H
#define History_H
#include "globals.h"
class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_rows, m_cols;
	int theRawRecord[MAXROWS][MAXCOLS];
};
#endif