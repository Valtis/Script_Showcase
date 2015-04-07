# Script_Showcase
A showcase for the script engine I am building for Helsinki University game project course. Allows script functions to be called from C++ and registered C++ member functions to be called from scripts.

There are three different examples.

First example calculates (1 + 2) * 3 as well as 6 / (1 + 2) and invokes native C++ function to print the results.  

Second example allocates an integer array and stores a value in this array. Then the example forces garbage collection by allocationg lots of arrays and immediately losing the references to these arrays. Then it prints the stored value in the original array to ensure the array is still accessible after garbage collection.

Third example invokes a class member function on two different instances of the class.

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

### RootSetProvider
An interface for a class that provides root set for garbage collector. Currently VM is the only implementing class.

### GarbageCollector
An interface for garbage collector. Memory manager uses this when garbage collection is required.

### CheneyCollector
Implementation of GarbageCollector interface. Currently this is the only implementation. The collector uses Cheney's algorithm: http://en.wikipedia.org/wiki/Cheney%27s_algorithm 

## Compilation

Tested with visual studio 2013. Project should compile without issues. 

## Expected output: 

![Expected output](https://raw.githubusercontent.com/Valtis/Script_Showcase/master/docs/sample_output.png)
