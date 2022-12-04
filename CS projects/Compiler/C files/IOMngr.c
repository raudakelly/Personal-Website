/*
 * Author: Raud Kelly
 * Date:   2/20/2020
 * Description: IO Manager .c file
 */

#include "IOMngr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static FILE* source;
static FILE* listing;
//1: listingName provided, 0: no listingName
int mode;
int line;
int col;
char sourceLine[100];

//open source file, if sourceName is null output goes to stdout
//returns 1 for listing, 0 for no listing
int openFiles(char * sourceName, char * listingName){
   source = fopen(sourceName, "r");
   if(listingName != NULL){
      mode = 1;
      listing = fopen(listingName, "w");
      line = 0;
      col = -1;
      return 1;
   }
   mode = 0;
   line = 0;
   col = -1;
   return 0;
}

//close given file
void closeFiles(){
   if(mode == 1){
      fclose(listing);
   }
   fclose(source);
}

//return the next source char
//also for echoing lines in the source file to listing file if exists
//lines are numbered
//return EOF when the end is reached
char getNextSourceChar(){
   if(col == -1){
      line++;
      col++;
      if(fgets(sourceLine, 100, source) == NULL){
         return EOF;
      }
   }   
   char next;
   if(col == strlen(sourceLine) - 1){
      next = '\n';
      col = -1;
   } else {
      next = sourceLine[col];
      col++;
   }
   if(mode == 1){
      if(col == 1){
         fprintf(listing, "%d. %s", line, sourceLine);
      }
   }
   return next;
}

//write a line containing a single '^'
void writeIndicator(int column){
   if(mode == 1){
      int i;
      for(i = 0; i < column; i++){
         fputc(' ', listing);
      }
      fputs("^\n", listing);
   } else {
      int i;
      for(i = 0; i < column; i++){
         printf(" ");
      }
      printf("^\n");
   }
}

//write the message on a separate line
void writeMessage(char * message){
   if(mode == 1){
      fputs(message, listing);
      fputs("\n", listing);
   } else {
      printf("%s\n", message);
   }
   
}

//return current line number
int getCurrentLineNum(){
   return line;
}

//return current column number
int getCurrentColumnNum(){
   return col;
}
