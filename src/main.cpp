//
//  main.cpp
//
//  Created by Adhiraj Sen
//

//main.cpp
#include <fstream>
#include <iostream>
#include <string>
#include "CtsFileSystem.h"
#include "FileSystem.h"
#include "IndexedFileSystem.h"
#include "ChainedFileSystem.h"

using namespace std;

int main(int argc, char* argv[]) {
int input;
int blockToDisplay;

CtsFileSystem ctsFs;
IndexedFileSystem indexedFs;
ChainedFileSystem chainedFs;
FileSystem* fs;

if (argc == 2) {
switch(stoi(argv[1])) {
case 0:
fs = &ctsFs;
break;
case 1:
fs = &indexedFs;
break;
case 2:
fs = &chainedFs;
break;
default:
fs = &ctsFs;
break;
}
}
else {
fs = &ctsFs;
}

char* inFileName = new char[8];
char* copyFileName = new char[8];

while (input != 8) {
cout << "1) Display a file" << endl;
cout << "2) Display the file table" << endl;
cout << "3) Display the free space bitmap" << endl;
cout << "4) Display a disk block" << endl;
cout << "5) Copy a file from the simulation to a file on the real system" << endl;
cout << "6) Copy a file from the real system to a file in the simulation" << endl;
cout << "7) Delete a file" << endl;
cout << "8) Exit" << endl;
cin.clear();

cin >> input;

cout << "\nChoice " << input << endl;

switch(input) {
case 1: // display a file
cout << "Display file by name: ";
cin >> inFileName;
        cout << endl;
fs->displayFile(inFileName);
break;
case 2: // print FS Table
fs->printTable();
break;
case 3:
fs->printBitmap(); // print FS Bitmap
break;
case 4: // display a disk block
cout << "Display block by number: ";
cin >> blockToDisplay;
fs->printBlock(blockToDisplay);
break;
case 5: // Copy file to disk
cout << "Copy from: ";
cin >> inFileName;
cout << "Copy to: ";
cin >> copyFileName;
fs->writeFile(inFileName, copyFileName);
break;
case 6: // Copy file from disk
cout << "Copy from: ";
cin >> inFileName;
cout << "Copy to: ";
cin >> copyFileName;
fs->readFile(inFileName, copyFileName);
cout << endl << "File " << inFileName << " copied" << endl;
break;
case 7: // delete file in simuulation
cout << "Delete file: ";
cin >> inFileName;
fs->deleteFile(inFileName);
break;
case 8: // terminate
cout << "terminating" << endl;
break;
default:
cout << "Invalid choice" << endl;
break;
}

}

return 0;

}


