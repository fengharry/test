#include <iostream>
#include <iomanip>

#include "BPatch.h"
#include "BPatch_binaryEdit.h"
#include "BPatch_function.h"
#include "InstructionDecoder.h"
#include "CFG.h"
#include "CodeSource.h"

#include "PatchObject.h"
#include "PatchModifier.h"
#include "BPatch_object.h"

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
    appBin->loadLibrary("liblib1.so");

    // get image
    BPatch_image *appImage = appBin->getImage ();

    // find main entry points
    std::vector<BPatch_function*> mainFuncs;
    appImage->findFunction("main", mainFuncs);
    std::vector<BPatch_point*>* points;
    points = mainFuncs[0]->findPoint(BPatch_locEntry);

    // find lib1.so function
    std::vector<BPatch_function*> lib1Funcs;
    appImage->findFunction("lib1_func1", lib1Funcs);

    // prepare function parameters
    std::vector<BPatch_snippet*> funcArgs;

    // generate function call
    BPatch_funcCallExpr lib1Call(*(lib1Funcs[0]), funcArgs);

    appBin->insertSnippet(lib1Call, *points);

    string outName = binaryPath;
    outName += ".out";
    appBin->writeFile(outName.c_str());

    return 0;
}
