#include <stdio.h>
#include <string.h>
#include "logic.h"

int main() {
    printf("Testing Cutting Plan Logic...\n");
    
    double pieces[] = {1500, 1500, 1500, 1000, 1000, 500};
    int count = 6;
    double bar_len = 5900;
    
    char method[32];
    char map[12800];
    
    // logic.c externs need to be linked. 
    // We are testing calculate_bars_for_group indirectly via logic.c
    // But logic.c helper is internal (static?) No, it's not static.
    
    // Prototype manual
    int calculate_bars_for_group(double* pieces, int count, double bar_length, double slice_val, char* method_out, char* map_out);
    
    int qty = calculate_bars_for_group(pieces, count, bar_len, 4.0, method, map);
    
    printf("Qty: %d\n", qty);
    printf("Method: %s\n", method);
    printf("Map:\n%s\n", map);
    
    if (qty > 0 && strlen(map) > 10) {
        printf("PASS: Map generated.\n");
        return 0;
    } else {
        printf("FAIL: No map or qty 0.\n");
        return 1;
    }
}
