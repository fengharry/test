#include <iostream>
#include <iomanip>

#include "CodeObject.h"
#include "InstructionDecoder.h"

using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
using namespace SymtabAPI;
using namespace InstructionAPI;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << "<binary path>\n";
        return -1;
    }
    char *binaryPath = argv[1];

    SymtabCodeSource *codeSource;
    CodeObject *codeObject;
    Symtab *symTab;

    // open the binary
    string binaryPathStr(binaryPath);
    bool isParsable = Symtab::openFile(symTab, binaryPathStr);
    if (isParsable == false)
    {
        cout << "error: file can not be parsed \n";
        return -1;
    }
    else
    {
        codeSource = new SymtabCodeSource(binaryPath);
        codeObject = new CodeObject(codeSource);
        codeObject->parse();
    }

    // get modules
    vector<Module *> modules;
    symTab->getAllModules(modules);
    cout << "module number: " << modules.size() << "\n";
    for (Module *module : modules)
    {
        cout << "module name: " << module->fileName() << "\n";
    }
    cout << "\n";

    // get regions
    vector<Region *> regions;
    symTab->getAllRegions(regions);
    cout << "region number: " << regions.size() << "\n";
    for (Region *region : regions)
    {
        cout.flags(ios::left);
        cout << "region name: " << setw(30) << region->getRegionName();
        cout << "type: " << Region::regionType2Str(region->getRegionType());
        cout << "\n";
    }
    cout << "\n";

    // text region
    Region* textRegion;
    if (!symTab->findRegion(textRegion, ".text"))
    {
        cout << "couldn't find text section\n";
        return -1;
    }
    else
    {
        const unsigned char* textBuffer;
        textBuffer = (const unsigned char*) textRegion->getPtrToRawData();
        unsigned int decoded_size = 0;
        cout << "text region size: " << textRegion->getMemSize() << "\n";
        cout << "text region offset: " << hex << textRegion->getMemOffset() << "\n";
        cout << "text region alignment: " << textRegion->getMemAlignment() << "\n";
        cout << "\n";

        InstructionDecoder insnDecoder(textBuffer, textRegion->getMemSize(), codeSource->getArch());
        Instruction::Ptr insn;
        while (decoded_size < textRegion->getMemSize())
        {
            insn = insnDecoder.decode();
            decoded_size += insn->size();
            cout << insn->format() << "\n";
        }
        cout << "\n";
    }

    return 0;
}
