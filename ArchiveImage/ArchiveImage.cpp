// ArchiveImage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <fstream>
#include "EncodeImage.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Usage:  ArchiveImage <Image URL> <Encoded archive>" << endl;
        return -1;
    }

    char *URL = argv[1];
    char *File = argv[2];

    size_t size;
    
    // Call with NULL the first time to get the required buffer size.
    EncodeImage(URL, NULL, &size);

    auto buffer = make_unique<char[]>(size);
    EncodeImage(URL, buffer.get(), &size);

    ofstream ofs(File);
    ofs.write(buffer.get(), size);

    return 0;
}
