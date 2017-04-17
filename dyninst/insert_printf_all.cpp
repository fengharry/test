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

	// save all registers
	bpatch.setLivenessAnalysis(false);

	// load libtest.so
    appBin->loadLibrary("libtest.so");
    std::vector<BPatch_function*> testFuncs;
    appImage->findFunction("test_func1", testFuncs);

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

            vector<BPatch_snippet*> funcArgs;
            BPatch_snippet* name = new BPatch_constExpr(funName.c_str());
            funcArgs.push_back(name);

            BPatch_funcCallExpr testCall(*(testFuncs[0]), funcArgs);

            appBin->insertSnippet(testCall, *points);
        }
        cout << "\n";
    }

    string outName = binaryPath;
    outName += ".out";
    appBin->writeFile(outName.c_str());

    return 0;
}
