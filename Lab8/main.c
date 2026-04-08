 /*
 ============================================================================
 Name        : main.c
 Author      : Austin Tian
 Revised by  :
 Version     :
 Copyright   : Copyright 2023
 Description : main code in C
 ============================================================================
 */
#include "header.h"

int main (int argc, char *argv[]) {                
        //reads the asm file, sets uo memory, loads the code into memory,
        //then start the CPU fetch and decode for lab8
	    char *mem = NULL;  // memory space pointer
        FILE *fp = NULL; //pointer for the asm input file
        int stopChar; //pauses program between steps
	    // Step 1: Parse the ASM code make sure asm file path is provided
		if (argc < 2) {
            puts("\nIncorrect number of arguments.");
            puts("Usage: ProgramName.exe MIPSCode.asm \n");
            stopChar = getchar();
            return EXIT_FAILURE;  //if the file cannot be found, stop
        }
        //opens the asm file
        if ((fp = fopen(argv[1], "r"))== NULL) {  // read the asm file.
                printf("Input file could not be opened.");
                stopChar = getchar();
                return EXIT_FAILURE;  //stops if it cant be opened
        }
        parse_MIPS(fp);  //parses the code into
                        // Data_storage, Instruction_Storage and labelTab 
		
        // Step 2: Setup the memory (lab 6)
        //memory should be filled with o instead of random values
        mem = init_memory();
        stopChar = getchar();
        // Step 3: Load the data and code section into memory (lab 7)
        puts("----Lab 7 Code Starts to Parse the ASM Code----");
        loadCodeToMem(mem);
        stopChar = getchar();
        //CPU gets each instruction from memory and decodes
		puts("----Lab 8 Code Starts to Fetch and Decode the Code ----");
        CPU(mem);
        stopChar = getchar();

		
		// puts("----Lab 9 Code Starts to Execute the Code ----");
        // Step 6: Execute the code until the code is finished(lab 9)                  
        free_memory(mem);  //free the memory
        return 0; 
}
