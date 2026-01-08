#include <stdio.h>
int main() {
    FILE* f = fopen("hello.txt", "w");
    if (f) {
        fprintf(f, "The compiler and execution are working!\n");
        fclose(f);
    }
    return 0;
}
