 /*
 ============================================================================
 Name        : Lab8.c
 Author      : Austin Tian
 Revised by  :
 Version     :
 Copyright   : Copyright 2020
 Description : Lab 8 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"
#include <time.h>
// Menu for the testing.
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 8)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()
{
    return rand()%256;  // generate a random number between 0 and 255. to give full 0x00-0xFF range
}
//---------------------------------------------------------------
void free_memory(char *base_address)
{
    free(base_address);  // free memory after use to avoid memory leakage.
    return;
}
//---------------------------------------------------------------
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);  // allocate the memory
    // loops thorugh every byte and fills in the memory contents with 0
    int i =0;
    if (mem ==NULL){
        puts("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i<MEM_SIZE; i++){
        *(mem+i) = 0;
    }
    return mem;
}


//---------------------------------------------------------------
//Writes a single byte 8 bits=1 byte to memory
//address is defined by base_address + offset
void write_byte(const char *base_address, const int offset, const unsigned char byte_data){
    if (offset <0 || offset >= MEM_SIZE)
    {
        puts("write byte failed cause of invalid offset");
        return;
    }
    *(unsigned char *)(base_address + offset) = byte_data;
}

//---------------------------------------------------------------
//Writes a double word 32 bits=4 bytes to memory
//address is defined by base_address + offset
//uses memcpy to copy all 4 bytes into memory
void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
    if (offset <0 || offset +(int)sizeof(unsigned int) > MEM_SIZE)
    {
        puts("write failed cause of invalid offset");
        return;
    }
    memcpy((void *)(base_address+offset), &dword_data, sizeof(unsigned int));
    if (DEBUG_CODE) {
    printf("Double word 0x%08X written to offset 0x%X\n", dword_data, offset);
    }
}

//---------------------------------------------------------------
//Reads a single byte from address defined by base_address + offset
//casting to unsigned char so it shows 0x00 to 0xFF not negative
unsigned char read_byte(const char *base_address, int offset){
    unsigned char data;
    if (offset < 0 || offset >= MEM_SIZE)
    {
        puts("Read byte failed cause of invalid offset");
        return 0;
    }
    data = *(unsigned char *)(base_address + offset);
    if (DEBUG_CODE)
       printf("-----The Byte at base: %p, offset %X: %02X ------\n", (void *)base_address, offset, data);
    return data;
}
//---------------------------------------------------------------
//Reads a double word, 4 bytes, from memory based on address(base_address + offset)
//uses memcpy to copy all 4 bytes into an unsigned int
unsigned int read_dword(const char *base_address, int offset){
    unsigned int data;
    // Step 4: return and print the double-word from address: "base_address + offset".
    if (offset < 0 || offset + (int)sizeof(unsigned int) > MEM_SIZE)
    {
        puts("Reading double word failed cause of invalid offset");
        return 0;
    }
    memcpy(&data, base_address + offset, sizeof(unsigned int));
    if (DEBUG_CODE)
       printf("-----The 32-bit word at base: %p, offset %X is: %08X ------\n", (void *)base_address, offset, data);
    return data;
}

//---------------------------------------------------------------
//Generates a memory dump display from base_address + offset 
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){
    unsigned char printout;
    int i=0, j=0;
    if (offset < 0 || offset >= MEM_SIZE)
    {
        puts("Memory dump failed");
        return;
    }
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;
    if ((unsigned int)offset + dumpsize > MEM_SIZE)
        dumpsize = MEM_SIZE - (unsigned int)offset;
    printf("\n");
    for (i = 0; i < (int)dumpsize; i += DUMP_LINE)
    {
        printf("%p: ", (void *)(base_address + offset + i));
        for (j = 0; j < DUMP_LINE; j++)
        {
            if (i + j < (int)dumpsize)
            {
                printout = (unsigned char)(*(base_address + offset + i + j));
                printf("%02X ", printout);
            }
            else
            {
                printf("   ");
            }
        }
        printf(" --- ");

        for (j = 0; j < DUMP_LINE; j++)
        {
            if (i + j < (int)dumpsize)
            {
                printout = (unsigned char)(*(base_address + offset + i + j));
                if (printout >= 0x20 && printout <= 0x7E)
                    printf("%c ", printout);
                else
                    printf(". ");
            }
        }
        printf("\n");
    }
    return;

}

/*
void clean_stdin(void)
{
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}*/

//---------------------------------------------------------------
void setup_memory()
{
    // Now we need to setup the memory controller for the computer system we
    // will build. Basic requirements:
    // 1. Memory size needs to be 1M Bytes
    // 2. Memory is readable/writable with Byte and Double-Word Operations.
    // 3. Memory can be dumped and shown on screen.
    // 4. Memory needs to be freed (released) at the end of the code.
    // 6. For lab 8, we need to have a user interface to fill in memory,
    //                                      read memory and do memory dump.
    char *mem = init_memory();  // initialize the memory.
    char options =0;
    unsigned int offset, dumpsize;
    char byte_data;    // 8-bit operation.
    unsigned int dword_data;      // 32-bit operation.
    do{
        if (options != 0x0a)  // if options has a return key input, skip it.
        {
            puts(menu); /* prints Memory Simulation */
                printf("\nThe base address of your memory is: %p (HEX)\n", (void *)mem); // output base memory first.
            puts("Please make a selection:");  // output base memory first.
        }
            options = getchar();

            switch (options)
            {               
                case '1':  // write double word.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", (unsigned int*)&offset);    // input an offset address (in HEX) to write.
                    puts("Please input your DOUBLE WORD data to be written (in HEX):");
                    scanf("%x", (unsigned int*)&dword_data);    // input data
                    write_dword (mem, offset, dword_data);  // write a double word to memory.
                    continue;
                case '2':  // read byte.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to write.
                    read_byte(mem, offset);
                    continue;
                case '3':  // read double word.
                    puts("Please input your memory's offset address (in HEX):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to write.
                    read_dword(mem, offset);
                    continue;
                case '4':  // generate memory dump starting at offset address (in HEX).
                    puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                    scanf("%x", &offset);    // input an offset address (in HEX) to start.
                    puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                    scanf("%d", &dumpsize);    // The size of the memory dump
                    memory_dump(mem, offset, dumpsize);  // generate a memory dump display of dumpsize bytes.
                    continue;
                case 'e':
                case 'E':
                    puts("Code finished, exit now");
                    free_memory(mem);
                    getchar();
                    return;  // return to main program.
                default:
                    // puts("Not a valid entry, please try again");
                    continue;
            }
    }while (1);  // make sure the only exit is from 'e'.
    return;
}
