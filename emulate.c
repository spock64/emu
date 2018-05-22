/*

emulate.c

*/

#include <stdio.h>
#include "emu.h"

// delaration for the memory block ...
_mem mem;

// Registers
char r[NUMREG];

// Register names ...

char r_name[NUMREG] = { 'A', 'B', 'C', 'D'};

// CPU State
int PC = 0; // Programme counter
cpu_state FLAGS = {0,0}; // State flags

void print_state()
{
  // regs
  printf("\n@ R: ");
  int i;
  for(i=0;i<NUMREG;i++)
    printf("%c %0.2X ", r_name[i], r[i]);

  // internals
  printf("\n@ PC: %0.4X EQ:%d GT:%d\n", PC, FLAGS.EQ, FLAGS.GT);
}

void emulate()
{

  printf( "@ Emulation starts\n"
          "@ Memory size %d\n", MEMSIZE);

  do {
    printf("%0.4X ", PC);

    unless(mem[PC] == HALT)
    {
      switch(mem[PC] & 0xF0)
      {
        case LD_REG:
          printf("LD %c,%c", r_name[(mem[PC] & 0x0C) >> 2], r_name[mem[PC] & 0x03]);
          r[(mem[PC] & 0x0C) >> 2] = r[mem[PC] & 0x03];
          ++PC;
          break;

        case LD_OP:
          printf("LD ");

          if(mem[PC] & 0x04)
          {
            // Load from address
            printf("%c,#%0.2X%0.2X", r_name[mem[PC] & 0x3], mem[PC+1], mem[PC+2]);
            r[mem[PC] & 0x3] = mem[(mem[PC+1]<<8) + mem[PC+2]];
            PC += 3;
          }
          else
          {
            // Load a direct value
            printf("%c,%0.2X", r_name[mem[PC] & 0x3], mem[PC+1]);
            r[mem[PC] & 0x3] = mem[PC+1];
            PC += 2;
          }
          break;

        case ST:
          printf("ST #%0.2X%0.2X,%c", mem[PC+1], mem[PC+2], r_name[mem[PC] & 0x3]);
          mem[(mem[PC+1]<<8) + mem[PC+2]] = r[mem[PC] & 0x03];
          PC += 3;
          break;

        case ADD:
          printf("ADD %c,%c,%c", r_name[mem[PC+1] & 0x03], r_name[(mem[PC] & 0x0C) >>2], r_name[mem[PC] & 0x03]);
          r[mem[PC+1] & 0x03] = r[(mem[PC] & 0x0C) >>2] + r[mem[PC] & 0x03];
          PC +=2;
          break;

        case SUB:
          printf("SUB %c,%c,%c", r_name[mem[PC+1] & 0x03], r_name[(mem[PC] & 0x0C) >>2], r_name[mem[PC] & 0x03]);
          r[mem[PC+1] & 0x03] = r[(mem[PC] & 0x0C) >>2] - r[mem[PC] & 0x03];
          PC +=2;
          break;

        case MUL:
          printf("MUL %c,%c,%c", r_name[mem[PC+1] & 0x03], r_name[(mem[PC] & 0x0C) >>2], r_name[mem[PC] & 0x03]);
          r[mem[PC+1] & 0x03] = r[(mem[PC] & 0x0C) >>2] * r[mem[PC] & 0x03];
          PC +=2;
          break;

        case DIV:
          printf("DIV %c,%c,%c", r_name[mem[PC+1] & 0x03], r_name[(mem[PC] & 0x0C) >>2], r_name[mem[PC] & 0x03]);
          r[mem[PC+1] & 0x03] = r[(mem[PC] & 0x0C) >>2] / r[mem[PC] & 0x03];
          PC +=2;
          break;

        case CMP:
          printf("CMP %c,%c", r_name[(mem[PC] & 0x0C) >> 2], r_name[mem[PC] & 0x03]);
          FLAGS.EQ = r[(mem[PC] & 0x0C) >> 2] == r[mem[PC] & 0x03];
          FLAGS.GT = r[(mem[PC] & 0x0C) >> 2] > r[mem[PC] & 0x03];
          PC++;
          break;

        case JMP: // or shift ... ! (odd instruction set)
          switch(mem[PC])
          {
            case JMP:
            printf("JMP %0.2X%0.2X", mem[PC+1], mem[PC+2]);
            PC = (mem[PC+1]<<8) + mem[PC+2];
            break;

            case JE:
            printf("JE %0.2X%0.2X", mem[PC+1], mem[PC+2]);
            if(FLAGS.EQ)
              PC = (mem[PC+1]<<8) + mem[PC+2];
            else
              PC += 3;
            break;

            case JNE:
            printf("JNE %0.2X%0.2X", mem[PC+1], mem[PC+2]);
            if(!FLAGS.EQ)
              PC = (mem[PC+1]<<8) + mem[PC+2];
            else
              PC += 3;
            break;

            case JG:
            printf("JG %0.2X%0.2X", mem[PC+1], mem[PC+2]);
            if(FLAGS.GT)
              PC = (mem[PC+1]<<8) + mem[PC+2];
            else
              PC += 3;
            break;

            case JL:
            printf("JL %0.2X%0.2X", mem[PC+1], mem[PC+2]);
            if(!FLAGS.GT)
              PC = (mem[PC+1]<<8) + mem[PC+2];
            else
              PC += 3;
            break;

            case SL:
            printf("SL %c,%.1X", r_name[(mem[PC+1] & 0x10) >> 4], mem[PC+1] & 0x07);
            r[(mem[PC+1] & 0x10) >> 4] <<= mem[PC+1] & 0x07;
            PC += 2;
            break;

            case SR:
            printf("SR %c,%.1X", r_name[(mem[PC+1] & 0x10) >> 4], mem[PC+1] & 0x07);
            r[(mem[PC+1] & 0x10) >> 4] >>= mem[PC+1] & 0x07;
            PC += 2;
            break;

          }
          break;

        case ROT: // NEEEDS WORK ****** Could be wrong !
          printf("ROT %c,%.1X", (mem[PC] & 0x08)? 'R' : 'L', mem[PC] & 07);
          if(mem[PC] & 0x07)
          {
            unsigned char tmp = r[0];
            if(mem[PC] & 0x08)
            {
              // Right
              r[0] >>= mem[PC] & 07;
              r[0] |= tmp << (8-(mem[PC] & 07));
            }
            else
            {
              // Left
              r[0] <<= mem[PC] & 07;
              r[0] |= tmp >> (8-(mem[PC] & 07));
            }
          }
          PC++;
          break;

        case AND:
          printf("AND %c,%c", r_name[(mem[PC] & 0x0C) >> 2], r_name[mem[PC] & 0x03]);
          r[(mem[PC] & 0x0C) >> 2] &= r[mem[PC] & 0x03];
          PC++;
          break;

        case OR:
          printf("OR");
          printf("LD %c,%c", r_name[(mem[PC] & 0x0C) >> 2], r_name[mem[PC] & 0x03]);
          r[(mem[PC] & 0x0C) >> 2] |= r[mem[PC] & 0x03];
          PC++;
          break;

        case XOR:
          printf("XOR %c,%c", r_name[(mem[PC] & 0x0C) >> 2], r_name[mem[PC] & 0x03]);
          r[(mem[PC] & 0x0C) >> 2] ^= r[mem[PC] & 0x03];
          PC++;
          break;

        default:
          printf("@ Unknown instruction %.02X\n", mem[PC]);
          return;
      }

      print_state();

      if(PC > MEMSIZE)
      {
        printf("@ PC > MEMSIZE\n");
        return;
      }
    }
  } while(mem[PC] != HALT);

}
