#ifndef PACKDATATEST_H
#define PACKDATATEST_H

/** test table built by linker */
struct packdatatest {
  char id;
};

extern unsigned packdatatable;
extern unsigned packdatatablelast;

#define PACKMEMBER __attribute((section(".rodata.packdata.1")))

#endif // PACKDATATEST_H
