#include <stdio.h>
#include <string.h>
#include "logic.h"

void test_serie(const char* serie, const char* name) {
    Opening op;
    memset(&op, 0, sizeof(Opening));
    op.width = 1500;
    op.height = 1200;
    op.quantity = 1;
    strncpy(op.serie, serie, 63);
    strncpy(op.color, "blanco", 63);
    op.dvh = false;

    printf("Testing %s (%s)...\n", name, serie);
    calculate_pieces(&op);

    char buffer[1024];
    get_glass_description(&op, buffer);
    printf("Glass Description:\n%s\n", buffer);
    
    // Verify specific values
     if (op.glass.is_complex) {
        printf("  Width Lateral: %.1f\n", op.glass.width_lateral);
        printf("  Width Central: %.1f\n", op.glass.width_central);
    } else {
        printf("  Width: %.1f\n", op.glass.width_central);
    } 
    printf("  Height: %.1f\n", op.glass.height);

    printf("--------------------------------------------------\n");
}

int main() {
    test_serie("s20", "Serie 20");
    test_serie("s25", "Serie 25");
    test_serie("s25TripleRiel", "Serie 25 Triple Riel");
    test_serie("probbaCorrediza", "Probba Corrediza");
    test_serie("galaCorredizaCuatroRieles", "Gala 4 Rieles");
    return 0;
}
