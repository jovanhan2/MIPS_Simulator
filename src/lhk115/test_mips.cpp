
#include "mips.h"
#include <iostream>
using namespace std;

void decode(uint32_t instr, uint32_t &rs1, uint32_t &rs2, uint32_t &rd, uint32_t &shift, uint32_t &fn); //decodes instruction and splits into appropirate bits

uint32_t makeinstr(uint32_t rs1, uint32_t rs2, uint32_t rd, uint32_t shift, uint32_t fn);

void testingroutine(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val, uint32_t outputval) {
	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, output, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;






	decode(instr, rs1, rs2, rd, shift, fn);




	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);

	//step
	err = mips_cpu_step(cpu);

	//read back dest register
	err = mips_cpu_get_register(cpu, rs2, &output);

	err = mips_cpu_get_register(cpu, rd, &output);




	//check if correct


	if ((err == mips_Success) && (output == outputval)) {
		passed = 1;

	}
	mips_test_end_test(testId, passed, instrName);



}
void testingroutineI(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val, uint32_t outputval) {
	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, output, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;






	decode(instr, rs1, rs2, rd, shift, fn);


	rd = rs2;

	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);

	//step
	err = mips_cpu_step(cpu);

	//read back dest register
	err = mips_cpu_get_register(cpu, rs2, &output);

	err = mips_cpu_get_register(cpu, rd, &output);




	//check if correct


	if ((err == mips_Success) && (output == outputval)) {
		passed = 1;

	}
	mips_test_end_test(testId, passed, instrName);



}

void testingroutineBranch(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val) {
	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, output, pc, branchpc, data;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;
	branchpc = 0;






	decode(instr, rs1, rs2, rd, shift, fn);

	data = instr & 65535;


	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);
	//step
	err = mips_cpu_step(cpu);

	//read back dest register

	err = mips_cpu_get_register(cpu, rs2, &output);
	err = mips_cpu_get_pc(cpu, &branchpc);
	//check if correct
	data = data << 2;
	/*
	*/

	if ((output == 0xFFFFFFFF) && (branchpc == pc + 4)) {
		passed = 1;
	}

	else if ((err == mips_Success) && (branchpc == pc + data)) {
		passed = 1;


	}

	mips_test_end_test(testId, passed, instrName);



}
void testingroutineBranchR1(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val) { //no R2




	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, output, pc, branchpc, data;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;
	branchpc = 0;






	decode(instr, rs1, rs2, rd, shift, fn);

	data = instr & 65535;


	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_set_register(cpu, rs1, reg1val);

	//step
	err = mips_cpu_step(cpu);

	//read back dest register

	err = mips_cpu_get_register(cpu, rs2, &output);
	err = mips_cpu_get_pc(cpu, &branchpc);
	//check if correct
	data = data << 2;
	/*
	*/

	if ((output == 0xFFFFFFFF) && (branchpc == pc + 4)) {
		passed = 1;
	}

	else if ((err == mips_Success) && (branchpc == pc + data)) {
		passed = 1;


	}



	mips_test_end_test(testId, passed, instrName);



}
void testingroutineJump(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val) {
	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, output, pc, branchpc, data, jump;
	jump = 0;
	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;
	branchpc = 0;






	decode(instr, rs1, rs2, rd, shift, fn);

	data = instr & 65535;
	jump = instr & 67108863;

	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory
	err = mips_cpu_set_register(cpu, rs1, reg1val);

	//step
	err = mips_cpu_step(cpu);

	//read back dest register
	err = mips_cpu_get_register(cpu, rs1, &output);

	err = mips_cpu_get_pc(cpu, &branchpc);
	//check if correct
	data = data << 2;
	/*
	*/

	if (output != 0) {

		if ((err == mips_Success) && (branchpc == (output * 4))) {
			passed = 1;
		}


	}


	else if ((err == mips_Success) && (branchpc == (jump << 2))) {
		passed = 1;

	}







	mips_test_end_test(testId, passed, instrName);



}
void testingroutineMULHILO(uint32_t instr, mips_cpu_h cpu, mips_mem_h mem, uint32_t &HI) {

	uint32_t  rs1, rs2, rd, shift, fn, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;






	decode(instr, rs1, rs2, rd, shift, fn);




	//int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);
	if (err == 0) {

	}

	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory


													  //step
	err = mips_cpu_step(cpu);

	//read back dest register



	err = mips_cpu_get_register(cpu, rd, &HI); // place value from reg into reg 1

											   //check if correct



											   //if ((err == mips_Success) && (LOREG == LO) && (HIREG == HI)) {
											   //	passed = 1;

											   //}
											   //mips_test_end_test(testId, passed, instrName);



}
void testingroutineMUL(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val, uint32_t LO, uint32_t HI) {
	int passed = 0;

	uint32_t  rs1, rs2, rd, shift, fn, pc, LOREG, HIREG;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;






	decode(instr, rs1, rs2, rd, shift, fn);




	int testId = mips_test_begin_test(instrName);

	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc


												//endianess
												//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

												//write to memory

												//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
												//set program counter
												//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);

	//step
	err = mips_cpu_step(cpu);

	//read back dest register



	// ---------------------------------------------- - --------

	//CHECK HI AND LO REGISTERS USING IMPLEMENTED FUNCTIONS

	//MFHI $3,$1,$2
	instr = 0b00000000000000000000100000010000;//instruction //unsigned case 0 case reg 1

	testingroutineMULHILO(instr, cpu, mem, HIREG);
	//MFLo $3,$1,$2
	instr = 0b00000000000000000000100000010010;//instruction //unsigned case 0 case
	testingroutineMULHILO(instr, cpu, mem, LOREG);
	/*
	tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);
	err = mips_cpu_get_pc(cpu, &pc);
	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_step(cpu);
	err = mips_cpu_get_register(cpu, rd, &HIREG); // place value from reg into reg 1
	*/
	/*//----------------------------------------------- --------
	//MFLo $3,$1,$2
	instr = 0b00000000000000000000100000010010;//instruction //unsigned case 0 case

	tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);
	err = mips_cpu_get_pc(cpu, &pc);
	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory

	err = mips_cpu_step(cpu);
	err = mips_cpu_get_register(cpu, rd, &LOREG); // place value from reg into reg 1
	*/
	//-------------------------------------------------------------------------------------------------------------------------------

	//err = mips_cpu_get_register(cpu, 17, &LOREG); //LOREG WITHIN TESTER, LO Is TESTING VALUE
	//err = mips_cpu_get_register(cpu, 18, &HIREG);
	//check if correct

	//&& (LOREG ==LO 

	if (((err == mips_Success)) && (LOREG == LO) && (HIREG == HI)) {
		passed = 1;

	}
	mips_test_end_test(testId, passed, instrName);



}

void testingroutineError(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val, uint32_t outputval, mips_error error) {
	int passed = 0;


	uint32_t  rs1, rs2, rd, shift, fn, output, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;

	decode(instr, rs1, rs2, rd, shift, fn);
	int testId = mips_test_begin_test(instrName);
	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc
	if (err == 0)
		err = mips_Success; //get rid of warnings



							//endianess
							//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

							//write to memory

							//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
							//set program counter
							//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory


	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);


	//step
	mips_error errstep = mips_cpu_step(cpu);

	//read back dest register

	err = mips_cpu_get_register(cpu, rd, &output);

	//check if correct
	//

	if ((errstep == error) && (output == outputval)) {
		passed = 1;

	}


	mips_test_end_test(testId, passed, instrName);



}
void testingroutineErrorI(uint32_t instr, const char* instrName, mips_cpu_h cpu, mips_mem_h mem, uint32_t reg1val, uint32_t reg2val, uint32_t outputval, mips_error error) {
	int passed = 0;


	uint32_t  rs1, rs2, rd, shift, fn, output, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	pc = 0xFFFF;

	decode(instr, rs1, rs2, rd, shift, fn);

	rd = rs2;
	int testId = mips_test_begin_test(instrName);
	mips_error err = mips_cpu_get_pc(cpu, &pc); //initialise pc
	if (err == 0)
		err = mips_Success; //get rid of warnings



							//endianess
							//uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

							//write to memory

							//err = mips_mem_write(mem, pc, (uint8_t*)&tmp);
							//set program counter
							//err = mips_cpu_set_pc(cpu, cpu->pc);


	uint32_t tmp = (instr << 24) | ((instr << 8) & 0x00FF0000) | ((instr >> 8) & 0x0000FF00) | (instr >> 24);

	err = mips_mem_write(mem, pc, 4, (uint8_t*)&tmp); //write to memory


	err = mips_cpu_set_register(cpu, rs1, reg1val);
	err = mips_cpu_set_register(cpu, rs2, reg2val);


	//step
	mips_error errstep = mips_cpu_step(cpu);

	//read back dest register

	err = mips_cpu_get_register(cpu, rd, &output);

	//check if correct
	//

	if ((errstep == error) && (output == outputval)) {
		passed = 1;

	}


	mips_test_end_test(testId, passed, instrName);



}




int main() {
	mips_error error;
	mips_mem_h mem = mips_mem_create_ram(4096);

	mips_cpu_h cpu = mips_cpu_create(mem);

	mips_cpu_reset(cpu);
	uint32_t instr;

	instr = 0;
	/*, rs1, rs2, rd, shift, fn, output, pc;

	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;

	output = 0;
	pc = 0;
	*/
	mips_test_begin_suite();

	//Order - Difficulty - 1 
	//----------------------------------------------------------------------------------------------
	//ADDU 
	//ADDU $3,$1,$2
	// addu 1+1 = 2
	instr = 0b00000000001000100001100000100001;//instruction

	testingroutine(instr, "ADDU", cpu, mem, 1, 1, 2);
	//----------------------------------------------------------------------------------------------
	//Bitwise AND
	//$3,$1,$2
	instr = 0b00000000001000100001100000100100;//instruction

	testingroutine(instr, "AND", cpu, mem, 0xFF0F, 0x0FFF, 0x0F0F);
	//----------------------------------------------------------------------------------------------
	//Bitwise OR
	//$3,$1,$2
	instr = 0b00000000001000100001100000100101;//instruction

	testingroutine(instr, "OR", cpu, mem, 0xFF00, 0x0FF0, 0xFFF0);

	//----------------------------------------------------------------------------------------------
	//SLTU Set on less than unsigned  TRUE CASE
	//$3,$1,$2
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00000000001000100001100000101011;//instruction

	testingroutine(instr, "SLTU", cpu, mem, 0x0001, 0x0FF0, 0x0001);


	//----------------------------------------------------------------------------------------------
	//SLTU Set on less than unsigned  FALSE CASE Bigger Number
	//$3,$1,$2
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00000000001000100001100000101011;//instruction
	testingroutine(instr, "SLTU", cpu, mem, 0xFFF0, 0x0FF0, 0x0000);
	//----------------------------------------------------------------------------------------------
	//SLTU Set on less than unsigned  SAME NUMBERS
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00000000001000100001100000101011;//instruction
	error = mips_ErrorInvalidArgument;
	testingroutineError(instr, "SLTU", cpu, mem, 0x0FF0, 0x0FF0, 0x0000, error);

	//----------------------------------------------------------------------------------------------
	//SUBTRACT UNSIGNED SUBU


	instr = 0b00000000001000100001100000100011;//instruction
	testingroutine(instr, "SUBU", cpu, mem, 0x000F, 0x0006, 0x0009);

	//----------------------------------------------------------------------------------------------
	//XOR
	instr = 0b00000000001000100001100000100110;//instruction
	testingroutine(instr, "XOR", cpu, mem, 0x0FF0, 0xFF00, 0xF0F0);

	//----------------------------------------------- -----------------------------------------------

	//DIFFICULTY - 2XX INSTR


	//----------------------------------------------- -----------------------------------------------

	//SB

	instr = 0b10100000001000100000000000000101;//instruction //2nd reg dst

	testingroutineI(instr, "SB", cpu, mem, 15, 0xFFFF, 0xFF); //I instr change dest reg
															  // ---------------------------------------------- - ---------------------------------------------- -
															  //LB
															  //LB	 $2,$1,0b1111
	instr = 0b10000000001000100000000000000101;//instruction //2nd reg dst

	testingroutineI(instr, "LB", cpu, mem, 15, 0x0FFFFFFF, 0xFF);



	//ADD (with NO OVERFLOW) //testing normal add
	instr = 0b00000000001000100001100000100000;//instruction
	testingroutine(instr, "ADD", cpu, mem, 2, 3, 5);
	//----------------------------------------------- -----------------------------------------------
	//ADD (with NO OVERFLOW) //testing add with negative numbers
	instr = 0b00000000001000100001100000100000;//instruction
	testingroutine(instr, "ADD", cpu, mem, 0x0002, 0xFFFFFFFF, 0x0001);
	//----------------------------------------------- -----------------------------------------------
	//ADD (with NO OVERFLOW) //testing two negative  
	instr = 0b00000000001000100001100000100000;//instruction

	testingroutine(instr, "ADD", cpu, mem, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE);
	//----------------------------------------------- -----------------------------------------------
	//ADD (with NO OVERFLOW) //testing two positive
	instr = 0b00000000001000100001100000100000;//instruction

	testingroutine(instr, "ADD", cpu, mem, 0x0FFFFFFF, 0x0FFFFFFF, 0x1FFFFFFE);

	//ADD (with OVERFLOW) //testing two positive
	instr = 0b00000000001000100001100000100000;//instruction
	error = mips_ExceptionArithmeticOverflow;
	testingroutineError(instr, "ADD", cpu, mem, 0x7FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFE, error);


	//----------------------------------------------- -----------------------------------------------
	//ADDIU (with NO OVERFLOW) //testing 7+4
	//addi $2,$1,4
	instr = 0b00100100001000100000000000000100;//instruction
	testingroutineI(instr, "ADDIU", cpu, mem, 7, 0x0FFFFFFF, 11);

	//----------------------------------------------- -----------------------------------------------
	//ADDI (with NO OVERFLOW) //testing 8+4
	//addi $2,$1,4

	instr = 0b00100000001000100000000000000100;//instruction

	testingroutineI(instr, "ADDI", cpu, mem, 8, 0x0FFFFFFF, 12);

	//----------------------------------------------- -----------------------------------------------
	//ADDI (with  OVERFLOW) //constant is 16 bits, overflows when Neg + Neg = pos
	//addi $2,$1,0xFFFF
	instr = 0b00100000001000101111111111111111;//instruction
	error = mips_ExceptionArithmeticOverflow;
	testingroutineI(instr, "ADDI", cpu, mem, 0xFFFFFFFF, 0x0FFFFFFF, 0xFFFFFFFE);

	//----------------------------------------------- -----------------------------------------------
	//ANDI (with  OVERFLOW) //constant is 16 bits, overflows when Neg + Neg = pos
	//ANDi $2,$1,0xFFFF

	instr = 0b00110000001000100000000000000100;//instruction

	testingroutineI(instr, "ANDI", cpu, mem, 0x00FFFFFFF, 0x0FFFFFFF, 4);


	//----------------------------------------------- -----------------------------------------------
	//LUI 
	//LUI $2,$1,0b1111

	instr = 0b00111100001000100000000000001111;//instruction //2nd reg dst

	testingroutineI(instr, "LUI", cpu, mem, 0x00FFFFFFF, 0x0FFFFFFF, 0x000F0000);

	//----------------------------------------------- --------
	//SLL $3,$1,$2
	instr = 0b00000000001000100001110000000000;//instruction //2nd reg dst

	testingroutine(instr, "SLL", cpu, mem, 0x0000FFFF, 0x0000FFFF, 0xFFFF0000);
	//----------------------------------------------- --------
	//SLT $3,$1,$2
	instr = 0b00000000001000100001100000101010;//instruction //unsigned case 0 case

	testingroutine(instr, "SLT", cpu, mem, 0xFFF0, 0x0FF0, 0x0000);
	//----------------------------------------------- --------
	//SLT $3,$1,$2
	instr = 0b00000000001000100001100000101010;//instruction //unsigned case 1 case

	testingroutine(instr, "SLT", cpu, mem, 0x00F0, 0x0FF0, 0x0001);

	//----------------------------------------------- --------
	//SLTU Set on less than unsigned  SAME NUMBERS
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00000000001000100001100000101010;//instruction
	error = mips_ErrorInvalidArgument;

	testingroutineError(instr, "SLT", cpu, mem, 0x0FF0, 0x0FF0, 0x0000, error);
	//----------------------------------------------- --------
	//SLT $3,$1,$2
	instr = 0b00000000001000100001100000101010;//instruction //single signed case 1 case

	testingroutine(instr, "SLT", cpu, mem, 0xFFFFFFF0, 0x0FF0, 1);

	//----------------------------------------------- --------
	//SLT $3,$1,$2
	instr = 0b00000000001000100001100000101010;//instruction //both signed case 1 case

	testingroutine(instr, "SLT", cpu, mem, 0xFFFFFFF0, 0xFFFFFFFF, 1);

	//----------------------------------------------- --------------------------------------------------------
	//SLT $3,$1,$2
	instr = 0b00000000001000100001100000101010;//instruction //both signed case 0 case

	testingroutine(instr, "SLT", cpu, mem, 0xFFFFFFFF, 0xFFFFFFF0, 0);
	//----------------------------------------------- --------------------------------------------------------
	//SRA $3,$1,$2
	instr = 0b00000000001000100001100100000011;//instruction //positive
	testingroutine(instr, "SRA", cpu, mem, 0xFFFFFFFF, 0xFFFF, 0xFFF);
	//SRA $3,$1,$2
	instr = 0b00000000001000100001100100000011;//instruction //negative

	testingroutine(instr, "SRA", cpu, mem, 0xFFFFFFFF, 0x800000FF, 0xF800000F);
	//----------------------------------------------- --------------------------------------------------------
	//SRL $3,$1,$2
	instr = 0b00000000001000100001100100000010;//instruction //positive
	testingroutine(instr, "SRL", cpu, mem, 0xFFFFFFFF, 0xFFFF, 0xFFF);
	//----------------------------------------------- --------------------------------------------------------
	//SUB $3,$1,$2 normal SUB
	instr = 0b00000000001000100001100000100010;//instruction 15-6 =9
	testingroutine(instr, "SUB", cpu, mem, 0x000F, 0x0006, 0x0009);

	//SUB $3,$1,$2 normal SUB SIGNED
	instr = 0b00000000001000100001100000100010;//instruction 15-16 = -1
	testingroutine(instr, "SUB", cpu, mem, 0x000F, 0xFFFFFFF0, 0xFFFFFFFF);
	//SUB $3,$1,$2 normal SUB SIGNED
	instr = 0b00000000001000100001100000100010;//instruction -16-15 = -31
	testingroutine(instr, "SUB", cpu, mem, 0xFFFFFFF0, 0x000F, 0xFFFFFFE1);

	//----------------------------------------------------------------------------------------------
	//SLLV
	instr = 0b00000000001000100001100000000100;//instruction
	testingroutine(instr, "SLLV", cpu, mem, 12, 0x0000FFFF, 0x0FFFF000);
	//----------------------------------------------- -----------------------------------------------
	//SW
	//SW $2,$1,0b1111
	instr = 0b10101100001000100000000000000101;//instruction //2nd reg dst

	testingroutineI(instr, "SW", cpu, mem, 3, 15, 15);

	//----------------------------------------------- -----------------------------------------------
	//LW
	//LW $2,$1,0b1111
	instr = 0b10001100001000100000000000000101;//instruction //2nd reg dst

	testingroutineI(instr, "LW", cpu, mem, 3, 0x0FFFFFFF, 15);


	//----------------------------------------------- --------
	//MTHI $3,$1,$2
	instr = 0b00000000001000000000000000010001;//instruction //unsigned case 0 case

	testingroutine(instr, "MTHI", cpu, mem, 62, 0x0FF0, 0);
	//----------------------------------------------- --------
	//MFHI $3,$1,$2
	instr = 0b00000000000000000000100000010000;//instruction //unsigned case 0 case

	testingroutine(instr, "MFHI", cpu, mem, 0, 0x0FF0, 62);


	//MTLO $3,$1,$2
	instr = 0b00000000001000000000000000010011;//instruction //unsigned case 0 case

	testingroutine(instr, "MTLO", cpu, mem, 63, 0x0FF0, 0);
	//----------------------------------------------- --------
	//MFLo $3,$1,$2
	instr = 0b00000000000000000000100000010010;//instruction //unsigned case 0 case

	testingroutine(instr, "MFLO", cpu, mem, 00, 0x0FF0, 63);
	//----------------------------------------------------------------------------------------------
	//SLTIU Set on less than unsigned  FALSE CASE Bigger Number
	//$3,$1,$2
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00101100001000100000000000001010;//instruction
	testingroutineI(instr, "SLTIU", cpu, mem, 1, 0x0FF0, 1);
	// less
	instr = 0b00101100001000100000000000001010;//instruction
	testingroutineI(instr, "SLTIU", cpu, mem, 11, 0x0FF0, 0);

	instr = 0b00101100001000100000000000001010;//instruction
	error = mips_ErrorInvalidArgument;

	testingroutineErrorI(instr, "SLTIU", cpu, mem, 10, 2, 0, error);

	//----------------------------------------------- -----------------------------------------------
	//ORI
	//ORi $2,$1,0b1111
	instr = 0b00110100010000110000000000000101;//instruction //2nd reg dst

	testingroutineI(instr, "ORI", cpu, mem, 0b1110, 0x0FFFFFFF, 15);

	//----------------------------------------------------------------------------------------------
	//XORI
	instr = 0b00111000001000100000000000000110;//instruction
	testingroutineI(instr, "XORI", cpu, mem, 0b1011, 99, 0b1101);









	//----------------------------------------------- -----------------------------------------------
	//COMPLEXITY 3





	//----------------------------------------------- -----------------------------------------------
	//----------------------------------------------- -----------------------------------------------
	//BEQ
	//BEQ$2,$1,0b1111
	instr = 0b00010000001000100000000000000101;//instruction //2nd reg dst
	testingroutineBranch(instr, "BEQ", cpu, mem, 1, 1);
	//----------------------------------------------- -----------------------------------------------
	//BEQ
	//BEQ$2,$1,0b1111
	instr = 0b00010000001000100000000000000101;//instruction //2nd reg dst NO BRANCH
	testingroutineBranch(instr, "BEQ", cpu, mem, 5, 1);
	//----------------------------------------------- -----------------------------------------------
	//BGEZ
	//BEQ$2,$1,0b1111
	instr = 0b00000100001000010000000000000101;//instruction //2nd reg dst BRANCH
	testingroutineBranchR1(instr, "BGEZ", cpu, mem, 1);

	//BGEZ
	//BEQ$2,$1,0b1111
	instr = 0b00000100001000010000000000000101;//instruction //2nd reg dst BRANCH 0 case
	testingroutineBranchR1(instr, "BGEZ", cpu, mem, 0);

	//BGEZ

	instr = 0b00000100001000010000000000000101;//instruction //Negative number
	testingroutineBranchR1(instr, "BGEZ", cpu, mem, 0xFFFFFFFF);



	//----------------------------------------------- -----------------------------------------------
	//BGTZ

	instr = 0b00011100001000000000000000000101;//instruction //BRANCH
	testingroutineBranchR1(instr, "BGTZ", cpu, mem, 1);
	//BGTZ
	//BEQ$2,$1,0b1111
	instr = 0b00011100001000000000000000000101;//instruction //ZERO CASE
	testingroutineBranchR1(instr, "BGTZ", cpu, mem, 0);
	//BGTZ
	//BEQ$2,$1,0b1111
	instr = 0b00011100001000000000000000000101;//instruction //NEGATIVE CASE
	testingroutineBranchR1(instr, "BGTZ", cpu, mem, 0xFFFFFFFF);


	//----------------------------------------------- -----------------------------------------------
	//BLEZ

	instr = 0b00011000001000000000000000000101;//instruction //BRANCH NEGATIVE CASE
	testingroutineBranchR1(instr, "BLEZ", cpu, mem, 0xFFFFFFFF);

	instr = 0b00011000001000000000000000000101;//instruction //BRANCH ZERO CASE
	testingroutineBranchR1(instr, "BLEZ", cpu, mem, 0);

	instr = 0b00011000001000000000000000000101;//instruction //BRANCH POSITIVE CASE
	testingroutineBranchR1(instr, "BLEZ", cpu, mem, 5);


	//----------------------------------------------- -----------------------------------------------
	//BLTZ

	instr = 0b00000100001000000000000000000101;//instruction //BRANCH NEGATIVE CASE
	testingroutineBranchR1(instr, "BLTZ", cpu, mem, 0xFFFFFFFF);

	instr = 0b00000100001000000000000000000101;//instruction //BRANCH ZERO CASE
	testingroutineBranchR1(instr, "BLTZ", cpu, mem, 0);

	instr = 0b00000100001000000000000000000101;//instruction //BRANCH POSITIVE CASE
	testingroutineBranchR1(instr, "BLTZ", cpu, mem, 5);



	//----------------------------------------------- -----------------------------------------------
	//BNE

	instr = 0b00010100001000100000000000000101;//instruction //NO BRANCH
	testingroutineBranch(instr, "BNE", cpu, mem, 1, 1);
	//BNE

	instr = 0b00010100001000100000000000000101;//instruction //BRANCH
	testingroutineBranch(instr, "BNE", cpu, mem, 1, 0);


	//----------------------------------------------- -----------------------------------------------

	//JUMP

	instr = 0b00001000000000000000000000001111;//instruction //BRANCH
	testingroutineJump(instr, "J", cpu, mem, 0);
	//----------------------------------------------- -----------------------------------------------

	//JR

	instr = 0b00000000001000000000000000001000;//instruction //BRANCH
	testingroutineJump(instr, "JR", cpu, mem, 0xFF);



	//----------------------------------------------------------------------------------------------
	//SLTIU Set on less than unsigned  FALSE CASE Bigger Number
	//$3,$1,$2
	//if $1 < $2 ,
	//$3 =1
	//else $3 =0
	instr = 0b00101000001000100000000000001010;//unsigned 0 case
	testingroutineI(instr, "SLTI", cpu, mem, 1, 0x0FF0, 1);

	instr = 0b00101000001000100000000000001010;//unsigned 1 case
	testingroutineI(instr, "SLTI", cpu, mem, 11, 0x0FF0, 0);



	instr = 0b00101000001000101111111111111111;//signed 1 case
	testingroutineI(instr, "SLTI", cpu, mem, 1, 0x0FF0, 1);

	instr = 0b00101000001000101111111111111111;//both signed 1case
	testingroutineI(instr, "SLTI", cpu, mem, 0xFFFF0000, 0x0FF0, 1);

	instr = 0b00101000001000101111111111111110;//both signed 1 case
	testingroutineI(instr, "SLTI", cpu, mem, 0xFFFFFFFF, 0x0FF0, 1);

	instr = 0b00101000001000100000000000001010;//signed 0 case

	testingroutineI(instr, "SLTI", cpu, mem, 0xFFFFFFFF, 0x0FF0, 1);


	//----------------------------------------------------------------------------------------------

	//SRAV $3,$1,$2
	instr = 0b00000000001000100001100000000111;//instruction //positive
	testingroutine(instr, "SRAV", cpu, mem, 4, 0xFFFF, 0xFFF);

	//SRAV $3,$1,$2
	instr = 0b00000000001000100001100000000111;//instruction //positive
	testingroutine(instr, "SRAV", cpu, mem, 8, 0x8000FFFF, 0xFF8000FF);

	//----------------------------------------------- --------------------------------------------------------
	//SRLV $3,$1,$2
	instr = 0b00000000001000100001100000000110;//instruction //positive

	testingroutine(instr, "SRLV", cpu, mem, 8, 0xFFFF, 0xFF);
	//----------------------------------------------- --------------------------------------------------------


	//DIFFICULTY 4 

	//----------------------------------------------------------------------------------------------


	//MULTU

	instr = 0b00000000001000100001100000011001;//instruction one LO
	testingroutineMUL(instr, "MULTU", cpu, mem, 8, 8, 64, 0);

	instr = 0b00000000001000100001100000011001;//instruction one HI 2^32 * 2
	testingroutineMUL(instr, "MULTU", cpu, mem, 2147483648, 2, 0, 1);
	instr = 0b00000000001000100001100000011001;//instruction one HI one LO 2^32 * 2
	testingroutineMUL(instr, "MULTU", cpu, mem, 0xFFFFFFFF, 32, 4294967264, 31);




	//MULT

	instr = 0b00000000001000100001100000011000;//instruction negative
	testingroutineMUL(instr, "MULT", cpu, mem, -5, -5, 25, 0);



	instr = 0b00000000001000100001100000011000;//positive
	testingroutineMUL(instr, "MULT", cpu, mem, 6, 6, 36, 0);


	instr = 0b00000000001000100001100000011000;//positivie //one high
	testingroutineMUL(instr, "MULT", cpu, mem, 2147483648, 2, 0, 0xFFFFFFFF);

	instr = 0b00000000001000100001100000011000;//one pos one neg
	testingroutineMUL(instr, "MULT", cpu, mem, -5, 10, -50, 0xFFFFFFFF);

	instr = 0b00000000001000100001100000011000;//instruction negative//one high one low
	testingroutineMUL(instr, "MULT", cpu, mem, 0xFFFFFFFF, 32, -32, -1);



	//DIVU

	instr = 0b00000000001000100000000000011011;//instruction 
	testingroutineMUL(instr, "DIVU", cpu, mem, 13, 5, 2, 3);


	//DIV

	instr = 0b00000000001000100000000000011010;//instruction normal
	testingroutineMUL(instr, "DIV", cpu, mem, 13, 5, 2, 3);
	//DIV

	instr = 0b00000000001000100000000000011010;//instruction 1st neg
	testingroutineMUL(instr, "DIV", cpu, mem, -61, 5, -12, -1);
	//DIV

	instr = 0b00000000001000100000000000011010;//instruction 2ng neg
	testingroutineMUL(instr, "DIV", cpu, mem, 82, -5, -16, 2);
	//DIV

	instr = 0b00000000001000100000000000011010;//instruction both neg
	testingroutineMUL(instr, "DIV", cpu, mem, -100, -10, 10, 0);



	/*





	//----------------------------------------------------------------------------------------------


	testingroutine(instr, "ADDU", cpu, mem, 1, 1, 2);


	mips_test_begin_suite();
	uint32_t currentinst = 0x00851021;

	testId=mips_test_begin_test("addu");

	uint32_t tmp = (currentinst << 24) | ((currentinst<<8)&0x00FF0000)| ((currentinst>>8)&0x0000FF00) | (currentinst>>24);

	mips_mem_write(mem,0,(uint8_t*)&tmp);

	mips_step_cpu(cpu);
	mips_test_end_test(testId,1,0);

	mips_test_end_suite();

	*/
	mips_test_end_suite();
}

void decode(uint32_t instr, uint32_t &rs1, uint32_t &rs2, uint32_t &rd, uint32_t &shift, uint32_t &fn) {

	rs1 = instr >> 21;
	rs1 = rs1 & 31;

	rs2 = instr >> 16;
	rs2 = rs2 & 31;

	rd = instr >> 11;
	rd = rd & 31;


	shift = instr >> 6;
	shift = shift & 31;

	fn = instr & 63;



}