#include "packdatatest.h"


static packdatatest PACKMEMBER first ={'F'};


unsigned __attribute((section(".rodata.packdata.0"))) packdatatable  =0x980f; //marker, value  
unsigned __attribute((section(".rodata.packdata.last"))) packdatatablelast =0xdeadbeef;
