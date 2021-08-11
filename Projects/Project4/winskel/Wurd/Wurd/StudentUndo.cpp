#include "StudentUndo.h"

//Declare constants
const Undo::Action INSERT = Undo::Action::INSERT;
const Undo::Action DELETE = Undo::Action::DELETE;
const Undo::Action JOIN = Undo::Action::JOIN;
const Undo::Action SPLIT = Undo::Action::SPLIT;


Undo* createUndo()
{
	return new StudentUndo;
}


void StudentUndo::submit(const Action action, int row, int col, char ch) {		//Submits an action to the undo stack
	if (ch == '\t')					//Treat tabs as 4 separate space actions
	{
		for (int i = 0; i < 4; i++)
		{
			userAction newAction;
			newAction.m_action = action;
			newAction.m_r = row;
			newAction.m_c = col - 3 + i;
			newAction.m_char = ' ';
			undoList.push(newAction);
		}
		return;
	}
	userAction newAction;				//Creates a new userAction struct to store all necessary info about an action
	newAction.m_action = action;
	newAction.m_r = row;
	newAction.m_c = col;
	newAction.m_char = ch;
	if (undoList.empty() == false)		//If this is not the first action
	{
		int lastR = undoList.top().m_r;		//Store the previous action's row and column for batching
		int lastC = undoList.top().m_c;
		if (action == DELETE && undoList.top().m_action == DELETE && lastR == row && lastC == col + 1)	//If backspace batching should occur, set the proper values here
		{
			newAction.sRdC = true;
			undoList.top().sRdC = true;

		}
		if (action == DELETE && undoList.top().m_action == DELETE && lastR == row && lastC == col)	//If deletion batching should occur, set the proper values here
		{
			newAction.sRsC = true;
			undoList.top().sRsC = true;

		}
		if (action == INSERT && undoList.top().m_action == INSERT && lastR == row && lastC == col - 1 && ch != ' ')	//If insertion batching should occur, set the proper values here
		{
			newAction.insertChain = true;
			if(undoList.top().m_char != ' ')		//Make sure that spaces are never a part of insertion batching
				undoList.top().insertChain = true;
		}

	}
	undoList.push(newAction);			//Add the new action to the top of the undo stack
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) {	//Prepares the most recent action for the editor. Applies batching if necessary 
	if (undoList.empty() == true)
		return Undo::Action::ERROR;
	userAction currentuAction = undoList.top();
	undoList.pop();
	Action currentAction = currentuAction.m_action;
	Action returnAction;
	int returnRow;
	int returnCol;								//Remove the top action and extract all relevant info. Remove this action from the undo stack

	switch (currentAction)
	{
	case(INSERT):							
	{
		returnAction = DELETE;					//Return the proper action and give the proper row and column
		row = currentuAction.m_r;
		col = currentuAction.m_c - 1;
		count = 1;
		while (undoList.empty() == false && undoList.top().insertChain)		//While batching should occur, prepare the editor with necessary info
		{
			count++;
			row = undoList.top().m_r;
			col = undoList.top().m_c - 1;
			undoList.pop();							//Always remove the top action if it is used in a batch
		}
		break;
	}
	case(DELETE):
	{
		returnAction = INSERT;
		row = currentuAction.m_r;					//Return the proper action and give the proper row and column
		col = currentuAction.m_c;
		count = 1;
		text = currentuAction.m_char;
		bool backSpace = false;				//Used to prevent deletion and backspace from being batched together
		while (undoList.empty() == false && undoList.top().sRdC)	//While batching should occur, prepare the editor with necessary info. This is used for backspace
		{
			count++;
			text += undoList.top().m_char;
			undoList.pop();			//Always remove the top action if it is used in a batch
			backSpace = true;
		}
		if (backSpace)
			break;
		while (undoList.empty() == false && undoList.top().sRsC)		//This is used for deletions
		{
			count++;
			string s(1, undoList.top().m_char);
			text = s + text;
			undoList.pop();
		}
		break;
	}
	case(SPLIT):		//If the initial action was to split, join to undo
	{
		returnAction = JOIN;
		row = currentuAction.m_r;
		col = currentuAction.m_c;			//No batching necessary here
		break;
	}
	case(JOIN):
	{
		returnAction = SPLIT;			//If the initial action was to join, split to undo
		row = currentuAction.m_r;
		col = currentuAction.m_c;		//No batching necessary
		break;
	}
	}
	if (returnAction != DELETE)		//If count and text weren't set from batching, set them here
		count = 1;
	if (returnAction != INSERT)
		text = "";
	return returnAction;
}

void StudentUndo::clear() {		//remove all current undo actions from the stack
	while (!undoList.empty())
		undoList.pop();
}