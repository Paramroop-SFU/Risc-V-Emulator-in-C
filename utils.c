#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n) {
  /* YOUR CODE HERE */
    field = (int)field<< (32-n);
     field = (int)field>> (32-n);
    return (int)field;
    //return (int)field << (32-n) >> (32-n);
  
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */
Instruction parse_instruction(uint32_t instruction_bits) {
  /* YOUR CODE HERE */
  Instruction instruction;
  // add x8, x0, x0     hex : 00000433  binary = 0000 0000 0000 0000 0000 01000
  // Opcode: 0110011 (0x33) Get the Opcode by &ing 0x1111111, bottom 7 bits
  instruction.opcode = instruction_bits & ((1U << 7) - 1);

  // Shift right to move to pointer to interpret next fields in instruction.
  instruction_bits >>= 7;

  switch (instruction.opcode) {
  // R-Type
  case 0x33:
     // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;
  case 0x2a:
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;
  // case for I-type 
case 0x13:
case 0x03:
case 0x73:
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.itype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.itype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.itype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.itype.imm = instruction_bits & ((1U << 12) - 1);
    

    // funct7: 0000 000
    
    break;
case 0x23: //Stype
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.stype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.stype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.stype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
     instruction.stype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.stype.imm7 = instruction_bits & ((1U << 7) - 1);
  break;

  case 0x63: //Btype
      // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.sbtype.imm5 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.sbtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.sbtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
     instruction.sbtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.sbtype.imm7 = instruction_bits & ((1U << 7) - 1);
    break;
  case 0x37: // Utype
    instruction.utype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.utype.imm = instruction_bits & ((1U << 20) - 1);
    break;

  case 0x6F:
    instruction.ujtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    instruction.ujtype.imm = instruction_bits & ((1U << 20) - 1);
  break;
  default:
    exit(EXIT_FAILURE);
  }
  return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using
 * the given branch instruction */
int get_branch_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int branch = 0b00000000000000000000000000000000;
  branch |= instruction.sbtype.imm5 &          0b00000000000000000000000000011110;
  branch |= (instruction.sbtype.imm7<<5) &     0b00000000000000000000011111100000;
  branch |= (instruction.sbtype.imm5<<11)&     0b00000000000000000000100000000000; //wrong
  branch |= ((instruction.sbtype.imm7>>6)<<12)&0b00000000000000000001000000000000; //wrong
  branch = sign_extend_number(branch,13);
  return branch;
  

  
}

/* Returns the number of bytes (from the current PC) to the jump label using the
 * given jump instruction */
int get_jump_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int jump = 0b00000000000000000000000000000000;
  jump |= (instruction.ujtype.imm >> 8) &       0b00000000000000000000011111111110;
  jump |= (instruction.ujtype.imm >> 8 << 11) & 0b00000000000000000000100000000000;
  jump |= (instruction.ujtype.imm << 12) &      0b00000000000011111111000000000000;
  jump = sign_extend_number(jump,20);
  return jump;
}

int get_store_offset(Instruction instruction) {
  /* YOUR CODE HERE */

  int store = 0b00000000000000000000000000000000;
  store |= instruction.stype.imm5 & 0b00000000000000000000000000011111;
  store |= (instruction.stype.imm7 << 5) & 0b00000000000000000000111111100000;
  store = sign_extend_number(store,12);
  return store;
}

void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}

void handle_invalid_read(Address address) {
  printf("Bad Read. Address: 0x%08x\n", address);
  exit(-1);
}

void handle_invalid_write(Address address) {
  printf("Bad Write. Address: 0x%08x\n", address);
  exit(-1);
}

