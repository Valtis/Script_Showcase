# Script_Showcase
A showcase for the script engine I am building for Helsinki University game project course. Allows script functions to be called from C++ and registered C++ member functions (up to 4 argument) to be called from scripts.

Main contains four different examples:

First one calls script function that calculates sum of its integer arguments and then multiplies this by 5. The value is then returned to caller. Result is printed.

Second one returns pointer to an allocated string to caller, which is then converted to std::string and printed.

Third one calculates sum of its integer arguments, then calls a script function that calculates sum of 2 and 3 and returns it. The return value is then used to multiply the original sum. This result is returned to caller and printed.

Fourth one calls a script function that invokes native C++ function that expects std::string and int as argument and which returns integer. A string and 2 are provided as arguments. The script then uses the return value from C++ function and subtracts 2 from it. The result is returned and printed.

## Structure

![Structure](https://raw.githubusercontent.com/Valtis/Script_Showcase/master/docs/structure.png)


### VMState
Contains functions (both native and VM) as well as permanent object pool. Each script is expected to have its own VMState. This allows multiple scripts to have functions with same name (such as 'update'), and allows them to have separate variable pools. 

### VM
VM contains the execution loop as well as the value stack and function frame stack. It uses VMState to get necessary runtime data (such as VMFunctions and native functions) required for execution. Uses functions defined in VMOperations.h to actually execute the bytecode.

Topmost value in stack when execution ends is returned to caller as the return value. In case stack is empty, dummy value is returned instead.

### VMFrame
Contains pointer to VMFunction, and program counter for the pointed function. Also contains local variables.

### VMFunction
Contains function bytecode and function name

### VMValue
Class that represents values inside VM. Contains type tag and actual value. Throws an exception if wrong type is requested (eg. float requested when contains an integer).

### NativeBinding
Wrappers around native functions that can be called from scripts. Currently only allows binding over member functions; free functions are yet to be supported. Provides helper macros for binding; for example CREATE_2_ARGS_BINDING(CLASS, MEMBER_FUNCTION, FIRST_ARG_TYPE, SECOND_ARG_TYPE);

### ConversionFunctions
Contains functions that allow conversion between VMValue and native types.

### MemoryManager
Manages VM heap. Currently only allows allocation of arrays. Garbage collector is not yet implemented either.

## Compilation

Should compile without problems with Visual Studio 2013. Not tested with other compilers

## Expected output: 

![Expected output](https://raw.githubusercontent.com/Valtis/Script_Showcase/master/docs/sample_output.png)
