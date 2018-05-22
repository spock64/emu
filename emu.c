/*

emu.c

An example implementation of the Starship Command Module CPU :-)

PJR 180522

TODO:



*/

#define PJR_VER "0.1"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "emu.h"

// "memory" for emulated system
extern _mem mem;
extern void emulate();

void usage()
{
  printf( "emu - emulate Starship Command Module CPU\n"
          "usage:\n"
          "\temu source_file\n"
        );
}

void error_exit(int e, int r)
{
  printf("*** ERROR: %d, reason %d\n", e, r);
  exit(e);
}

void load_file(char * path)
{
  FILE *src;

  if((src = fopen(path,"rb")) == NULL)
    error_exit(e_CANTOPEN, errno);

  if(fseek(src, 0, SEEK_END) == -1)
    error_exit(e_CANTSEEKEND, errno);

  long file_size;

  if((file_size = ftell(src)) == -1)
    error_exit(e_FSIZE, errno);

  if(file_size > MEMSIZE)
    error_exit(e_TOOBIG,-1);

  if(fseek(src, 0, SEEK_SET) == -1)
    error_exit(e_CANTSEEKSTART, errno);

  if(fread(mem, 1, file_size, src) != file_size)
    error_exit(e_CANTRD, errno);

  if(fclose(src) == -1)
    error_exit(e_CANTCLOSE, errno);
}

int main(
  int argc,
  char *argv[]
)
{

  printf("*** Emu (%s) v" PJR_VER " " __DATE__ " " __TIME__ " ***\n", argv[0]);

  // TODO: properly scan arguments - SJR learning
  //  use argv[1] as file path

  if(argc < 2)
  {
    usage();
    error_exit(e_NOSRC,0);
  }

  // TODO: Should the file name argument be validated ?
  load_file(argv[1]);

  printf("Source file '%s' loaded, emulation starts ...\n", argv[1]);

  // start emulation
  emulate();

  printf("*** Emu: Ends\n");
}
