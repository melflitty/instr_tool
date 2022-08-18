#include <iostream>
#include <fstream>
#include "pin.H"
#include <unistd.h>
extern "C" {
#include "xed-interface.h"
}
//#include "utils/copy_register.h"
#include <immintrin.h>
#include <bitset>  
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
static bool* mask_elt;
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
    template<typename T>
    void PIN_FAST_ANALYSIS_CALL zero_reg_at_position(T* dst,int pos)
    {
      OutFile << "I arrived at zero reg at position "  <<  pos << std::endl;
      dst[pos]=0;

    }


     static bool PIN_FAST_ANALYSIS_CALL check_mask(__mmask16* const mask,int I)
     {
        return ((*mask) & ((1) << (I))) != 0;
     }

    static void PIN_FAST_ANALYSIS_CALL get_mask_element(__mmask16* const mask, bool* element,int pos)
    {
      //*element=mask[pos];
      OutFile << "mask element at pos " << mask[pos]<< pos << " is" << std::endl;
    }

VOID Instruction(INS ins, VOID* v)
{    
      
      auto xed = INS_XedDec(ins);
      auto iform = xed_decoded_inst_get_iform_enum(xed);
      int counter=0;
      bool ismasking=xed_decoded_inst_masking(xed);
      bool ismerging=xed_decoded_inst_merging(xed);
      bool iszeroing=xed_decoded_inst_zeroing(xed);
      REG mask_reg = INS_MaskRegister(ins); // returns REG_INVALID if the instruction does not use mask
      switch(iform)
      {
        case xed_iform_enum_t:: XED_IFORM_VADDPS_ZMMf32_MASKmskw_ZMMf32_ZMMf32_AVX512:
        {
        OutFile << "Found one avx512 reg reg zmm" << std::endl;
        if(REG_valid(mask_reg))
        {
            for(int i=0;i<16; ++i)
            {
              INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)get_mask_element,
              IARG_FAST_ANALYSIS_CALL,
              IARG_REG_CONST_REFERENCE,mask_reg,
              IARG_PTR,mask_elt,
              IARG_UINT32 ,i,
              IARG_END);
            }
            break;
        }
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
    if (!REG_valid(tmp_reg1) || !REG_valid(tmp_reg2) || !REG_valid(tmp_reg_output))
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
