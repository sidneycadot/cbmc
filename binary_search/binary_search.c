
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

void test_bsearch(unsigned size)
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

void sort(unsigned char x[], unsigned size)
{
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = i + 1; j < size; ++j)
        {
            if (x[j] < x[i])
            {
                unsigned char t = x[i];
                x[i] = x[j];
                x[j] = t;
            }
        }
    }
}

void test_sort(unsigned size)
{
    unsigned x[size];

    for (unsigned i = 0; i < size; ++i)
    {
        x[i] = nondet_uchar() & 15; // nondeterministic number in range 0 .. 15;
    }

    sort(x, size);

    assert(is_sorted(x, size));
}

int main()
{
    if (true)
    {
        for (unsigned size = 0; size <= 10; ++size)
        {
            test_bsearch(size);
        }
    }

    if (true)
    {
        for (unsigned size = 0; size <= 10; ++size)
        {
            test_sort(size);
        }
    }

    return 0;
}
