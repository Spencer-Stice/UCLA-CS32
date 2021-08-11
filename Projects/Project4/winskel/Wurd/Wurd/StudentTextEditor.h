#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include "Undo.h"
#include <list>
#include <map>

using namespace std;


class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:

	int m_r;	//store the current row
	int m_c;	//store the current column

	list<string> editingLines;			//Create a list of strings to store lines of text
	list<string>::iterator m_cLinePtr;	//Create a list iterator that will 'point' to the current line to make edits faster
	int m_numLines;						//Store the current number of lines
	void submitAction(Undo::Action a, int r, int c, char ch);	//Helper function for submitting an action to undo


};

#endif // STUDENTTEXTEDITOR_H_
