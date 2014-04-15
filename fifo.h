#ifndef FIFO_H
#define FIFO_H


#include "core-atomic.h"
/** a fifo suitable for one way data flow between routines running at different nvic priorities.
*/
template <int quantity> class Fifo {
  unsigned count;
  int readIndex;
  int writeIndex;
  /** the memory*/
  unsigned char mem[quantity];
public:
  Fifo(){
    clear();
  }

  /** forget the content */
  void clear(){
    readIndex=writeIndex=count=0;
  }

  /** forget the content AND wipe the memory, useful for debug.*/
  void wipe(){
    clear();
    for(int i=count;i-->0;){
      mem[i]=0;
    }
  }
  /** try to put a byte into the memory, @return whether there was room*/
  bool insert(unsigned char incoming){
    if(count<quantity){
      mem[writeIndex++]=incoming;
      if(writeIndex==quantity){
        writeIndex=0;
      }
      atomic_increment(count);
    }
  }
  /** @returns bytes present, but there may be more or less real soon. */
  inline int available()const{
    return count;
  }
  /** read and remove a byte from the memory, @return the byte, or -1 if there wasn't one*/
  int remove(){
    if(count>0){
      int pulled=mem[readIndex++];
      if(readIndex==quantity){
        readIndex=0;
      }
      atomic_decrement(count);
      return pulled;
    } else {
      return -1;
    }
  }

};

#endif // FIFO_H
