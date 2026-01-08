#include <stdio.h>
#include <string.h>
#include "logic.h"

void test_serie(const char* serie) {
    Opening op;
    memset(&op, 0, sizeof(Opening));
    op.width = 1500;
    op.height = 1200;
    op.quantity = 1;
    strcpy(op.serie, serie);
    strcpy(op.color, "blanco");
    op.dvh = false;

    calculate_pieces(&op);
    printf("Results for %s (1500x1200):\n", serie);
    for (int i = 0; i < op.frame_count; i++) {
        Frame* f = &op.frames[i];
        if (f->is_complex) {
            printf("  %s: W=%.1f (x%d), H=%.1f (x%d)\n", f->spanish_name, f->length_width, f->width_quantity, f->length_height, f->height_quantity);
        } else {
            printf("  %s: %.1f (x%d)\n", f->spanish_name, f->length, f->quantity);
        }
    }
    printf("\n");
}

int main() {
    test_serie("s20");
    test_serie("s25");
    test_serie("probbaCorrediza");
    return 0;
}
