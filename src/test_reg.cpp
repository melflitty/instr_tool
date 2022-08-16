/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>
#include "pin.H"
extern "C" {
#include "xed-interface.h"
}
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;

ofstream OutFile;

// The running count of instructions is kept here
// make it static to help the compiler optimize docount
static UINT64 icount = 0;
float  cptr1=0.0;
float  cptr2=0.0;
float  cptr3=0.0;
double cptr4=0.0;
// This function is called before every instruction is executed

void mul_float(float *a, float *b,float *res)
{
*res=(*a)*(*b);
cptr1=*res;
}
void mul_double(double *a, double *b,double *res)
{
*res=(*a)*(*b);
cptr4=*res;
}
void sub_float_2(float *a, float *b,float *res)
{
*res=*a-*b-*b;
cptr2=*res;
}
void sub_float(float *a, float *b,float *res)
{
*res=*a-*b;
cptr3=*res;
}
VOID docount(){ icount++;}
// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID* v)
{
    // Insert a call to docount before every instruction, no arguments are passed
      auto xed = INS_XedDec(ins); // TODO check that pointer does not need to be freed
      auto iform = xed_decoded_inst_get_iform_enum(xed);
      //OPCODE iclass = INS_Opcode(ins);
      switch(iform)
      {   
     
          
           case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_XMMss:
           //INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)docount,IARG_END);
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sub_float, IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_MEMss:
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sub_float,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_XMMd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sub_float, IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_MEMd:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)sub_float,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
            case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_XMMq:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)mul_double, IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 2),
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
             case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_MEMq:
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)mul_double,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
            
            default:
             break;
       }
       //OPCODE iclass = INS_Opcode(ins);
       //if (iclass == xed_iclass_enum_t::XED_ICLASS_ADDSS)
       
        //INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)docount,IARG_END);
        
}

KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "test-reg.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "Result of sub_float: " << cptr1 << "Result of sub_float_2:  "<< cptr2 << "Result of mul_float: " << cptr3 << "Result of mul_double" << cptr4 << endl;
    OutFile << "Count: " << icount  << endl;
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
