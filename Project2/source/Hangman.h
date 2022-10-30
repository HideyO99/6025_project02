#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "XML.h"

#define WORDLIST "list.txt"

class Hangman
{
public:
	Hangman();
	~Hangman();

	void run();
	bool newGame();
	bool getInput(std::string& uInput);
	bool randWordList(std::string& wordSelected);
	void isOK(); 
	bool checkWord(const std::string input);
	void checklive();
	bool saveState();
	bool loadState();

	bool result_;
	XML messageList_;
	XML saveFile_;
	

private:
	
	void toCapital(std::string& s);
	bool print(int live);
	template<typename K, typename V>
	void print_map(std::map<K, V> const& m, bool printFirst);

	enum gameState
	{
		GAME_NOTSTART,
		GAME_NEW,
		GAME_STARTED,
		GAME_OVER,
		GAME_WIN,
		GAME_LOAD,
		GAME_SAVE,
		GAME_ERROR
	};
	enum errorStatus
	{
		ERROR_NONE,
		ERROR_INVALID_INPUT,
		ERROR_DUPLICATED_INPUT,
		ERROR_NOTFOUND,
		ERROR_UNKNOW
	};

	std::string theWord;
	gameState gameState_;
	errorStatus errorStatus_;
	int live_;
	std::map<size_t, std::string> foundPos;
	std::map<size_t, std::string> myWord;
	std::map<std::string, size_t> input_;
};

template<typename K, typename V>
inline void Hangman::print_map(std::map<K, V> const& m, bool printFirst)
{
	for (auto const& pair : m) {
		if (printFirst)
		{
			std::cout << pair.first << " ";
		}
		else
		{
			std::cout << pair.second << " ";
		}
		
	}
}
