#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();

void write_read_demo(){
    int32_t data_to_write = 0xFFF; // equal to 4095
    int32_t address = 0x98; // equal to 152
    char* mem_file = "mem.txt";

    // Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
    int32_t write = write_address(data_to_write, address, mem_file);
    if(write == (int32_t) NULL)
        printf("ERROR: Unsucessful write to address %0X\n", 0x40);
    int32_t read = read_address(address, mem_file);

    printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}



void print_regs();

void print_regs(){
    int col_size = 10;
    for (int i = 0; i < 8; i++){
        printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
        printf("X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
        printf("X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
        printf("X%02i:%.*lld", i+24, col_size, (long long int) reg[i+24]);
    }
}
/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
    int reg_amount = 32;
    reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
    for(int i = 0; i < 32; i++)
        reg[i] = i;
}

int string_length(char str[]){
    int length = 0;
    for(int i = 0; str[i]!= '\0'; ++i) {
        length++;
    }
    return length;
}


bool check_strings(char str1[], char str2[]){
    for(int i = 0; i < string_length(str1); i++){
        if(str1[i] != str2[i]){
            return false;
        }
    }
    return true;
}

void load_word(char* token1, char* token2, char* token3){
    int32_t token_reg2 = atoi(token2);
    int32_t token_reg3 = atoi(token3);
    int32_t load_sum = reg[token_reg2] + token_reg3;
    int32_t loaded = read_address(load_sum,"mem.txt");
    reg[atoi(token1)]= loaded;
}

void store_word(char* token1, char* token2, char* token3){
    int32_t token_rs1 = atoi(token2);
    int32_t token_rs2 = atoi(token3);
    int32_t add_sum = reg[token_rs1] + token_rs2;
    int32_t stored = read_address(add_sum,"mem.txt");
    reg[atoi(token1)]= stored;
}

void add_reg(char* token1, char* token2, char* token3){
    int32_t token_rd = atoi(token1);
    int32_t token_rs1 = atoi(token2);
    int32_t token_rs2 = atoi(token3);
    reg[token_rd] = reg[token_rs1] + reg[token_rs2];
}

void addi_reg(char* token1, char* token2, char* token3){
    int32_t token_rd = atoi(token1);
    int32_t token_rs1 = atoi(token2);
    int32_t token_imm = atoi(token3);
    reg[token_rd] = reg[token_rs1] + reg[token_imm];
}

void or_reg(char* token1, char* token2, char* token3){
    int32_t token_rd = atoi(token1);
    int32_t token_rs1 = atoi(token2);
    int32_t token_rs2 = atoi(token3);
    reg[token_rd] = reg[token_rs1] || reg[token_rs2];
}

void and_reg(char* token1, char* token2, char* token3){
    int32_t token_rd = atoi(token1);
    int32_t token_rs1 = atoi(token2);
    int32_t token_rs2 = atoi(token3);
    reg[token_rd] = reg[token_rs1] + reg[token_rs2];
}
/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
//this method is use so that we can properly interpert, it will use pointers
bool interpret(char* instr) {
    char** tokens = (char **) strtok(instr, " ");
        if (check_strings(tokens[0], "LW") == 1) {
            load_word(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else if(check_strings(tokens[0], "SW") == 1){
            store_word(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else if(check_strings(tokens[0], "ADD") == 1){
            add_reg(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else if (check_strings(tokens[0], "ADDI") == 1){
            addi_reg(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else if (check_strings(tokens[0], "AND") == 1) {
            and_reg(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else if (check_strings(tokens[0], "OR")){
            or_reg(tokens[1], tokens[2], tokens[3]);
            print_regs();
        } else {
            printf("Try again");
        }
    return true;
}
/**
 * Your code goes in the main
 *
 */
int main(){
    // Do not write any code between init_regs
    scanf("\n");
    init_regs(); // DO NOT REMOVE THIS LINE
    char input[1000]; // User input can only be 100 chars long.
    printf("Enter d to quit.\n");
    //while loop so it can do the functions over and over until user press x, therefore it breaks
    while(1){
        printf("Input: ");
        fgets(input, sizeof(input), stdin);
        if (*input == 'd'){break;}
        interpret(input);
        printf("\n");
    }
    print_regs();

    return 0;
}