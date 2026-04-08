
// Lab 8 performs the following functions:
//   1. Fetch the machine code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will continue with executing the code stored in the memory and print the results
#include "header.h"
#include "lab8header.h"
extern char *regNameTab[N_REG];
extern unsigned int PCRegister; // PC register stores the address of next instruction.  

//Like a CPU loop, it starts from code section, fetches one instruction at a time,
// decodes and then stops when it reaches 0x00000000
void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;
    PCRegister = CODESECTION;  //PCRegister is the starting point,
    do{
      printf("\nPC:%x \n", PCRegister);
      //gets one 32-bit instruction from memory using PC
      machineCode = CPU_fetchCode(mem, PCRegister);
      if (machineCode == 0)  //when machineCode is 0, quit, its the end of the code
          break;  // break the infinite loop. 
      PCRegister += 4;   //updates the program counter  moves the PC to the next instruction, 4 bytes later                                                  // update the program counter
      //decoding the fetched instruction
      opcode = CPU_Decode(machineCode);
      printf("Decoded Opcode is: %02X. \n", opcode);

      // Lab 9: Finish the execution of the code.
      // Only finish this part when the CPU_Decode is done.
      // CPU_Execution(opcode, machineCode, mem);
    }while (1); 

    printRegisterFiles();     //Prints all the register contents on screen.
    printDataMemoryDump(mem); //Prints the memory dump of the data section.
}

// Lab 8 - Step 1
//This function gets one 32-but machine code instruction from memory
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    unsigned int machineCode =0;
    //reads one double word from mem at a given offset
    machineCode = read_dword(mem,codeOffset);
    //optional but it debugs the output to show the fetched instruction
    if (DEBUG_CODE){
        printf("Fetched machine code: %08X \n", machineCode);
    }
    return machineCode;
}

// Lab 8 - Step 2
//Decodes the instruction and returns opcode
//For R type instruction the opcode field is 0, so we are returning function field
unsigned char CPU_Decode(unsigned int machineCode){
    unsigned char opcode =0;
    unsigned char function=0;
    //gets top 6 bits (the opcode field)
    opcode = (machineCode >> 26) & 0x3F;
    //if opcode 0 then its an R type instruction
    //and the real operation is in the last 6 bits
    if (opcode ==0){
        function = machineCode & 0x3F;
        return function;
    }
    //I type and J type instructios use opcode field directly
    return opcode;

}
// Lab 9: Finish the function CPU_Execution to run all the instructions.
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){
    unsigned char rt = 0;
    switch (opcode)  // execute different functions when opcode is set differently. 
    {
		// This is an example how lab will be executed. Please follow this example and finish exections of the code.
		// Hint: you need to implement the following instructions here: 
		//       la, add, lb, bge, lw, sw, addi, j
        case 0b101111:   //"la" instruction. 
            // assign the address rt = immediate address stored in machineCode;
            // first find the rt index in the register array. 
            rt = (machineCode & 0x001F0000) >> 16; 
            // assign the address stored in immediate field to regFile[rt];
            regFile[rt] = machineCode & 0x0000FFFF;  // get the last 16 bit as address. 
            // update PCregister ???? Pay special attention to branch instructions. 
            PCRegister += 4;
            if (DEBUG_CODE){   // print the hints to the user in DEBUG_MODE
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);                
            }            
            break;
        case 0b100000://"lb" instruction. 
		    //....
			break;
		
        // continue to all the other cases used in the program.
        // case ......:    
            
            
        // Should never go to default part when complete. Otherwise, that is a mistake. 
        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode,  machineCode);
            system("PAUSE");
            exit(3);  // exit the program if running here.     
            break;
    }
}
// Lab 8 - Step 3
// prints all registers and the current values
void printRegisterFiles(){
    int i =0;
    printf("\n     Register File Dump    \n");
    //loops all registers an prints register name + value
    for (i=0; i<N_REG; i++){
        printf("%-5s = 0x%08X \n", regNameTab[i], regFile[i]);
    }
}

 // Lab 8 - Step 4
 //prints first 256 bytes of the data section
 //the start of data area in memeory is DATASECTION
void printDataMemoryDump(char *mem){
    printf("\n     Data Section Dump     \n");
    memory_dump(mem, DATASECTION, 256);
}
