/* Daniel Engbert CMSC 331 Section 01 Fall 2016 */
/* this parser defines and validates gimli code */
/* command to run: make; ./gimli < test.gimli */

/* code to be inserted into the generated C code: */
%{
#include <stdio.h>
%}

/* start symbol */
%start program

/* all the tokens defined in gimli.lex */
%token START END IF THEN ELSE EQUALS SEMI
%token READ PRINT FUNCTION INT FLOAT
%token COMMA OPAREN CPAREN OCURLY CCURLY PLUS MINUS TIMES DIVIDE PERCENT
%token LESS_THAN EQUALS_EQUALS GREATER_THAN AND OR NOT
%token COMMENT CONSTANT IDENTIFIER STRING

/* make these tokens left associative to prevent shift/reduce errors */
/* see http://stackoverflow.com/a/34712754 */
%left AND OR LESS_THAN EQUALS_EQUALS GREATER_THAN
%nonassoc NOT

/* beginning of rules section */
%%

/* I changed it to allow an empty program (just start and end) */
program : START statements END
    | START END
    | { yyerror("didn't find a program"); }
    ;

statements : statement
    | statement statements
    ;

/* I'm allowing comments in the code and considering them a statment */
/* I also changed how semicolons are needed. now semicolons are required */
/* as part of every statement except ifStmt and COMMENT */
statement : declaration SEMI
    | assignmentStmt SEMI
    | readStmt SEMI
    | printStmt SEMI
    | callStmt SEMI
    | ifStmt
    | COMMENT
    ;

declaration : FUNCTION identifierList
    | INT identifierList
    | FLOAT identifierList
    ;

assignmentStmt : IDENTIFIER EQUALS expr
    ;

readStmt : READ identifiers
    ;

printStmt : PRINT printable
    ;

callStmt : IDENTIFIER OPAREN argumentList CPAREN
    ;

/* I am requiring curly braces to prevent a shift/reduce conflicts */
/* otherwise its ambiguous where the else statments end and the rest */
/* of the program continues */
ifStmt : IF expr THEN OCURLY statements CCURLY ELSE OCURLY statements CCURLY
    ;


printable : STRING
    | expr
    | printable STRING
    | printable expr
    ;

identifiers : identifiers IDENTIFIER
    | IDENTIFIER
    ;

argumentList : argument
    | argument COMMA argumentList
    ;

identifierList : IDENTIFIER
    | IDENTIFIER COMMA identifierList
    ;

argument : expr
    | STRING
    ;

/* I removed logicalOps and just put AND OR NOT right in the definition */
/* of expr so that %left would work. did the same with relOps for same reason */
/* see http://stackoverflow.com/a/9727450/5500073 */
expr : term
    | OPAREN expr CPAREN
    | term addop factor
    | expr AND expr
    | expr OR expr
    | NOT expr
    | expr GREATER_THAN expr
    | expr EQUALS_EQUALS expr
    | expr LESS_THAN expr
    ;

factor : IDENTIFIER
    | CONSTANT
    ;

term : factor multop term
    | factor
    ;

addop : PLUS
    | MINUS
    ;

multop : TIMES
    | DIVIDE
    | PERCENT
    ;

%%
int main()
{
  fprintf(stderr, "Starting parse...\n");
  yyparse();
  fprintf(stderr, "reached end of file.\n\n");
}

void yyerror(s)
char *s;
{
  fprintf(stderr, "%s\n", s);
}

int yywrap()
{
  return(1);
}
