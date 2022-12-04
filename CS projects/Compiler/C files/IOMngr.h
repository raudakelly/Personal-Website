/*
 * Author: Raud Kelly
 * Date:   2/20/2020
 * Description: IO Manager .h file
 */
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 1024

int openFiles(char * sourceName, char * listingName);
//open source file, if sourceName is null output goes to stdout

void closeFiles();
//close given file

char getNextSourceChar();
//return the next source char
//also for echoing lines in the source file to listing file if exists
//lines are numbered
//return EOF when the end is reached

void writeIndicator(int column);
//write a line containing a single '^'

void writeMessage(char * message);
//write the message on a separate line

int getCurrentLineNum();
//return current line number

int getCurrentColumnNum();
//return current column number
