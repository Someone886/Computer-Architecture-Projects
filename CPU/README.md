# CS61CPU

Part a:

1. alu.circ:

   Using ALUSel and Mux to select which operation result is needed, and output that result.

2. regfile:

   Using DMX and Write_reg to decide writing to which register is allowed before writing the data to it. Connect x0 with constant 0 since writing to x0 is ignored and not allowed.

   Using Mux and rs1/rs2 to decide which register's data is read.

3. imm-gen:

   Generate different types of immediate based on instruction formats, and use ImmSel and Mux to select which immediate is needed. 0 - I, 1 - S, 2 - SB, 3 - U, 4 - UJ

4. branch-comp:

   Using comparator to output two results BrEq and BrLt; using BrUn to select if signed BrLt or unsigned one is needed.

5. mem (Pre-implemented):

   Notes: 0000 - no writing; 0001 - sb; 0011 - sh; 1111 - sw.

6. control-logic (implemented so far):

   - Using opcode to decide WBSel: 0 - mem; 1 (0x13, 17, 33) - ALU; 2 (0x67, 6f) - PC+4; 3 (0x37) - generated immediate.

   - Using opcode to decide MemRW: opcode == 23, then MemWR = 1; otherwise, 0.

   - Using opcode and the last two bits of funct 3 to decide LoadType: 00 - lb; 01 - lh; 10 - lw.

   - Using MemRW and last two digits of funct3 to decide Write_Enable: 00 - sb -> 0001; 01 - sh -> 0011; 10 - sw -> 1111. Note that is MemRW is 0, Write_Enable is set to 0 regardless.

7. CPU (implemented so far and not explicitly explained in the lecture CPU design):

   Not going to explain stuff like Decoding, Mux Selection that the lecture CPU design has enough details.

   Using WBSel to select among four data as the write_back data; include the immediate generated for lui as the 4th option.

   Using Load_Type to select how to process the READ_DATA from DMEM. If lb, take the first 8 bits and sign extend it; if lh, take the first 16 bits and sign extend it; if lw, no change.



Part b:

1. imm-gen:

   Added another immediate type for csrwi: zero extend instr[15:19] to get the unsigned imm.

2. Control Logic:

   - MemRW, LoadType, Write_Enable are explained in part a.

   - ImmSel, CSRWen, CSRSel, ASel, BSel, RegWen, BrUn: Using opcode along with funct3 to determine which value should be selected.

   - WBSel: when WBSel = 3, the write back value is changed to tohost value from CSR register (original write back value for WBSel = 3 is lui immediate). lui value is combined into ALU output using Bselect function of the ALU unit.

   - PCSel: PCSel = 1 (takes ALU output) only when at one of the following situations:

     a. Branch and branch condition met;

     b. jalr with opcode = 0x6f;

     c. jal with opcode = 0x67;

     Notice that when PCSel = 1, the branching is taken, which means the next instruction has to be stalled. Therefore, it is nopped before it is loaded into the pipeline register that keeps the next instruction. 

   - BrUn: Using funct3 == 6 or 7 to determine if unsigned is required.

   - ALUSel: the implementation for ALUSel is following the design below:

     a. Except for R format, I format, and lui, do add for the rest of instructions.

     b. For lui, do Bselect.

     c. For any instruction in R and I formats,

     - if its funct7 == 0, its arithmetic function would be the default function for that opcode/funct3 pair;
     - using opcode, funct3, and funct7 to decide if the default function should be updated before it is sent to ALUSel;
     - for example, the default value for opcode == 0x33 and funct3 == 0x5 is srl, but if funct7 == 0x20, the function would be changed to sra;
     - for the R and I instruction pair, they share the same funct3 and funct7 value (e.g., both srai and sra have funct3 = 0x5 and funct7 = 0x20).

3. CPU (not explicitly covered in the lecture and not explained in Part a):

   - Write back value: select tohost value when WBSel is 3.

   - CSR:

     a. Instruction[20:31] would be the address;

     b. Using CSRSel to decide if the input value is the one in register 1 or the generated immediate;

     c. CSRWen to decide if writing to the CSR register is allowed;

   - Accommodations for aligned sb and sh:

     a. shift left the raw WE_Enable by ALU_Result[0, 1] (the last 2 digits of the address) to get the correct WE_Enable bits; the new WE_Enable would be the one sent to MEM;

     b. shift left the raw WRITE_DATA by unsigned 8 * ALU_Result[0, 1] (the last 2 digits of the address) to the right address for writing; the new WRITE_DATA would be sent to MEM.

   - Accommodations for aligned lb and lh:

     a. shift right the READ_DATA by unsigned 8 * ALU_Result[0, 1] to put the right data at the index 0;

     b. sign extend the shifted data according to the Load_Type returned by the control logic;

     c. the result data would be the Processed_Data and sent to Write Back selection.

   - PC:

     Since PC are 2-bytes apart, the 0th index value is ignored and assumed to be 0.

     For example, if PC is set to 17 by jalr, PC at address of 16 should be accessed since 17 = 0b10001 is changed to 0b10000 before sent to MEM.

4. Advantages/Disadvantages of your design:

   Advantages: 

   Since I used opcode, funct3, and funct7 to decide value for each Sel, the implementation is really straightforward and easy to debug. If a new condition should be added to a Sel, I could just wire it to this Sel by adding a new comparison unit. Also, the process to determine each Sel is separated from another, so I could easily see how the process goes and debug it accordingly.

   Disadvantages:

   However, since I chose not to use the truth table to decide each Sel's value, a lot of comparisons for opcode, funct3, and funct7 would be required to determine right values for the control logic. This means my CPU would require more time to implement a certain instruction.

5. Largest design challenge:

   The largest design challenge is to write to the right address in the MEM when a sb/sh instruction is encountered. Since the write address could only determine the word block instead of the exact byte address, shifting to WRITE_ENABLE and WRITE_DATA is required. The WRITE_ENABLE must be shifted by ALU_Result[0, 1] bit(s) to enable the writing to the right place, while WRITE_DATA must be shifted by ALU_Result[0, 1] byte(s) to go to the right byte address for writing.

   For WRITE_ENABLE shifting, a shifter with data bits of 4 is used cuz WRITE_ENABLE has 4 bits, and the shifting value ALU_Result[0, 1] is automatically implemented by the shifter. However, for WRITE_DATA shifting, since the unit is in byte, and the maximum value to shift is 8*3 = 24 bits, the multiplier is set to have data bits of 5. Also, a bit extender with zero extend is needed for ALU_Result[0, 1] before its multiplication with the constant 8. 

   Similar implementations for lb/lh is also implemented in my CPU and explained above. Basically, I was just shifting the read data to right by ALU_Result[0, 1] byte(s).