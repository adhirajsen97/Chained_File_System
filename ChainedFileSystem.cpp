//
//  ChainedFileSystem.cpp
//  CS4348-Project3
//
//  Created by Adhiraj Sen.
//


#include "ChainedFileSystem.h"

ChainedFileSystem::ChainedFileSystem() {
maxBuffer = 512;
maxSize = 512 * 10;
for (int i = 0; i < 131072; i++) {

bytes[i] = '0';
if (i < 512 && i % FILE_TABLE_FIXED == 0) {
bytes[i] = '~';
}
}
bytes[512] = '1';
bytes[513] = '1';
}

// abstract
void ChainedFileSystem::readFile(char* fileName, char* targetName) {
ifstream is (fileName, ifstream::binary);

if (is) {
vector<int> blocks;
int lastByte;

// get length of file:
is.seekg (0, is.end);
int length = is.tellg();
int encodingLength = length + MAX_EXTRA_ENCODING;

if (encodingLength > maxSize) {

cout << "File too big, skipping" << endl;
is.close();
}
else {

is.seekg (0, is.beg);

char * buffer = new char [length];

// read data as a block:
is.read (buffer,length);

blocks = claimBlocks(length);

if (blocks[0] == 0) {

cout << "not enough space" << endl;
}

else {

if (is) {

lastByte = writeToSystem(buffer, blocks);
writeToTable(targetName, blocks.front(), lastByte);
cout << "all characters read successfully.";
}
else {

cout << endl << "error: only " << is.gcount() << " could be read";
}
}

is.close();

delete[] buffer;
}
}
}

// TO DO: prevent over allocation
vector<int> ChainedFileSystem::claimBlocks(int fileSize) {
vector<int> blocks;
vector<int> claimedBlocks;
int tempSize = fileSize;
int counter = 0;
srand (time(NULL));
int randomNumber;
int currentBlock;

for(int i = 514; i < 768; i++) {
if (bytes[i] == '0') blocks.push_back(i);
}

while (counter < 10 && tempSize > 0) {
randomNumber = rand() % blocks.size() + 1;
currentBlock = blocks.at(randomNumber) - BLOCK_OFFSET;
claimedBlocks.push_back(currentBlock);
bytes[currentBlock + BLOCK_OFFSET] = '1';
blocks.erase(blocks.begin() + randomNumber, blocks.begin() + randomNumber);
counter++;
tempSize = tempSize - 509;

}

return claimedBlocks;
}

int ChainedFileSystem::writeToSystem(char* buffer, vector<int> blocks) {
int currentByte;
unsigned int bufferPosition = 0;
char nextBlock[4];
int blockCounter;

for (auto it = blocks.begin(); it != blocks.end(); it++) {
blockCounter = 0;
currentByte = blockStart(*it);
bytes[currentByte] = buffer[bufferPosition];
currentByte++;
bufferPosition++;
blockCounter++;

while(blockCounter < 509 && bufferPosition < strlen(buffer)) {
bytes[currentByte] = buffer[bufferPosition];
currentByte++;
bufferPosition++;
blockCounter++;
}

if (it == blocks.end()) {
for(int i = 0; i < 4; i++) {
bytes[currentByte] = '0';
currentByte++;
bytes[currentByte] = '0';
currentByte++;
bytes[currentByte] = '0';
currentByte++;
}
}
else {
sprintf(nextBlock, "%03d", *next(it, 1));
for(unsigned int i = 0; i < 4; i++) {
bytes[currentByte] = nextBlock[i];
currentByte++;
}
}
}

return currentByte - 4; // no need to write the end of the encoding to the file
}

// abstract
void ChainedFileSystem::writeToTable(char* targetName, int firstBlock, int lastByte) {
char* buffer = new char [20];
int fileTablePosition = -1;
int counter = 0;
while (fileTablePosition == -1 && counter < 512) {
if (bytes[counter] == '~') {
fileTablePosition = counter;
}
else {
counter++;
}
}

for (unsigned int i = 0; i < strlen(targetName); i++) {
bytes[fileTablePosition] = targetName[i];
fileTablePosition++;
}

bytes[fileTablePosition] = '|';
fileTablePosition++;

sprintf(buffer,"%d",lastByte);

for (unsigned int i = 0; i < strlen(buffer); i++) {
bytes[fileTablePosition] = buffer[i];
fileTablePosition++;
}

bytes[fileTablePosition] = '|';
fileTablePosition++;

sprintf(buffer,"%d", firstBlock);

for (unsigned int i = 0; i < strlen(buffer); i++) {
bytes[fileTablePosition] = buffer[i];
fileTablePosition++;
}

bytes[fileTablePosition] = '|';
fileTablePosition++;

bytes[fileTablePosition] = '\n';
fileTablePosition++;

delete[] buffer;
}

vector<int> ChainedFileSystem::findFileBlocks(char * fileName) {
vector<int> blocks;
smatch m;
string delimiter = "|";
string fileTable;
string token;
string tempName = fileName;
tempName += "\\|";
int startByte;
string nextBlock;

regex re(tempName);

for (int i = 0; i < 512; i++) {

fileTable += bytes[i];
}

regex_search(fileTable, m, re);

// find exact match in file table
if (m.position(0) != 512) {

blocks.push_back(m.position(0)); // starting position of liine on File table
token = fileTable.substr(m.position(0), fileTable.find('\n')); // copy of the whole line
token.erase(0, token.find(delimiter) + delimiter.length());
while (token.find(delimiter) + delimiter.length()) {
blocks.push_back(stoi(token.substr(0, token.find(delimiter))));
token.erase(0, token.find(delimiter) + delimiter.length());
}

for (int i = blockStart(blocks.at(2)) + 509; i < blockStart(blocks.at(2)) + 512; i++) {
nextBlock += bytes[i];
}

while(nextBlock != "000") {
blocks.push_back(stoi(nextBlock));

startByte = blockStart(stoi(nextBlock)) + 509;
nextBlock = bytes[startByte];
startByte++;
nextBlock += bytes[startByte];
startByte++;
nextBlock += bytes[startByte++];
}
}


return blocks;
}

void ChainedFileSystem::displayFile(char * fileName) {
vector<int> blocks;
blocks = findFileBlocks(fileName);
int firstByte;

printVector(blocks);

if(!blocks.empty()) {

blocks.erase(blocks.begin());
int lastByte = blocks.front();
blocks.erase(blocks.begin());

for (auto it = blocks.begin(); it != blocks.end(); it++) {
firstByte = blockStart(*it);
for (int i = firstByte; i < (firstByte + 509) && i != lastByte; i++) {
cout << bytes[i];
}
}
cout << endl;
}

else {

cout << "File " << fileName << " not found!" << endl;
}
}

void ChainedFileSystem::deleteFile(char* fileName) {
vector<int> blocks;
blocks = findFileBlocks(fileName);
int firstByte;

if(!blocks.empty()) {

// find exact match in file table
for (int i = blocks.front(); i < blocks.front() + FILE_TABLE_FIXED; i++) {
bytes[i] = '0';
if (i == blocks.front()) {
bytes[i] = '~';
}
}

blocks.erase(blocks.begin());
blocks.erase(blocks.begin());

for (auto it = blocks.begin(); it != blocks.end(); it++) {
bytes[*it + BLOCK_OFFSET] = '0';
firstByte = blockStart(*it);
for (int i = firstByte; i < firstByte + 512; i++) {
bytes[i] = '0';
}
}
}

else {
cout << "File " << fileName << " not found!" << endl;
}
}

void ChainedFileSystem::writeFile(char* fileName, char* targetName) {
vector<int> blocks;
int firstByte;
ofstream os (targetName);
blocks = findFileBlocks(fileName);

if(os && !blocks.empty()) {

blocks.erase(blocks.begin());
int lastByte = blocks.front();
blocks.erase(blocks.begin());

for (auto it = blocks.begin(); it != blocks.end(); it++) {
firstByte = blockStart(*it);
for (int i = firstByte; i < (firstByte + 509) && i != lastByte; i++) {
os << bytes[i];
}
}
os.close();

cout << "Wrote: " << fileName << " to the HDD" << endl;
}

else {

cout << "File " << fileName << " not found!" << endl;
}
}
