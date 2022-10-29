#include "XML.h"
#include <iostream>

XML::XML()
{
}

XML::~XML()
{
}

//bool XML::createXMLObj(const std::string& fname)
//{
//	return false;
//}
//
//void XML::readXML(const std::string& fname)//std::string fname, std::string fpath)
//{
//	pugi::xml_document music_xml;
//	pugi::xml_parse_result result = music_xml.load_file("sound_list.xml");
//	if (!result) 
//	{
//		std::cout << "Unable to load XML file" << std::endl;
//		return ;
//	}
//	std::cout << "Loaded XML" << std::endl;
//	pugi::xml_object_range<pugi::xml_node_iterator> music = music_xml.child("music").children();
//	if (music.empty()) 
//	{
//		std::cout<<"There are no entries"<<std::endl;
//		return ;
//	}
//
//	pugi::xml_node_iterator music_i;
//	int i = 0;
//	int j = 0;
//	for (music_i = music.begin(); music_i != music.end(); ++music_i)
//	{
//		pugi::xml_node fname = *music_i;
//		pugi::xml_node_iterator musicit;
//		//std::string s = fname.child_value();
//		my_music[i].fname = fname.child_value();
//		my_music[i].fname.erase(std::remove(my_music[i].fname.begin(), my_music[i].fname.end(), '\n'), my_music[i].fname.cend());	//remove \n
//		my_music[i].fname.erase(std::remove(my_music[i].fname.begin(), my_music[i].fname.end(), '\t'), my_music[i].fname.cend());	//remove \t
//		for (musicit = fname.children().begin(); musicit != fname.children().end(); ++musicit)
//		{
//			pugi::xml_node pathNode = *musicit;
//
//			if (!pathNode.children().empty())
//			{
//				my_music[i].path[j] = pathNode.child_value();
//				j++;
//			}
//		}
//		i++; 
//		j = 0;
//	}
//	
//	pugi::xml_object_range<pugi::xml_node_iterator> fx = music_xml.child("fx").children();
//	if (fx.empty())
//	{
//		std::cout << "There are no entries" << std::endl;
//		return;
//	}
//
//	pugi::xml_node_iterator fx_i;
//	i = j = 0;
//
//	for (fx_i = fx.begin(); fx_i != fx.end(); fx_i++)
//	{
//		pugi::xml_node fx_path = *fx_i;
//		pugi::xml_node_iterator fx_it;
//		for (fx_it = fx_path.children().begin(); fx_it != fx_path.children().end(); fx_it++)
//		{
//			pugi::xml_node pathNode = *fx_it;
//			if (!pathNode.children().empty())
//			{
//				my_fx_path[i] = pathNode.child_value();
//				i++;
//			}
//		}
//	}
//	//music_xml.~xml_document();
//}
//
//XML::MyMusic::MyMusic()
//{
//	path[0] = "";
//	path[1] = "";
//}
