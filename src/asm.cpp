#include "asm.hpp"

void Reg_init_bit8(Reg *self, RegBit8 val) {
    self->kind = RegKind_Bit8;
    self->val.bit8 = val;
}

void Reg_init_bit16(Reg *self, RegBit16 val) {
    self->kind = RegKind_Bit16;
    self->val.bit16 = val;
}

void Reg_init_bit32(Reg *self, RegBit32 val) {
    self->kind = RegKind_Bit32;
    self->val.bit32 = val;
}

void Reg_init_bit64(Reg *self, RegBit64 val) {
    self->kind = RegKind_Bit64;
    self->val.bit64 = val;
}

void Reg_fini(Reg *self) {

}

void InstrParam_init_reg(InstrParam *self, Reg *reg) {

}

void InstrParam_init_mem(InstrParam *self, Mem *mem) {

}

void InstrParam_init_imm(InstrParam *self, Imm *imm) {

}

void InstrParam_fini(InstrParam *self) {

}

void Instr_init_0(Instr *self, InstrOp op) {
    self->op = op;
    self->num_params = 0;
}

void Instr_init_1(Instr *self, InstrOp op, InstrParam *a) {
    self->op = op;
    self->num_params = 1;
    self->params[0] = *a;
}

void Instr_init_2(Instr *self, InstrOp op, InstrParam *a, InstrParam *b) {
    self->op = op;
    self->num_params = 2;
    self->params[0] = *a;
    self->params[1] = *b;
}

void Instr_init_3(Instr *self, InstrOp op, 
    InstrParam *a, InstrParam *b, InstrParam *c) {
    self->op = op;
    self->num_params = 3;
    self->params[0] = *a;
    self->params[1] = *b;
    self->params[2] = *c;
}

void Instr_fini(Instr *self) {

}