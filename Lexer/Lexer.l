%{
 #include <stdlib.h>
 #include <string.h>
 #include "dataClass/dataClasses.hpp"
 #include "Syntax.tab.hpp"
 int yyparse();
 int yylineno = 1;
 bool isStart = false;
 bool isComment = false;
 std::string lineCommand = "";
 std::string lineStr = "";
%}


%%
"%" {
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}
	}
}

"global" {
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return GLOBAL;
		}
	}
}

 /* Types */
"int" { 
printf("int ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return INT;
		}
	}

}

"double" { 
printf("double ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return DOUBLE;
		}
	}
}

"float" {
printf("float "); 
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return FLOAT;
		}
	}
}

"char" { 
printf("char ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return CHAR;
		}
	}
}

"void" { 
printf("VOID ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return VOID;
		}
	}
}

 /* Loops */
"if" { 
printf("if ");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return IF;} }
}

"while" { 
printf("while ");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return WHILE;} }
}

"for" {
printf("for ");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return FOR;} }
}

 /* Brackts */
"{" { 
printf("{ \n");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return LBRACE;} }
}
"}" {
printf("} \n");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return RBRACE;} }
}

"(" { 
printf("( ");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return LPAREN;}}
}
")" { 
printf(" )");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return RPAREN;}}
}
 
 /* others */
"\"" {
	if(isComment) { lineCommand += yytext;}else{
		if(!isStart)
		{
			isStart = true;
		}else
		{
			isStart = false;
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(lineStr.c_str());
			lineStr = "";
			printf("%s", yylval.base->text.c_str());
			return STRING;
		}
	}
}
 
";" { 
printf(";");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return SEMI;}}
}
"," { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return COMMA;} }
}
"return" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return RETURN;} }
}

 /* variables */
[a-zA-Z]+[0-9a-zA-Z]* {
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return ID;
		}
	}
 //printf("%s", yytext);

}
 
 /* integers */
[0-9]+ {
 /*yylval.num = atoi(yytext);*/
	if(isComment) { lineCommand += yytext;}else{
		if(isStart)
		{
			lineStr += yytext;
		}else
		{
			yylval.base = new Base();
			yylval.base->text = (char *) strdup(yytext);
			return INTEGER;
		}
	}
 printf("%s", yytext);
 
}

 /* Loops-Operators */
"==" { 
printf(" == ");
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return EQ;} }
}
">=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return BE;} }
}
"<=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return LE;} }
}
">" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return B;} }
}
"<" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{ return L;} } 
}
"||" { 
printf(" || ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{return OR;}
	}
}
"&&" { 
	printf(" && ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{return AND;}
	}
}

 /* Operators */
"=" { 
printf(" = ");
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{return ASSIGN;}
	}
}
"+" { 
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{yylval.base = new Base(); yylval.base->text = (char *) strdup(yytext); return PLUS; }
	}
}
"-" {
	if(isComment) { lineCommand += yytext;}else{	
		if(isStart){lineStr += yytext;}else{yylval.base = new Base(); yylval.base->text = (char *) strdup(yytext); return MINUN;}		
	}
}
"*" { 
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{yylval.base = new Base(); yylval.base->text = (char *) strdup(yytext); return MUL;} 
	}
}
"/" { 
	if(isComment) { lineCommand += yytext;}else{
		if(isStart){lineStr += yytext;}else{return DIV;}	
	}
}
"+=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{return *yytext;} }
}
"-=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{return *yytext;} }
}
"*=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{return *yytext;} }
}
"/=" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;}else{return *yytext;} }
}

"\\d" { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += "\\0A";}}
}

"//" { if(!isComment && !isStart){isComment = true;}else{isComment = false; lineCommand = "";}}

 /* skip whitespace */
[ \t] { if(isStart){lineStr += yytext;} };
[ \n] { yylineno++; isComment = false; lineCommand = "";}; 
 /* anything else is an error */
. { 
if(isComment) { lineCommand += yytext;}else{
if(isStart){lineStr += yytext;} printf("Invalid chars\n");}
};
%%

int yywrap(void) {
   return 1;
}




