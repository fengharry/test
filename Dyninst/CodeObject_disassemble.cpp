#include <iostream>
#include <iomanip>

#include "CodeObject.h"
#include "InstructionDecoder.h"

using namespace std;
using namespace Dyninst;
using namespace ParseAPI;
//using namespace SymtabAPI;
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
    SymtabAPI::Symtab *symTab;
    Instruction::Ptr instr;

    // open the binary
    string binaryPathStr(binaryPath);
    bool isParsable = SymtabAPI::Symtab::openFile(symTab, binaryPathStr);
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

    std::vector<CodeRegion *> regions = codeSource->regions();
    cout << regions.size() << " regions\n";
    for(CodeRegion* region : regions)
    {
        codeObject->parseGaps(region);
    }

    //get list of all functions in the binary
    const CodeObject::funclist &all = codeObject->funcs();
    if (all.size() == 0)
    {
        cout << "error: no functions in file \n";
        return -1;
    }

    auto fit = all.begin();
    Function *f = *fit;
    //create an Instruction decoder which will convert the binary opcodes to strings
    InstructionDecoder decoder(f->isrc()->getPtrToInstruction(f->addr()),
            InstructionDecoder::maxInstructionLength, f->region()->getArch());
    for (; fit != all.end(); ++fit)
    {
        Function *f = *fit;

        //get address of entry point for current function
        Address crtAddr = f->addr();
        int instr_count = 0;
        instr = decoder.decode((unsigned char *) f->isrc()->getPtrToInstruction(crtAddr));
        auto fbl = f->blocks().end();
        fbl--;
        Block *b = *fbl;
        Address lastAddr = b->last();

        //if current function has zero instructions, don’t output it
        if (crtAddr == lastAddr)
        {
            continue;
        }

        cout << "\n\n\"" << f->name() << "\" :";
        while (crtAddr < lastAddr)
        {
            //decode current instruction
            instr = decoder.decode((unsigned char *) f->isrc()->getPtrToInstruction(crtAddr));
            cout << "\n" << hex << crtAddr;
            cout << ": \"" << instr->format() << "\"";
            crtAddr += instr->size();
            instr_count++;
        }
    }
    cout << "\n";

    return 0;
}
