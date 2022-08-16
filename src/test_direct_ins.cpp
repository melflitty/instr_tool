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
static UINT64 icountf=0;
//static UINT64 icountd=0;


static REG tmp_reg1;
static REG tmp_reg2;
static REG tmp_reg_output;
static REG tmp_reg_first;
template<typename T, void (*op)(T, T, T*)>
void wrap(T* const a, T* const b, T* c)
 {
             op(*a,*b,c);
        }

void mul_float(float a, float b,float* res)
{
icountf++;
OutFile << "I reached mul_float function for the "<< icountf << " time"<< std::endl;
*res=(a)*(b);


}
void add_float(float a, float b, float* cptr)
 { 
    OutFile << "I reached add float" << std::endl;
     *cptr = (a) + (b);
     auto x=*cptr;
     OutFile << (a) << " + "  <<(b) << " equals " << x <<  std::endl;
     OutFile << "//////////////////////////" << std::endl;
      }
    
void sub_float(float a, float b, float* cptr)
 { 
    OutFile << "I reached sub float" << std::endl;
     *cptr = (a) - (b);
     auto  x=*cptr;
     OutFile << (a) << " - "  <<(b) << " equals " << x <<  std::endl;
     OutFile << "//////////////////////////" << std::endl;
      }

void div_float(float a, float b, float* cptr)
 { 
    OutFile << "I reached div float" << std::endl;
     *cptr = (a) / (b);
      auto  x=*cptr;
     OutFile << (a) << " / "  <<(b) << " equals " << x <<  std::endl;
     OutFile << "//////////////////////////" << std::endl;
      }

void add_double(double a, double b, double* cptr) 
{ 
  OutFile << "I reached add double" << std::endl;
  *cptr = a + b;
   auto  x=*cptr;
     OutFile << (a) << " + "  <<(b) << " equals " << x <<  std::endl;
     OutFile << "//////////////////////////" << std::endl;
   }
void sub_double(double a, double b, double* cptr) 
{
      OutFile << "I reached sub double" << std::endl;
     *cptr = a - b; 
     auto  x=*cptr;
     OutFile << (a) << " + "  <<(b) << " equals " << x <<  std::endl;
     OutFile << "//////////////////////////" << std::endl;
      }
void mul_double(double a, double b, double* cptr)
 { 
      OutFile << "I reached mul double" << std::endl;
      *cptr = a * b; 
      auto  x=*cptr;
      OutFile << (a) << " + "  <<(b) << " equals " << x <<  std::endl;
      OutFile << "//////////////////////////" << std::endl;
      }
void div_double(double a, double b, double* cptr)
 {
      OutFile << "I reached div double" << std::endl;
       *cptr = a / b; 
        auto  x=*cptr;
      OutFile << (a) << " + "  <<(b) << " equals " << x <<  std::endl;
      OutFile << "//////////////////////////" << std::endl;
 }


    
    template<typename T>
    void PIN_FAST_ANALYSIS_CALL copy_from_input(T* src,T* dst,int i)
    {
      OutFile << "I arrived at copy register from input for the  "  << i  << " time" << std::endl;
     (*dst)=(src)[i];


    }
    template<typename T>
    void PIN_FAST_ANALYSIS_CALL copy_to_output(T* src,T* dst,int i)
    {
      OutFile << "I arrived at copy register to output for the  "  << i  << " time" << std::endl;
      dst[i]=(*src);

    }
    
    /*
    void PIN_FAST_ANALYSIS_CALL copy_from_input(float* src,float* dst,int i)
    {
      OutFile << "I arrived at copy register from input for the  "  << i  << " time" << std::endl;
      (*dst)=(src)[i];


    }
    
    void PIN_FAST_ANALYSIS_CALL copy_to_output(float* src,float* dst,int i)
    {
      OutFile << "I arrived at copy register to output for the  "  << i  << " time" << std::endl;
      dst[i]=(*src);

    }*/

 
VOID Instruction(INS ins, VOID* v)
{    
      
      auto xed = INS_XedDec(ins);
      auto iform = xed_decoded_inst_get_iform_enum(xed);
      switch(iform)
      {
      
       case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_XMMss:
       {   
          OutFile << "Found one addss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_ADDSS_XMMss_MEMss:
       {
            OutFile << "Found one addss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_XMMss:
       {   
          OutFile << "Found one subss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBSS_XMMss_MEMss:
       {
            OutFile << "Found one subss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_XMMss:
       {   
          OutFile << "Found one mulss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULSS_XMMss_MEMss:
       {
            OutFile << "Found one mulss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_XMMss:
       {   
          OutFile << "Found one divss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVSS_XMMss_MEMss:
       {
            OutFile << "Found one divss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,div_float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_ADDSD_XMMsd_XMMsd:
       {   
          OutFile << "Found one addsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_ADDSD_XMMsd_MEMsd:
       {
            OutFile << "Found one addsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,add_double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBSD_XMMsd_XMMsd:
       {   
          OutFile << "Found one subsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBSD_XMMsd_MEMsd:
       {
            OutFile << "Found one subsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,sub_double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULSD_XMMsd_XMMsd:
       {   
          OutFile << "Found one mulsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULSD_XMMsd_MEMsd:
       {
            OutFile << "Found one mulsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,mul_double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVSD_XMMsd_XMMsd:
       {   
          OutFile << "Found one divsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVSD_XMMsd_MEMsd:
       {
            OutFile << "Found one divsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,div_double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_XMMps:
       {
        
        OutFile << "Found one addps reg reg!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
            
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       
      case xed_iform_enum_t::XED_IFORM_ADDPS_XMMps_MEMps:
       {
          OutFile << "Found one addps reg mem!" <<std::endl;
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_XMMps:
       {
        
        OutFile << "Found one subps reg reg!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_SUBPS_XMMps_MEMps:
       {
          OutFile << "Found one subps reg mem!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_XMMps:
       {
        
        OutFile << "Found one mulps reg reg!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_MULPS_XMMps_MEMps:
       {
          OutFile << "Found one mulps reg mem!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_XMMps:
       {
        
        OutFile << "Found one divps reg reg!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_DIVPS_XMMps_MEMps:
       {
          OutFile << "Found one divps reg mem!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       //les pd 
       
       case xed_iform_enum_t::XED_IFORM_ADDPD_XMMpd_XMMpd:
       {
        
        OutFile << "Found one addpd reg reg!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
          
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_ADDPD_XMMpd_MEMpd:
       {
          OutFile << "Found one addpd reg mem!" <<std::endl;
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_SUBPD_XMMpd_XMMpd:
       {
        
        OutFile << "Found one subpd reg reg!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_SUBPD_XMMpd_MEMpd:
       {
          OutFile << "Found one subpd reg mem!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_MULPD_XMMpd_XMMpd:
       {
        
        OutFile << "Found one mulpd reg reg!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_MULPD_XMMpd_MEMpd:
       {
          OutFile << "Found one mulpd reg mem!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_DIVPD_XMMpd_XMMpd:
       {
        
        OutFile << "Found one divpd reg reg!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_DIVPD_XMMpd_MEMpd:
       {
          OutFile << "Found one divpd reg mem!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,0),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }

       // les avx
       
         case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_XMMd:
       {   
          OutFile << "Found one vaddss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VADDSS_XMMdq_XMMdq_MEMd:
       {
            OutFile << "Found one vaddss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,add_float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_XMMd:
       {   
          OutFile << "Found one vsubss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBSS_XMMdq_XMMdq_MEMd:
       {
            OutFile << "Found one vsubss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,sub_float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_XMMd:
       {   
          OutFile << "Found one vmulss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULSS_XMMdq_XMMdq_MEMd:
       {
            OutFile << "Found one vmulss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,mul_float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_XMMd:
       {   
          OutFile << "Found one vdivss reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVSS_XMMdq_XMMdq_MEMd:
       {
            OutFile << "Found one vdivss reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_XMMq:
       {   
          OutFile << "Found one vaddsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VADDSD_XMMdq_XMMdq_MEMq:
       {
            OutFile << "Found one vaddsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_XMMq:
       {   
          OutFile << "Found one vsubsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBSD_XMMdq_XMMdq_MEMq:
       {
            OutFile << "Found one vsubsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_XMMq:
       {   
          OutFile << "Found one mulsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULSD_XMMdq_XMMdq_MEMq:
       {
            OutFile << "Found one mulsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_XMMq:
       {   
          OutFile << "Found one divsd reg reg!" <<std::endl; 
           INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVSD_XMMdq_XMMdq_MEMq:
       {
            OutFile << "Found one divsd reg mem!" <<std::endl; 
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, INS_OperandReg(ins, 0),
              IARG_END);
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vaddps reg reg xmm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
            
        }
           
           
            INS_Delete(ins);
            break;
       }
       
      case xed_iform_enum_t::XED_IFORM_VADDPS_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vaddps reg mem xmm!" <<std::endl;
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       
        case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_YMMqq:
        {
        
        OutFile << "Found one vaddps reg reg ymm!" <<std::endl;
       
         for(int i=0;i<8;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
         
        }
           
           
            INS_Delete(ins);
            break;
       }
       
        case xed_iform_enum_t::XED_IFORM_VADDPS_YMMqq_YMMqq_MEMqq:
         {
        
        OutFile << "Found one vaddps reg mem ymm!" <<std::endl;
       
         for(int i=0;i<8;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,add_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        }
           
           
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vsubps reg reg xmm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VSUBPS_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vsubps reg mem xmm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_YMMqq:
       {
        
        OutFile << "Found one vsubps reg reg ymm!" <<std::endl;
       
         for(int i=0;i<8;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBPS_YMMqq_YMMqq_MEMqq:
       {
          OutFile << "Found one vsubps reg mem xmm!" <<std::endl;
         
       for(int i=0;i<8;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,sub_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vmulps reg reg xmm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VMULPS_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vmulps reg mem xmm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_YMMqq:
       {
        
        OutFile << "Found one vmulps reg reg ymm!" <<std::endl;
       
         for(int i=0;i<8;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULPS_YMMqq_YMMqq_MEMqq:
       {
          OutFile << "Found one vmulps reg mem ymm!" <<std::endl;
         
       for(int i=0;i<8;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,mul_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vdivps reg reg xmm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VDIVPS_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vdivps reg mem xmm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_YMMqq:
       {
        
        OutFile << "Found one vdivps reg reg ymm!" <<std::endl;
       
         for(int i=0;i<8;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<float>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VDIVPS_YMMqq_YMMqq_MEMqq:
       {
          OutFile << "Found one vdivps reg mem ymm!" <<std::endl;
         
       for(int i=0;i<8;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<float,div_float>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       //les pd 
       
       case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vaddpd reg reg xmm!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
          
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VADDPD_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vaddpd reg mem xmm!" <<std::endl;
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_YMMqq:
       {
        
        OutFile << "Found one vaddpd reg reg ymm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
          
        }
           
           
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_VADDPD_YMMqq_YMMqq_MEMqq:
         {
          OutFile << "Found one vaddpd reg mem ymm!" <<std::endl;
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,add_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       
       case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vsubpd reg reg xmm!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VSUBPD_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vsubpd reg mem xmm!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_YMMqq:
        {
        
        OutFile << "Found one vsubpd reg reg xmm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VSUBPD_YMMqq_YMMqq_MEMqq:
          {
          OutFile << "Found one vsubpd reg mem ymm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,sub_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vmulpd reg reg xmm!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
     
      case xed_iform_enum_t::XED_IFORM_VMULPD_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vmulpd reg mem xmm!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_YMMqq:
        {
        
        OutFile << "Found one vmulpd reg reg ymm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VMULPD_YMMqq_YMMqq_MEMqq:
       {
          OutFile << "Found one vmulpd reg mem ymm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,mul_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }

       case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_XMMdq:
       {
        
        OutFile << "Found one vdivpd reg reg xmm!" <<std::endl;
       
         for(int i=0;i<2;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
      case xed_iform_enum_t::XED_IFORM_VDIVPD_XMMdq_XMMdq_MEMdq:
       {
          OutFile << "Found one vdivpd reg mem xmm!" <<std::endl;
         
       for(int i=0;i<2;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_YMMqq:
       {
        
        OutFile << "Found one vdivpd reg reg ymm!" <<std::endl;
       
         for(int i=0;i<4;i++)
        { 
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,2),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
             
        }
           
           
            INS_Delete(ins);
            break;
       }
       case xed_iform_enum_t::XED_IFORM_VDIVPD_YMMqq_YMMqq_MEMqq:
       {
          OutFile << "Found one vdivpd reg mem ymm!" <<std::endl;
         
       for(int i=0;i<4;i++)
         {
             INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,INS_OperandReg(ins,1),
              IARG_REG_REFERENCE,tmp_reg1,
              IARG_UINT32 ,i,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_from_input<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_MEMORYREAD_EA,
              IARG_REG_REFERENCE, tmp_reg2,
              IARG_UINT32,i,
              IARG_END);
             INS_InsertCall(ins, IPOINT_BEFORE,(AFUNPTR)wrap<double,div_double>,
              IARG_REG_CONST_REFERENCE, tmp_reg1,
              IARG_REG_CONST_REFERENCE, tmp_reg2,
              IARG_REG_REFERENCE, tmp_reg_output,
              IARG_END);
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)copy_to_output<double>,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,tmp_reg_output,
              IARG_REG_REFERENCE,INS_OperandReg(ins,0),
              IARG_UINT32 ,i,
              IARG_END);
        
         }  
            INS_Delete(ins);
            break;
       }
       default: break;


       
      
      }
}
KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "test-direct_ins.out", "specify output file name");

// This function is called when the application exits
VOID Fini(INT32 code, VOID* v)
{
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "End of instrumentation" << std::endl;
    //OutFile << "Result is " << cptr1 << std::endl;
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
    tmp_reg_output=PIN_ClaimToolRegister();
    tmp_reg_first=PIN_ClaimToolRegister();
    if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2) || !REG_valid(tmp_reg_output) || !REG_valid(tmp_reg_first))
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
