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
	}
	else if (input == "\\l" || input == "\\L")
	{
		//load state
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
	}
	else if (input == "\\2" )
	{
		//load german
	}
	else if (input == "\\3")
	{
		//load french
	}
	else if (isalpha(input.front()))
	{
		if (gameState_ != GAME_STARTED)
		{
			std::cout << "please start new game" << std::endl; //todo load msg from xml
			return true;
		}
		if (input.size() > 1)
		{
			std::cout << "only first character accepted"<<std::endl; //todo load msg from xml
		}

		uInput = toupper(input.front());
	}
	else
	{
		std::cout << "Please input a-z only" << std::endl; //todo load msg from xml
		errorStatus_ = ERROR_INVALID_INPUT;
	}

	return true;
}

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
				std::cout << "win!!" << std::endl;
				gameState_ = GAME_WIN;	
				errorStatus_ = ERROR_NONE;
			}
		}
		else
		{
			std::cout << "not find" << std::endl; //todo load msg from xml
			live_--;
			gameState_ = GAME_STARTED;
			errorStatus_ = ERROR_NOTFOUND;
			//std::cout << "live = " << live_ << std::endl; //todo load msg from xml
		}
	}
	else
	{
		std::cout << "duplicated input" << std::endl; // todo load msg from xml
		errorStatus_ = ERROR_DUPLICATED_INPUT;
	}
	return true;
}

void Hangman::checklive()
{
	if (live_ <= 0)
	{
		std::cout << "game over" << std::endl; //todo load msg from xml
		gameState_ = GAME_OVER;
	}
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
		std::cout << "\t\t\t\\n to start new game\t\t\t\\1:English" << std::endl;
		std::cout << "\t\t\t\\l to load saved game\t\t\t\\2:Deutsch" << std::endl;
		std::cout << "\t\t\t\\e to exit game \t\t\t\\3:francaise" << std::endl;
	}
	else
	{
		std::cout << std::endl;
		std::cout << "Guess:\t\t\t";
		print_map(myWord, false);
		std::cout << std::endl << std::endl;
		std::cout << "Your input:\t\t\t";
		print_map(input_, true);
		std::cout << std::endl;

		if (gameState_ == GAME_STARTED)
		{
			std::cout << "\t\t\t\\n to start new game\t\t\t\\1:English" << std::endl;
			std::cout << "\t\t\t\\l to load saved game\t\t\t\\2:Deutsch" << std::endl;
			std::cout << "\t\t\t\\s to save state\t\t\t\\3:française" << std::endl;
			std::cout << "\t\t\t\\e to exit game" << std::endl;
			std::cout << "\t\t\tYou can input many keys but only first character will accepted" << std::endl;
		}
		if (gameState_ == GAME_WIN)
		{
			std::cout << std::endl << "\t\t\tYOU WIN!!" << std::endl;
			std::cout << "\t\t\t\\n to start new game" << std::endl;
			std::cout << "\t\t\t\\e to exit game" << std::endl;
		}
		if (gameState_ == GAME_OVER)
		{
			std::cout << std::endl << "\t\t\tYOU LOOSE!!" << std::endl;
			std::cout << "\t\t\t\\n to start new game" << std::endl;
			std::cout << "\t\t\t\\e to exit game" << std::endl;
		}
		if (gameState_ == GAME_SAVE)
		{
			std::cout << std::endl << "\t\t\tSave completed" << std::endl;

		}
		if (gameState_ == GAME_LOAD)
		{
			std::cout << std::endl << "\t\t\tLoaded the last save" << std::endl;

		}
		if (errorStatus_ == ERROR_INVALID_INPUT)
		{
			std::cout << std::endl << "\t\t\tPlease input a-z only" << std::endl;
		}
		if (errorStatus_ == ERROR_DUPLICATED_INPUT)
		{
			std::cout << std::endl << "\t\t\tDuplicated INPUT" << std::endl;
		}
		if (errorStatus_ == ERROR_NOTFOUND)
		{
			std::cout << std::endl << "\t\t\tincorrected" << std::endl;
		}
		if (errorStatus_ == ERROR_NONE)
		{
			std::cout << std::endl; 
			std::cout << std::endl;
		}
	}
	std::cout << theWord << std::endl;
	//std::cout << std::endl<< "\t\t\t"<<std::endl;
	return true;
}


