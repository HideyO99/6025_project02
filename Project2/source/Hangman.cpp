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

//////////////////////////////////////////////////////////////
//															//
// run : set languge to english and loop until user exit  	//
// input param : none										//
//															//
//////////////////////////////////////////////////////////////
void Hangman::run()
{
	std::string input;
	messageList_.loadLangSetFromXML("lang_en");
	lang_ = "lang_en";
	
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

//////////////////////////////////////////////////////////////
//															//
// newGame : set new game state	machine and random word 	//
// input param : none										//
//															//
//////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////
//															//
// getInput :	check user input and update state machine	//
//					\s to save								//
//					\l to load								//
//					\e to exit								//
//					\n to new game							//
//					\1 to set language to english			//
//					\2 to set language to german			//
//					\3 to set language to french			//
//				The input key will set to uppercase and 	//
//				throw back									//
// input param :uInput  <- throw back the input to caller	//
//															//
//////////////////////////////////////////////////////////////
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
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML(lang_.c_str());
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
		lang_ = "lang_en";
	}
	else if (input == "\\2" )
	{
		//load german
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML("lang_de");
		lang_ = "lang_de";
	}
	else if (input == "\\3")
	{
		//load french
		messageList_.xmlmssglist.clear();
		messageList_.loadLangSetFromXML("lang_fr");
		lang_ = "lang_fr";
	}
	else if (isalpha(input.front()))
	{
		if (gameState_ != GAME_STARTED)
		{
			//std::cout << "please start new game" << std::endl; //todo load msg from xml
			return true;
		}

		uInput = toupper(input.front());
	}
	else
	{
		//std::cout << "Please input a-z only" << std::endl; //todo load msg from xml
		errorStatus_ = ERROR_INVALID_INPUT;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//																	//
// randWordList :	open wordlist file and random the word			//
// input param :	wordSelected <- throw back word selected		//
//					to caller										//
//																	//
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
//																	//
// isOK :	check status from result_								//
// input param :	none											//
//																	//
//////////////////////////////////////////////////////////////////////
void Hangman::isOK()
{
	if (!result_)
	{
		std::cout << "game error\n"; //todo load msg from xml
		gameState_ = GAME_ERROR;
		errorStatus_ = ERROR_UNKNOW;
	}
}

//////////////////////////////////////////////////////////////////////
//																	//
// checkWord :	check input and put it into map to keep input		//
//				history												//
// input param : string	input										//
//																	//
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
//																	//
// checklive :	check remain live									//
// input param : none												//
//																	//
//////////////////////////////////////////////////////////////////////
void Hangman::checklive()
{
	if (live_ <= 0)
	{
		//std::cout << "game over" << std::endl; //todo load msg from xml
		gameState_ = GAME_OVER;
	}
}

//////////////////////////////////////////////////////////////////////
//																	//
// saveState :	pass the state value to xml class					//
// input param : none												//
//																	//
//////////////////////////////////////////////////////////////////////
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
	savedata.xml_lang_ = lang_;

	return saveFile_.saveState(savedata);
}

//////////////////////////////////////////////////////////////////////
//																	//
// saveState :	call xml to load state and set it back to internal	//
//				variable											//
// input param : none												//
//																	//
//////////////////////////////////////////////////////////////////////
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
	lang_ = savedata.xml_lang_;

	return true;
}

//////////////////////////////////////////////////////////////////////
//																	//
// toCapital :	set word to capital letter							//
// input param : string s											//
//																	//
//////////////////////////////////////////////////////////////////////
void Hangman::toCapital(std::string& s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		s[i]=toupper(s[i]);
	}
}

//////////////////////////////////////////////////////////////////////
//																	//
// print :	print screen											//
// input param : live												//
//																	//
//////////////////////////////////////////////////////////////////////
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


