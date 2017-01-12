#include "packdatatest.h"


static const packdatatest PACKMEMBERTAG(7) next ={'N'};
static const packdatatest PACKMEMBERTAG(2) first={'F'};

static const unsigned PACKMEMBERTAG(0) packdatatable  =0x980f; //marker, value not needed
static const unsigned PACKMEMBERTAG(9999999) packdatatablelast =0xD03E;//marker, value not needed

const packdatatest *const packdataBegin=reinterpret_cast<const packdatatest *const >(&packdatatable+1);
const packdatatest *const packdataEnd=reinterpret_cast<const packdatatest *const >(&packdatatablelast);

const int packdatacount(packdataEnd-packdataBegin);
