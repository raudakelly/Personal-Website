/* Semantics.h
   The action and supporting routines for performing semantics processing.
*/

/* Semantic Records */
struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
};

struct ExprRes {
  int Reg;
  struct InstrSeq * Instrs;
};

struct ExprResList {
	struct ExprRes *Expr;
	struct ExprResList * Next;
};

struct BExprRes {
  char * Label;
  int Reg;
  struct InstrSeq * Instrs;
};

//new struct to be used for reading in comma delimited list of variables
struct intList {
  char * name;
  struct intList * next;
};

//new struct to be used for printing comma delimited list of expressions
struct ExprList {
  struct ExprRes * Expr;
  struct ExprList * Next;
};


/* Semantics Actions */
extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);

//Arithmetic Operators
extern struct ExprRes *  doAdd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doSub(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doUSub(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMult(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doMod(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doDiv(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doExpo(struct ExprRes * Res1,  struct ExprRes * Res2);

//Boolean Operators
extern struct ExprRes *  doAnd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doOr(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doNot(struct ExprRes * Res1);

//Relational Operators
extern struct ExprRes * doEqualEqual (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes * doLTE (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes * doGTE (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes * doNEQ (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes * doLessThan (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes * doGreatThan (struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct BExprRes * doEvalResult(struct ExprRes * Res1);

//used for reading comma delimited list of variables
extern struct intList * doNewIntList(char * name);
extern struct intList * doAppendIntList(struct intList * iList, char * name);
extern struct InstrSeq * doRead(struct intList * iList);

//used for printing comma delimited list of expressions
extern struct ExprList * doNewExprList(struct ExprRes * res);
extern struct ExprList * doAppendExprList(struct ExprList * eList, struct ExprRes * res);

extern struct InstrSeq *  doPrint(struct ExprList * eList);
extern struct InstrSeq *  doPrintSpaces(struct ExprRes * Expr);
extern struct InstrSeq *  doPrintLine();

//control structures
extern struct InstrSeq * doIf(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct BExprRes *bRes, struct InstrSeq * seq1, struct InstrSeq * seq2);
extern struct InstrSeq * doWhile(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doFor(struct InstrSeq * Stmt1, struct BExprRes *bRes, struct InstrSeq * Stmt2, struct InstrSeq * seq);

//arrays functions
extern struct InstrSeq *  doAssignArrayElement(char * name, struct ExprRes * Expr1, struct  ExprRes * Expr2);

/* commented out because function does not work
extern struct ExprRes *  doEvalArrayElement(char * name, struct ExprRes * Expr);
*/
//finish
extern void	Finish(struct InstrSeq *Code);
