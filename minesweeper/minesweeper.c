#include <stdbool.h>

#define NUM_ROWS         3
#define NUM_COLS         3
#define KNOWN_BOMB_COUNT 2

void init_bombs(bool BOMB[NUM_ROWS][NUM_COLS])
{
    for (unsigned r = 0; r < NUM_ROWS; ++r)
    {
        for (unsigned c = 0; c < NUM_COLS; ++c)
        {
            BOMB[r][c] = nondet_bool();
        }
    }
}

int main(void)
{
    bool BOMB[NUM_ROWS][NUM_COLS];

    {
    }

    int STATE[NUM_ROWS][NUM_COLS] = {
        { 1, -1, -1},
        { 1, -1, -1},
        {-1, -1, -1}
    };

    {
        for (unsigned r = 0; r < NUM_ROWS; ++r)
        {
            for (unsigned c = 0; c < NUM_COLS; ++c)
            {
                STATE[r][c] = -1;
            }
        }
    }

    STATE[0][0] = 1;
    STATE[1][0] = 1;

    BOMB[2][1] = true;

    bool consistent = true;

    if (KNOWN_BOMB_COUNT >= 0)
    {
        unsigned ACTUAL_BOMB_COUNT = 0;
        for (unsigned r = 0; r < NUM_ROWS; ++r)
        {
            for (unsigned c = 0; c < NUM_COLS; ++c)
            {
                if (BOMB[r][c])
                {
                    ACTUAL_BOMB_COUNT += 1;
                }
            }
        }

        if (KNOWN_BOMB_COUNT != ACTUAL_BOMB_COUNT)
        {
            consistent = false;
        }
    }

    for (unsigned r = 0; r < NUM_ROWS; ++r)
    {
        for (unsigned c = 0; c < NUM_COLS; ++c)
        {
            if (STATE[r][c] >= 0)
            {
                unsigned NEIGHBOURS = 0;
                for (unsigned dr = -1; dr <= +1; ++dr)
                {
                    for (unsigned dc = -1; dc <= +1; ++dc)
                    {
                        if (dc != dr)
                        {
                            unsigned rr = r + dr;
                            unsigned cc = c + dc;
                            if (rr >= 0 && rr < NUM_ROWS && cc >= 0 && cc < NUM_COLS)
                            {
                                if (BOMB[rr][cc])
                                {
                                    NEIGHBOURS += 1;
                                }
                            }
                        }
                    }
                }

                if (STATE[r][c] != NEIGHBOURS)
                {
                    consistent = false;
                }
            }
        }
    }

    // If the verification is succesful, we never read ca consistent state.

    assert(!consistent);

    return 0;
}
