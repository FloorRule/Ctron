#include "dataClasses.hpp"
#include "../GlobalBuffer/global_buffer.hpp"
#include "../Sym3/symbolTableV2.h"
#include <iostream>
#include <vector>

#define MIN_VARS 70

using std::string;
using std::to_string;
using namespace Ctron;

int regCount = 0;
int strCounter = 0;
int labelCounter = 0;

string freshReg() {
	regCount++;
	return "%var" + std::to_string(regCount);
}

string freshStr(){
    strCounter++;
    return "@.Str" + std::to_string(strCounter);
}

string genDivLabel(){
    string label = "validateDivLabel_" + to_string(labelCounter);
    labelCounter++;
    return label;
}

vector<pair<int,BranchLabelIndex>> makelist(pair<int,BranchLabelIndex> item)
{
	vector<pair<int,BranchLabelIndex>> newList;
	newList.push_back(item);
	return newList;
}

bool replace(string& str, const string& from, const string& to, const BranchLabelIndex index) 
{
	size_t pos;
	if (index == SECOND) {
		pos = str.find_last_of(from);
	}
	else { //FIRST
		pos = str.find_first_of(from);
	}
    if (pos == string::npos)
        return false;
    str.replace(pos, from.length(), to);
    return true;
}

void bpatch(global_buffer& gb, const vector<pair<int,BranchLabelIndex>>& address_list, const std::string &label)
{
	int count = 0;
	std::list<std::string> list = gb.get_commands_container();

    for(vector<pair<int,BranchLabelIndex>>::const_iterator i = address_list.begin(); i != address_list.end(); i++)
	{
		count = 0;
    	int address = (*i).first;
    	BranchLabelIndex labelIndex = (*i).second;
		for (auto j = list.begin(); j != list.end(); ++j)
		{
			if(count == address)
			{
				replace((*j), "@", "%" + label, labelIndex);
			}
			count ++;
		}
    }
}



void emit(FILE* yyout, string line)
{
	fprintf(yyout, "%s\n", line);
}

void newLocalVar(global_buffer& gb, string func_name, int offset, string regValue = "0") 
{
	printf("%d ", regCount);
	string varLoc = freshReg();
	string line = "";
	
	if (offset >= 0) {
		
		line = " "+varLoc + " = getelementptr [70 x i32], [70 x i32]* %" + func_name + "varplace, i32 0, i32 " + std::to_string(offset);
		gb.insert_command(line);
		gb.insert_command(" store i32 " + regValue + ", i32* " + varLoc);
		//fprintf(yyout, "store i32 %s, i32* %s\n", regValue.c_str(), varLoc.c_str() ); 
	}
}

void checkNumber(FILE* ybugs, int lineNumber, Exp* expr1, Exp* expr2){
    if(expr1->text == "INT")
	{
		std::cout<< expr1->text << " " << expr2->text << std::endl;
        if(expr2 != NULL || expr2->text != "INT"){
			fprintf(ybugs, "Type Mismatch In Line: %d", lineNumber);
        }
    } else
	{
        
    }
}

void validateDivide(global_buffer& gb, string reg)
{
	string line = "";
    string cmp = freshReg();
    string error = freshReg();

    string divideByZero = genDivLabel();
    string NotByZero = genDivLabel();
    gb.insert_command(" "+cmp+" = icmp eq i32 0, " + reg);

    gb.insert_command(" br i1 " + cmp + ", label %" + divideByZero + ", label %"+ NotByZero);

    gb.insert_command(" "+divideByZero + ":");
    gb.insert_command(" "+error + " = getelementptr [23 x i8], [23 x i8]* @.str_zero, i32 0, i32 0");
    gb.insert_command(" call void @print(i8* "+error+")");
    gb.insert_command(" call void @exit(i32 1)");
    gb.insert_command(" br label %"+ NotByZero);
    gb.insert_command(" "+NotByZero + ":");	
}

string Operate_MultiplyDivide(global_buffer& gb, Binop* bmd) {
    bmd->reg = freshReg();
	string line = "";
    if (bmd->getBinop()->text == "*") {
		line = " "+bmd->reg + " = mul i32 " + bmd->get_first()->reg + ", " + bmd->get_second()->reg;
		gb.insert_command(line);
    }
    else if (bmd->getBinop()->text == "/") {
        //validateDivide(gb, bmd->get_second()->reg);
        line = " "+bmd->reg + " = sdiv i32 " + bmd->get_first()->reg + ", " + bmd->get_second()->reg;
		gb.insert_command(line);
    }
	return bmd->reg;
}

string Operate_PlusMinus(global_buffer& gb, Binop* bpm) 
{
    bpm->reg = freshReg();
	string line = "";
	printf("- %s -",bpm->getBinop()->text.c_str());
    if (bpm->getBinop()->text == "+") 
	{
        line = " "+ bpm->reg + " = add i32 " + bpm->get_first()->reg + ", " + bpm->get_second()->reg;
		gb.insert_command(line);
    }
    else if (bpm->getBinop()->text == "-") 
	{
        line = " "+ bpm->reg + " = sub i32 " + bpm->get_first()->reg + ", " + bpm->get_second()->reg;
		gb.insert_command(line);
    }
	
	return bpm->reg;
}
int replaceAll(std::string& str, const std::string& from, const std::string& to) {
    int count = 0;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		count++;
        str.replace(start_pos, from.length(), to);
        
    }
	return count;
}
string setGlobalString(global_buffer& gb, string str){
    string id = freshStr();
	
	int sizeStr = str.size()+1;
	
	string copy = str;
	int count = replaceAll(copy, "\\0A", "  ");
	
	sizeStr -= (count * 2);
	
	gb.insert_global_variable_decleration(" "+id + " = constant [" + std::to_string(sizeStr) + " x i8] c\"" + str + "\\00\"");
    string new_reg = freshReg();
    gb.insert_command(" "+new_reg + " = getelementptr [" + std::to_string(sizeStr) +" x i8] , ["+ std::to_string(sizeStr) + " x i8] * " + id + ", i32 0, i32 0");
    return new_reg;
}

void validateAssignment(FILE* ybugs, int LineNum, Token* type, Exp* value) 
{
	std::cout << type->text << " " << value->text << std::endl;
	Token* tk = new Token(type->text);
	if(tk->text != value->text )
	{
        printf("Mismatch!");
		fprintf(ybugs, "Assign Mismatch In Line:%d", LineNum);
    }
}

void declareFunction(global_buffer& gb, string retype, string funcName, std::vector<string> args) {
	string cmd = "";
	cmd += " define ";
	if (retype == "VOID") {
		cmd += "void ";
	}
	else {
		cmd += "i32 ";
	}
	cmd += "@" + funcName + "(";

	for (int i = 0; i < args.size(); i++) {

		if (i == args.size() - 1)
		{
			cmd += "i32";
		}
		else
		{
			cmd += "i32, ";
		}
	}
	cmd += "){";
	gb.insert_command(cmd);
	//fprintf(yyout, "%s\n", cmd.c_str());
	

	string ebp = freshReg();
	string line = " %" + funcName + "varplace = alloca [" + std::to_string(MIN_VARS) + " x i32]";
	gb.insert_command(line);
}

string makeArgsString(string funcName, ExpList* head)
{
    ExpList* curr = head;
	std::vector<string> register_List;
	std::vector<string> type_List;
	string Args_Line = "";
	int index = 0;
	
	
    while (curr != NULL)
	{
        register_List.push_back(curr->getExpr()->reg);
        type_List.push_back(curr->getExpr()->text);
        curr = curr->getExprList();
    }
    if(register_List.empty())
	{
        return "";
    }
    
    for (int i = 0; i < register_List.size(); i++) 
	{
        if(i == register_List.size() - 1 )
		{
			//std::cout << " " << type << " " <<type_List[index]<< std::endl;
			
			if(type_List[index] == "int" || type_List[index] == "INT")
			{
				Args_Line += "i32 " + register_List[i];
			}else if(type_List[index] == "STRING")
			{
				Args_Line += "i8* " + register_List[i];
			}
			
		} 
        else
		{
			
			if(type_List[index] == "int" || type_List[index] == "INT")
			{
				Args_Line += "i32 " + register_List[i]+ " ,";
			}else if(type_List[index] == "STRING")
			{
				Args_Line += "i8* " + register_List[i]+ " ,";
			}
			
        }
		index++;
    }
    
    return Args_Line;
}

string makeFuncCall(global_buffer& gb, string funcName, ExpList* head, string return_Type)
{
    string Args_Line = makeArgsString(funcName, head);
    string reg = "";
	string Line = "";
	
	if(funcName == "printf")
	{
		Line = " call i32 (i8*, ...) @printf(" + Args_Line + ")\n";
		gb.insert_command(Line);
    }else if (return_Type == "VOID")
	{
        
		Line = " call void @" + funcName + "(" + Args_Line + ")\n";
		//fprintf(yyout, "%s\n", Line.c_str());
		gb.insert_command(Line);
    }else
	{
        reg = freshReg();
        //emit(reg + " = call i32 "+ "@" + funcName + "(" + Args_Line + ")");
		Line = " "+reg + " = call i32 "+ "@" + funcName + "(" + Args_Line + ")\n";
		gb.insert_command(Line); 
    }
    return reg;
}

string loadVar(global_buffer& gb, string funcName, int offset){
    string res;
    string ptrRes = freshReg();
	string line;
    res = freshReg();
	
    line = " "+ptrRes + " = getelementptr [70 x i32], [70 x i32]* %"+funcName+"varplace, i32 0, i32 " + std::to_string(offset);
	gb.insert_command(line); 
    line = " "+res + " = load i32, i32* "+ptrRes;
    gb.insert_command(line); 
    return res;
}
