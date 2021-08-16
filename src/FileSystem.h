#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>
#include <vector>

#define DATA_START 1024
#define BLOCK_OFFSET 511
#define FILE_TABLE_FIXED 64
#define MAX_BYTES 131072

using namespace std;

class FileSystem {
protected:
int fileTablePosition;
int maxBuffer;
int maxSize;
unsigned char bytes[MAX_BYTES];
public:
FileSystem();
int blockStart(int);
int writeToSystem(char *, int*);
int* claimBlocks(int);
int* findFileBlocks(char *);
virtual void deleteFile(char *);
virtual void displayFile(char *);
virtual void readFile(char *, char *);
void printBitmap();
void printBlock(int);
void printTable();
void printVector(vector<int>);
virtual void writeFile(char *, char *);
void writeToTable(char *, int*, int);
};

#endif
