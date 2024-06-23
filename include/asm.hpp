#pragma once
#include "num.hpp"

typedef enum RegBit8 {
    RegBit8_AL, 
    RegBit8_CL, 
    RegBit8_DL, 
    RegBit8_BL, 
    RegBit8_SIL, 
    RegBit8_DIL, 
    RegBit8_SPL, 
    RegBit8_BPL, 
    RegBit8_R8B, 
    RegBit8_R9B, 
    RegBit8_R10B, 
    RegBit8_R11B, 
    RegBit8_R12B, 
    RegBit8_R13B, 
    RegBit8_R14B, 
    RegBit8_R15B,
} RegBit8;

typedef enum RegBit16 {  
    RegBit16_AX, 
    RegBit16_CX, 
    RegBit16_DX, 
    RegBit16_BX, 
    RegBit16_SI, 
    RegBit16_DI, 
    RegBit16_SP, 
    RegBit16_BP, 
    RegBit16_R8W, 
    RegBit16_R9W, 
    RegBit16_R10W, 
    RegBit16_R11W, 
    RegBit16_R12W, 
    RegBit16_R13W, 
    RegBit16_R14W, 
    RegBit16_R15W,
} RegBit16;

typedef enum RegBit32 { 
    RegBit32_EAX, 
    RegBit32_ECX, 
    RegBit32_EDX, 
    RegBit32_EBX, 
    RegBit32_ESI, 
    RegBit32_EDI, 
    RegBit32_ESP, 
    RegBit32_EBP, 
    RegBit32_R8D, 
    RegBit32_R9D, 
    RegBit32_R10D, 
    RegBit32_R11D, 
    RegBit32_R12D, 
    RegBit32_R13D, 
    RegBit32_R14D, 
    RegBit32_R15D,
} RegBit32;

typedef enum RegBit64 { 
    RegBit64_RAX, 
    RegBit64_RCX, 
    RegBit64_RDX, 
    RegBit64_RBX, 
    RegBit64_RSI, 
    RegBit64_RDI, 
    RegBit64_RSP, 
    RegBit64_RBP, 
    RegBit64_R8, 
    RegBit64_R9, 
    RegBit64_R10, 
    RegBit64_R11, 
    RegBit64_R12, 
    RegBit64_R13, 
    RegBit64_R14, 
    RegBit64_R15,
} RegBit64;

typedef enum RegKind {
    RegKind_Bit8,
    RegKind_Bit16,
    RegKind_Bit32,
    RegKind_Bit64,
} RegKind;

typedef union RegVal {
    RegBit8 bit8;
    RegBit16 bit16;
    RegBit32 bit32;
    RegBit64 bit64;
} RegVal;

typedef struct Reg {
    RegKind kind;
    RegVal val;
} Reg;

void Reg_init_bit8(Reg *self, RegBit8 val);
void Reg_init_bit16(Reg *self, RegBit16 val);
void Reg_init_bit32(Reg *self, RegBit32 val);
void Reg_init_bit64(Reg *self, RegBit64 val);
void reg_fini(Reg *self);

typedef enum MemKind {
    MemKind_Bit8,
    MemKind_Bit16,
    MemKind_Bit32,
    MemKind_Bit64,
} MemKind;

typedef union MemVal {
    uint8_t mem8;
    uint16_t mem16;
    uint32_t mem32;
    uint64_t mem64;
} MemVal;

typedef struct Mem {
    MemKind kind;
    MemVal val;
} Mem;

typedef enum ImmKind {
    ImmKind_int8_t,
    ImmKind_int16_t,
    ImmKind_int32_t,
    ImmKind_int64_t,
    ImmKind_uint8_t,
    ImmKind_uint16_t,
    ImmKind_uint32_t,
    ImmKind_uint64_t,
    ImmKind_float32_t,
    ImmKind_float64_t,
} ImmKind;

typedef union ImmVal {
    int8_t int8;
    int16_t int16;
    int32_t int32;
    int64_t int64;
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    uint64_t uint64;
    float float32;
    double float64;
} ImmVal;

typedef struct Imm {
    ImmKind kind;
    ImmVal val;
} Imm;

typedef enum InstrOp {
    InstrOp_MOV, 
    InstrOp_PUSH, 
    InstrOp_POP,
    InstrOp_CTWL, 
    InstrOp_CLTQ, 
    InstrOp_CQTO,
    InstrOp_INC, 
    InstrOp_DEC, 
    InstrOp_NEG, 
    InstrOp_NOT,
    InstrOp_LEAQ, 
    InstrOp_ADD, 
    InstrOp_SUB, 
    InstrOp_IMUL, 
    InstrOp_XOR, 
    InstrOp_OR, 
    InstrOp_AND,
    InstrOp_SAL, 
    InstrOp_SHL, 
    InstrOp_SAR, 
    InstrOp_SHR,
    InstrOp_IMULQ, 
    InstrOp_MULQ, 
    InstrOp_IDIVQ, 
    InstrOp_DIVQ,
    InstrOp_CMP, 
    InstrOp_TEST,
    InstrOp_SETE, 
    InstrOp_SETZ, 
    InstrOp_SETNE, 
    InstrOp_SETNZ, 
    InstrOp_SETS, 
    InstrOp_SETNS, 
    InstrOp_SETG, 
    InstrOp_SETNLE,
    InstrOp_SETGE, 
    InstrOp_SETNL,
    InstrOp_SETL, 
    InstrOp_SETNGE,
    InstrOp_SETLE, 
    InstrOp_SETNG,
    InstrOp_SETA, 
    InstrOp_SETNBE,
    InstrOp_SETAE, 
    InstrOp_SETNB,
    InstrOp_SETB, 
    InstrOp_SETNAE,
    InstrOp_SETBE, 
    InstrOp_SETNA,
} InstrOp;

typedef enum InstrParamKind {
    InstrParamKind_Reg,
    InstrParamKind_Mem,
    InstrParamKind_Imm,
} InstrParamKind;

typedef union InstrParamVal {
    Reg reg;
    Mem mem;
    Imm imm;
} InstrParamVal;

typedef struct InstrParam {
    InstrParamKind kind;
    InstrParamVal val;
} InstrParam;

void InstrParam_init_reg(InstrParam *self, Reg *reg);
void InstrParam_init_mem(InstrParam *self, Mem *mem);
void InstrParam_init_imm(InstrParam *self, Imm *imm);
void InstrParam_fini(InstrParam *self);

typedef struct Instr {
    InstrOp op;
    uint64_t num_params;
    InstrParam params[3];
} Instr;

void Instr_init_0(Instr *self, InstrOp op);
void Instr_init_1(Instr *self, InstrOp op, InstrParam *a);
void Instr_init_2(Instr *self, InstrOp op, InstrParam *a, InstrParam *b);
void Instr_init_3(Instr *self, InstrOp op, 
    InstrParam *a, InstrParam *b, InstrParam *c);
void Instr_fini(Instr *self);
