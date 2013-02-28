char simname[] = "Y86 Processor: seq+-std.hcl";
#include <stdio.h>
#include "isa.h"
#include "sim.h"
int sim_main(int argc, char *argv[]);
int gen_new_pc(){return 0;}
int main(int argc, char *argv[])
  {plusmode=1;return sim_main(argc,argv);}
int gen_pc()
{
    return (((prev_icode) == (I_CALL)) ? (prev_valc) : (((prev_icode) == (I_JMP)) & (prev_bcond)) ? (prev_valc) : ((prev_icode) == (I_RET)) ? (prev_valm) : 1 ? (prev_valp) : 0);
}

int gen_need_regids()
{
    return ((icode) == (I_RRMOVL)||(icode) == (I_ALU)||(icode) == (I_PUSHL)||(icode) == (I_POPL)||(icode) == (I_IRMOVL)||(icode) == (I_RMMOVL)||(icode) == (I_MRMOVL));
}

int gen_need_valC()
{
    return ((icode) == (I_IRMOVL)||(icode) == (I_RMMOVL)||(icode) == (I_MRMOVL)||(icode) == (I_JMP)||(icode) == (I_CALL));
}

int gen_instr_valid()
{
    return ((icode) == (I_NOP)||(icode) == (I_HALT)||(icode) == (I_RRMOVL)||(icode) == (I_IRMOVL)||(icode) == (I_RMMOVL)||(icode) == (I_MRMOVL)||(icode) == (I_ALU)||(icode) == (I_JMP)||(icode) == (I_CALL)||(icode) == (I_RET)||(icode) == (I_PUSHL)||(icode) == (I_POPL));
}

int gen_srcA()
{
    return (((icode) == (I_RRMOVL)||(icode) == (I_RMMOVL)||(icode) == (I_ALU)||(icode) == (I_PUSHL)) ? (ra) : ((icode) == (I_POPL)||(icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_srcB()
{
    return (((icode) == (I_ALU)||(icode) == (I_RMMOVL)||(icode) == (I_MRMOVL)) ? (rb) : ((icode) == (I_PUSHL)||(icode) == (I_POPL)||(icode) == (I_CALL)||(icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_dstE()
{
    return (((icode) == (I_RRMOVL)||(icode) == (I_IRMOVL)||(icode) == (I_ALU)) ? (rb) : ((icode) == (I_PUSHL)||(icode) == (I_POPL)||(icode) == (I_CALL)||(icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_dstM()
{
    return (((icode) == (I_MRMOVL)||(icode) == (I_POPL)) ? (ra) : 1 ? (REG_NONE) : 0);
}

int gen_aluA()
{
    return (((icode) == (I_RRMOVL)||(icode) == (I_ALU)) ? (vala) : ((icode) == (I_IRMOVL)||(icode) == (I_RMMOVL)||(icode) == (I_MRMOVL)) ? (valc) : ((icode) == (I_CALL)||(icode) == (I_PUSHL)) ? -4 : ((icode) == (I_RET)||(icode) == (I_POPL)) ? 4 : 0);
}

int gen_aluB()
{
    return (((icode) == (I_RMMOVL)||(icode) == (I_MRMOVL)||(icode) == (I_ALU)||(icode) == (I_CALL)||(icode) == (I_PUSHL)||(icode) == (I_RET)||(icode) == (I_POPL)) ? (valb) : ((icode) == (I_RRMOVL)||(icode) == (I_IRMOVL)) ? 0 : 0);
}

int gen_alufun()
{
    return (((icode) == (I_ALU)) ? (ifun) : 1 ? (A_ADD) : 0);
}

int gen_set_cc()
{
    return ((icode) == (I_ALU));
}

int gen_mem_read()
{
    return ((icode) == (I_MRMOVL)||(icode) == (I_POPL)||(icode) == (I_RET));
}

int gen_mem_write()
{
    return ((icode) == (I_RMMOVL)||(icode) == (I_PUSHL)||(icode) == (I_CALL));
}

int gen_mem_addr()
{
    return (((icode) == (I_RMMOVL)||(icode) == (I_PUSHL)||(icode) == (I_CALL)||(icode) == (I_MRMOVL)) ? (vale) : ((icode) == (I_POPL)||(icode) == (I_RET)) ? (vala) : 0);
}

int gen_mem_data()
{
    return (((icode) == (I_RMMOVL)||(icode) == (I_PUSHL)) ? (vala) : ((icode) == (I_CALL)) ? (valp) : 0);
}

