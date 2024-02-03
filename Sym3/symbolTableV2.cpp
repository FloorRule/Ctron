#include "symbolTableV2.h"

Ctron::symbolTable::symbolTable()
{
	_index = -1;
	_position = 0;
}

Ctron::symbolTable::~symbolTable()
{
}

void Ctron::symbolTable::open_stack()
{
	_index++;
	std::vector<std::pair<Base*, Base*>> a;
	std::vector<int> b;

	_positionTable.push_back(b);
	_positionTable[_index].push_back(0);
	_table.push_back(a);
}

void Ctron::symbolTable::close_stack()
{
	_table[_index].clear();
	_positionTable[_index].clear();
	_index--;
}

void Ctron::symbolTable::insert(Base* id, Base* type)
{
	_table[_index].push_back(std::pair<Base*, Base*>(id, type));
	_positionTable[_index].push_back(_position);
	_position++;
}

void Ctron::symbolTable::insertFunc(Base* id, Base* type, ParamList* param)
{
	_funcTable.push_back(std::pair<Base*, Base*>(id, type));
	_paramList.push_back(param);
}

std::vector<string> Ctron::symbolTable::getArgsId(ParamList *list)
{
	std::vector<string> args;
	ParamList* temp = list;
    while (temp != NULL) {
        if(temp->getParamDec() != NULL) {
            args.push_back(temp->getParamDec()->get_id()->text);
        }
        temp = temp->getNextParam();
    }
	return args;
}

std::string Ctron::symbolTable::check_if_param(string var, string funcName)
{
	std::vector<string> args;
	for (int i = 0; i < _funcTable.size(); i++)
	{
		if (_funcTable[i].first->text == funcName)
		{
			args = getArgsId(_paramList[i]);
			for(int j = 0; j < args.size(); j++)
			{
				if(args[j] == var)
				{
					return "%"+std::to_string(j);
				}
			}
		}
	}
	return "NEW";
}


bool Ctron::symbolTable::isDeclared(string id)
{
	for (int i = 0; i <= _index; i++)
	{
		for (int j = 0; j < _table[i].size(); j++)
		{
			if (_table[i][j].first->text == id)
			{
				std::cout << "Found" << std::endl;
				return true;
			}
		}
	}
	return false;
}

bool Ctron::symbolTable::isFuncDeclared(string id)
{
	for (int i = 0; i < _funcTable.size(); i++)
	{
		if (_funcTable[i].first->text == id)
		{
			std::cout << "Found Func" << std::endl;
			return true;
		}
	}
	return false;
}

bool Ctron::symbolTable::check_return_type(string id, string type)
{
	for (int i = 0; i < _funcTable.size(); i++)
	{
		if (_funcTable[i].first->text == id)
		{
			std::cout << _funcTable[i].second->text << " " << type << std::endl;
			Token* tk = new Token(_funcTable[i].second->text);
			if (tk->text == type || _funcTable[i].second->text == type)
			{
				return true;
			}
			
		}
	}
	return false;
}

std::pair<Base*, Base*> Ctron::symbolTable::get_element_by_id(string id)
{
	for (int i = 0; i <= _index; i++)
	{
		for (int j = 0; j < _table[i].size(); j++)
		{
			if (_table[i][j].first->text == id)
			{
				return _table[i][j];
			}
		}
	}
	Token* a = new Token("ERROR");
	Token* b = new Token("ERROR");
	return std::pair<Base*, Base*>(a, b);
}
std::pair<Base*, Base*> Ctron::symbolTable::get_func_by_id(string id)
{
	for (int i = 0; i < _funcTable.size(); i++)
	{
		if (_funcTable[i].first->text == id)
		{
			return _funcTable[i];
		}
	}
	Token* a = new Token("ERROR");
	Token* b = new Token("ERROR");
	return std::pair<Base*, Base*>(a, b);
}

bool Ctron::symbolTable::checkReturn(Base* type)
{
	string funcType = _funcTable[_funcTable.size() - 1].second->text;
	if (type == NULL && funcType != "VOID")
	{
		return false;
	}
	else
	{
		if (funcType == "INT" && type->text != "INT")
		{
			return false;
		}
		else if(funcType != type->text)
		{
			return false;
		}

	}
	
	return true;
}

int Ctron::symbolTable::getOffsetofId(string id)
{
	for (int i = 0; i <= _index; i++)
	{
		for (int j = 0; j < _table[i].size(); j++)
		{
			if (_table[i][j].first->text == id)
			{
				return _positionTable[i][j];
			}
		}
	}
}
