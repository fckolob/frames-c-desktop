
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logic.h"

// Mock or link against logic.c
// Since logic.c is in the same dir, we can compile them together.

void test_waste() {
    double pieces[] = { 1000.0 };
    int count = 1;
    double bar_len = 6000.0;
    double slice = 4.0;
    char method[32];
    char map[12800];

    calculate_bars_for_group(pieces, count, bar_len, slice, method, map);

    printf("Output Map:\n%s\n", map);

    // Expected: "(Resto: 4996)"
    // Current likely: "(Resto: 5000)"
    
    if (strstr(map, "(Resto: 4996)")) {
        printf("PASS: Found expected Resto: 4996\n");
    } else if (strstr(map, "(Resto: 5000)")) {
        printf("FAIL: Found incorrect Resto: 5000 (Slice assumed unused)\n");
    } else {
        printf("FAIL: Unexpected Output.\n");
    }
}

int main() {
    test_waste();
    return 0;
}
