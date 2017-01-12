#include "packdatatest.h"
extern "C" {
bool wtf(int where){
  return false;
}
}

int main(){

  for(const packdatatest *pointer=packdataBegin;pointer<packdataEnd;++pointer){
    char x=pointer->id;
  }
  //the following does the same as the above.
  const packdatatest *pointer=packdataBegin;
  //the for-loop init below corrupts the stack where 'pointer' is held, count as well.
  //the IDE disassembler can't make sense of the code. 
  for(int count=packdataCount();count-->0;){
    char x=pointer->id;
    ++pointer;
  }
  return 0;
}

/*
unnecessary init code (re computes a constant and then writes to the rom, where that value already existed
courtesy of the linker, started doing wild jumps into high address space (small negative address).
I removed the variable being initialized and went on.

what seemed like a reasonable reordering of sections in the linker file resulted in a value being written 
smack dab in the middle of the .rodata section, on top of data allocated there.

after restoring an older version the linker code the first for-loop above works fine (again).
The second for-loop computes the correct value then trashes the stack frame.

*/