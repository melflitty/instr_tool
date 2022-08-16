/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>
#include "pin.H"
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;
using std::hex;
using std::dec;
using std::cout;
ofstream OutFile;


// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    //auto xed = INS_XedDec(ins);
    //auto iform = xed_decoded_inst_get_iform_enum(xed);
    // Insert a call to docount before every instruction, no arguments are passed
    OPCODE iclass = INS_Opcode(ins);
     
    
    if (INS_IsValidForIarg(ins, IARG_MULTI_ELEMENT_OPERAND))
    {
    if (iclass == XED_ICLASS_VADDPS || iclass == XED_ICLASS_VADDPD || iclass == XED_ICLASS_VADDSS || iclass == XED_ICLASS_VADDPS || iclass == XED_ICLASS_ADDPS || iclass == XED_ICLASS_ADDPD || iclass == XED_ICLASS_ADDSS || iclass == XED_ICLASS_ADDSD)
    {
        std::cout << INS_Mnemonic(ins) << std::endl;
        for (UINT32 op=0; op < INS_OperandCount(ins); op++)
        { 
            std::cout << "Number of elements of operand " << std::dec << op << " is " << INS_OperandElementCount(ins,op) << std::endl;
            if(INS_OperandElementCount(ins,op)>1)
            for (UINT32 i=0; i<INS_OperandElementCount(ins,op) ; i++)
             {
                 std::cout << "Size of  single element "<< std::dec << i <<" is  " << INS_OperandElementSize(ins,op) << " bytes " << std::endl;
             }
             
            }
    
    std::cout << "////////////////////////////" << std::endl;
}
}
}

KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "inscount.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Count " << endl;
    OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line: pin -t <toolname> -- ...    */
/* ===================================================================== */

int main(int argc, char* argv[])
{
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    OutFile.open(KnobOutputFile.Value().c_str());

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
