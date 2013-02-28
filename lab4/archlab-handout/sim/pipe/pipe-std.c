char simname[] = "Y86 Processor: pipe-std.hcl";
#include <stdio.h>
#include "isa.h"
#include "pipeline.h"
#include "stages.h"
#include "sim.h"
int sim_main(int argc, char *argv[]);
int main(int argc, char *argv[]){return sim_main(argc,argv);}
int gen_f_pc()
{
    return ((((ex_mem_curr->icode) == (I_JMP)) & !(ex_mem_curr->takebranch)) ? (ex_mem_curr->vala) : ((mem_wb_curr->icode) == (I_RET)) ? (mem_wb_curr->valm) : 1 ? (pc_curr->pc) : 0);
}

int gen_need_regids()
{
    return ((if_id_next->icode) == (I_RRMOVL)||(if_id_next->icode) == (I_ALU)||(if_id_next->icode) == (I_PUSHL)||(if_id_next->icode) == (I_POPL)||(if_id_next->icode) == (I_IRMOVL)||(if_id_next->icode) == (I_RMMOVL)||(if_id_next->icode) == (I_MRMOVL));
}

int gen_need_valC()
{
    return ((if_id_next->icode) == (I_IRMOVL)||(if_id_next->icode) == (I_RMMOVL)||(if_id_next->icode) == (I_MRMOVL)||(if_id_next->icode) == (I_JMP)||(if_id_next->icode) == (I_CALL));
}

int gen_instr_valid()
{
    return ((if_id_next->icode) == (I_NOP)||(if_id_next->icode) == (I_HALT)||(if_id_next->icode) == (I_RRMOVL)||(if_id_next->icode) == (I_IRMOVL)||(if_id_next->icode) == (I_RMMOVL)||(if_id_next->icode) == (I_MRMOVL)||(if_id_next->icode) == (I_ALU)||(if_id_next->icode) == (I_JMP)||(if_id_next->icode) == (I_CALL)||(if_id_next->icode) == (I_RET)||(if_id_next->icode) == (I_PUSHL)||(if_id_next->icode) == (I_POPL));
}

int gen_new_F_predPC()
{
    return (((if_id_next->icode) == (I_JMP)||(if_id_next->icode) == (I_CALL)) ? (if_id_next->valc) : 1 ? (if_id_next->valp) : 0);
}

int gen_new_E_srcA()
{
    return (((if_id_curr->icode) == (I_RRMOVL)||(if_id_curr->icode) == (I_RMMOVL)||(if_id_curr->icode) == (I_ALU)||(if_id_curr->icode) == (I_PUSHL)) ? (if_id_curr->ra) : ((if_id_curr->icode) == (I_POPL)||(if_id_curr->icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_new_E_srcB()
{
    return (((if_id_curr->icode) == (I_ALU)||(if_id_curr->icode) == (I_RMMOVL)||(if_id_curr->icode) == (I_MRMOVL)) ? (if_id_curr->rb) : ((if_id_curr->icode) == (I_PUSHL)||(if_id_curr->icode) == (I_POPL)||(if_id_curr->icode) == (I_CALL)||(if_id_curr->icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_new_E_dstE()
{
    return (((if_id_curr->icode) == (I_RRMOVL)||(if_id_curr->icode) == (I_IRMOVL)||(if_id_curr->icode) == (I_ALU)) ? (if_id_curr->rb) : ((if_id_curr->icode) == (I_PUSHL)||(if_id_curr->icode) == (I_POPL)||(if_id_curr->icode) == (I_CALL)||(if_id_curr->icode) == (I_RET)) ? (REG_ESP) : 1 ? (REG_NONE) : 0);
}

int gen_new_E_dstM()
{
    return (((if_id_curr->icode) == (I_MRMOVL)||(if_id_curr->icode) == (I_POPL)) ? (if_id_curr->ra) : 1 ? (REG_NONE) : 0);
}

int gen_new_E_valA()
{
    return (((if_id_curr->icode) == (I_CALL)||(if_id_curr->icode) == (I_JMP)) ? (if_id_curr->valp) : ((id_ex_next->srca) == (id_ex_curr->deste)) ? (ex_mem_next->vale) : ((id_ex_next->srca) == (ex_mem_curr->destm)) ? (mem_wb_next->valm) : ((id_ex_next->srca) == (ex_mem_curr->deste)) ? (ex_mem_curr->vale) : ((id_ex_next->srca) == (mem_wb_curr->destm)) ? (mem_wb_curr->valm) : ((id_ex_next->srca) == (mem_wb_curr->deste)) ? (mem_wb_curr->vale) : 1 ? (d_regvala) : 0);
}

int gen_new_E_valB()
{
    return (((id_ex_next->srcb) == (id_ex_curr->deste)) ? (ex_mem_next->vale) : ((id_ex_next->srcb) == (ex_mem_curr->destm)) ? (mem_wb_next->valm) : ((id_ex_next->srcb) == (ex_mem_curr->deste)) ? (ex_mem_curr->vale) : ((id_ex_next->srcb) == (mem_wb_curr->destm)) ? (mem_wb_curr->valm) : ((id_ex_next->srcb) == (mem_wb_curr->deste)) ? (mem_wb_curr->vale) : 1 ? (d_regvalb) : 0);
}

int gen_aluA()
{
    return (((id_ex_curr->icode) == (I_RRMOVL)||(id_ex_curr->icode) == (I_ALU)) ? (id_ex_curr->vala) : ((id_ex_curr->icode) == (I_IRMOVL)||(id_ex_curr->icode) == (I_RMMOVL)||(id_ex_curr->icode) == (I_MRMOVL)) ? (id_ex_curr->valc) : ((id_ex_curr->icode) == (I_CALL)||(id_ex_curr->icode) == (I_PUSHL)) ? -4 : ((id_ex_curr->icode) == (I_RET)||(id_ex_curr->icode) == (I_POPL)) ? 4 : 0);
}

int gen_aluB()
{
    return (((id_ex_curr->icode) == (I_RMMOVL)||(id_ex_curr->icode) == (I_MRMOVL)||(id_ex_curr->icode) == (I_ALU)||(id_ex_curr->icode) == (I_CALL)||(id_ex_curr->icode) == (I_PUSHL)||(id_ex_curr->icode) == (I_RET)||(id_ex_curr->icode) == (I_POPL)) ? (id_ex_curr->valb) : ((id_ex_curr->icode) == (I_RRMOVL)||(id_ex_curr->icode) == (I_IRMOVL)) ? 0 : 0);
}

int gen_alufun()
{
    return (((id_ex_curr->icode) == (I_ALU)) ? (id_ex_curr->ifun) : 1 ? (A_ADD) : 0);
}

int gen_set_cc()
{
    return ((id_ex_curr->icode) == (I_ALU));
}

int gen_mem_addr()
{
    return (((ex_mem_curr->icode) == (I_RMMOVL)||(ex_mem_curr->icode) == (I_PUSHL)||(ex_mem_curr->icode) == (I_CALL)||(ex_mem_curr->icode) == (I_MRMOVL)) ? (ex_mem_curr->vale) : ((ex_mem_curr->icode) == (I_POPL)||(ex_mem_curr->icode) == (I_RET)) ? (ex_mem_curr->vala) : 0);
}

int gen_mem_read()
{
    return ((ex_mem_curr->icode) == (I_MRMOVL)||(ex_mem_curr->icode) == (I_POPL)||(ex_mem_curr->icode) == (I_RET));
}

int gen_mem_write()
{
    return ((ex_mem_curr->icode) == (I_RMMOVL)||(ex_mem_curr->icode) == (I_PUSHL)||(ex_mem_curr->icode) == (I_CALL));
}

int gen_F_bubble()
{
    return 0;
}

int gen_F_stall()
{
    return ((((id_ex_curr->icode) == (I_MRMOVL)||(id_ex_curr->icode) == (I_POPL)) & ((id_ex_curr->destm) == (id_ex_next->srca)||(id_ex_curr->destm) == (id_ex_next->srcb))) | ((I_RET) == (if_id_curr->icode)||(I_RET) == (id_ex_curr->icode)||(I_RET) == (ex_mem_curr->icode)));
}

int gen_D_stall()
{
    return (((id_ex_curr->icode) == (I_MRMOVL)||(id_ex_curr->icode) == (I_POPL)) & ((id_ex_curr->destm) == (id_ex_next->srca)||(id_ex_curr->destm) == (id_ex_next->srcb)));
}

int gen_D_bubble()
{
    return ((((id_ex_curr->icode) == (I_JMP)) & !(ex_mem_next->takebranch)) | (!(((id_ex_curr->icode) == (I_MRMOVL)||(id_ex_curr->icode) == (I_POPL)) & ((id_ex_curr->destm) == (id_ex_next->srca)||(id_ex_curr->destm) == (id_ex_next->srcb))) & ((I_RET) == (if_id_curr->icode)||(I_RET) == (id_ex_curr->icode)||(I_RET) == (ex_mem_curr->icode))));
}

int gen_E_stall()
{
    return 0;
}

int gen_E_bubble()
{
    return ((((id_ex_curr->icode) == (I_JMP)) & !(ex_mem_next->takebranch)) | (((id_ex_curr->icode) == (I_MRMOVL)||(id_ex_curr->icode) == (I_POPL)) & ((id_ex_curr->destm) == (id_ex_next->srca)||(id_ex_curr->destm) == (id_ex_next->srcb))));
}

int gen_M_stall()
{
    return 0;
}

int gen_M_bubble()
{
    return 0;
}

