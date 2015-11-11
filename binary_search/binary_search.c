
// Use "cbmc --unwind 20 binary_search.c" to verify.

#include <assert.h>
#include <stdbool.h>

bool is_sorted(unsigned char x[], unsigned size)
{
    for (unsigned i = 1; i < size; ++i)
    {
        if (x[i] < x[i - 1])
        {
            return false;
        }
    }
    return true;
}

int linear_search(unsigned char x[], unsigned size, unsigned char search_value)
{
    for (unsigned i = 0; i < size; ++i)
    {
        if (x[i] == search_value)
        {
            return i;
        }
    }
    return -1; // not found
}

int binary_search(unsigned char x[], unsigned size, unsigned char search_value)
{
    // Invariants is conjunction of:
    //   (1) a <= b
    //   (2) If there is a smaller index i such that x[i] == search_value,
    //       it will be in the half-open interval [a, b).

    unsigned a = 0;
    unsigned b = size;

    int result = -1;

    while (b != a)
    {
        unsigned mid = (a + b) / 2;

        if (x[mid] == search_value)
        {
            result = mid;
        }

        if (x[mid] < search_value)
        {
            a = mid + 1;
        }
        else
        {
            b = mid;
        }
    }

    return result;
}

void test(unsigned size)
{
    unsigned x[size];

    for (unsigned i = 0; i < size; ++i)
    {
        x[i] = nondet_uchar() & 15; // nondeterministic number in range 0 .. 15;
    }

    unsigned char search_value = nondet_uchar();

    if (is_sorted(x, size))
    {
        int linear_search_result = linear_search(x, size, search_value);
        int binary_search_result = binary_search(x, size, search_value);

        assert(linear_search_result == binary_search_result);
    }
}

int main()
{
    for (unsigned size = 0; size <= 10; ++size)
    {
        test(size);
    }
    return 0;
}
