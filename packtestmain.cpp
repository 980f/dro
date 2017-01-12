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
  for(int count=packdataCount();count-->0;){
    char x=pointer->id;
    ++pointer;
  }
  return 0;
}