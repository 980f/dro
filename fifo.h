#ifndef FIFO_H
#define FIFO_H

/** a fifo suitable for one way data flow between routines running at different nvic priorities.
 */
class Fifo {
  unsigned count;
  unsigned char *reader;
  unsigned char *writer;
  /** the memory*/
  unsigned char *mem;
  unsigned char *end;
  const unsigned quantity;

  /** circularly increment reader or writer */
  inline void incrementPointer(unsigned char *&pointer){
    if(++pointer== end) {
      pointer= mem;
    }
  }

public:
  Fifo(unsigned quantity,unsigned char *mem);

  /** forget the content */
  void clear();

  /** forget the content AND wipe the memory, useful for debug.*/
  void wipe();
  /** @returns bytes present, but there may be more or less real soon. */
  inline int available() const {
    return count;
  }
  /** try to put a byte into the memory, @return whether there was room*/
  bool insert(unsigned char incoming);
  int attempt_insert(unsigned char incoming);

  /** read and remove a byte from the memory, @return the byte, or -1 if there wasn't one*/
  int remove(); // remove
  int attempt_remove();
};

#endif // FIFO_H
