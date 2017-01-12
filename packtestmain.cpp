#include "packdatatest.h"
extern "C" {
bool wtf(int where){
  return false;
}
}

int main(){
//  unsigned *first=&packdatatable;
//  unsigned marker=*first++;
//  packdatatest *tabler=reinterpret_cast<packdatatest *>(first);
//
//  while(char x=tabler->id){
//    ++tabler;
//  }

  for(const packdatatest *pointer=packdataBegin;pointer<packdataEnd;++pointer){
    char x=pointer->id;
  }

  const packdatatest *pointer=packdataBegin;
  for(int count=packdatacount;count-->0;){
    char x=pointer->id;
    ++pointer;
  }


  return 0;
}