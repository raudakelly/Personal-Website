%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOMngr.h"
#include "SymTab.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
extern int yyerror(char *);
void dumpTable();

extern SymTab *table;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
  struct intList * intList;
  struct ExprList * ExprList;
}

//%type <val> Index
%type <string> Id
%type <string> Index
%type <intList> iList
%type <ExprList> ExpList
%type <ExprRes> Piece
%type <ExprRes> Factor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <ExprRes> ExprResult
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <InstrSeq> ForStmt
%type <BExprRes> BExpr
	
%token Ident
%token IntLit 	
%token Int
%token Write
%token Printline
%token Printspaces
%token Read
%token IF
%token Else
%token While
%token For
%token EQ
%token LTE //<=
%token GTE //>=
%token NEQ //!=	
%token AND
%token OR

%%

Prog		:	Declarations StmtSeq							{Finish($2); } ;
Declarations	:	Dec Declarations							{ };
Declarations	:										{ };
Dec		:	Int Id '[' Index ']' ';'						{ enterName(table, $2); setCurrentAttr(table,$4); } ;
Dec		:	Int Id ';'								{ enterName(table, $2); } ;
StmtSeq 	:	Stmt StmtSeq								{ $$ = AppendSeq($1, $2); } ;
StmtSeq		:										{ $$ = NULL; } ;
Stmt		:	Read '(' iList ')' ';'							{ $$ = doRead($3) ; } ;
Stmt		:	Write '(' ExpList ')' ';'						{ $$ = doPrint($3); } ;	
ForStmt		:	Id '=' Expr								{ $$ = doAssign($1, $3); } ;
Stmt		:	Id '=' Expr ';'								{ $$ = doAssign($1, $3); } ;
Stmt		:	Id '[' Expr ']' '=' Expr ';'						{ $$ = doAssignArrayElement($1, $3, $6); } ;
Stmt		:	Printline ';'								{ $$ = doPrintLine(); } ;
Stmt		:	Printspaces '(' Expr ')' ';'						{ $$ = doPrintSpaces($3); } ;
Stmt		:	IF '(' BExpr ')' '{' StmtSeq '}' Else '{' StmtSeq '}'			{ $$ = doIfElse($3, $6, $10); } ;
Stmt		:	IF '(' BExpr ')' '{' StmtSeq '}'					{ $$ = doIf($3, $6); };	
Stmt		:	While '(' BExpr ')' '{' StmtSeq '}'					{ $$ = doWhile($3, $6); } ;
Stmt		:	For '(' ForStmt ';' BExpr ';' ForStmt ')' '{' StmtSeq '}'		{ $$ = doFor($3, $5, $7, $10); } ;
BExpr		:	ExprResult								{ $$ = doEvalResult($1); } ;
ExprResult	:	ExprResult AND ExprResult						{ $$ = doAnd($1, $3); };
ExprResult	:	ExprResult OR ExprResult						{ $$ = doOr($1, $3); };	
ExprResult	:	'!' ExprResult								{ $$ = doNot($2); } ;	
ExprResult	:	Expr LTE Expr								{ $$ = doLTE($1, $3); };
ExprResult	:	Expr GTE Expr								{ $$ = doGTE($1, $3); };
ExprResult	:	Expr NEQ Expr								{ $$ = doNEQ($1, $3); };
ExprResult	:	Expr '<' Expr								{ $$ = doLessThan($1, $3); };
ExprResult	:	Expr '>' Expr								{ $$ = doGreatThan($1, $3); };
ExprResult	:	Expr EQ Expr								{ $$ = doEqualEqual($1, $3); };
ExpList		:	ExpList ',' Expr							{ $$ = doAppendExprList($1, $3); } ;
ExpList		:	Expr									{ $$ = doNewExprList($1); } ;
//Expr		:	Id '[' Expr ']'								{ $$ = doEvalArrayElement($1, $3); };	// <-broken function
Expr		:	Expr '+' Term								{ $$ = doAdd($1, $3); } ;
Expr		:	Expr '-' Term								{ $$ = doSub($1, $3); } ;
Expr		:	Term									{ $$ = $1; } ;
Term		:	Term '*' Factor								{ $$ = doMult($1, $3); } ;
Term		:	Term '/' Factor								{ $$ = doDiv($1, $3); } ;
Term		:	Term '%' Factor								{ $$ = doMod($1, $3); } ;
Term		:	Factor									{ $$ = $1; } ;
Factor		:	Factor '^' Factor							{ $$ = doExpo($1, $3); } ;
Factor		:	'-' Piece								{ $$ = doUSub($2, doIntLit("-1")); } ;
Factor		:	Piece									{ $$ = $1; } ;
Piece		:	IntLit									{ $$ = doIntLit(yytext); } ;
Piece		:	Id									{ $$ = doRval($1); } ;
iList		:	iList ',' Id								{ $$ = doAppendIntList($1, $3); } ;
iList		:	Id									{ $$ = doNewIntList($1); } ;
Id		: 	Ident									{ $$ = strdup(yytext); } ;
Index		:	IntLit									{ $$ = strdup(yytext); }
 
%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
