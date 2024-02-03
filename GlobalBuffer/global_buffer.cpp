/**
 * @file global_buffer.cpp
 * @author Avihai Shalom <avihais12344@gmail.com>
 * @brief The implementaion of global_buffer class.
 * @version 0.1
 * @date 2021-02-01
 * 
 * @copyright Copyright (c) 2021
 * If we have license, view it. If not, notify the author.
 */


#include "global_buffer.hpp"    //Ctron::global_buffer.
#include <sstream>              //std::ostringstream.
#include <algorithm>

using namespace Ctron;

global_buffer::global_buffer()
: _commands(), _global_vars_declerations(), _num_labels(0)
{}

///The newline char to help seperate commands.
constexpr char newline = '\n';
constexpr char var_name[] = "var_";

global_buffer::global_buffer(const global_buffer& other)
: _commands(other._commands), _global_vars_declerations(other._global_vars_declerations), _num_labels(other._num_labels)
{
}

global_buffer& global_buffer::operator=(const global_buffer& other) 
{
    //copying the commands.
    _global_vars_declerations = other._global_vars_declerations;
    _commands = other._commands;
    _num_labels = other._num_labels;
    return *this;
}

std::string global_buffer::get_commands() const
{
    std::ostringstream full_commands;
    for(const auto& command : _commands)
    {
        full_commands << command;
    }

    return full_commands.str();
}

std::string global_buffer::get_global_vars_declerations() const
{
    std::ostringstream full_commands;
    for(const auto& decleration : _global_vars_declerations)
    {
        full_commands << decleration;
    }

    return full_commands.str();
}

const global_buffer::command_container_t& global_buffer::get_commands_container() const noexcept
{
    return _commands;
}

const global_buffer::command_container_t& global_buffer::get_global_vars_declerations_container() const noexcept
{
    return _global_vars_declerations;
}

int global_buffer::insert_command(const std::string& command) 
{
    std::ostringstream command_stream(command);
    command_stream << newline;
    _commands.emplace_back(std::move(command_stream.str()));
	return _commands.size() - 1;
}

int Ctron::global_buffer::insert_command(const char* str)
{
	std::string command(str);
	command += newline;
	_commands.push_back(command);
	return _commands.size() - 1;
}

void Ctron::global_buffer::replaceLine(std::vector<string> replacedLine, std::vector<int> posL)
{
	int sizeR = replacedLine.size() - 1;
	int index = 0;
	string sub_str;
	int count = 0;
	
	for (auto i = _commands.begin(); i != _commands.end(); ++i)
	{
		for(int j = 0; j < posL.size(); j++)
		{
			if(count == posL[j])
			{
				
				sub_str = replacedLine[sizeR];
				sub_str += " ";
				index = (*i).find("icmp")+5;
				
				(*i).replace(index, sub_str.length(), sub_str); //remove and replace from that position
				sizeR--;
			}
		
		}
		count++;
	}
	
	
}

void Ctron::global_buffer::replaceCallLine(string line, int pos)
{
	int index = 0;
	string sub_str;
	int count = 0;

	line += _commands.back();

	for (auto i = _commands.begin(); i != _commands.end(); ++i)
	{
		
		if (count == pos)
		{

			sub_str = line;
			sub_str += " ";
			index = (*i).find("call");

			(*i).replace(index, sub_str.length(), sub_str); //remove and replace from that position
		}

		count++;
	}
}

int global_buffer::insert_command(const char* str, std::size_t size) 
{
    std::string command(str, size);
    command += newline;
    _commands.push_back(command);
	return _commands.size() - 1;
}

void global_buffer::insert_global_variable_decleration(const std::string& command) 
{
    std::ostringstream command_stream(command);
    command_stream << newline;
    _global_vars_declerations.emplace_back(std::move(command_stream.str()));
}

void global_buffer::insert_global_variable_decleration(const char* str) 
{
    std::string command(str);
    command += newline;
    _global_vars_declerations.push_back(command);
    
}

void global_buffer::insert_global_variable_decleration(const char* str, std::size_t size) 
{
    std::string command(str, size);
    command += newline;
    _global_vars_declerations.push_back(command);
}

void Ctron::global_buffer::write_global_container_to_file(FILE* file)
{
	std::list<std::string> list = get_global_vars_declerations_container();

	for (auto i = list.begin(); i != list.end(); ++i)
	{
		fprintf(file, "%s", (*i).c_str());
	}
		
}

void Ctron::global_buffer::write_container_to_file(FILE* file)
{
	std::list<std::string> list = get_commands_container();

	for (auto i = list.begin(); i != list.end(); ++i)
	{
		fprintf(file, "%s", (*i).c_str());
	}
}

std::string global_buffer::generate_label()
{
    return std::string(var_name) + std::to_string(_num_labels++);
}
