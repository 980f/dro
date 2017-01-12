#ifndef PACKDATATEST_H
#define PACKDATATEST_H

/** test table built by linker.

This stuff makes a table of items each created without any knowledge of the other instances. 
Each compilation unit (~cpp file) can have entries in the table without knowing anything about the possible presence of other entries.

This implementation is for building a table of constant objects, the table also constant.
 the class to be put into such a table must have an alignas() in the type so that array chunks will have the same alignment that the linker 
 forces for chunks from different files.
 */
struct alignas(4) packdatatest {
  char id;
};

//these are used to iterate over the table.
extern const packdatatest * const packdataBegin;
extern const packdatatest * const packdataEnd;

//I changed the type of the following to int as the compiler does signed math on when converting the difference in bytes to a difference in quantities.
extern const int packdatacount;

//this must be put in the declaration of data. prior=0 and =zzzzzzzz are sacred to the iteration, 
// ... you can use any other alpha numeric string so long as it is alphabetically between those.
#define PACKMEMBERTAG(prior) __attribute((section(".rodata.packdata." #prior )))

//the number here might be considered the 'default' priority.
#define PACKMEMBER PACKMEMBERTAG(5)

#if 0  /*
this can be done a little bit easier if you don't mind adding a line to the linker control file for each table.
That does entail coordinating names between the files, and you stuff will be left out silently if the names don't match.

If your structs have mutable members then you will have to use .data instead of .rodata in the mechanism.

If much of your struct is naturally const and only a little changes then you can put the mutable part into a different struct and have the const one point to that.

Note that the compiler will give you an error if all the PACKMEMBER's in a compilation unit are not of the same constness, but it doesn't recognize that .rodata objects must be const.
It is up to you to figure that out. You do get warnings when there is a mismatch, but they aren't well connected to the problem.

Another restriction is that the class must have a constructor that can run at compile time, ie another place you may have to split a class and have the classy part refer to the POD tabulated part.


*/
#endif


#endif // PACKDATATEST_H
