#include "VM/VM.h"
#include "VM/VMFunction.h"
#include "VM/VMState.h"
#include "VM/ByteCode.h"
#include "VM/MemoryManager.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/NativeBinding.h"
#include <iostream>


uint32_t AllocateString(VMState &state, const std::string &str) {

  VMValue pointerToString = MemMgrInstance().AllocateArray(ObjectType::CHAR, str.length());
  // write the string into the array
  // we use the fact that string uses continuous storage inside (char array almost certainly)
  // so address of the first element is the address of the array and we can pass this as a pointer
  // to the memory manager
  MemMgrInstance().WriteToArrayIndex(pointerToString, &str[0], 0, str.length());
  // save the pointer to constant object pool so it may be used later
  
  // store the pointer in the VMState permanent object storage, which is used
  // for compile time constants (assuming we actually had a compiler)
  return state.AddPermanentStorageObject(pointerToString);
}




void example_1_call_script_function_from_cpp() {
  std::cout << "Example one: calculating (1 + 2) * 5 with script function\n";
  VM vm;

  // state stores all functions and constant objects. These should be loaded from a file, but
  // for now, that is not implemented, so we need to create these by hand
  VMState state{"this/feature/not/implemented.datafile"};
  
  // function that adds its arguments together, then multiplies the result with five
  VMFunction function;
  function.AddByteCode(ByteCode::ADD_INTEGER);
  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(5)); // operands are part of instruction stream
  function.AddByteCode(ByteCode::MUL_INTEGER);
  function.AddByteCode(ByteCode::RETURN);

  state.AddFunction("testFunction", function);


  // topmost stack value is returned to caller
  VMValue result = vm.InvokeFunction(state, "testFunction", { 1, 2 });
  std::cout << "Result from script function: " << result.as_int() << "\n";
}






void example_2_script_returns_string() {
  std::cout << "\nExample two: script returns a string to c++ function\n";
  VM vm;

  VMState state{ "this/feature/not/implemented.datafile" };

  auto index = AllocateString(state, "Hello!");


  VMFunction function;
  
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  function.AddByteCode(static_cast<ByteCode>(index));
  function.AddByteCode(ByteCode::RETURN);

  state.AddFunction("testFunction", function);


  // this is a pointer to string
  VMValue result = vm.InvokeFunction(state, "testFunction", { 1, 2 });

  // so we need to call ToNativeType<std::string> to actually convert it to a string
  std::cout << "Result from script function: " << ToNativeType<std::string>(result) << "\n";
}






void example_3_script_function_calls_another_script_function() {
  std::cout << "\nExample three: Same as the first example, but number five is returned from\nanother script function that calculates 2 + 3\n";
  VM vm;

  VMState state{ "this/feature/not/implemented.datafile" };

  const std::string functionName = "return_5";
  auto index = AllocateString(state, functionName);

  // function that adds its arguments together, then multiplies the result with five
  VMFunction function;
  function.AddByteCode(ByteCode::ADD_INTEGER);
  // functions are called by their registered name, which is a string
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT); 
  function.AddByteCode(static_cast<ByteCode>(index));
  function.AddByteCode(ByteCode::INVOKE_MANAGED); 
  function.AddByteCode(ByteCode::MUL_INTEGER);
  function.AddByteCode(ByteCode::RETURN);
  
  state.AddFunction("testFunction", function);

  VMFunction returns5;
  returns5.AddByteCode(ByteCode::PUSH_INTEGER);
  returns5.AddByteCode(static_cast<ByteCode>(2)); 
  returns5.AddByteCode(ByteCode::PUSH_INTEGER);
  returns5.AddByteCode(static_cast<ByteCode>(3));
  returns5.AddByteCode(ByteCode::ADD_INTEGER);
  returns5.AddByteCode(ByteCode::RETURN);



  state.AddFunction(functionName, returns5);

  // topmost stack value is returned to caller
  VMValue result = vm.InvokeFunction(state, "testFunction", { 1, 2 });
  std::cout << "Result from script function: " << result.as_int() << "\n";
}





class SomeClass {

public:
  SomeClass(int32_t value) : m_value(value) { }

  int32_t MemberFunction(const std::string &str, int32_t integer) {
    std::cout << "\n\nGreetings from c++ function. I got '" << str << "' and " << integer << " as arguments\n";
    std::cout << "Multiplying second argument with " << m_value << "\n";
    return integer * m_value;
  };
private:
  int32_t m_value;
};

void example_4_invoking_cpp_function_from_script() {
  std::cout << "\nExample four: Script calculates 6/3, then calls c++ function that expects a\nstring and an integer as arguments, then";
  std::cout << " subtracts 2 from the c++ function\nreturn value and returns that.";


  VM vm;

  VMState state{ "this/feature/not/implemented.datafile" };


  const std::string cppFunctionName = "my_cpp_function";
  auto function_index = AllocateString(state, cppFunctionName);


  const std::string text = "just some random text";
  auto text_index = AllocateString(state, text);

  // create binding to SomeClass::MemberFunctrion with std::string and int as arg types
  auto binding = CREATE_2_ARGS_BINDING(SomeClass, MemberFunction, std::string, int);
  
  state.AddNativeBinding(cppFunctionName, binding);

  VMFunction function;
  // push first argument for the native function, that is, string
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  function.AddByteCode(static_cast<ByteCode>(text_index));

  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(6));
  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(3));
  function.AddByteCode(ByteCode::DIV_INTEGER);

  // push the name of the cpp function we are about to call
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  function.AddByteCode(static_cast<ByteCode>(function_index));

  function.AddByteCode(ByteCode::INVOKE_NATIVE);

  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(2));

  function.AddByteCode(ByteCode::SUB_INTEGER);


  function.AddByteCode(ByteCode::RETURN);

  state.AddFunction("function", function);


  SomeClass someClassObject{ 25 };
  auto result = vm.InvokeFunction(state, "function", { &someClassObject });
  std::cout << "Result from script function: " << result.as_int() << "\n";
}




int main() {
  example_1_call_script_function_from_cpp();
  example_2_script_returns_string();
  example_3_script_function_calls_another_script_function();
  example_4_invoking_cpp_function_from_script();

  // prevent console window from closing; needed with Visual Studio at least
  int dummy;
  std::cin >> dummy; 
}