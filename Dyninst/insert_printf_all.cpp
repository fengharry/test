#include <iostream>
#include <iomanip>
#include <string>

#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "InstructionDecoder.h"
#include "CFG.h"
#include "CodeSource.h"

using namespace std;
using namespace Dyninst;
using namespace InstructionAPI;
using namespace ParseAPI;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << "<binary path>\n";
        return -1;
    }
    char *binaryPath = argv[1];

    BPatch bpatch;

    // open the binary
    BPatch_binaryEdit *appBin = bpatch.openBinary (binaryPath, false);
    if (appBin == NULL)
    {
        cerr << "Failed to open binary\n";
        return -1;
    }

    // get image
    BPatch_image *appImage = appBin->getImage ();
    vector < BPatch_module * > *modules = appImage->getModules();
    int muduleNum = modules->size();
    cout << "module number: " << muduleNum << "\n\n";

    // load libc for printf
    appBin->loadLibrary("libc.so.6");

    // find printf
    vector<BPatch_function*> printfFuncs;
    appImage->findFunction("printf", printfFuncs);
    if (printfFuncs.size() == 0)
    {
        cerr << "Could not find printf\n";
        return -1;
    }

    cout << "module number: " << modules->size() << "\n\n";
    for (int i=0; i<muduleNum; i++)
    {
        char moduleName[1024];
        (*modules)[i]->getName(moduleName, 1024);
        string strModule(moduleName);
        cout << "module name: " << strModule << "\n";

        vector <BPatch_function *> * funcs = (*modules)[i]->getProcedures();
        cout << funcs->size() << " functions in the module \n\n";
        for(BPatch_function * func : *funcs)
        {
            string funName = func->getName();
            funName = "####\t" + funName + "\n";
            cout << funName;

            vector<BPatch_point*>* points = func->findPoint(BPatch_locEntry);

            vector<BPatch_snippet*> printfArgs;
            BPatch_snippet* fmt = new BPatch_constExpr(funName.c_str());
            printfArgs.push_back(fmt);

            BPatch_funcCallExpr printfCall(*(printfFuncs[0]), printfArgs);

            appBin->insertSnippet(printfCall, *points);
        }
        cout << "\n";
    }

    string outName = binaryPath;
    outName += ".out";
    appBin->writeFile(outName.c_str());

    return 0;
}
