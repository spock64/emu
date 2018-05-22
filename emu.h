/*

emu.h

Standard definitions / dimensions

TODO:

*/

#define MEMSIZE 20480
#define NUMREG 4

#define unless(a) if(!(a))

typedef enum _INSTR {
          HALT,
          LD_REG = 0x10,
          LD_OP = 0x90,
          ST = 0x20,
          ADD = 0x30,
          SUB = 0x40,
          MUL = 0x50,
          DIV = 0x60,
          CMP = 0x70,
          JMP = 0x80,
          JE = 0x81,
          JNE = 0x82,
          JG = 0x83,
          JL = 0x84,
          SL = 0x85,
          SR = 0x86,
          ROT = 0xa0,
          AND = 0xc0,
          OR = 0xd0,
          XOR = 0xe0
} instr;

typedef enum _errors {
          e_SUCCESS,
          e_NOSRC,
          e_CANTOPEN,
          e_CANTSEEKEND,
          e_FSIZE,
          e_CANTSEEKSTART,
          e_CANTRD,
          e_CANTCLOSE,
          e_TOOBIG
} errors;

typedef unsigned char _mem[MEMSIZE];

typedef struct _cpu_state
      {
        unsigned char EQ:1;
        unsigned char GT:1;
      } cpu_state;
