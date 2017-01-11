#include "packdatatest.h"
extern "C" {
bool wtf(int where){
  return false;
}
}

int main(){
  unsigned *first=&packdatatable;
  unsigned marker=*first++;
  packdatatest *tabler=reinterpret_cast<packdatatest *>(first);

  while(char x=tabler->id){
    ++tabler;
  }
  return marker;
}