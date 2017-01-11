#include "packdatatest.h"


static packdatatest first PACKMEMBER ={'F'};


unsigned packdatatable __attribute((section(".rodata.packdata.0"))) =0x980f; //marker, value  
unsigned packdatatablelast __attribute((section(".rodata.packdata.last"))) =0;
