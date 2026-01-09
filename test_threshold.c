#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logic.h"

// Declaration to access internal functions if needed or just use logic.h interface
// We will simple rely on performance difference or just trust the code change if performance is hard to measure accurately in small test.
// However, recursive bin packing for 20 items is VERY slow usually.

int calculate_bars_for_group(double* pieces, int count, double bar_length, double slice_val);

void test_size(int size) {
    printf("Testing size %d...\n", size);
    double pieces[100];
    for (int i=0; i<size; i++) {
        pieces[i] = 1000.0 + (i * 10); // arbitrary pieces
    }
    
    clock_t start = clock();
    int bars = calculate_bars_for_group(pieces, size, 6000.0, 4.0);
    clock_t end = clock();
    
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Size %d: %d bars found in %.4f seconds.\n", size, bars, time_taken);
}

int main() {
    // 18 should use optimal (might be slowish but doable?)
    // Actually recursive for 18 might take a few seconds if not well pruned.
    // Let's test 22. 22 with recursive is practically impossible to finish quickly.
    // 22 with greedy is instant.
    
    test_size(25);
    return 0;
}
