/* Semantics.c
   Support and semantic action routines.
   
*/

#include <strings.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"

extern SymTab *table;

/* Semantics support routines */

struct ExprRes *  doIntLit(char * digits)  { 

   struct ExprRes *res;
  
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);

  return res;
}

struct ExprRes *  doRval(char * name)  { 

   struct ExprRes *res;
  
   if (!findName(table, name)) {
	//printf(name);
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   }
  res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
  res->Reg = AvailTmpReg();
  res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);

  return res;
}

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes * doSub(struct ExprRes * Res1, struct ExprRes * Res2) {
   int reg;

   reg = AvailTmpReg();
   AppendSeq(Res1->Instrs,Res2->Instrs);
   AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));
   ReleaseTmpReg(Res1->Reg);
   ReleaseTmpReg(Res2->Reg);
   Res1->Reg = reg;
   free(Res2);
   return Res1;
}

struct ExprRes * doUSub(struct ExprRes * Res1, struct ExprRes * Res2) {
  int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  { 

   int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  { 

  int reg;
   
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  
  AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
                                       TmpRegName(reg),
                                       TmpRegName(Res1->Reg),
                                       TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  Res1->Reg = reg;
  free(Res2);
  return Res1;
}

struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2)  { 

  char * label1;
  char * label2;
  char * label3;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"blt",TmpRegName(Res1->Reg),"$zero",label2));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"j",label1,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(label2,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",TmpRegName(Res1->Reg),TmpRegName(Res1->Reg),TmpRegName(Res2->Reg)));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

struct ExprRes *  doExpo(struct ExprRes * Res1, struct ExprRes * Res2)  { 

  int reg;
  struct ExprRes * res;
  res = doIntLit("1");
  reg = AvailTmpReg();
  AppendSeq(Res1->Instrs,Res2->Instrs);
  AppendSeq(Res1->Instrs,res->Instrs);
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",TmpRegName(reg),TmpRegName(Res2->Reg),"0"));
  char * label1 = GenLabel();
  char * label2 = GenLabel();
  AppendSeq(Res1->Instrs,GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"beqz",TmpRegName(reg),label2,NULL));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",TmpRegName(res->Reg),TmpRegName(res->Reg),TmpRegName(Res1->Reg)));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"add",TmpRegName(reg),TmpRegName(reg),"-1"));
  AppendSeq(Res1->Instrs,GenInstr(NULL,"j",label1,NULL,NULL));
  AppendSeq(Res1->Instrs,GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res1->Reg);
  ReleaseTmpReg(Res2->Reg);
  ReleaseTmpReg(reg);
  Res1->Reg = res->Reg;
  free(Res2);
  free(res);
  return Res1;
}

//relational operators
extern struct ExprRes * doEqualEqual(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beq",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doLTE(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"ble",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doGTE(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"bge",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doNEQ(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"bne",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doLessThan(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"blt",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doGreatThan(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * label1;
  char * label2;
  label1 = GenLabel();
  label2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"bgt",TmpRegName(Res1->Reg),TmpRegName(Res2->Reg),label1));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",label2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(label1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(label2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

//boolean operators
extern struct ExprRes * doAnd(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * l1;
  char * l2;
  l1 = GenLabel();
  l2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beqz",TmpRegName(Res1->Reg),l2,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beqz",TmpRegName(Res2->Reg),l1,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",l2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(l2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doOr(struct ExprRes * Res1,  struct ExprRes * Res2) {
  AppendSeq(Res1->Instrs, Res2->Instrs);
  char * l1;
  char * l2;
  l1 = GenLabel();
  l2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beqz",TmpRegName(Res1->Reg),l1,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",l2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beqz",TmpRegName(Res2->Reg),l2,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(l2,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(Res2->Reg);
  free(Res2);
  return Res1;
}

extern struct ExprRes * doNot(struct ExprRes * Res1) {
  char * l1;
  char * l2;
  l1 = GenLabel();
  l2 = GenLabel();
  AppendSeq(Res1->Instrs, GenInstr(NULL,"beqz",TmpRegName(Res1->Reg),l1,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"0",NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"j",l2,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(Res1->Instrs, GenInstr(NULL,"li",TmpRegName(Res1->Reg),"1",NULL));
  AppendSeq(Res1->Instrs, GenInstr(l2,NULL,NULL,NULL,NULL));
  return Res1;
}

//used for reading in comma delimited list of variables
extern struct intList * doNewIntList(char * name) {
  struct intList * res;
  

  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }

  res = (struct intList *) malloc(sizeof(struct intList));
  res->name = name;
  return res;
}

extern struct intList * doAppendIntList(struct intList * iList, char * name) {
  struct intList * res;
  

  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared variable");
  }

  res = (struct intList *) malloc(sizeof(struct intList));
  res->name = name;
  struct intList * temp;
  temp = iList;
  while(NULL!=temp->next){
    temp = temp->next;
  }
  temp->next = res;
  return iList;
}

extern struct InstrSeq * doRead(struct intList * iList) {
  struct InstrSeq * Instrs;
  struct intList * prev;
  Instrs = GenInstr(NULL,"li","$v0","5",NULL);
  AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(Instrs,GenInstr(NULL,"sw","$v0",iList->name,NULL));
  prev = iList;
  iList = iList->next;
  free(prev);
  while(NULL!=iList){
    AppendSeq(Instrs,GenInstr(NULL,"li","$v0","5",NULL));
    AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    AppendSeq(Instrs,GenInstr(NULL,"sw","$v0",iList->name,NULL));
    prev = iList;
    iList = iList->next;
    free(prev);
  }
  return Instrs;
}

//used for printing comma delimited list of expressions
extern struct ExprList * doNewExprList(struct ExprRes * res) {
  struct ExprList * eList;
  eList = (struct ExprList *) malloc(sizeof(struct ExprList));
  eList->Expr = res;
  return eList;
}

extern struct ExprList * doAppendExprList(struct ExprList * eList, struct ExprRes * res) {
  struct ExprList * newList;
  newList = (struct ExprList *) malloc(sizeof(struct ExprList));
  newList->Expr = res;
  struct ExprList * temp;
  temp = eList;
  while(NULL!=temp->Next){
    temp = temp->Next;
  }
  temp->Next = newList;
  return eList;
}

struct InstrSeq * doPrint(struct ExprList * eList) { 

  struct InstrSeq * Instrs;
  struct ExprList * prevList;
  struct ExprRes * Expr;
  struct ExprRes * prevExpr;
  Expr = eList->Expr;
  Instrs = Expr->Instrs;  

  AppendSeq(Instrs,GenInstr(NULL,"li","$v0","1",NULL));
  AppendSeq(Instrs,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
  AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

  prevList = eList;
  eList = eList->Next;
  prevExpr = Expr;
  free(prevList);

  while(NULL!=eList){

    Expr = eList->Expr;
    ReleaseTmpReg(prevExpr->Reg);
    free(prevExpr);

    AppendSeq(Instrs,Expr->Instrs);
    AppendSeq(Instrs,GenInstr(NULL,"li","$v0","11",NULL));
    AppendSeq(Instrs,GenInstr(NULL,"li","$a0","32",NULL));
    AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
    AppendSeq(Instrs,GenInstr(NULL,"li","$v0","1",NULL));
    AppendSeq(Instrs,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
    AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));

    prevList = eList;
    eList = eList->Next;
    prevExpr = Expr;
    free(prevList);
  }
  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  return Instrs;
}

extern struct InstrSeq *  doPrintSpaces(struct ExprRes * Expr){
  struct InstrSeq * Instrs;
  Instrs = Expr->Instrs;
  char * l1 = GenLabel();
  char * l2 = GenLabel();
  AppendSeq(Instrs,GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(Instrs,GenInstr(NULL,"beq",TmpRegName(Expr->Reg),"$zero",l2));
  AppendSeq(Instrs,GenInstr(NULL,"li","$v0","11",NULL));
  AppendSeq(Instrs,GenInstr(NULL,"li","$a0","32",NULL));
  AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(Instrs,GenInstr(NULL,"sub",TmpRegName(Expr->Reg),TmpRegName(Expr->Reg),"1"));
  AppendSeq(Instrs,GenInstr(NULL,"j",l1,NULL,NULL));
  AppendSeq(Instrs,GenInstr(l2,NULL,NULL,NULL, NULL));
  ReleaseTmpReg(Expr->Reg);
  return Instrs;
}

extern struct InstrSeq *  doPrintLine(){
  struct InstrSeq * Instrs;
  Instrs = GenInstr(NULL,"li","$v0","11",NULL);
  AppendSeq(Instrs,GenInstr(NULL,"li","$a0","10",NULL));
  AppendSeq(Instrs,GenInstr(NULL,"syscall",NULL,NULL,NULL));
  return Instrs;
}

struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) { 

  struct InstrSeq *code;
  

   if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
   }

  code = Expr->Instrs;
  
  AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));

  ReleaseTmpReg(Expr->Reg);
  free(Expr);
  
  return code;
}

//special for taking an Expression, and creating the label and bne statement
//basically just the last step before passing BExpr to the if
extern struct BExprRes * doEvalResult(struct ExprRes * Res1) {
  struct BExprRes * res;
  res = (struct BExprRes *) malloc(sizeof(struct BExprRes));
  res->Label = GenLabel();
  res->Instrs = Res1->Instrs;
  res->Reg = Res1->Reg;
  free(Res1);
  return res;
}

//control structures
extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
  struct InstrSeq * seq2;
  AppendSeq(bRes->Instrs,GenInstr(NULL,"beqz",TmpRegName(bRes->Reg),bRes->Label,NULL));
  ReleaseTmpReg(bRes->Reg);
  seq2 = AppendSeq(bRes->Instrs, seq);
  AppendSeq(seq2, GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
  free(bRes);
  return seq2;
}

extern struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq1, struct InstrSeq * seq2) {
  struct InstrSeq * seq3;
  AppendSeq(bRes->Instrs,GenInstr(NULL,"beqz",TmpRegName(bRes->Reg),bRes->Label,NULL));
  ReleaseTmpReg(bRes->Reg);
  seq3 = bRes->Instrs;
  char * l1 = GenLabel();
  char * l2 = GenLabel();
  AppendSeq(seq3,GenInstr(NULL,"j",l1,NULL,NULL));
  AppendSeq(seq3,GenInstr(bRes->Label, NULL, NULL, NULL, NULL));
  AppendSeq(seq3,seq2);
  AppendSeq(seq3,GenInstr(NULL,"j",l2,NULL,NULL));
  AppendSeq(seq3,GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(seq3,seq1);
  AppendSeq(seq3,GenInstr(l2,NULL,NULL,NULL,NULL));
  free(bRes);
  return seq3;
}

extern struct InstrSeq * doWhile(struct BExprRes *bRes, struct InstrSeq * seq){
  char * l1 = GenLabel();
  struct InstrSeq * Instrs = GenInstr(l1,NULL,NULL,NULL,NULL);
  AppendSeq(Instrs,bRes->Instrs);
  AppendSeq(bRes->Instrs,GenInstr(NULL,"beqz",TmpRegName(bRes->Reg),bRes->Label,NULL));
  AppendSeq(Instrs,seq);
  AppendSeq(Instrs,GenInstr(NULL,"j",l1,NULL,NULL));
  AppendSeq(Instrs,GenInstr(bRes->Label,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(bRes->Reg);
  free(bRes);
  return Instrs;
}

extern struct InstrSeq * doFor(struct InstrSeq * Stmt1, struct BExprRes *bRes, struct InstrSeq * Stmt2, struct InstrSeq * seq){
  struct InstrSeq * Instrs;
  Instrs = Stmt1;
  char * l1 = GenLabel();
  AppendSeq(Instrs,GenInstr(l1,NULL,NULL,NULL,NULL));
  AppendSeq(Instrs,bRes->Instrs);
  AppendSeq(bRes->Instrs,GenInstr(NULL,"beqz",TmpRegName(bRes->Reg),bRes->Label,NULL));
  AppendSeq(Instrs,seq);
  AppendSeq(Instrs,Stmt2);
  AppendSeq(Instrs,GenInstr(NULL,"j",l1,NULL,NULL));
  AppendSeq(Instrs,GenInstr(bRes->Label,NULL,NULL,NULL,NULL));
  ReleaseTmpReg(bRes->Reg);
  free(bRes);
  return Instrs;
}

//arrays functions
extern struct InstrSeq *  doAssignArrayElement(char * name, struct ExprRes * Expr1, struct  ExprRes * Expr2){
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared array");
  }
  printf("Reached");
  struct InstrSeq * Instrs;
  int reg1 = AvailTmpReg();
  int reg2 = AvailTmpReg();
  Instrs = Expr1->Instrs;
  AppendSeq(Instrs,Expr2->Instrs);
  AppendSeq(Instrs,GenInstr(NULL,"la",TmpRegName(reg1),name,NULL));
  AppendSeq(Instrs,GenInstr(NULL,"mul",TmpRegName(reg2),TmpRegName(Expr1->Reg),"4"));
  AppendSeq(Instrs,GenInstr(NULL,"add",TmpRegName(reg1),TmpRegName(reg1),TmpRegName(reg2)));
  
  char * addr = (char *)malloc(10*sizeof(char));
  strcat(addr, "0(");
  strcat(addr,TmpRegName(reg1));
  strcat(addr,")");

  AppendSeq(Instrs,GenInstr(NULL,"sw",TmpRegName(Expr2->Reg),addr,NULL));

  ReleaseTmpReg(reg1);
  ReleaseTmpReg(reg2);
  ReleaseTmpReg(Expr1->Reg);
  ReleaseTmpReg(Expr2->Reg);
  free(Expr1);
  free(Expr2);
  return Instrs;
}

/* commented out because function does not work
extern struct ExprRes *  doEvalArrayElement(char * name, struct ExprRes * Expr){
  if (!findName(table, name)) {
    writeIndicator(getCurrentColumnNum());
    writeMessage("Undeclared array");
  }
  struct ExprRes * res;
  int reg1 = AvailTmpReg();
  int reg2 = AvailTmpReg();
  res->Instrs = Expr->Instrs;
  AppendSeq(res->Instrs,GenInstr(NULL,"la",TmpRegName(reg1),name,NULL));
  AppendSeq(res->Instrs,GenInstr(NULL,"mul",TmpRegName(reg2),TmpRegName(Expr->Reg),"4"));
  AppendSeq(res->Instrs,GenInstr(NULL,"add",TmpRegName(reg1),TmpRegName(reg1),TmpRegName(reg2)));

  char * addr = (char *)malloc(10*sizeof(char));
  strcat(addr, "0(");
  strcat(addr,TmpRegName(reg1));
  strcat(addr,")");

  AppendSeq(res->Instrs,GenInstr(NULL,"lw",TmpRegName(reg2),addr,NULL));
  
  ReleaseTmpReg(Expr->Reg);
  ReleaseTmpReg(reg1);
  res->Reg = reg2;
  free(Expr);
  return res;
} */

void							 
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
  //struct SymEntry *entry;
  int hasMore;
  struct Attr * attr;


  code = GenInstr(NULL,".text",NULL,NULL,NULL);
  //AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
  AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
  AppendSeq(code,Code);
  AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL)); 
  AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
  AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
  AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));

  hasMore = startIterator(table);
  while (hasMore) {
    //if name is an array element; declare space for it
    //else (if name is not an array)
    if(getCurrentAttr(table)!=NULL){
      char * name = (char *)getCurrentAttr(table);
      int index;
      sscanf(name, "%d", &index);
      index = index * 4;
      sprintf(name, "%d", index);
      AppendSeq(code,GenInstr((char *) getCurrentName(table),".space",name,NULL,NULL));
      hasMore = nextEntry(table);
    } else {
      AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
      hasMore = nextEntry(table);
    }
    
  }


  WriteSeq(code);
  
  return;
}




