#include <iostream>
#include <iomanip>
#include "InstructionDecoder.h"

using namespace std;
using namespace Dyninst;
using namespace InstructionAPI;

int main(int argc, char **argv)
{
    const unsigned char textBuffer[] = {0x66, 0x45, 0x0f, 0x38, 0xdc, 0x45, 0xb0};
    unsigned int decoded_size = 0;

    InstructionDecoder insnDecoder(textBuffer, sizeof(textBuffer), Arch_x86_64);
    Instruction::Ptr insn;

    string strDelimiter = "----------";
    cout << strDelimiter << "start decoding" << strDelimiter << "\n\n";
    while (decoded_size < sizeof(textBuffer))
    {
        insn = insnDecoder.decode();
        cout << left << setw(20) << decoded_size << insn->format() << "\n";
        decoded_size += insn->size();
    }
    cout << "\n" << strDelimiter << "finish decoding" << strDelimiter << "\n";

    return 0;
}
