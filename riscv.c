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
/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
//this method is use so that we can properly interpert, it will use pointers
int find_instr(char* instr) {
    char *tokens = strtok(instr, " ");
    if (check_strings(&tokens[0], "LW") == 1) {
        return 2;
    } else if (check_strings(&tokens[0], "SW") == 1) {
        return 3;
    } else if (check_strings(&tokens[0], "ADD") == 1) {
        return 4;
    } else if (check_strings(&tokens[0], "ADDI") == 1) {
        return 5;
    } else if (check_strings(&tokens[0], "AND") == 1) {
        return 6;
    } else if (check_strings(&tokens[0], "OR")) {
        return 7;
    }
    return 0;
}


bool interpret(char* instr) {
    int token_rs1;
    int token_rs2;
    int token_rd;
    int token_imm;

    char storing_delim[] = {"SW X LW ()"}; //holds the delimeters for the storing
    char adding_delim[] = {"ADD X ADDI"};
    char extra_delim[] = {"AND X OR"};

    char *token = strtok(instr, adding_delim); //tokenize using the adding delimeters (ADD, ADDI)
    char *token2 = strtok(instr, storing_delim);//tokenize using the storing delimeters (SW, LW)
    char *extra_token = strtok(instr, extra_delim);//tokenize using the extra delimeters (AND, OR)

    if (find_instr(instr) ==2){
        printf("-------------------------- \n");
        printf("You selected: Load Word \n");
        printf("-------------------------- \n");
        token_rd = atoi(token2);
        token2 = strtok(NULL, storing_delim); //make jump to the next token
        token_rs1 = atoi(token2);
        token2 = strtok(NULL, storing_delim); //make jump to the next token
        token_imm = atoi(token2);
        int32_t read = read_address(token_rs1 + token_imm, "mem.txt"); //read the address at rs1 and imm
        reg[token_rd] = read; //update register
        printf("\n");
        printf("You have loaded:%d \n"); //display result

    } else if (find_instr(instr) ==3) {
        printf("-------------------------- \n");
        printf("You selected: Store Word \n");
        printf("-------------------------- \n");
        token_rd = atoi(token2);
        token2 = strtok(NULL, storing_delim); //make jump to the next token
        token_rs1 = atoi(token2);
        token2 = strtok(NULL, storing_delim); //make jump to the next token
        token_imm = atoi(token2);
        int32_t address = token_rs1 + token_imm; //
        reg[token_rd] = write_address(reg[token_rd],address,"mem.txt"); //write to address and update register
        printf("\n");
        printf("You have stored:%d \n");

    } else if (find_instr(instr) == 4) {
        printf("-------------------------- \n");
        printf("You selected: Add \n");
        printf("-------------------------- \n");
        token_rd = atoi(token);
        token = strtok(NULL, adding_delim); //make jump to the next tokenized token
        token_rs1 = atoi(token);
        token = strtok(NULL, adding_delim); //make jump to the next tokenized token
        token_rs2 = atoi(token);
        int sum = token_rs1 + token_rs2;
        reg[token_rd] = sum; //write result to register
        printf("\n");
        printf("You have added:%d \n", sum);
        return true;

    } else if (find_instr(instr) == 5) {
        printf("-------------------------- \n");
        printf("You selected: Addi \n");
        printf("-------------------------- \n");
        token_rd = atoi(token);
        token = strtok(NULL, adding_delim); //make jump to the next tokenized token
        token_rs1 = atoi(token);
        token = strtok(NULL, adding_delim); //make jump to the next tokenized token
        token_imm = atoi(token);
        int sum2 = token_rs1 + token_imm;
        reg[token_rd] = sum2; //write result to register
        printf("\n");
        printf("You have added:%d \n", sum2);

    } else if (find_instr(instr) == 6) {
        printf("-------------------------- \n");
        printf("You selected: And \n");
        printf("-------------------------- \n");
        token_rd = atoi(extra_token);
        extra_token = strtok(NULL, extra_delim); //make jump to the next tokenized token
        token_rs1 = atoi(extra_token);
        extra_token = strtok(NULL, extra_delim); //make jump to the next tokenized token
        token_rs2 = atoi(extra_token);
        int and_result = token_rs1 & token_rs2;
        reg[token_rd] = and_result; //write result to register
        printf("\n");
        printf("Your result is:%d \n", and_result);

    } else if (find_instr(instr) == 7) {
        printf("-------------------------- \n");
        printf("You selected: Or \n");
        printf("-------------------------- \n");
        token_rd = atoi(extra_token);
        extra_token = strtok(NULL, extra_delim); //make jump to the next tokenized token
        token_rs1 = atoi(extra_token);
        extra_token = strtok(NULL, extra_delim); //make jump to the next tokenized token
        token_rs2 = atoi(extra_token);
        int or_result = token_rs1 || token_rs2;
        reg[token_rd] = or_result; //write to register
        printf("\n");
        printf("Your result is:%d \n", or_result);
    } else {
        printf("Invalid instruction. Try again. \n");
    }
    return true;
}
/**
 * Your code goes in the main
 *
 */
int main(){
    init_regs();
    printf("RISCV Simulator \n");
    printf("-------------------------- \n");
    print_regs();
    printf("\n");
    printf("-------------------------- \n");
    // Do not write any code between init_regs./

    char input[1000]; // User input can only be 100 chars long.

    printf("\n");
    printf("Write your desired RISC-V instruction, or enter [x] to quit.\n");
    printf("-------------------------- \n");
    //while loop so it can do the functions over and over until user press x, therefore it breaks
    while(1){
        printf("Enter input: ");
        fgets(input, sizeof(input), stdin);
        if (*input == 'x'){
            break;
        }
        interpret(input);
        printf("-------------------------- \n");
        print_regs();
        printf("\n");
        printf("-------------------------- \n");
    }
    print_regs();
    printf("\n");
    printf("-------------------------- \n");
    printf("Thank you for using RISCV Simulator!");

    return 0;
}