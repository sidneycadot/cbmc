
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __GNUC__
void __CPROVER_assume_function(bool b, const char * filename, unsigned linenumber)
{
    if (!b)
    {
        printf("File '%s', line %u: assumption violated.\n", filename, linenumber);
        exit(EXIT_FAILURE);
    }
}

#define __CPROVER_assume(b) __CPROVER_assume_function(b, __FILE__, __LINE__)

void __CPROVER_assert_function(bool b, const char * msg, const char * filename, unsigned linenumber)
{
    if (!b)
    {
        printf("File '%s', line %u: assertion violated: '%s'.\n", filename, linenumber, msg);
        exit(EXIT_FAILURE);
    }
}

#define __CPROVER_assert(b, msg) __CPROVER_assert_function(b, msg, __FILE__, __LINE__)

#endif

typedef unsigned char Unsigned;

enum opcode_enum_t
{
    OPC_NOP   =   0,
    OPC_FALSE = 100,
    OPC_TRUE  = 101,
    OPC_NOT   = 110,
    OPC_AND   = 120,
    OPC_OR    = 121,
    OPC_NAND  = 122,
    OPC_XOR   = 123,
    OPC_COPY1 = 201,
    OPC_COPY2 = 202,
    OPC_COPY3 = 203,
    OPC_COPY4 = 204,
    OPC_COPY5 = 205,
    OPC_COPY6 = 206,
    OPC_COPY7 = 207,
    OPC_COPY8 = 208,
    OPC_COPY9 = 209
};

typedef enum opcode_enum_t opcode_t;

void run_program(bool STACK[], Unsigned * stackptr, opcode_t PROGRAM[], Unsigned PROGRAMSIZE)
{
    Unsigned STACKPTR = *stackptr;
    for (Unsigned PC = 0; PC < PROGRAMSIZE; ++PC)
    {
        opcode_t opcode = PROGRAM[PC];

        switch (opcode)
        {
            // no operation

            case OPC_NOP:
            {
                break;
            }

            // zero-argument functions

            case OPC_TRUE:
            {
                STACK[STACKPTR++] = true;
                ++STACKPTR;
                break;
            }
            case OPC_FALSE:
            {
                STACK[STACKPTR] = false;
                ++STACKPTR;
                break;
            }

            // one-argument functions

            case OPC_NOT:
            {
                if (STACKPTR >= 1)
                {
                    STACK[STACKPTR] = !STACK[STACKPTR - 1];
                    ++STACKPTR;
                }
                break;
            }

            // two-argument functions

            case OPC_AND:
            {
                if (STACKPTR >= 2)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 2] & STACK[STACKPTR - 1];
                    ++STACKPTR;
                }
                break;
            }

            case OPC_OR:
            {
                if (STACKPTR >= 2)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 2] | STACK[STACKPTR - 1];
                    ++STACKPTR;
                }
                break;
            }

            case OPC_NAND:
            {
                if (STACKPTR >= 2)
                {
                    STACK[STACKPTR] = !(STACK[STACKPTR - 2] & STACK[STACKPTR - 1]);
                    ++STACKPTR;
                }
                break;
            }

            case OPC_XOR:
            {
                if (STACKPTR >= 2)
                {
                    STACK[STACKPTR] = !(STACK[STACKPTR - 2] ^ STACK[STACKPTR - 1]);
                    ++STACKPTR;
                }
                break;
            }

            // copy operations

            case OPC_COPY1:
            {
                if (STACKPTR >= 1)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 1];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY2:
            {
                if (STACKPTR >= 2)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 2];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY3:
            {
                if (STACKPTR >= 3)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 3];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY4:
            {
                if (STACKPTR >= 4)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 4];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY5:
            {
                if (STACKPTR >= 5)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 5];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY6:
            {
                if (STACKPTR >= 6)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 6];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY7:
            {
                if (STACKPTR >= 7)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 7];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY8:
            {
                if (STACKPTR >= 8)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 8];
                    ++STACKPTR;
                }
                break;
            }
            case OPC_COPY9:
            {
                if (STACKPTR >= 9)
                {
                    STACK[STACKPTR] = STACK[STACKPTR - 9];
                    ++STACKPTR;
                }
                break;
            }
        }
    }
    *stackptr = STACKPTR;
}

unsigned nondet_unsigned();

void init_program_cbmc(opcode_t program[], Unsigned program_size)
{
    for (Unsigned i = 0; i < program_size; ++i)
    {
        program[i] = nondet_unsigned();

        opcode_t opcode = program[i];

        __CPROVER_assume(
            opcode == OPC_NOP    ||
            opcode == OPC_FALSE  ||
            opcode == OPC_TRUE   ||
            opcode == OPC_NOT    ||
            opcode == OPC_AND    ||
            opcode == OPC_OR     ||
            opcode == OPC_NAND   ||
            opcode == OPC_XOR    ||
            opcode == OPC_COPY1  ||
            opcode == OPC_COPY2  ||
            opcode == OPC_COPY3  ||
            opcode == OPC_COPY4  ||
            opcode == OPC_COPY5  ||
            opcode == OPC_COPY6  ||
            opcode == OPC_COPY7  ||
            opcode == OPC_COPY8  ||
            opcode == OPC_COPY9);

        //__CPROVER_assume(opcode != OPC_TRUE);
        //__CPROVER_assume(opcode != OPC_FALSE);
        //__CPROVER_assume(opcode != OPC_NOT);
        //__CPROVER_assume(opcode != OPC_AND);
        //__CPROVER_assume(opcode != OPC_OR);

        //program[i] = opcode;
    }
}

#define PROGRAMSIZE 13
#define STACKSIZE (PROGRAMSIZE + 3)

int main(void)
{
    opcode_t PROGRAM[PROGRAMSIZE];// = { 208, 202, 100, 101, 207, 208, 208, 204, 122 };

    // Initialize the program nondeterministically.
    init_program_cbmc(PROGRAM, PROGRAMSIZE);

    bool all_ok = true;

    for (Unsigned all_inputs = 0; all_inputs < 8; ++all_inputs)
    {
        bool in1 = (all_inputs & 4) != 0;
        bool in2 = (all_inputs & 2) != 0;
        bool in3 = (all_inputs & 1) != 0;

        bool out1_desired = (in1 & in2) | (in1 & in3) | (in2 & in3);
        bool out2_desired = in1 ^ in2 ^ in3;

        // initialize stack; run the program; and fetch result.

        bool STACK[STACKSIZE];
        for (Unsigned i = 0; i < STACKSIZE; ++i)
        {
            STACK[i] = false;
        }
        Unsigned STACKPTR = 0;

        STACK[STACKPTR++] = in1;
        STACK[STACKPTR++] = in2;
        STACK[STACKPTR++] = in3;

        run_program(STACK, &STACKPTR, PROGRAM, PROGRAMSIZE);

        bool out1 = STACK[STACKPTR - 2];
        bool out2 = STACK[STACKPTR - 1];

        bool ok = ((out1 == out1_desired) & (out2 == out2_desired));

        //printf("all_inputs %u --> %d\n", all_inputs, ok);

        if (!ok)
        {
            all_ok = false;
        }
    }

    //printf("all_ok: %d\n", all_ok);

    __CPROVER_assert(!all_ok, "found a program that works in all cases.");

    return 0;
}
