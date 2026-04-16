
/* 
 * File:   lab8header.h
 * Author: 
 * For lab 8, these functions used in fetching, decoding and printing CPU data
 */

#ifndef LAB8HEADER_H
#define LAB8HEADER_H

unsigned int CPU_fetchCode(char *mem, int codeOffset);
unsigned char CPU_Decode(unsigned int machineCode); //decodes fetched machine code and returns its opcode or function field
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem);
void printRegisterFiles(); //register names and their current values
void printDataMemoryDump(char *mem); //prints first 256 bytes of the data section
unsigned int PCRegister = 0;  // PC register,stores the address of the next instruction to be fetched

#endif /* LAB8HEADER_H */

