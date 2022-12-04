/*
 * Author: Raud Kelly
 * Date:   2/02/2020
 * Description: Symbol Table .h file
 */

typedef struct { 
   char * name; 
   void * attribute; 
   struct SymEntry * next; 
} SymEntry; 

typedef struct { 
   int size; 
   SymEntry **contents; 
   SymEntry *current; 
} SymTab; 

SymTab * createSymTab(int size); 
/* PRE: size >= 0
   size is an estimate of the number of items that will be stored in the symbol
        table
   Return a pointer to a new symbol table
*/

void destroySymTab (SymTab * table);
//recover space created by the symbol table functions
//no functions should use the symbol table after it is destroyed

int enterName(SymTab * table, char *name);
/*if name is not in the symbol table, a copy of name is added to the symbol table
   with a NULL attribute, set current to reference the new (name, attribute) pair
   and return 1
  if name is in the symbol table, set current to reference the (name, attribute)
   pair and return 0
*/

int findName(SymTab * table, char *name);
/*if name is in the symbol table, set current to reference the (name, attribute)
   pair and return 1
  otherwise do not change current and return 0
*/

int hasCurrent(SymTab * table);
//if current references a (name, attribute) pair return 1
//otherwise return 0;

void setCurrentAttr(SymTab * table, void * attr);
//PRE: hashCurrent() == 1 
//change the attribute value of the current (name, attribute) pair to attr

void * getCurrentAttr(SymTab * table);
//PRE: hasCurrent() == 1
//return the attribute in the current (name, attribute) pair

char * getCurrentName(SymTab * table);
//PRE: hasCurrent() == 1
//return the name in the current (name, attribute) pair

int startIterator(SymTab * table);
//if the symbol table is empty, return 0
//otherwise set current to the "first" (name, attribute) pair in the symbol table and return 1

int nextEntry(SymTab * table);
/*if all (name, attribute) pairs have been visited since the last call to
  startIterator, return 0
  otherwise set current to the "next" (name, attribute) pair and return 1
*/

int hashFxn(SymTab * table, char * s);
/*hash function for finding an index
  returns index from 0 to (size-1)
  pre: size >=1
*/
