#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable* st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception& e)
    {
        cout << e.what();
    }
    delete st;
}

void validSubmittedFiles(string filename, string* allowedIncludingFiles, int numOfAllowedIncludingFiles = 1)
{
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        if (line.find("#include") != string::npos)
        {
            for (int i = 0; i < numOfAllowedIncludingFiles; i++)
            {
                if (line.find(allowedIncludingFiles[i]) != string::npos)
                    continue;
                cout << "Use disallowed file: " + line << endl;
                exit(1);
            }
        }
    }
    infile.close();
}

int main(int argc, char** argv)
{
    /*if (argc < 2)
        return 1;

    string allowedH[] = { "main.h" };
    validSubmittedFiles("SymbolTable.h", allowedH);

    string allowedCPP[] = { "SymbolTable.h" };
    validSubmittedFiles("SymbolTable.cpp", allowedCPP);
    test(argv[1]);*/
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    test("__test__.txt");
    _CrtDumpMemoryLeaks();
    return 0;
}