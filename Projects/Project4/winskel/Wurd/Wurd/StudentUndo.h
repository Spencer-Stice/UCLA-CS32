#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	struct userAction		//Define a struct to store all user actions. The undo stack will be filled with these structs.
	{
		Action m_action;	//Stores the action to be undone
		int m_r;
		int m_c;
		int m_char;
		bool sRdC = false;		//Used for batching
		bool sRsC = false;		//Used for batching
		bool insertChain = false;	//Used for batching
	};
	stack<userAction> undoList;
};

#endif // STUDENTUNDO_H_
