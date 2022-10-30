#include "XML.h"
#include <iostream>

XML::XML()
{
}

XML::~XML()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// loadLangSetFromXML :	open locale.xml which contain words in each language to show up in game	//
// input param :	langSet 																	//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool XML::loadLangSetFromXML(const pugi::char_t* langSet)
{
	pugi::xml_document locale_xml;
	pugi::xml_parse_result result = locale_xml.load_file("locale.xml");
	if (!result)
	{
		std::cout << "Unable to load XML file" << std::endl;
		return false;
	}
	std::cout << "Loaded XML" << std::endl;
	pugi::xml_object_range<pugi::xml_node_iterator> messageList = locale_xml.child("Localize").child(langSet).children();
	if (messageList.empty())
	{
		std::cout << "There are no entries" << std::endl;
		return false;
	}

	pugi::xml_node_iterator mssglist_i;
	int i = 0;

	for (mssglist_i = messageList.begin(); mssglist_i != messageList.end(); ++mssglist_i)
	{
		pugi::xml_node mssgnode = *mssglist_i;
		pugi::xml_node_iterator mssgnode_it;

		xmlmssglist.emplace(i, mssgnode.child_value());
		i++;

	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// saveState :	write game state to saved.xml													//
// input param :	savedata <- which passed from hangman class									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool XML::saveState(saveData savedata)
{
	pugi::xml_document saved_xml;
	pugi::xml_node saved = saved_xml.append_child("saved");
	pugi::xml_node stheword = saved.append_child("theWord");
	stheword.append_child(pugi::node_pcdata).set_value(savedata.xml_theWord.c_str());

	pugi::xml_node sgamestate = saved.append_child("gameState_");
	sgamestate.append_child(pugi::node_pcdata).set_value(std::to_string(savedata.xml_gameState_).c_str());

	pugi::xml_node serrorstatus = saved.append_child("errorStatus_");
	serrorstatus.append_child(pugi::node_pcdata).set_value(std::to_string(savedata.xml_errorStatus_).c_str());

	pugi::xml_node slive = saved.append_child("live_");
	slive.append_child(pugi::node_pcdata).set_value(std::to_string(savedata.xml_live_).c_str());

	pugi::xml_node slang = saved.append_child("lang_");
	slang.append_child(pugi::node_pcdata).set_value(savedata.xml_lang_.c_str());

	pugi::xml_node sfoundPos = saved.append_child("foundPos");
	std::map<size_t, std::string>::iterator foundPos_it;
	for (foundPos_it = savedata.xml_foundPos.begin(); foundPos_it != savedata.xml_foundPos.end(); foundPos_it++)
	{
		pugi::xml_node smap_foundpos_key = sfoundPos.append_child("key");
		smap_foundpos_key.append_child(pugi::node_pcdata).set_value(std::to_string(foundPos_it->first).c_str());
		pugi::xml_node smap_foundpos_data = smap_foundpos_key.append_child("value");
		smap_foundpos_data.append_child(pugi::node_pcdata).set_value(foundPos_it->second.c_str());
	}

	pugi::xml_node smyWord = saved.append_child("myWord");
	std::map<size_t, std::string>::iterator myword_it;
	for (myword_it = savedata.xml_myWord.begin(); myword_it != savedata.xml_myWord.end(); myword_it++)
	{
		pugi::xml_node smap_myword_key = smyWord.append_child("key");
		smap_myword_key.append_child(pugi::node_pcdata).set_value(std::to_string(myword_it->first).c_str());
		pugi::xml_node smap_myword_data = smap_myword_key.append_child("value");
		smap_myword_data.append_child(pugi::node_pcdata).set_value(myword_it->second.c_str());
	}

	pugi::xml_node sinput = saved.append_child("input_");
	std::map<std::string, size_t>::iterator input_it;
	for (input_it = savedata.xml_input_.begin(); input_it != savedata.xml_input_.end(); input_it++)
	{
		pugi::xml_node smap_input_key = sinput.append_child("key");
		smap_input_key.append_child(pugi::node_pcdata).set_value(input_it->first.c_str());
		pugi::xml_node smap_input_data = smap_input_key.append_child("value");
		smap_input_data.append_child(pugi::node_pcdata).set_value(std::to_string(input_it->second).c_str());
	}

	saved_xml.save_file("saved.xml");
	//saved_xml.~xml_document();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//																								//
// loadState :	load game state from saved.xml													//
// input param :	savedata <- thow back data to hangman class									//
//																								//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool XML::loadState(saveData* savedata )
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("saved.xml");
	std::vector<std::string> buf;
	std::map<std::string, std::string> buf_map[8];

	if (!result)
	{
		std::cout << "Unable to load XML file" << std::endl;
		return false;
	}

	pugi::xml_object_range<pugi::xml_node_iterator> load_data = doc.child("saved").children();
	if (load_data.empty())
	{
		std::cout << "There are no entries" << std::endl;
		return false;
	}

	pugi::xml_node_iterator datalist_i;
	int i = 0;
	for (datalist_i = load_data.begin(); datalist_i != load_data.end(); ++datalist_i)
	{
		std::string s;
		pugi::xml_node datalist = *datalist_i;

		s = datalist.child_value();
		if (s!="")
		{
			buf.push_back(datalist.child_value()); //store normal node element 
		}
		pugi::xml_node_iterator map_it;

		for (map_it = datalist.children().begin(); map_it != datalist.children().end(); map_it++)
		{
			pugi::xml_node map = *map_it;
			pugi::xml_node_iterator mapdata_it;
			std::string m_key;
			std::vector<std::string> vm_data;
			m_key = map.child_value();
			if (m_key != "")
			{
				for (mapdata_it = map.children().begin(); mapdata_it != map.children().end(); mapdata_it++)
				{
					std::string m_data;
					pugi::xml_node map_data = *mapdata_it;
					m_data = map_data.child_value();
					if (m_data != "")
					{
						vm_data.push_back(m_data); //store map value to temp vector
					}

				}
				std::string data_pop = vm_data.front().c_str();
				buf_map[i].emplace(m_key, data_pop); // store map both key and value
			}
		}
		i++;
	}

	//set the read data to struct and hangman class will access later
	savedata->xml_theWord = buf[0].c_str();
	savedata->xml_gameState_ = std::stoi(buf[1].c_str());
	savedata->xml_errorStatus_ = std::stoi(buf[2].c_str());
	savedata->xml_live_ = std::stoi(buf[3].c_str());
	savedata->xml_lang_ = buf[4].c_str();

	for (auto const& pair : buf_map[5]) 
	{
		savedata->xml_foundPos.emplace(std::stoi(pair.first), pair.second);
	}
	for (auto const& pair : buf_map[6])
	{
		savedata->xml_myWord.emplace(std::stoi(pair.first), pair.second);
	}
	for (auto const& pair : buf_map[7])
	{
		savedata->xml_input_.emplace(pair.first, std::stoi(pair.second));
	}
	return true;
}

