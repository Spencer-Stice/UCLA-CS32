#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {	//Clears trie using resursive helper
	node** currArray = trieStart;
	deleteTree(currArray, true);
}

//recursive delete helper
void StudentSpellCheck::deleteTree(node** root, bool isFirst)		//Recursively free all trie memory
{
	bool isEmpty = true;
	if (root == nullptr)		//Base case 1
		return;
	if (isFirst)
	{
		for (int i = 0; i < 26; i++)
		{
			if (root[i] != nullptr)
			{
				isEmpty = false;
				break;				//Base case 2
			}
		}
	}
	else
	{
		for (int i = 0; i < 28; i++)
		{
			if (root[i] != nullptr)
			{
				isEmpty = false;
				break;				//Base case 2
			}
		}
	}
	if (isEmpty)
	{
		return;
	}
	if (!isFirst)
	{
		for (int i = 0; i < 28; i++)		//For each of the nodes, call the delete tree function on them and then delete the original node
		{
			if (root[i] != nullptr)
			{
				deleteTree(root[i]->m_children, false);
				delete root[i];
			}
			root[i] = nullptr;
		}
	}
	else
	{
		for (int i = 0; i < 26; i++)			//Same thing as above except check more values if this isn't the starting node
		{
			if (root[i] != nullptr)
			{
				deleteTree(root[i]->m_children, false);
				delete root[i];
			}
			root[i] = nullptr;
		}
	}
	return;
}




bool StudentSpellCheck::load(std::string dictionaryFile) {		//Loads a dictionary into a trie
	ifstream infile(dictionaryFile);
	int numWords = 0;
	if (!infile)
	{
		return false;
	}
	string cReadingLine;
	while (getline(infile, cReadingLine))			//For each line
	{
		string clReadingLine = "";
		for (int i = 0; i < cReadingLine.length(); i++)
		{
			if(isalpha(cReadingLine[i]) || cReadingLine[i] == '\'')
				clReadingLine += tolower(cReadingLine[i]);		//Lowercase all letters for easier searching
		}
		node** currArray = trieStart;
		for (int i = 0; i < clReadingLine.length(); i++)		//For each letter, try to insert it into the trie. If it is the last letter of a word, mark it with a .
		{
			bool isLast = false;
			if (i == clReadingLine.length() - 1)
				isLast = true;
			char currChar = clReadingLine[i];
			int index = currChar - 'a';
			if (currChar == '\'')				//Special case for apostrophe's
				index = 26;
			if (currArray[index] == nullptr)
			{
				node* newNode = new node;
				newNode->m_char = currChar;
				currArray[index] = newNode;
				if (isLast)
				{
					node* endMarker = new node;
					endMarker->m_char = '.';
					newNode->m_children[27] = endMarker;		//If the end of a word is found, mark its children with a '
				}
				currArray = currArray[index]->m_children;
			}
			else
			{
				if (isLast)
				{
					node* endMarker = new node;
					endMarker->m_char = '.';
					currArray[index]->m_children[27] = endMarker;
				}
				else
				{
					currArray = currArray[index]->m_children;
				}
			}
		}
		numWords++;				//Count the number of words added, used for debugging
	}
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {	//Spellcheck a given word and give suggestions
	string dictWord = "";
	bool manual = true;
	int wLength = word.length();
	int i = 0;
	node** currArray = trieStart;
	string fWord = "";
	bool isFirst = true;
	for (int k = 0; k < wLength; k++)
	{
		fWord += tolower(word[k]);			//Lowercase all letters
	}
	while (dictWord != fWord && i < wLength)		//Search the trie for the given word in a similar way to insertion
	{
		char cLetter = fWord[i];
		int cLetterIndex = cLetter - 'a';
		if (!isFirst && cLetter == '\'')
		{
			string addMe(1, cLetter);
			if (*currArray != nullptr && currArray[26] != nullptr)
			{
				dictWord += addMe;
				currArray = currArray[26]->m_children;
				i++;
				continue;
			}
			else
				break;
		}
		if (currArray[cLetterIndex] == nullptr)
			{
				break;								//If the word is not found, stop trying to find it
			}
		if (currArray[cLetterIndex]->m_char == cLetter)
			{
				string addMe(1, cLetter);
				dictWord += addMe;
				currArray = currArray[cLetterIndex]->m_children;
			}
		if(isFirst)
			isFirst = false;
		i++;
	}
	if (dictWord == fWord && currArray[27] != nullptr && currArray[27]->m_char == '.')	//If the word is found and there is a . to mark the end of a word
	{
		return true;
	}
	//implement suggestions
	string wordC = fWord;
	for (int q = 0; q < wLength; q++)					//For each letter in the word, try changing the letter and searching for it in the trie. If it is found, it is a valid suggestion
	{
		fWord = wordC;
		char c = '\'';
		int i = 0;
		bool isFirst = true;
		while ((c <= 'z' || c == '\'') && suggestions.size() < max_suggestions)		//Continue until enough suggestions are found or no suggestions are found
		{
			fWord.erase(q, 1);
			string iM(1, c);
			fWord.insert(q, iM);
			node** currArray = trieStart;
			string newWord = "";
			i = 0;
			bool isFirsttwo = true;
			while (fWord != newWord && i < wLength)							//Search the trie for the candidate word similar to previous techniques
			{
				char cLetter = fWord[i];
				int cLetterIndex = cLetter - 'a';
				if (cLetter == '\'' && !isFirsttwo)
				{
					cLetterIndex = 26;
				}
				else if(cLetter == '\'')
					break;								//If this candidate is not in the trie, break
				if (currArray[cLetterIndex] == nullptr)
				{
					break;								//If this candidate is not in the trie, break
				}
				if (currArray[cLetterIndex]->m_char == cLetter)
				{
					string addMe(1, cLetter);
					newWord += addMe;
					currArray = currArray[cLetterIndex]->m_children;
				}
				i++;
				if (isFirsttwo)
					isFirsttwo = false;
			}
			if (fWord == newWord && currArray[27] != nullptr && currArray[27]->m_char == '.')
			{
				suggestions.push_back(newWord);				//If the candidate is in the trie, add it to the suggestions vector
			}
			if (isFirst)
			{
				c = 'a';
				isFirst = false;				//After checking for apostrophe's continue with the alphabet
				continue;
			}
			c++;
		}
	}
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {	//Spellchecks a given line using its helper function and the spellcheck function while not checking for suggestions
	vector<wordAndPos> m_postExtract;
	vector<string> toPass;
	problems.clear();
	detectWords(line, m_postExtract);			//Call helper function to extract words
	vector<wordAndPos>::iterator it = m_postExtract.begin();
	while (it != m_postExtract.end())
	{
		if ((*it).m_word != "" && !spellCheck((*it).m_word, 0, toPass))		//If the word is not empty and it is not in the dictionary, add it to the problems vector
		{
			Position newP;
			newP.start = (*it).start;
			newP.end = (*it).end;
			problems.push_back(newP);
		}
		it++;
	}
}

//private functions

void StudentSpellCheck::detectWords(string line, vector<wordAndPos>& detectedWords)	//Extracts words from a given line
{
	int length = line.length();
	string cWord = "";
	int i = 0;
	int cStart = 0;
	int t = 0;
	for (; i < length; i++)
	{
		char cChar = line[i];
		if (isalpha(cChar) == false && cChar != '\'')		//If the character is not a possible word character, discard it and end the current word
		{
			wordAndPos newP;
			newP.m_word = cWord;
			newP.start = cStart;
			newP.end = i - 1;
			detectedWords.push_back(newP);			//Add the current word to the detected words vector
			t++;
			cWord = "";
			cStart = i + 1;
			continue;
		}
		cWord += cChar;					//Add the character to the current word
		if (i == length - 1)
		{
			wordAndPos newP;
			newP.m_word = cWord;
			newP.start = cStart;
			newP.end = i ;
			detectedWords.push_back(newP);		//If the end is reached, add the current word to the vector
			t++;
		}
	}
}