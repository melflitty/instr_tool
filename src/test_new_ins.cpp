/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>
#include "pin.H"
#include <unistd.h>
extern "C" {
#include "xed-interface.h"
}
//#include "utils/copy_register.h"
#include <immintrin.h>
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;

ofstream OutFile;

static REG tmp_reg1;
static REG tmp_reg2;
      
// The running count of instructions is kept here
// make it static to help the compiler optimize docount
//static UINT64 icount = 0;
static float cptr1=0;
static double cptr2=0;
static UINT64 icountf=0;
static UINT64 icountd=0;

void mul_float(float a, float b,float* res)
{
icountf++;
OutFile << "I reached mul_float function for the "<< icountf << " time"<< std::endl;
*res=a*b;
cptr1=*res;

}
void mul_double(double a, double b,double* res)
{
icountd++;
OutFile << "I reached mul_double function for the "<< icountd << " time"<< std::endl;
*res=a*b;
cptr2=*res;

}
template<int N>
    static void PIN_FAST_ANALYSIS_CALL copy_register(char* dst_, char* const src_)
    { 
     OutFile << "I reached copy register" << std::endl;
      for (int i = 0; i < N; ++i)
      {
        dst_[i] = src_[i];
      }
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<1>(char* dst, char* const src)
    { 
      OutFile << "I reached copy register 1" << std::endl;
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<2>(char* dst_, char* const src_)
    { 
      OutFile << "I reached copy register 2" << std::endl;
      auto dst = reinterpret_cast<UINT16*>(dst_);
      auto src = reinterpret_cast<UINT16*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<4>(char* dst_, char* const src_)
    {
      OutFile << "I reached copy register 4" << std::endl;
      auto dst = reinterpret_cast<UINT32*>(dst_);
      auto src = reinterpret_cast<UINT32*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<8>(char* dst_, char* const src_)
    {
      OutFile << "I reached copy register 8" << std::endl;
      auto dst = reinterpret_cast<UINT64*>(dst_);
      auto src = reinterpret_cast<UINT64*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<16>(char* dst_, char* const src_)
    {
      OutFile << "I reached copy register 16" << std::endl;
      auto dst = reinterpret_cast<__m128*>(dst_);
      auto src = reinterpret_cast<__m128*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<32>(char* dst_, char* const src_)
    {
      OutFile << "I reached copy register 32" << std::endl;
      auto dst = reinterpret_cast<__m256*>(dst_);
      auto src = reinterpret_cast<__m256*>(src_);
      dst[0] = src[0];
    }
    template<>
    void PIN_FAST_ANALYSIS_CALL copy_register<64>(char* dst_, char* const src_)
    {
      OutFile << "I reached copy register 64" << std::endl;
      auto dst = reinterpret_cast<__m512*>(dst_);
      auto src = reinterpret_cast<__m512*>(src_);
      dst[0] = src[0];
    }

// This function is called before every instruction is executed
template <int N, bool uint8 = N <= 8, bool uint16 = N <= 16, bool uint32 = N <= 32>
        struct get_mask
        {
          using type = UINT64;
        };

        template<int N> struct get_mask<N, true, true, true>
        {
          using type = UINT8;
        };

        template<int N> struct get_mask<N, false, true, true>
        {
          using type = UINT16;
        };

        template<int N> struct get_mask<N, false, false, true>
        {
          using type = UINT32;
        };


        template <int SIZE>
        static auto save_registers(INS ins, xed_bool_t is_memory, REG tmp_reg1, REG tmp_reg2,UINT32 numOperands)
        { 
          if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
          OutFile << "I arrived at save registers" << std::endl;
          REG reg1 = INS_OperandReg(ins, 1);
          REG reg2 ;
          
          /*
          if (INS_OperandReg(ins, 0) == reg1)
          { OutFile << "Yes first op equals reg1"<< std::endl;
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_END);
            reg1 = tmp_reg1;
          }
          */
        
          
          if (is_memory)
          { OutFile << "is memory true" << std::endl;
            
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_MEMORYREAD_EA,
              IARG_END);    
            OutFile << "copy " << std::endl;  
            reg2 = tmp_reg2;
            OutFile << "There is a memory operand" << std::endl;
          }
          else
          { 
            OutFile << "There is no memory operand" << std::endl;
          
            
           if(numOperands>3)
           {
            reg2 = INS_OperandReg(ins, 3);
            // if result is overwritten in operand, save the operand
            if (INS_OperandReg(ins, 0) == reg2)
            { OutFile << "yes reg2 equals op0" << std::endl;
              if (INS_OperandReg(ins, 1) == reg2)
              { OutFile << "yes reg2 equals op1 too" << std::endl;
                reg2 = reg1;
              }
              else
              { 
                OutFile << "I am here before the call of insert call" << std::endl;
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_register<SIZE>,
                  IARG_FAST_ANALYSIS_CALL,
                  IARG_REG_REFERENCE, tmp_reg2,
                  IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 3),
                  IARG_END);
                reg2 = tmp_reg2;
                
              }
            }
          }
          else
           reg2 = INS_OperandReg(ins, 2);
          }
          struct RetVals
          {
            REG r1;
            REG r2;
          };
          OutFile << "I will return from save registers" << std::endl;
          return RetVals{ reg1, reg2 };
         
        }

        template<int NB_ELTS_IN_REG, typename T, void (*op)(T, T, T*)>
        struct wrap {
          using mask_t = typename get_mask<NB_ELTS_IN_REG>::type;

          template <int I>
          static bool PIN_FAST_ANALYSIS_CALL check_mask(mask_t* const mask)
          {
            return ((*mask) & ((1) << (I))) != 0;
          }

          template <bool broadcast, int I>
          static void PIN_FAST_ANALYSIS_CALL compute(T* const a, T* const b, T* c)
          { OutFile << "I arrived at compute" << std::endl;
            if constexpr (broadcast)
            {
              op(a[I], b[0], &c[I]);
            }
            else
            { OutFile << "It is not broadcast" << std::endl;
              op(a[I], b[I], &c[I]);
            }
          }

          template<int N = NB_ELTS_IN_REG, int I = 0>
          static void apply(INS ins, xed_bool_t is_broadcast, xed_bool_t is_memory,bool ismasking, REG tmp_reg1, REG tmp_reg2,UINT32 numOperands)
          { OutFile << "I arrived at apply" << std::endl;
            if constexpr (I < N)
            { 
              OutFile << "still I < N" << std::endl;
              if constexpr (I == 0)
              { 
                OutFile << "Yes I equals zero" << std::endl;
                auto [reg1, reg2] = save_registers<N * sizeof(T)>(ins, is_memory, tmp_reg1, tmp_reg2,numOperands);
                tmp_reg1 = reg1;
                tmp_reg2 = reg2;
              }
              if(!ismasking)
              {
              OutFile << "I arrived at non masking " << std::endl;
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)compute<false,I>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
              OutFile << "I finished non masking!" << std::endl;
              apply<N, I + 1>(ins, is_broadcast, is_memory,ismasking, tmp_reg1, tmp_reg2,numOperands);
              }
              else
              {
              INS_InsertIfCall(ins, IPOINT_AFTER, (AFUNPTR)check_mask<I>,
                IARG_FAST_ANALYSIS_CALL,
                IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
                IARG_END
              );
              INS_InsertThenCall(ins, IPOINT_AFTER, (AFUNPTR)compute<false, I>,
                IARG_FAST_ANALYSIS_CALL,
                IARG_REG_CONST_REFERENCE, tmp_reg1,
                IARG_REG_CONST_REFERENCE, tmp_reg2,
                IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
                IARG_END);

              apply<N, I + 1>(ins, is_broadcast, is_memory,ismasking, tmp_reg1, tmp_reg2,numOperands);
              }

              
            }
          }
        };
 

VOID instrument_reg_reg(INS ins,void (*interflop_fct)(float*,float*,float*), REG reg1, REG reg2, REG regOutput)
{      
       
       INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)mul_float,
       IARG_FAST_ANALYSIS_CALL, 
       IARG_REG_CONST_REFERENCE,reg1,
       IARG_REG_CONST_REFERENCE,reg2,
       IARG_REG_REFERENCE,regOutput,
       IARG_END
       );
       INS_Delete(ins);
}

VOID Instruction(INS ins, VOID* v)
{    
    if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
      auto xed = INS_XedDec(ins);
      UINT32 numOperands;
      numOperands = INS_OperandCount(ins);
      auto iform = xed_decoded_inst_get_iform_enum(xed);
     
          
          
             if (iform == xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_XMMd)
             {
             OutFile<< "///////////////////////////////////" << std::endl;
             OutFile << "Found one reg reg avx!" << std::endl;
             UINT32 nb=INS_OperandElementCount(ins,0);
             wrap<1, float, mul_float>::template apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
             INS_Delete(ins);
             }
             else if (iform == xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_MEMd)
             {
            OutFile<< "///////////////////////////////////" << std::endl; 
             OutFile << "Found one reg mem avx !" << std::endl;
             wrap<1, float, mul_float>::template apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
            
             INS_Delete(ins);
               
             }
             else if(iform == xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_XMMss)
             { 
             OutFile<< "///////////////////////////////////" << std::endl;
             OutFile << "Found one reg reg sse  !" << std::endl;
             wrap<1, float, mul_float>::template apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
             
             INS_Delete(ins);

             }
             else if (iform == xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_MEMss)
             {
            OutFile<< "///////////////////////////////////" << std::endl;
             OutFile << "Found one reg mem sse !" << std::endl;
             wrap<1, float, mul_float>::template apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
             
             INS_Delete(ins);
             }
             else if (iform == xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_XMMdq)
             {
             OutFile<< "///////////////////////////////////" << std::endl;
             OutFile << "Found one reg reg avx!" << std::endl;
             wrap<4, float, mul_float>::template apply<1>(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
             
             INS_Delete(ins);
             }
             else if(iform == xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_XMMdq)
             {
             OutFile<< "///////////////////////////////////" << std::endl;
             OutFile << "Found one reg reg avx!" << std::endl;
             wrap<2, double, mul_double>::template apply(ins, xed_decoded_inst_uses_embedded_broadcast(xed),INS_IsMemoryRead(ins),false, tmp_reg1, tmp_reg2,numOperands);
            
             INS_Delete(ins);
             }
             
       
       
        
}

KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "test-reg.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "End of instrumentation" << std::endl;
    OutFile << "result is " << cptr1 << endl;
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
    tmp_reg1=PIN_ClaimToolRegister();
    tmp_reg2=PIN_ClaimToolRegister();
    if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2))
        {
          std::cerr << "Cannot allocate a scratch register.\n";
          std::cerr << std::flush;
          PIN_ExitApplication(1);
        }
     
    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
