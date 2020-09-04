/* Simple Hello World Program C++ Version
FILE: hello_world.cpp

Compilation on dmc.asc.edu

   to compile the program

     Using GNU Compiler
     module load gcc
     g++ hello_world.cpp -o hello_world_gnu

     Using Intel Compiler
     module load intel
     icpc hello_world.cpp -o hello_world_intel

     Using NVIDIA hpc Compiler
     module load pgi/18.1
     pgc++ hello_world.cpp -o hello_world_hpc
*/

using namespace std;
#include <iostream>
int main (int argc, char *argv[]) {
   #ifdef __PGI
      // Compiled with the NVIDA HPC Compiler
      cout << "Hello world from NVIDA HPC compiler" << endl << flush;
   #elif  __INTEL_COMPILER
      // Compiled with Intel Compiler
      cout << "Hello world from Intel compiler" << endl << flush;
   #elif __GNUC__
      // Compiled with GNU Compiler
      cout << "Hello world from GNU compiler" << endl << flush;
   #endif
}
