#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"
#include <vector>
#include <string>
using namespace std;
class node;

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	struct node						//Create a node struct to store values for the trie
	{
		char m_char;
		node* m_children[28] = { nullptr };		//Set all starting values to nullptr
	};
	node* trieStart[26] = { nullptr };			//Create the start of the trie with all starting values of nullptr
	struct wordAndPos							//Create a struct to store words and positions
	{
		string m_word;
		int start;
		int end;
	};
	void detectWords(string line, vector<wordAndPos>& detectedWords);	//Declare helper functions
	void deleteTree(node** root, bool isFirst);
};

#endif  // STUDENTSPELLCHECK_H_
