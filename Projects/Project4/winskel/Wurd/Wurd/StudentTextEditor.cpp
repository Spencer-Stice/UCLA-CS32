#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

//Declare constants
const Undo::Action INSERT = Undo::Action::INSERT;
const Undo::Action DELETE = Undo::Action::DELETE;
const Undo::Action JOIN = Undo::Action::JOIN;
const Undo::Action SPLIT = Undo::Action::SPLIT;

//Declare helper functions
void StudentTextEditor::submitAction(Undo::Action a, int r, int c, char cha)
{
	getUndo()->submit(a, r, c, cha);
}


TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)		//Initialize all private data members. Runs in constant time.
 : TextEditor(undo) {
	m_r = m_c = 0;
	string firstLine = "";
	editingLines.push_back(firstLine);					//Constant time because editingLines will initially be empty
	m_cLinePtr = editingLines.begin();
	m_numLines = 1;
}

StudentTextEditor::~StudentTextEditor()					//Free all dynamically allocated memory
{
	m_numLines = 0;
	editingLines.clear();
}

bool StudentTextEditor::load(std::string file) {		//Processes the given file into the text editor. Process line by line, which is more efficient or equally efficient as the spec's requirement
	reset();											//Clear the editor
	ifstream infile(file);
	if (!infile)
	{
		return false;
	}
	string cReadingLine;
	list<string>::iterator it;
	while (getline(infile, cReadingLine))					//Continue reading lines until the end of the file
	{
		string fReadingLine = "";
		for (int j = 0; j < cReadingLine.length(); j++)
		{
			if (cReadingLine[j] != '\r')
			{
				fReadingLine += cReadingLine[j];			//Strip \r characters, if any
			}
		}
		editingLines.push_back(fReadingLine);				//Insert new lines into the editor
		if (editingLines.size() == 1)
			it = editingLines.begin();
		else
			it++;
		m_numLines++;
	}
	m_cLinePtr = editingLines.begin();						//Reset cursor position
	m_r = 0;
	m_c = 0;
	
	return true;
	
}

bool StudentTextEditor::save(std::string file) {		//Saves the current document
	ofstream outfile(file);
	if (!outfile)
	{
		return false;
	}
	list<string>::iterator it = editingLines.begin();
	while (it != editingLines.end())					//Save line by line which is equally efficient or more efficient than the spec requires
	{

		string toWrite = (*it);				//Newline character is already appended; my code saves like the sample does
		outfile << toWrite;
		it++;
	}
	
	return true;
}

void StudentTextEditor::reset() {						//Clears all lines
	
	list<string>::iterator it = editingLines.begin();
	m_cLinePtr = it;
	while (it != editingLines.end())
	{
		it = editingLines.erase(it);
		m_numLines--;
	}
	m_cLinePtr = editingLines.begin();
	m_r = m_c = 0;
	
	getUndo()->clear();							//Calls clear on undo, which clears the undo stack
}

void StudentTextEditor::move(Dir dir) {		//Moves the cursor in the given direction, constant time
	
	switch (dir)
	{
	case(UP):									//Moves up if possible, sets cursor to one position after the upper line if necessary
	{
		if (m_r == 0)
			return;
		m_r--;
		m_cLinePtr--;
		if (m_c > m_cLinePtr->size())
			m_c = m_cLinePtr->size();
		break;
	}
	case(DOWN):									//Moves down if possible, sets cursor to one position after the lower line if necessary
	{
		if (m_r + 1 == m_numLines)
			return;
		m_r++;
		m_cLinePtr++;
		if (m_c > m_cLinePtr->size())
			m_c = m_cLinePtr->size();
		break;
	}
	case(LEFT):										//Moves left if possible, sets cursor to one position after the upper line if necessary
	{
		if (m_r == 0 && m_c == 0)
		{
			return;
		}
		if (m_c == 0)
		{
			m_r--;
			list<string>::iterator useful = m_cLinePtr;
			useful--;
			m_c = (useful)->length();
			m_cLinePtr = useful;
			return;
		}
		m_c--;
		break;
	}
	case(RIGHT):									//Moves right if possible, sets cursor to beginning of lower line if necessary
	{
		if (m_r +1 == m_numLines && m_c == (*m_cLinePtr).length())
		{
			return;
		}
		if (m_c >= (*m_cLinePtr).length())
		{
			m_r++;
			m_c = 0;
			m_cLinePtr++;
			return;
		}
		m_c++;
		break;
	}
	case(HOME):
	{
		m_r = 0;
		m_c = 0;
		m_cLinePtr = editingLines.begin();
		break;
	}
	case(END):
	{
		m_c = m_cLinePtr->length();
		break;
	}
	}
	
}

void StudentTextEditor::del() {					//Deletes the character as specified by the spec
	
	list<string>::iterator testEnd = editingLines.end();
	testEnd--;
	if (m_c == (m_cLinePtr)->length() && m_cLinePtr == testEnd)
	{
		return;
	}
	testEnd = m_cLinePtr;
	testEnd++;
	if (m_c == (m_cLinePtr)->length())			//Handle the special case where a merge of lines must take place
	{
		string nextLine = (*testEnd);
		(*m_cLinePtr) += nextLine;
		editingLines.erase(testEnd);
		m_numLines--;
		submitAction(JOIN, m_r, m_c, '1');
		return;
	}
	string thisLine = (*m_cLinePtr);
	char justDeleted = thisLine[m_c];
	(m_cLinePtr)->erase(m_c, 1);				//Remove the character
	submitAction(DELETE, m_r, m_c, justDeleted);	//Submit this action as a deletion to the undo stack
	
}

void StudentTextEditor::backspace() {			//Performs a backspace operation as specified by the spec
	
	if (m_r == 0 && m_c == 0)
		return;
	if (m_c == 0 || (m_cLinePtr)->length() == 0 && m_r > 0)		//Special case is handled for backspace
	{
		list<string>::iterator useful = m_cLinePtr;
		useful--;
		string temp = (*m_cLinePtr);
		int oldLength = (useful)->length();
		(*useful) += temp;
		m_r--;
		m_c = oldLength;
		editingLines.erase(m_cLinePtr);
		m_cLinePtr = useful;
		m_numLines--;
		submitAction(JOIN, m_r, m_c, '2');
		return;
	}
	string thisLine = (*m_cLinePtr);
	char justDeleted = thisLine[m_c - 1];
	(m_cLinePtr)->erase(m_c - 1, 1);			//Removes the character
	m_c--;
	submitAction(DELETE, m_r, m_c, justDeleted);		//Submits the action as a delete action to the undo stack
}

void StudentTextEditor::insert(char ch) {					//Inserts a character as specified by spec. Time complexity is O(L), where is the the length of the line because searching a hash table can be considered constant time complexity.
	if (ch == '\t')
	{
		string insertMe = "    ";
		m_cLinePtr->insert(m_c, insertMe);
		m_c += 4;
		submitAction(INSERT, m_r, m_c, ch);
		return;
	}
	else {
		string insertMe(1, ch);
		m_cLinePtr->insert(m_c, insertMe);
	}
	m_c++;										//Note: use of the private data member that is an iterator allows this function to not depend on the number of lines edited
	//Talk to undo
	submitAction(INSERT, m_r, m_c, ch);
}

void StudentTextEditor::enter() {					//Performs an enter based on the spec
	list<string>::iterator tempo = m_cLinePtr;
	string temp = (*m_cLinePtr);
	string firstPart = temp.substr(0, m_c);			//Splits the line in half
	string secondPart = temp.substr(m_c);
	(*m_cLinePtr) = firstPart;
	m_cLinePtr++;
	editingLines.insert(m_cLinePtr, secondPart);
	tempo++;
	m_cLinePtr = tempo;
	submitAction(SPLIT, m_r, m_c, '1');					//Submits this action as a split action to the undo stack
	m_r++;
	m_c = 0;
	m_numLines++;
	
}

void StudentTextEditor::getPos(int& row, int& col) const {		//Returns the current cursor position based on the private data members
	row = m_r;
	col = m_c;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {		//Returns the lines requested in the given vector
	if (startRow < 0 && numRows < 0 || startRow > m_numLines)
		return 1;
	int toRet = 0;
	list<string>::const_iterator it;
	it = editingLines.begin();
	for (int i = 0; i < startRow; i++)
		it++;
	for (int i = 0; i < numRows; i++)
	{
		if (it != editingLines.end())				//For each line requested, add the line to the liens vector
		{
			lines.push_back(*it);
			toRet++;
			it++;
		}
	}
	return toRet;

}

void StudentTextEditor::undo() {					//Handles the undo operations based on the actions in the undo stack
	int n_row, n_col, n_count;
	string undoString;
	Undo::Action toUndo = getUndo()->get(n_row, n_col, n_count, undoString);	//First, retrieves the next action to undo 
	switch (toUndo)
	{
	case(INSERT):
	{
		if (m_r > n_row)						//Move the current line pointer data member to the line where the undo action will occur; this part is common to all undo actions
			while (m_r != n_row)
			{
				m_cLinePtr--;
				m_r--;
			}
		else
			while (m_r != n_row)
			{
				m_cLinePtr++;
				m_r++;
			}
		m_r = n_row;
		m_c = n_col;
		int length = undoString.length();
		for (int i = length - 1; i > -1; i--)		//Insert the undo string back into the line in such a way that it will end up back where it used to be
		{
			string s(1, undoString[i]);
			m_cLinePtr->insert(m_c, s);
		}
		break;
	}
	case(DELETE):
	{
		if (m_r > n_row)
			while (m_r != n_row)
			{
				m_cLinePtr--;
				m_r--;
			}
		else
			while (m_r != n_row)
			{
				m_cLinePtr++;
				m_r++;
			}
		m_r = n_row;
		m_c = n_col;
		for (int i = 0; i < n_count; i++)				//For each of the requested deletion times, delete the correct character from the string based on the given location
		{
			list<string>::iterator testEnd = editingLines.end();
			testEnd--;
			testEnd = m_cLinePtr;
			testEnd++;
			string thisLine = (*m_cLinePtr);
			char justDeleted = thisLine[m_c];
			(m_cLinePtr)->erase(m_c, 1);				//Essesntially, this function copies the delete function without submitting an undo action
		}
		if(m_c > 0)
			m_c--;
		break;
	}
	case(SPLIT):
	{
		if (m_r > n_row)
			while (m_r != n_row)
			{
				m_cLinePtr--;
				m_r--;
			}
		else
			while (m_r != n_row)
			{
				m_cLinePtr++;
				m_r++;
			}
		m_c = n_col;
		list<string>::iterator tempo = m_cLinePtr;
		string temp = (*m_cLinePtr);
		string firstPart = temp.substr(0, m_c);				//Essentially, this function copies the enter function without submitting a new delete action; see enter function comments
		string secondPart = temp.substr(m_c);
		(*m_cLinePtr) = firstPart;
		m_cLinePtr++;
		editingLines.insert(m_cLinePtr, secondPart);
		m_cLinePtr = tempo;
		m_r = n_row;
		m_numLines++;
		break;
	}
	case(JOIN):
	{
		if(m_r > n_row)
			while (m_r != n_row)
			{
				m_cLinePtr--;
				m_r--;
			}
		else
			while (m_r != n_row)
			{
				m_cLinePtr++;
				m_r++;
			}
		m_r = n_row;
		m_c = n_col;
		list<string>::iterator testEnd = m_cLinePtr;		//Essentially, this function copies the delete function in the special case without submitting an undo action; see delete comments
		testEnd++;
		string nextLine = (*testEnd);
		(*m_cLinePtr) += nextLine;
		editingLines.erase(testEnd);
		m_numLines--;
		break;
	}
	default:
	{
		break;
	}
	}
}

//private functions

