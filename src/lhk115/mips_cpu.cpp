#include "mips.h"
#include <iostream>
#include <bitset>
using namespace std;


//mips_error mips_decoder(uint32_t &currentinst, uint32_t &rs1, uint32_t &rs2, uint32_t &rd, uint32_t &shift, uint32_t &fncode, uint32_t &address)
//decodes the instruction set and get the relevant source and dest
void signExt(uint32_t &data);
struct mips_cpu_impl

{
	uint32_t pc;
	uint32_t regs[32];
	uint32_t HI;
	uint32_t LO;
	mips_mem_h mem;

};

mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
	) {
	state->pc = pc;
	return mips_Success;
}
mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc) {
	*pc = state->pc;
	return mips_Success;
}

mips_cpu_h mips_cpu_create(mips_mem_h mem)
{
	mips_cpu_impl *state = new mips_cpu_impl;

	state->pc = 0;

	for (unsigned i = 0; i < 32; i++) {
		state->regs[i] = 0;
	}
	state->mem = mem;
	state->HI = 0;
	state->LO = 0;

	return state;

}
//get register values
mips_error mips_cpu_reset(mips_cpu_h state) {
	state->pc = 0;

	//reset registers
	for (int i = 0; i < 32; i++) {

		state->regs[i] = 0;
	}
	return mips_Success;
}

mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
	)
{
	if (state == 0)
		return mips_ErrorInvalidHandle;
	if (index >= 32)
		return mips_ExceptionInvalidAlignment;

	*value = state->regs[index]; //location to write the variable to // write whats in the register to the location
	return mips_Success;

}


//need one cpu step
mips_error mips_cpu_step(mips_cpu_h state) {
	int overflow = 0;
	//endian
	uint32_t tmp = 0;
	//initialize
	uint32_t rs1, rs2, rd, shift, fn, currentinst, opcode, data, jump;
	uint64_t mulDest;
	uint32_t destination, p; // registers for load and store

							 //point
	jump = 0;
	rs1 = 0;
	rs2 = 0;
	rd = 0;
	shift = 0;
	fn = 0;
	data = 0;
	currentinst = 0;
	opcode = 9999;
	//uint32_t instr = 0b00000000001000100001100101000110;
	//advances pc by 4
	state->regs[0] = 0;


	mips_error	err = mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&currentinst); //target pointer recieves bytes
	if (err != 0)
		return mips_ErrorNotImplemented;

	tmp = (currentinst << 24) | ((currentinst << 8) & 0x00FF0000) | ((currentinst >> 8) & 0x0000FF00) | (currentinst >> 24);
	currentinst = tmp;


	state->pc = state->pc + 4; // advances pc after reading
							   //mips_decoder(currentinst, rs1, rs2, rd, shift, fn, address);
							   //if statment




	opcode = currentinst >> 26;


	rs1 = currentinst >> 21;
	rs1 = rs1 & 31;

	rs2 = currentinst >> 16;
	rs2 = rs2 & 31;

	rd = currentinst >> 11;
	rd = rd & 31;
	jump = currentinst & 67108863;
	data = currentinst & 65535;

	shift = currentinst >> 6;
	shift = shift & 31;

	fn = currentinst & 63;
	//testing





	if (opcode == 0) { //R type
					   //testing addu
		switch (fn) {
		case 0://SLL
			state->regs[rd] = state->regs[rs2] << (shift);
			return mips_Success;
			break;
		case 2://SRL

			state->regs[rd] = state->regs[rs2] >> (shift);
			return mips_Success;
			break;

		case 3://SRA


			if (((state->regs[rs2]) & 0x80000000) == 0x80000000) {

				state->regs[rd] = state->regs[rs2] >> (shift);
				state->regs[rd] = state->regs[rd] | (0xFFFFFFFF << ((32 - shift)));
				return mips_Success;

			}
			else {


				state->regs[rd] = state->regs[rs2] >> (shift);
				return mips_Success;
			}
			break;

		case 4:
			//SLLV
			state->regs[rd] = state->regs[rs2] << state->regs[rs1];
			return mips_Success;
			break;

		case 6://SRLV

			state->regs[rd] = state->regs[rs2] >> (state->regs[rs1]);
			return mips_Success;
			break;
		case 7://SRAV


			if (((state->regs[rs2]) & 0x80000000) == 0x80000000) {

				state->regs[rd] = state->regs[rs2] >> (state->regs[rs1]);
				state->regs[rd] = state->regs[rd] | (0xFFFFFFFF << ((32 - state->regs[rs1])));
				return mips_Success;

			}
			else {


				state->regs[rd] = state->regs[rs2] >> (state->regs[rs1]);
				return mips_Success;
			}
			break;

		case 8: //JR




			state->pc = (state->regs[rs1] << 2);

			return mips_Success;

			break;

		case 16:
			//MFHI
			//state->HI = 100;
			state->regs[rd] = state->HI;


			return mips_Success;
			break;

		case 17:
			//MTHi
			state->HI = state->regs[rs1];

			//state->regs[rd] = state->HI;

			return mips_Success;
			break;
		case 18:
			//MFLO

			//state->LO = 90;
			state->regs[rd] = state->LO;
			return mips_Success;
			break;
		case 19:
			//MTLO
			state->LO = state->regs[rs1];




			return mips_Success;

			break;

		case 24://MULT



			if ((((state->regs[rs1]) & 0x80000000) == 0) && (((state->regs[rs2]) & 0x80000000) == 0x0)) {


				mulDest = (uint64_t)state->regs[rs1] * state->regs[rs2];
				state->LO = (uint32_t)mulDest;
				state->HI = mulDest >> 32;
				//place HI and LO into register 17 and 18 resp for checking



			}

			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x0)) {
				state->regs[rs1] = ~state->regs[rs1] + 1;//convert to positive

				mulDest = (uint64_t)state->regs[rs1] * state->regs[rs2];
				mulDest = ~mulDest + 1; //convert to negative


				state->LO = (uint32_t)mulDest;
				state->HI = mulDest >> 32;
				//place HI and LO into register 17 and 18 resp for checking





			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0) && (((state->regs[rs2]) & 0x80000000) == 0x80000000)) {
				state->regs[rs2] = ~state->regs[rs2] + 1; //convert to positive

				mulDest = (uint64_t)state->regs[rs1] * state->regs[rs2];
				mulDest = ~mulDest + 1; //convert to negative


				state->LO = (uint32_t)mulDest;
				state->HI = mulDest >> 32;
				//place HI and LO into register 17 and 18 resp for checking




			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x80000000)) {
				state->regs[rs2] = ~state->regs[rs2] + 1;//convert to positive
				state->regs[rs1] = ~state->regs[rs1] + 1;//convert to positive

				mulDest = (uint64_t)state->regs[rs1] * state->regs[rs2];



				state->LO = (uint32_t)mulDest;
				state->HI = mulDest >> 32;
				//place HI and LO into register 17 and 18 resp for checking



			}







			return mips_Success;
			break;


		case 25://MULTU
			mulDest = (uint64_t)state->regs[rs1] * state->regs[rs2];
			state->LO = (uint32_t)mulDest;
			state->HI = mulDest >> 32;
			//place HI and LO into register 17 and 18 resp for checking



			return mips_Success;
			break;
		case 26://DIV


			state->LO = state->regs[rs1] / state->regs[rs2];


			state->HI = state->regs[rs1] % state->regs[rs2];

			//place HI and LO into register 17 and 18 resp for checking



			if ((((state->regs[rs1]) & 0x80000000) == 0) && (((state->regs[rs2]) & 0x80000000) == 0x0)) {

				state->LO = state->regs[rs1] / state->regs[rs2];


				state->HI = state->regs[rs1] % state->regs[rs2];

				//place HI and LO into register 17 and 18 resp for checking



			}

			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x0)) {

				state->regs[rs1] = ~state->regs[rs1] + 1;//convert to positive

				state->LO = state->regs[rs1] / state->regs[rs2];
				state->LO = ~state->LO + 1;
				state->HI = state->regs[rs1] % state->regs[rs2];


				state->HI = ~state->HI + 1;



			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0) && (((state->regs[rs2]) & 0x80000000) == 0x80000000)) {
				state->regs[rs2] = ~state->regs[rs2] + 1;//convert to positive


				state->LO = state->regs[rs1] / state->regs[rs2];
				state->LO = ~state->LO + 1;
				state->HI = state->regs[rs1] % state->regs[rs2];






			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x80000000)) {
				state->regs[rs2] = ~state->regs[rs2] + 1;//convert to positive
				state->regs[rs1] = ~state->regs[rs1] + 1;//convert to positive

				state->LO = state->regs[rs1] / state->regs[rs2];


				state->HI = state->regs[rs1] % state->regs[rs2];




			}







			return mips_Success;
			break;



		case 27://DIVU


			state->LO = state->regs[rs1] / state->regs[rs2];


			state->HI = state->regs[rs1] % state->regs[rs2];

			//place HI and LO into register 17 and 18 resp for checking




			return mips_Success;
			break;

		case 32://ADD
				/*If the sum of two positive numbers yields a negative result, the sum has overflowed.
				If the sum of two negative numbers yields a positive result, the sum has overflowed.
				Otherwise, the sum has not overflowed.*/
			state->regs[rd] = state->regs[rs1] + state->regs[rs2];


			if ((((state->regs[rs1]) & 0x80000000) == 0x0) && (((state->regs[rs2]) & 0x80000000) == 0x0) && (((state->regs[rd]) & 0x80000000) == 0x80000000)) {
				//two positive gets a negative 
				overflow = 1;
			};
			if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x80000000) && (((state->regs[rd]) & 0x80000000) == 0x0)) {
				//two negative yield positive
				overflow = 1;
			};





			if (overflow == 1) {
				return mips_ExceptionArithmeticOverflow;

			}
			else {

				return mips_Success;
			}
			break;



		case 33://ADDU
			state->regs[rd] = state->regs[rs1] + state->regs[rs2];
			return mips_Success;
			break;

		case 34://SUB

				//if ((state->regs[rs1] & 0x80000000) == 0x80000000) {
				//state->regs[rs1] = ~(state->regs[rs1]) + 1;
				//state->regs[rs1] = -(state->regs[rs1]);
				//}

			if ((state->regs[rs2] & 0x80000000) == 0) {
				state->regs[rs2] = ~(state->regs[rs2]) + 1;
				//state->regs[rs2] = -(state->regs[rs2]);

			}



			state->regs[rd] = state->regs[rs1] + state->regs[rs2];
			return mips_Success;


			break;










		case 35://SUBU
			state->regs[rd] = state->regs[rs1] - state->regs[rs2];
			return mips_Success;
			break;

		case 36://AND
			state->regs[rd] = state->regs[rs1] & state->regs[rs2];
			return mips_Success;
			break;

		case 37://OR
			state->regs[rd] = state->regs[rs1] | state->regs[rs2];
			return mips_Success;
			break;
		case 38://XOR
			state->regs[rd] = state->regs[rs1] ^ state->regs[rs2];
			return mips_Success;
			break;
		case 42://SLT


			if (state->regs[rs1] == state->regs[rs2]) {
				state->regs[rd] = 0;
				return mips_ErrorInvalidArgument;

			}

			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x0)) {


				state->regs[rd] = 1;
				return mips_Success;
			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((state->regs[rs2]) & 0x80000000) == 0x80000000)) {
				overflow = state->regs[rs2] - state->regs[rs1];
				if (overflow > 0) {
					state->regs[rd] = 1;
					return mips_Success;
				}
				else
					state->regs[rd] = 0;
				return mips_Success;
			}


			else {
				if (state->regs[rs1] < state->regs[rs2]) {
					state->regs[rd] = 1;
				}
				else
					state->regs[rd] = 0;
			}

			return mips_Success;

			break;




		case 43://SLTU
			if (state->regs[rs1] == state->regs[rs2]) {
				return mips_ErrorInvalidArgument;
				state->regs[rd] = 0;
			}
			else if (state->regs[rs1] < state->regs[rs2]) {
				state->regs[rd] = 1;
			}
			else
				state->regs[rd] = 0;

			return mips_Success;

			break;

		}

	}


	else if (opcode != 0) {


		rd = rs2;

		switch (opcode) {


		case 1: //BRANCHING


			switch (rs2) {
			case 0://BLTZ
				signExt(data);
				if ((((state->regs[rs1]) & 0x80000000) == 0x80000000)) { //if neg
					data = data << 2;
					state->pc = state->pc - 4;
					state->pc = state->pc + data;
					return mips_Success;



					// current pcorigPc //As we have incremented the pc automatically
				}
				else {
					state->regs[rs2] = 0xFFFFFFFF;
					return mips_Success;
				}
				break;
			case 1://BGEZ
				signExt(data);
				if (((state->regs[rs1]) & 0x80000000) == 0x80000000) { //if neg


					state->regs[rs2] = 0xFFFFFFFF;
					return mips_Success;

					// current pcorigPc //As we have incremented the pc automatically
				}
				else {
					data = data << 2;
					state->pc = state->pc - 4;
					state->pc = state->pc + data;
					return mips_Success;
				}
				break;

			}


			break;

		case 2: //Jump




			state->pc = (jump << 2);

			return mips_Success;
			break;

		case 4: //BEQ

			signExt(data);


			if (state->regs[rs1] == state->regs[rs2]) {
				data = data << 2;



				state->pc = state->pc - 4;
				state->pc = state->pc + data;
				return mips_Success;
				// current pcorigPc //As we have incremented the pc automatically
			}
			else {
				state->regs[rs2] = 0xFFFFFFFF;
				return mips_Success;
			}
			break;


		case 5: //BNE
			signExt(data);

			if (state->regs[rs1] != state->regs[rs2]) {
				data = data << 2;

				state->pc = state->pc - 4;
				state->pc = state->pc + data;
				return mips_Success;
				// current pcorigPc //As we have incremented the pc automatically
			}
			else
			{
				state->regs[rs2] = 0xFFFFFFFF;
				return mips_Success;
			}
			break;





		case 6://BLEZ
			signExt(data);
			if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) | (state->regs[rs1] == 0)) { //if neg
				data = data << 2;
				state->pc = state->pc - 4;
				state->pc = state->pc + data;
				return mips_Success;



				// current pcorigPc //As we have incremented the pc automatically
			}
			else {
				state->regs[rs2] = 0xFFFFFFFF;
				return mips_Success;
			}
			break;

		case 7://BGTZ
			signExt(data);
			if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) | (state->regs[rs1] == 0)) { //if neg


				state->regs[rs2] = 0xFFFFFFFF;
				return mips_Success;

				// current pcorigPc //As we have incremented the pc automatically
			}
			else {
				data = data << 2;
				state->pc = state->pc - 4;
				state->pc = state->pc + data;
				return mips_Success;
			}
			break;


		case 8: //ADDI
			signExt(data);






			state->regs[rd] = state->regs[rs1] + data;

			if ((((state->regs[rs1]) & 0x80000000) == 0x0) && (((data)& 0x8000) == 0x0)) {
				//two positive gets a negative 
				overflow = 1;
			};
			if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((data)& 0x8000) == 0x8000)) {
				//two negative yield positive
				overflow = 1;
			};





			if (overflow == 1) {
				return mips_ExceptionArithmeticOverflow;

			}
			else {

				return mips_Success;
			}
			break;

			state->regs[rd] = state->regs[rs1] + data;


			return mips_Success;

			break;

		case 9:
			//ADDIU




			state->regs[rd] = state->regs[rs1] + data;


			return mips_Success;

			break;
		case 10://SLTI


			if (state->regs[rs1] == data) {
				state->regs[rd] = 0;
				return mips_ErrorInvalidArgument;

			}

			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((data)& 0x8000) == 0x0)) {


				state->regs[rd] = 1;
				return mips_Success;
			}
			else if ((((state->regs[rs1]) & 0x80000000) == 0x80000000) && (((data)& 0x8000) == 0x8000)) {
				overflow = state->regs[rs2] - state->regs[rs1];
				if (overflow > 0) {
					state->regs[rd] = 1;
					return mips_Success;
				}
				else
					state->regs[rd] = 0;
				return mips_Success;
			}


			else {
				if (state->regs[rs1] < data) {
					state->regs[rd] = 1;
				}
				else
					state->regs[rd] = 0;
			}

			return mips_Success;

			break;



		case 11://SLTIU


			if (state->regs[rs1] >= data) {

				state->regs[rd] = 0;
				return mips_ErrorInvalidArgument;
			}

			else if (state->regs[rs1] < data) {
				state->regs[rd] = 1;
				return mips_Success;
			}


			return mips_Success;

			break;
		case 12:
			//ANDI

			signExt(data);



			state->regs[rd] = state->regs[rs1] & data;


			return mips_Success;

			break;
		case 13:
			//ORI
			signExt(data);
			state->regs[rd] = state->regs[rs1] | data;

			return mips_Success;

			break;
		case 14:
			//XORI
			signExt(data);

			state->regs[rd] = state->regs[rs1] ^ data;





			return mips_Success;

			break;
		case 15:
			//LUI

			signExt(data);


			state->regs[rd] = (data << 16);


			return mips_Success;

			break;
		case 32:
			//LB
			//Load byte


			p = (state->regs[rs1]) + data;

			if ((p % 4) == 0) {

				err = mips_mem_read(state->mem, p, 4, (uint8_t*)&destination); //read from memory
			}
			else
				return mips_ErrorInvalidArgument;


			tmp = (destination << 24) | ((destination << 8) & 0x00FF0000) | ((destination >> 8) & 0x0000FF00) | (destination >> 24);





			// mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&currentinst)

			state->regs[rd] = tmp;


			return mips_Success;

			break;
		case 35:
			//LW
			//Load word at


			p = (state->regs[rs1]) + data;

			if ((p % 4) == 0) {

				err = mips_mem_read(state->mem, p, 4, (uint8_t*)&destination); //read from memory
			}
			else
				return mips_ErrorInvalidArgument;


			tmp = (destination << 24) | ((destination << 8) & 0x00FF0000) | ((destination >> 8) & 0x0000FF00) | (destination >> 24);





			// mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&currentinst)

			state->regs[rd] = tmp;


			return mips_Success;

			break;
		case 40:
			//SB
			//store byte



			state->regs[rs2] = state->regs[rs2] & 0xFF;

			tmp = (state->regs[rs2] << 24) | ((state->regs[rs2] << 8) & 0x00FF0000) | ((state->regs[rs2] >> 8) & 0x0000FF00) | (state->regs[rs2] >> 24);




			p = state->regs[rs1] + data;

			if ((p % 4) == 0) {

				err = mips_mem_write(state->mem, p, 4, (uint8_t*)&(tmp)); //write to memory //read from memory
			}
			else
				return mips_ErrorInvalidArgument;






			// mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&currentinst)


			return mips_Success;

			break;
		case 43:
			//SW
			//store word at



			tmp = (state->regs[rs2] << 24) | ((state->regs[rs2] << 8) & 0x00FF0000) | ((state->regs[rs2] >> 8) & 0x0000FF00) | (state->regs[rs2] >> 24);

			p = state->regs[rs1] + data;

			if ((p % 4) == 0) {

				err = mips_mem_write(state->mem, p, 4, (uint8_t*)&(tmp)); //write to memory //read from memory
			}
			else
				return mips_ErrorInvalidArgument;






			// mips_mem_read(state->mem, state->pc, 4, (uint8_t*)&currentinst)


			return mips_Success;

			break;



		}
	}
	return mips_Success;



}


mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
	) {

	if (state == 0)
		return mips_ErrorInvalidHandle;
	if (index >= 32)
		return mips_ExceptionInvalidAlignment;

	state->regs[index] = value;

	return mips_Success;


}

mips_error mips_cpu_set_debug_level(mips_cpu_h, unsigned, FILE *)
{
	return mips_Success;
}

void mips_cpu_free(mips_cpu_h)
{}

void signExt(uint32_t &data) {
	if (((data)& 0x8000) == 0x8000) {
		data = data | 0xFFFF0000; //convert to neg sign extend
	}
}




