#include "Hangman.h"

Hangman::Hangman()
{
	srand((int)time(0));
	gameState_ = GAME_NOTSTART;
	errorStatus_ = ERROR_NONE;

	result_ = false;
	live_ = 9;
}

Hangman::~Hangman()
{
}

void Hangman::run()
{
	std::string input;
	messageList_.loadLangSetFromXML("lang_en");
	
	while (1)
	{
		result_ = print(live_);
		result_ = getInput(input);
		if (input == "\\e"||input=="\\E")
		{
			break;
		}
		if (gameState_ == GAME_STARTED)
		{

			if (input.size() > 0 )
			{
				result_ = checkWord(input);
				input.clear();
				checklive();
				//result_ = print(live_);
			}
		}
	}
}

bool Hangman::newGame()
{

	result_ = randWordList(theWord);
	gameState_ = GAME_NEW;
	live_ = 9;
	input_.clear();
	foundPos.clear();
	myWord.clear();
	for (size_t i = 0; i < theWord.size(); i++)
	{
		myWord.emplace(i, "_");
	}
	print(live_);

	gameState_ = GAME_STARTED;
	errorStatus_ = ERROR_NONE;

	return true;
}

bool Hangman::getInput(std::string& uInput)
{
	std::string input;
	std::cout << "> ";
	std::cin >> input;
	if (input == "\\s" || input == "\\S")
	{
		//save state
		result_ = saveState();
	}
	else if (input == "\\l" || input == "\\L")
	{
		//load state
		result_ = loadState();
	}
	else if (input == "\\n" || input == "\\N")
	{
		result_ = newGame(); //new game
	}
	else if (input == "\\e" || input == "\\E")
	{
		uInput = input;//exit game
	}
	else if (input == "\\1" )
	{
		//load english
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML("lang_en");
	}
	else if (input == "\\2" )
	{
		//load german
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML("lang_de");
	}
	else if (input == "\\3")
	{
		//load french
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML("lang_fr");
	}
	else if (isalpha(input.front()))
	{
		if (gameState_ != GAME_STARTED)
		{
			//std::cout << "please start new game" << std::endl; //todo load msg from xml
			return true;
		}
		//if (input.size() > 1)
		//{
		//	std::cout << "only first character accepted"<<std::endl; //todo load msg from xml
		//}

		uInput = toupper(input.front());
	}
	else
	{
		//std::cout << "Please input a-z only" << std::endl; //todo load msg from xml
		errorStatus_ = ERROR_INVALID_INPUT;
	}

	return true;
}
//random select word from list.txt
bool Hangman::randWordList(std::string& wordSelected)
{
	
	std::string word;
	int randIndex = (rand() % 100) + 1;
	std::ifstream fList(WORDLIST);

	for (size_t i = 0; i < randIndex; i++)
	{
		std::getline(fList, word);
		if (i == randIndex - 1)
		{
			toCapital(word);
			wordSelected = word;
			fList.close();
			return true;
		}
	}
	fList.close();


	return false;
}

void Hangman::isOK()
{
	if (!result_)
	{
		std::cout << "game error\n"; //todo load msg from xml
		gameState_ = GAME_ERROR;
		errorStatus_ = ERROR_UNKNOW;
	}
}

bool Hangman::checkWord(const std::string input)
{
	if (input_.find(input) == input_.end())
	{
		input_.emplace(input,0);
		std::size_t found = theWord.find(input);
		if (found != std::string::npos)
		{
			foundPos.try_emplace(found,input);
			myWord.insert_or_assign(found, input);
			gameState_ = GAME_STARTED;
			errorStatus_ = ERROR_NONE;

			while (found != std::string::npos)
			{
				found = theWord.find(input, found + 1);
				if (found != std::string::npos)
				{
					foundPos.try_emplace(found, input);
					myWord.insert_or_assign(found, input);
					
				}
			}
			
			if (foundPos.size() == theWord.size())
			{
				//std::cout << "win!!" << std::endl;
				gameState_ = GAME_WIN;	
				errorStatus_ = ERROR_NONE;
			}
		}
		else
		{
			//std::cout << "not find" << std::endl; //todo load msg from xml
			live_--;
			gameState_ = GAME_STARTED;
			errorStatus_ = ERROR_NOTFOUND;
			//std::cout << "live = " << live_ << std::endl; //todo load msg from xml
		}
	}
	else
	{
		//std::cout << "duplicated input" << std::endl; // todo load msg from xml
		errorStatus_ = ERROR_DUPLICATED_INPUT;
	}
	return true;
}

void Hangman::checklive()
{
	if (live_ <= 0)
	{
		//std::cout << "game over" << std::endl; //todo load msg from xml
		gameState_ = GAME_OVER;
	}
}

bool Hangman::saveState()
{
	//parser data to xml obj
	XML::saveData savedata;
	savedata.xml_theWord = theWord;
	savedata.xml_gameState_ = gameState_;
	savedata.xml_errorStatus_ = errorStatus_;
	savedata.xml_live_ = live_;
	savedata.xml_foundPos = foundPos;
	savedata.xml_myWord = myWord;
	savedata.xml_input_ = input_;

	return saveFile_.saveState(savedata);
}

bool Hangman::loadState()
{
	XML::saveData savedata;
	saveFile_.loadState(&savedata);

	theWord = savedata.xml_theWord;
	gameState_ = (gameState)savedata.xml_gameState_;
	errorStatus_ = (errorStatus)savedata.xml_errorStatus_;
	live_ = savedata.xml_live_;
	foundPos = savedata.xml_foundPos;
	myWord = savedata.xml_myWord;
	input_ = savedata.xml_input_;
	return false;
}

void Hangman::toCapital(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		s[i]=toupper(s[i]);
	}
}

bool Hangman::print(int live)
{
	system("cls");
	std::cout << "\t\t\t  _   _                                         " << std::endl;
	std::cout << "\t\t\t | | | | __ _ _ __   __ _ _ __ ___   __ _ _ __  " << std::endl;
	std::cout << "\t\t\t | |_| |/ _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\ " << std::endl;
	std::cout << "\t\t\t |  _  | (_| | | | | (_| | | | | | | (_| | | | |" << std::endl;
	std::cout << "\t\t\t |_| |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_|" << std::endl;
	std::cout << "\t\t\t                    |___/                       " << std::endl;
	switch (live)
	{
	case 0:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |       /|\\			"	<< std::endl;
		std::cout << "\t\t\t          |        |			"	<< std::endl;
		std::cout << "\t\t\t          |       / \\			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;

	case 1:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |       /|\\			"	<< std::endl;
		std::cout << "\t\t\t          |        |			"	<< std::endl;
		std::cout << "\t\t\t          |         \\			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 2:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |       /|\\			"	<< std::endl;
		std::cout << "\t\t\t          |        |			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 3:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |       /|\\			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 4:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |        |\\			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 5:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |        |			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 6:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |        0			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 7:
		std::cout << "\t\t\t          |--------|			"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 8:
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t          |						"	<< std::endl;
		std::cout << "\t\t\t__________|_____________________"	<< std::endl;
		break;
	case 9:
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t								"	<< std::endl;
		std::cout << "\t\t\t________________________________"	<< std::endl;
		break;
	default:
		break;
	}

	if (gameState_ == GAME_NOTSTART)
	{
		std::cout << "\t\t\t" << messageList_.xmlmssglist.at(0).data() << std::endl; //new game
		std::cout << "\t\t\t" << messageList_.xmlmssglist.at(1).data() << std::endl; //load game
		std::cout << "\t\t\t" << messageList_.xmlmssglist.at(3).data() << std::endl; //exit game
		std::cout << "\t\t\t\\1:English" << std::endl;
		std::cout << "\t\t\t\\2:Deutsch" << std::endl;
		std::cout << "\t\t\t\\3:française" << std::endl;
	}
	else
	{
		std::cout << std::endl;
		std::cout << messageList_.xmlmssglist.at(4).data() << "\t\t\t"; //guess
		print_map(myWord, false);
		std::cout << std::endl << std::endl;
		std::cout << messageList_.xmlmssglist.at(5).data() << "\t\t\t"; //your input
		print_map(input_, true);
		std::cout << std::endl;

		if (gameState_ == GAME_STARTED)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(0).data() << std::endl; //new game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(1).data() << std::endl; //load game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(2).data() << std::endl; //save game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(3).data() << std::endl; //exit game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(6).data() << std::endl; //rule
		}
		if (gameState_ == GAME_WIN)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(7).data() << std::endl; //win
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(0).data() << std::endl; //new game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(3).data() << std::endl; //exit game
		}
		if (gameState_ == GAME_OVER)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(8).data() << std::endl; //lose
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(0).data() << std::endl; //new game
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(3).data() << std::endl; //exit game
		}
		if (gameState_ == GAME_SAVE)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(9).data() << std::endl; //saved

		}
		if (gameState_ == GAME_LOAD)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(10).data() << std::endl; //loaded

		}
		if (errorStatus_ == ERROR_INVALID_INPUT)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(11).data() << std::endl; //only a-z allow
		}
		if (errorStatus_ == ERROR_DUPLICATED_INPUT)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(12).data() << std::endl; //duplicate input
		}
		if (errorStatus_ == ERROR_NOTFOUND)
		{
			std::cout << "\t\t\t" << messageList_.xmlmssglist.at(13).data() << std::endl; //incorrect
		}
		if (errorStatus_ == ERROR_NONE)
		{
			std::cout << std::endl; 
			std::cout << std::endl;
		}
	}
	//std::cout << theWord << std::endl; //<-- to show the answer

	return true;
}


