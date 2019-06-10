%{

#include <stdio.h>
#include "y.tab.h"

%}

%%

"start"                         { return(START); }
"end"                           { return(END); }

\/\*.*\*\/                      { return(COMMENT); }

"if"                            { return(IF); }
"then"                          { return(THEN); }
"else"                          { return(ELSE); }

"="                             { return(EQUALS); }
";"                             { return(SEMI); }

"read"                          { return(READ); }
"print"                         { return(PRINT); }

"function"                      { return(FUNCTION); }
"int"                           { return(INT); }
"float"                         { return(FLOAT); }

","                             { return(COMMA); }
"("                             { return(OPAREN); }
")"                             { return(CPAREN); }
"{"                             { return(OCURLY); }
"}"                             { return(CCURLY); }

"+"                             { return(PLUS); }
"-"                             { return(MINUS); }
"*"                             { return(TIMES); }
"/"                             { return(DIVIDE); }
"%"                             { return(PERCENT); }

"<"                             { return(LESS_THAN); }
"=="                            { return(EQUALS_EQUALS); }
">"                             { return(GREATER_THAN); }
"and"                           { return(AND); }
"or"                            { return(OR); }
"not"                           { return(NOT); }


[a-zA-Z][a-zA-Z0-9]*            { return(IDENTIFIER); }
[0-9]+                          { return(CONSTANT); }
\"[a-zA-Z0-9 ]*\"               { return(STRING); }

[.\n ]                             ;  /* eat up and discard any stray characters */

%%
