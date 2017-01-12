#include "packdatatest.h"

//tagging the table, conveniently using the pointers to begin and end as the markers for the extent of the table
const packdatatest *const packdataBegin PACKMEMBERTAG(0) = reinterpret_cast<const packdatatest *const >(&packdataBegin+1);
//the tag numbers above and below are what define the range of priorites, They look like numbers but are text.
const packdatatest *const packdataEnd PACKMEMBERTAG(9999999) = reinterpret_cast<const packdatatest *const >(&packdataEnd);


// In days of yore the linker was trusted to do this math. It does now as well, but then the compiler emits startup code to also compute it and then assign to the const data. That startup code presently segfaults.
//const int packdatacount(packdataEnd-packdataBegin);

//some test data
//this should come out last
static const packdatatest PACKMEMBERTAG(7) next {'L'};
//this should be first
static const packdatatest PACKMEMBERTAG(2) first{'F'};

