
#include <assert.h>
#include <stdbool.h>

unsigned nondet_unsigned(); // prototype

#ifndef FFT_BITS
#define FFT_BITS 2
#endif

#define N           (1 << FFT_BITS)
#define MOD_N_MASK  (N - 1)
#define NLAYERS     (FFT_BITS + 1)

unsigned reverse_bits(unsigned n, unsigned numbits)
{
    unsigned r = 0;
    for (unsigned i = 0; i < numbits; ++i)
    {
        r <<= 1;
        r |= (n & 1);
        n >>= 1;
    }
    return r;
}

bool imp(bool x, bool y)
{
    return !x || y;
}

bool is_power_of_two(unsigned x)
{
    return (x!= 0) && !(x & (x - 1));
}

int main()
{
    unsigned input[N][N];
    unsigned ORD[N];

    unsigned state[NLAYERS][N][N];

    unsigned SRC1[NLAYERS - 1][N];
    unsigned SRC2[NLAYERS - 1][N];

    unsigned CONJ1[NLAYERS - 1][N];
    unsigned CONJ2[NLAYERS - 1][N];

    unsigned ROT1[NLAYERS - 1][N];
    unsigned ROT2[NLAYERS - 1][N];

    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j = 0; j < N; ++j)
        {
            input[i][j] = (j == 0) ? (1 <<  i) : 0;
        }
    }

    for (unsigned i = 0; i < N; ++i)
    {
        //ORD[i] = nondet_unsigned() & MOD_N_MASK;
        //ORD[i] = (i / 2) + 1 - (i % 2); // nondet_unsigned() & MOD_N_MASK;
        //ORD[i] = i; // nondet_unsigned() & MOD_N_MASK;
        ORD[i] = reverse_bits(i, FFT_BITS);
    }

    //if (0)
    //__CPROVER_assume(!(
    //));

    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j = 0; j < N; ++j)
        {
            state[0][i][j] = input[ORD[i]][j];
        }
    }

    for (unsigned z = 0; z < NLAYERS - 1; ++z)
    {
        for (unsigned i = 0; i < N; ++i)
        {
            SRC1 [z][i] = nondet_unsigned() & MOD_N_MASK;
            SRC2 [z][i] = nondet_unsigned() & MOD_N_MASK;
            CONJ1[z][i] = nondet_unsigned() & 1;
            CONJ2[z][i] = nondet_unsigned() & 1;
            ROT1 [z][i] = nondet_unsigned() & MOD_N_MASK;
            ROT2 [z][i] = nondet_unsigned() & MOD_N_MASK;

            __CPROVER_assume(SRC1[z][i] < SRC2[z][i]);

            __CPROVER_assume(CONJ1[z][i] == 0);
            __CPROVER_assume(CONJ2[z][i] == 0);

            //__CPROVER_assume(is_power_of_two(ROT1[z][i]));
            //__CPROVER_assume(is_power_of_two(ROT2[z][i]));

            //__CPROVER_assume((SRC1[z][i] == i) || (SRC2[z][i] == i));

            //__CPROVER_assume((SRC2[z][i] - SRC1[z][i]) <= 2);

            //__CPROVER_assume((ROT1[z][i] == 0) || (ROT2[z][i] == 0));

            __CPROVER_assume((ROT1[z][i] == 0));

            //__CPROVER_assume(
            //    imp((SRC1[z][i] == i), ROT2[z][i] == 0) &&
            //    imp((SRC2[z][i] == i), ROT1[z][i] == 0)
            //);

            //dr += (ROT1[z][i] != 0);
            //dr += (ROT2[z][i] != 0);
        }

        // Check butterfly structure.
        if (true)
        {
            for (unsigned i = 0; i < N; ++i)
            {
                if (SRC1[z][i] == i)
                {
                    // I am the left side of a butterfly
                    // The left-source of my right-source must be me.
                    __CPROVER_assume(SRC1[z][SRC2[z][i]] == i);
                }
                else if (SRC2[z][i] == i)
                {
                    // I am the right side of a butterfly
                    // The right-source of my left-source must be me.
                    __CPROVER_assume(SRC2[z][SRC1[z][i]] == i);
                }
                else
                {
                    __CPROVER_assume(false);
                }
            }
        }
    }

    for (unsigned z = 1; z < NLAYERS; ++z)
    {
        for (unsigned i = 0; i < N; ++i) // node in row
        {
            for (unsigned j = 0; j < N; ++j) // along the unit circle
            {
                unsigned j1 = (j - ROT1[z - 1][i]);
                unsigned j2 = (j - ROT2[z - 1][i]);

                if (CONJ1[z - 1][i]) j1 = -j1;
                if (CONJ2[z - 1][i]) j2 = -j2;

                j1 &= MOD_N_MASK;
                j2 &= MOD_N_MASK;

                unsigned x1 = state[z - 1][SRC1[z - 1][i]][j1];
                unsigned x2 = state[z - 1][SRC2[z - 1][i]][j2];

                __CPROVER_assume((x1 & x2) == 0); // no overlap.

                state[z][i][j] = (x1 ^ x2);
            }
        }
    }

    bool ok = true;

    for (unsigned i = 0; i < N; ++i)
    {
        for (unsigned j = 0; j < N; ++j)
        {
            unsigned expected_value = 0;

            for (unsigned k = 0; k < N; ++k)
            {
                if (((i * k + j) & MOD_N_MASK) == 0)
                {
                    expected_value |= (1 << k);
                }
            }

            if (state[NLAYERS - 1][i][j] != expected_value)
            {
                ok = false;
            }
        }
    }

    assert(!ok);

    return 0;
}
