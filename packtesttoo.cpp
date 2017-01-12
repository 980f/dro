#include "packdatatest.h"

static const packdatatest PACKMEMBER too    {'T'};
//something clobbers the 'C'th element,
static const packdatatest PACKMEMBERTAG(3) more[]= {{'A'},{'B'},{'T'},{'D'}};
static const packdatatest PACKMEMBER three  {'R'};

