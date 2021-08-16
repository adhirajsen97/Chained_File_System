#ifndef INDEXED_FILESYSTEM_H
#define INDEXED_FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>
#include <string>
#include <vector>
#include <cstdlib>
#include "FileSystem.h"

using namespace std;

class IndexedFileSystem : public FileSystem {
public:
IndexedFileSystem();
int writeToSystem(char *, vector<int>);
vector<int> claimBlocks(int);
vector<int> findFileBlocks(char *);
void deleteFile(char *);
void displayFile(char *);
void readFile(char *, char *);
void writeFile(char *, char *);
void writeToTable(char *, vector<int>, int);
};

#endif
