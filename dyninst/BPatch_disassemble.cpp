#include <iostream>
#include <iomanip>

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

    // get modules
    BPatch_image *appImage = appBin->getImage ();
    vector < BPatch_module * > *modules = appImage->getModules();
    cout << "module number: " << modules->size() << "\n\n";
    for (BPatch_module *module : *modules)
    {
        char moduleName[1024];
        module->getName(moduleName, 1024);
        cout << "module name: " << moduleName << "\n";
        cout << "module base address: " << module->getBaseAddr()<< "\n";
        cout << "module load address: " << module->getLoadAddr()<< "\n";

        vector <BPatch_function *> * funcs = module->getProcedures();
        cout << funcs->size() << " functions in the module \n\n";
        for(BPatch_function * func : *funcs)
        {
            cout << func->getName() << "\n";
            void *addr_start, *addr_end;
            func->getAddressRange(addr_start, addr_end);
            unsigned long func_size = (unsigned long)addr_end - (unsigned long)addr_start;

            Function *f = convert(func);
            InstructionDecoder insnDecoder(f->isrc()->getPtrToInstruction(f->addr()), func_size, f->region()->getArch());
            Instruction::Ptr insn;
            unsigned long addr = (unsigned long)addr_start;
            while (addr < (unsigned long)addr_end)
            {
                insn = insnDecoder.decode();
                unsigned char* pBuf = (unsigned char*)insn->ptr();
                addr += insn->size();
                cout << left << setw(20) << addr;
                for(int i=0; i<insn->size(); i++)
                {
                    cout << setw(2) << hex << (int)pBuf[i] << " ";
                }
                cout << left << setw(48-3*insn->size()) << " ";
                cout << insn->format() << "\n";
            }
            cout << "\n";
        }
    }

    return 0;
}
