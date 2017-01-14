#include "packdatatest.h"

static const packdatatest PACKMEMBER too    {'T'};
//something clobbers the 'C'th element, and not related to being an array, I moved the lines of code here about and it was always the same distance from the start of section.
//fixed by restoring linker file order to an older one, someday I'll figure out why it mattered! I had changed the order so that the chunk I was working on would be near the start
//of the elf listing instead of the end.
static const packdatatest PACKMEMBERTAG(8) more[] {{'A'},{'B'},{'C'},{'D'}};
static const packdatatest PACKMEMBER three  {'R'};

