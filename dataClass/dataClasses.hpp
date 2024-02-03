#pragma once
#include <string>
#include <iostream>
#include <vector>
using std::string;
using std::vector;
using std::pair;

enum BranchLabelIndex {FIRST, SECOND};

class Base
{
public:
	Base() {
		text = "";
		reg = "";
		label = "";
	}
	vector<pair<int,BranchLabelIndex>> truelist;
    vector<pair<int,BranchLabelIndex>> falselist;
	string text;
	string reg;
	string label;
};

class Token : public Base
{
public:
	Token(string content)
	{
		if (content == "int")
		{
			text = "INT";
		}
		else if(content == "void")
		{
			text = "VOID";
		}
		else if(content == "float")
		{
			text = "FLOAT";
		}
		else if(content == "double")
		{
			text = "DOUBLE";
		}
		else
		{
			text = content;
		}
	}
private:
};

class Element : public Base 
{
public:
	Element(Token* t, Token* i, Token* value)
	{
		type = t;
		id = i;
		val = value;
	}

	Element(Token* t, Token* i)
	{
		type = t;
		id = i;
		val = NULL;
	}

	Token* get_type()
	{
		return type;
	}

	Token* get_id()
	{
		return id;
	}

	Token* get_value()
	{
		return val;
	}

private:
	Token* type;
	Token* id;
	Token* val;
};



class Exp : public Base 
{
public:
	Exp(string t) 
	{
		text = t;
	}
	Exp()  
	{
		text = "";
	}
};

class Assign : public Base
{
public:
	Assign(Base* id, Base* val)
	{
		this->id = id;
		this->value = val;
	}
private:
	Base* id;
	Base* value;
};

class Num : public Exp
{
public:
	Num(string val)
	{
		this->value = new Token(val);
		text = "INT";
	}
	
	Token* value;
};

class ExpList : public Base
{
public:
	ExpList(Exp* exp) 
	{
		expr = exp;
		next_expr = NULL;
	}

	ExpList(Exp* exp, ExpList* nextList) 
	{
		expr = exp;
		next_expr = nextList;
	}
	Exp* getExpr()
	{
		return expr;
	}
	ExpList* getExprList()
	{
		return next_expr;
	}
private:
	Exp* expr;
	ExpList* next_expr;
};

class Func_Call : public Exp{
public:
    Func_Call(Token* identifier, string returnType){
        text = returnType;
		ide = identifier;
        expr_List = NULL;
    }

    Func_Call(Token* identifier, ExpList* expr_List, string returnType){
        text = returnType;
		ide = identifier;
        this->expr_List = expr_List;
    }
private:
	Token* ide;
    ExpList* expr_List;

};

class StringExp : public Exp
{
public:
    string val;
    StringExp(Token* val)
	{
        this->val = val->text;
        text = "STRING";
    }
private:
};

class While : public Base
{
public:
	While(Base* cond, Base* state, Base* else_state)
	{
		condition = cond;
		statements = state;
		this->else_state = else_state;
	}

	While(Base* cond, Base* state) 
	{
		condition = cond;
		statements = state;
		this->else_state = NULL;
	}
private:
	Base* condition;
	Base* statements;
	Base* else_state;

};

class If : public Base
{
public:
	If(Base* cond, Base* state, Base* else_state)
	{
		condition = cond;
		statements = state;
		this->else_state = else_state;
	}

	If(Base* cond, Base* state) 
	{
		condition = cond;
		statements = state;
		this->else_state = NULL;
	}
private:
	Base* condition;
	Base* statements;
	Base* else_state;
};

class Exp_Op : public Exp
{
public:
	Exp_Op()
	{
		_first = NULL;
		_second = NULL;
	}

	Exp* get_first()
	{
		return _first;
	}

	Exp* get_second()
	{
		return _second;
	}

protected:
	Exp* _first;
	Exp* _second;
};

class Binop : public Exp_Op
{
public:
	Binop(Exp* first_part, Token* operat, Exp* second_part)
	{
		_first = first_part;
		this->binop = operat;
		_second = second_part;

		if (_first->text == "INT")
		{
			text = "INT";
		}
	}
	
	Token* getBinop()
	{
		return binop;
	}
	
private:
	Token* binop;
};

class Relop: public Exp_Op 
{
public:
	Relop(Exp* first_part, Token* operat, Exp* second_part) {
		_first = first_part;
		this->relop = operat;
		_second = second_part;
		
		text = "BOOL";
	}
private:
	Token* relop;
};

class And : public Exp_Op
{
public:
	And(Exp* first_Expr, Exp* second_Expr)
	{
		_first = first_Expr;
		_second = second_Expr;
		text = "BOOL";
	}
};

class Or : public Exp_Op
{
public:
	Or(Exp* first_Expr, Exp* second_Expr)
	{
		_first = first_Expr;
		_second = second_Expr;
		text = "BOOL";
	}
};



class Functions : public Base
{
public:
	Functions(Base* declares, Base* function)
	{
		this->function = function;
		this->declares = declares;
	}
private:
	Base* function;
	Base* declares;
};

class Return : public Base
{
public:
	Return(Base* expr)
	{
		exp = expr;
	}

	Return()
	{
		exp = NULL;
	}
private:
	Base* exp;
};


class Statements : public Base
{
public:
	Statements(Base* state) 
	{
		statement = state;
		next_state = NULL;
	}

	Statements(Base* statements, Base* state) 
	{
		statement = state;
		next_state = statements;
	}
private:
	Base* statement;
	Base* next_state;
};

class ParamList : public Base
{
public:
	ParamList() 
	{
		param_Declare = NULL;
		next_Param = NULL;
	}

	ParamList(Element* param) 
	{
		param_Declare = param;
		next_Param = NULL;
	}

	ParamList(Element* param_Declare, ParamList* param_List) 
	{	
		param_Declare = param_Declare;
		next_Param = param_List;
	}
	
	Element* getParamDec()
	{
		return param_Declare;
	}
	
	ParamList* getNextParam()
	{
		return next_Param;
	}
	
private:
	Element* param_Declare;
	ParamList* next_Param;
};



