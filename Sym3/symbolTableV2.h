#pragma once

#include "../dataClass/dataClasses.hpp"

namespace Ctron
{
	class symbolTable 
	{
	public:
		symbolTable();
		~symbolTable();

		void open_stack();
		void close_stack();
		void insert(Base* id, Base* type);
		void insertFunc(Base* id, Base* type, ParamList* param);
		bool isDeclared(string id);
		bool isFuncDeclared(string id);
		std::pair<Base*, Base*> get_element_by_id(string id);
		std::pair<Base*, Base*> get_func_by_id(string id);
		std::string check_if_param(string var, string funcName);
		bool check_return_type(string id, string type);
		bool checkReturn(Base* type);
		int getOffsetofId(string id);
		
		std::vector<string> getArgsId(ParamList* list);

	private:
		int _index;
		int _position;
		std::vector<std::vector<std::pair<Base*, Base*>>> _table;
		std::vector<std::pair<Base*, Base*>> _funcTable;
		std::vector<ParamList*> _paramList;
		std::vector<std::vector<int>> _positionTable;

	};


}