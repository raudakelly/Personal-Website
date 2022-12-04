/*
 * Author: Raud Kelly
 * Date:   2/02/2020
 * Description: Symbol Table .c file
 */

/* PRE: size >= 0
   size is an estimate of the number of items that will be stored in the symbol
        table
   Return a pointer to a new symbol table
*/

#include "SymTab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int tableIndex;
int hash;

SymTab * createSymTab(int size){
   tableIndex = 0;
   SymTab *table = malloc(sizeof(SymTab));
   table->size = size;

   SymEntry **contents = malloc(size * sizeof(SymEntry*));
   table->contents = contents;

   table->current = NULL;
   tableIndex = 0;
   hash = -1;
   return table;

}

//recover space created by the symbol table functions
//no functions should use the symbol table after it is destroyed
void destroySymTab (SymTab * table){
   int flag = 0;
   int index = 0;
   SymEntry * temp = table->contents[0];
   SymEntry * freed;
   while(flag == 0){
      if(temp == NULL){
         index++;
         if(index == table->size){
            flag = 1;
         } else {
            temp = table->contents[index];
         }
      } else { 
         free(temp->name);
         if(temp->attribute != NULL){
            free(temp->attribute);
         }
         freed = temp;
         temp = (SymEntry *)temp->next;
         free(freed);
      }
   }
   free(table->contents);
   free(table);
}

/*if name is not in the symbol table, a copy of name is added to the symbol table
   with a NULL attribute, set current to reference the new (name, attribute) pair
   and return 1
  if name is in the symbol table, set current to reference the (name, attribute)
   pair and return 0
*/
int enterName(SymTab * table, char *name){
   if(findName(table, name) == 1){
      return 0;
   } else {
      int index = hashFxn(table, name);
      SymEntry * temp = (SymEntry *)table->contents[index];
      //newEntry is the name being added to the table
      SymEntry * newEntry = malloc(sizeof(SymEntry));
      newEntry->name = malloc(strlen(name) * sizeof(char));
      strcpy(newEntry->name, name);
      //if the first thing in contents[index] is null then add it there
      if(temp == NULL){
         table->contents[index] = (SymEntry *)newEntry;
         table->current = table->contents[index];
      } else {
         while(temp->next != NULL){
            temp = (SymEntry *)temp->next;
         }
         temp->next = (struct SymEntry *)newEntry;
         table->current = (SymEntry *)temp->next;
         tableIndex = index;
      }
      
      
   }
   return 1;
}

/*if name is in the symbol table, set current to reference the (name, attribute)
   pair and return 1
  otherwise do not change current and return 0
*/
int findName(SymTab * table, char *name){
   SymEntry * temp = table->contents[0];
   int flag = 0;
   int index = 0;
   while(flag == 0){
      if(temp == NULL){
         index++;
         if(index < table->size){
            temp = table->contents[index];
         } else {
            return 0;
         }
      } else {
         if(strcmp(temp->name, name) == 0){
            table->current = temp;
            tableIndex = index;
            return 1;
         } else {
            temp = (SymEntry *)temp->next;
         }
      }
   }
}

//if current references a (name, attribute) pair return 1
//otherwise return 0;
int hasCurrent(SymTab * table){
   return(table->current!=NULL);
}

//PRE: hashCurrent() == 1 
//change the attribute value of the current (name, attribute) pair to attr
void setCurrentAttr(SymTab * table, void * attr){
   table->current->attribute = attr;
}

//PRE: hasCurrent() == 1
//return the attribute in the current (name, attribute) pair
void * getCurrentAttr(SymTab * table){
   return table->current->attribute;
}

//PRE: hasCurrent() == 1
//return the name in the current (name, attribute) pair
char * getCurrentName(SymTab * table){
   return table->current->name;
}

//if the symbol table is empty, return 0
//otherwise set current to the "first" (name, attribute) pair in the symbol table and return 1
int startIterator(SymTab * table){
   int flag = 0;
   int index = 0;
   SymEntry * temp = table->contents[0];
   while(flag == 0){
      if(temp == NULL){
         index++;
         if(index == table->size){
            return 0;
         } else {
            temp = table->contents[index];
         }
      } else {
         table->current = temp;
         tableIndex = index;
         return 1;
      }
   }
}

/*if all (name, attribute) pairs have been visited since the last call to
  startIterator, return 0
  otherwise set current to the "next" (name, attribute) pair and return 1
*/
int nextEntry(SymTab * table){
   int flag = 0;
   int index = tableIndex;
   SymEntry * temp = table->current;
   while(flag == 0){
      if(temp == NULL){
         index++;
         if(index == table->size){
            return 0;
         } else {
            temp = table->contents[index];
            if(temp != NULL){
               table->current = temp;
               tableIndex = index;
               return 1;
            }
         }
      } else {
         temp = (SymEntry *)temp->next;
         if(temp != NULL){
            table->current = temp;
            tableIndex = index;
            return 1;
         }
      }
   }
}

/*hash function for finding an index
  returns index from 0 to (size-1)
  pre: size >=1
*/
int hashFxn(SymTab * table, char * s){
   if(hash == (table->size - 2)){
      hash = -1;
   }
   hash++;
   return hash;
}


