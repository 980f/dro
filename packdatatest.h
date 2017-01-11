#ifndef PACKDATATEST_H
#define PACKDATATEST_H

/** test table built by linker */
struct alignas(4) packdatatest {
  char id;
  char pad[3];
};

extern unsigned packdatatable;
extern unsigned packdatatablelast;

//[[bracketing]] didn't work, sections were discarded
#define PACKMEMBER __attribute((section(".rodata.packdata.1")))

#endif // PACKDATATEST_H
