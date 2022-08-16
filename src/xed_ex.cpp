#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
extern "C" {
#include "xed-interface.h"
}
std::ofstream outFile;

static UINT64 icount_mul_scal_single = 0;
static UINT64 icount_mul_scal_double = 0;
static UINT64 icount_mul_packed_single = 0;
static UINT64 icount_mul_packed_double = 0;
static UINT64 icount_add_scal_single = 0;
static UINT64 icount_add_scal_double = 0;
static UINT64 icount_add_packed_single = 0;
static UINT64 icount_add_packed_double = 0;
static UINT64 icount_sub_scal_single = 0;
static UINT64 icount_sub_scal_double = 0;
static UINT64 icount_sub_packed_single = 0;
static UINT64 icount_sub_packed_double = 0;
static UINT64 icount_div_scal_single = 0;
static UINT64 icount_div_scal_double = 0;
static UINT64 icount_div_packed_single = 0;
static UINT64 icount_div_packed_double = 0;

enum VEC{
    SCALAR,
    PACKED
};

enum PREC{
    FLOAT,
    DOUBLE
    };
enum Op{
    ADD,
    SUB,
    MUL,
    DIV
};

bool IsAdd(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_ADDSS:
      return true;
    default:
      return false;
 
}
    return false;
}
bool IsSub(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_SUBPD:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_SUBSS:
      return true;
    default:
      return false;
 
}
    return false;
}
bool IsMul(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_MULPD:
    case XED_ICLASS_MULPS:
    case XED_ICLASS_MULSD:
    case XED_ICLASS_MULSS:
      return true;
    default:
      return false;
    
    } 
return false;
 
}
bool IsDiv(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_DIVSS:
      return true;
    default:
      return false;
    
    } 
return false;
 
}
bool IsScalar(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_MULSS:
    case XED_ICLASS_DIVSS:
    case XED_ICLASS_SUBSS:
      return true;
    default:
      return false;
       
}
return false;
 
}
bool IsPacked(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_MULPS:
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_MULPD:
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_SUBPD:
      return true;
    default:
      return false;
}
return false;
 
}
bool IsSingle_prec(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_MULSS:
    case XED_ICLASS_DIVSS:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_MULPS:
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_SUBPS:
      return true;
    default:
      return false;
       
}
return false;
 
}
bool IsDouble_prec(OPCODE iclass)
{  switch(iclass){
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_MULSD:
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_MULPD:
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_SUBPD:
      return true;
    default:
      return false;
       
}
return false;
 
}
bool IsFP(OPCODE iclass)
{ switch (iclass){
    //add
    case XED_ICLASS_ADDPD:
    case XED_ICLASS_ADDPS:
    case XED_ICLASS_ADDSD:
    case XED_ICLASS_ADDSS:
    //addsub
    case XED_ICLASS_ADDSUBPD:
    case XED_ICLASS_ADDSUBPS:
    //compare
    case XED_ICLASS_CMPPD:
    case XED_ICLASS_CMPPS:
    case XED_ICLASS_CMPSD:
    case XED_ICLASS_CMPSS:
    case XED_ICLASS_COMISD:
    case XED_ICLASS_COMISS:
    //div
    case XED_ICLASS_DIVPD:
    case XED_ICLASS_DIVPS:
    case XED_ICLASS_DIVSD:
    case XED_ICLASS_DIVSS:
    //mul 
    case XED_ICLASS_MULPD:
    case XED_ICLASS_MULPS:
    case XED_ICLASS_MULSD:
    case XED_ICLASS_MULSS:
    //sub
    case XED_ICLASS_SUBPD:
    case XED_ICLASS_SUBPS:
    case XED_ICLASS_SUBSD:
    case XED_ICLASS_SUBSS:
    //FMAdd
    case XED_ICLASS_VFMADD132PD:
    case XED_ICLASS_VFMADD132PS:
    case XED_ICLASS_VFMADD132SD:
    case XED_ICLASS_VFMADD132SS:
    case XED_ICLASS_VFMADD213PD:
    case XED_ICLASS_VFMADD213PS:
    case XED_ICLASS_VFMADD213SD:
    case XED_ICLASS_VFMADD213SS:
    case XED_ICLASS_VFMADD231PD:
    case XED_ICLASS_VFMADD231PS:
    case XED_ICLASS_VFMADD231SD:
    case XED_ICLASS_VFMADD231SS:
    //FMAddSub
    case XED_ICLASS_VFMADDSUB132PD:
    case XED_ICLASS_VFMADDSUB132PS:
    case XED_ICLASS_VFMADDSUB213PD:
    case XED_ICLASS_VFMADDSUB213PS:
    case XED_ICLASS_VFMADDSUB231PD:
    case XED_ICLASS_VFMADDSUB231PS:
    //FMSub
    case XED_ICLASS_VFMSUB132PD:
    case XED_ICLASS_VFMSUB132PS:
    case XED_ICLASS_VFMSUB132SD:
    case XED_ICLASS_VFMSUB132SS:
    case XED_ICLASS_VFMSUB213PD:
    case XED_ICLASS_VFMSUB213PS:
    case XED_ICLASS_VFMSUB213SD:
    case XED_ICLASS_VFMSUB213SS:
    case XED_ICLASS_VFMSUB231PD:
    case XED_ICLASS_VFMSUB231PS:
    case XED_ICLASS_VFMSUB231SD:
    case XED_ICLASS_VFMSUB231SS:
    //FMSubAdd
    case XED_ICLASS_VFMSUBADD132PD:
    case XED_ICLASS_VFMSUBADD132PS:
    case XED_ICLASS_VFMSUBADD213PD:
    case XED_ICLASS_VFMSUBADD213PS:
    case XED_ICLASS_VFMSUBADD231PD:
    case XED_ICLASS_VFMSUBADD231PS:

       return true;

    default:
        return false;
    

}
return false;
}

VOID count_mul_scal_single(){ icount_mul_scal_single++; }
VOID count_mul_scal_double(){ icount_mul_scal_double++;}
VOID count_mul_packed_single(){ icount_mul_packed_single++;}
VOID count_mul_packed_double(){ icount_mul_packed_double++;}
VOID count_add_scal_single(){ icount_add_scal_single++;}
VOID count_add_scal_double(){ icount_add_scal_double++;}
VOID count_add_packed_single(){ icount_add_packed_single++;}
VOID count_add_packed_double(){ icount_add_packed_double++;}
VOID count_sub_scal_single(){ icount_sub_scal_single++;}
VOID count_sub_scal_double(){ icount_sub_scal_double++;};
VOID count_sub_packed_single(){ icount_sub_packed_single++;}
VOID count_sub_packed_double(){ icount_sub_packed_double++;}
VOID count_div_scal_single(){ icount_div_scal_single++;}
VOID count_div_scal_double(){icount_div_scal_double++;}
VOID count_div_packed_single(){icount_div_packed_single++;}
VOID count_div_packed_double(){icount_div_packed_double++;}


VOID Trace(TRACE trace, VOID *v)
{
     for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
        {  for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
            {   //const xed_decoded_inst_t* xed_ins = INS_XedDec(ins);
                //xed_iclass_enum_t inst_class= xed_decoded_inst_get_iclass(xed_ins);
                OPCODE iclass = INS_Opcode(ins);
                
                //UINT32 numOperands = INS_OperandCount(ins);
                if(IsFP(iclass))
                {   REG operand1 = INS_OperandReg(ins, 0);
                  if(IsMul(iclass))
                 { if(IsScalar(iclass))
                       {if(IsSingle_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul_scal_single, IARG_END);
            
                        else 
                        { if (IsDouble_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul_scal_double, IARG_END); 
                        }}
                    else if (IsPacked(iclass))
                    {
                       if(IsSingle_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul_packed_single, IARG_END);
                        else 
                        { if (IsDouble_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_mul_packed_double, IARG_END);  }
                        }
                    
                    
                }
                
                  if(IsAdd(iclass))
                 { if(IsScalar(iclass))
                       {if(IsSingle_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add_scal_single, IARG_END);
                        else 
                        { if (IsDouble_prec(iclass))
                           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add_scal_double, IARG_END);   
                        }}
                    else if (IsPacked(iclass))
                    {
                       if(IsSingle_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add_packed_single, IARG_END);
                        else 
                        { if (IsDouble_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_add_packed_single, IARG_END);}
                        
                    
                    }
                }
                
                  if(IsSub(iclass))
                 { if(IsScalar(iclass))
                       {if(IsSingle_prec(iclass))
                           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub_scal_single, IARG_END);
                        else 
                        { if (IsDouble_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub_scal_double, IARG_END);  
                        }}
                    else if (IsPacked(iclass))
                    {
                       if(IsSingle_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub_packed_single, IARG_END);
                        else 
                        { if (IsDouble_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_sub_packed_double, IARG_END);  }
                    }
                 } 
                    
                    
        
                  if(IsDiv(iclass))
                 { if(IsScalar(iclass))
                       {if(IsSingle_prec(iclass))
                            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div_scal_single, IARG_END);  
                        else 
                        { if (IsDouble_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div_scal_double, IARG_END);   
                        }}
                    else if (IsPacked(iclass))
                    {if(IsScalar(iclass))
                       {if(IsSingle_prec(iclass))
                             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div_packed_single, IARG_END);  
                        else 
                        { if (IsDouble_prec(iclass))
                                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)count_div_packed_double, IARG_END);  }
                        }
                    
                    }
                }
                }
             
             }    

        }
}
INT32 Usage()
{
    std::cerr << "This tool prints IA-32 and Intel(R) 64 instructions" << std::endl;
    std::cerr << KNOB_BASE::StringKnobSummary();
    std::cerr << std::endl;
    return -1;
}
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    outFile.setf(std::ios::showbase);
    outFile << "Count multiplication - float - scalar:" << icount_mul_scal_single << std::endl;
    outFile << "Count multiplication - double - scalar:" << icount_mul_scal_double << std::endl;
    outFile << "Count multiplication - float - packed:" << icount_mul_packed_single << std::endl;
    outFile << "Count multiplication - double - packed:" << icount_mul_packed_double << std::endl;

    outFile << "Count addition - float - scalar:" << icount_add_scal_single << std::endl;
    outFile << "Count addition - double - scalar:" << icount_add_scal_double << std::endl;
    outFile << "Count addition - float - packed:" << icount_add_packed_single << std::endl;
    outFile << "Count addition - double - packed:" << icount_add_packed_double << std::endl;

    outFile << "Count subtraction - float - scalar:" << icount_sub_scal_single << std::endl;
    outFile << "Count subtraction - double - scalar:" << icount_sub_scal_double << std::endl;
    outFile << "Count subtraction - float - packed:" << icount_sub_packed_single << std::endl;
    outFile << "Count  subtraction - double - packed:" << icount_sub_packed_double << std::endl;

     outFile << "Count  division - float - scalar:" << icount_div_scal_single << std::endl;
    outFile << "Count division - double - scalar:" << icount_div_scal_double << std::endl;
    outFile << "Count  division - float - packed:" << icount_div_packed_single << std::endl;
    outFile << "Count  division - double - packed:" << icount_div_packed_double << std::endl;
   
    outFile << "Finish" << std::endl;
    outFile.close();
}

KNOB<std::string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool","o", "test.out", "specify output file name");
int main(int argc, char * argv[])
{   // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

   outFile.open(KnobOutputFile.Value().c_str());
 

    TRACE_AddInstrumentFunction(Trace , 0);
    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
