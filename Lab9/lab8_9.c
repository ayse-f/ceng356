// Lab 9 MIPS Architecture Design, CPU Code Execution 
// This lab executes the code stored in the memory and prints the results
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
      
      //decoding the fetched instruction
      opcode = CPU_Decode(machineCode);
      printf("Decoded Opcode is: %02X. \n", opcode);

      // Lab 9: Finish the execution of the code.
      CPU_Execution(opcode, machineCode, mem);
    }while (1); 

    printRegisterFiles();     //Prints all the register contents on screen.
    printDataMemoryDump(mem); //Prints the memory dump of the data section.
}

// Lab 8 - Step 1
//This function gets one 32-bit machine code instruction from memory
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
    unsigned char rs = 0;
    unsigned char rd = 0;
    short immediate = 0;
    unsigned int address = 0;
    unsigned int memAddress = 0;
    unsigned char realOpcode = 0;
    realOpcode = (machineCode >> 26) & 0x3F;//to get the real opcode from the first 6 bits
    //to handle R type add seperatly because CPU_Decode returns function field for R type
    if (realOpcode == 0 && opcode== 0b100000){ //add instruction
        //getting the source and destination register numbers from the instruction
        rs = (machineCode & 0x03E00000) >> 21;
        rt = (machineCode & 0x001F0000) >> 16;
        rd = (machineCode & 0x0000F800) >> 11;

        //adding the two source registers and storing the result in rd
        regFile[rd] = regFile[rs] + regFile[rt];
        PCRegister += 4; //moving program counter to the next instruction

        if (DEBUG_CODE){
            printf("Code Executed: %08X\n", machineCode);
            printf("       PC Register is %08X      \n", PCRegister);
        }
        regFile[0] = 0; //keeping $zero always equal to 0
        return;        

    }
    switch (opcode)  // execute different functions when opcode is set differently. 
    {
		// implementing the following instructions here: 
		// la, add, lb, bge, lw, sw, addi, j
        case 0b101111:   //"la" instruction. 
            // assigning the address rt = immediate address stored in machineCode;
            // first finding the rt index in the register array. 
            rt = (machineCode & 0x001F0000) >> 16; 
            // assigning the address stored in immediate field to regFile[rt];
            regFile[rt] = machineCode & 0x0000FFFF;  // get the last 16 bit as address. 
            PCRegister += 4;
            if (DEBUG_CODE){   // print the hints to the user in DEBUG_MODE
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);                
            }            
            break;
        case 0b100000://"lb" instruction
            //getting the base register, target register and offset
		    rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = machineCode & 0x0000FFFF;

            //building the data memory address
            memAddress = DATASECTION+ regFile[rs] + immediate;
            //reading one byte from memory and storing it in rt
            regFile[rt] = (signed char) read_byte(mem, memAddress);

            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }    
			break;
		
        case 0b000001: //"bge" instruction
            //Getting the two register to compare & the branch target
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            address = machineCode & 0x0000FFFF;
            // if rs >=rt jumps to the target address, else we continue to the next instruction
            if (regFile[rs] >= regFile[rt]){
                PCRegister = address << 2;
            }
            else{ 
                PCRegister += 4;
            }

            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        case 0b100011:   //"lw" instruction
            //getting the base register, target register and offset
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = machineCode & 0x0000FFFF;

            memAddress = DATASECTION+ regFile[rs] + immediate;
            //reading one 32 bit word from memory and storing it in rt
            regFile[rt] = read_dword(mem, memAddress); 

            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        case 0b101011:   //"sw" instruction
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = machineCode & 0x0000FFFF;

            memAddress = DATASECTION+ regFile[rs] + immediate;
            // writing one 32 bit word from rt into memory
            write_dword(mem, memAddress, regFile[rt]); 

            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;
    
        case 0b001000:   //"addi" instruction
        //getting the source register, target register and immediate value
            rs = (machineCode & 0x03E00000) >> 21;
            rt = (machineCode & 0x001F0000) >> 16;
            immediate = machineCode & 0x0000FFFF;
            //adding the immeadiate value to rs and storing the result in rt
            regFile[rt] = regFile[rs] + immediate;

            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;
        
        case 0b000010:   //"j" instruction
            address = machineCode & 0x03FFFFFF; //getting the jump target address
            PCRegister = address << 2; //jumping directly to that address

            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;    

        // Should never go to default part when complete. Otherwise, that is a mistake. 
        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode,  machineCode);
            system("PAUSE");
            exit(3);  // exit the program if running here     
            break;
    regFile[0]= 0; //keeping $zero always equal to 0 after every instruction
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
