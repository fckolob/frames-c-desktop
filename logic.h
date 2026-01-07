#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>

#define MAX_CODE_ENTRIES 5
#define MAX_FRAMES_PER_OPENING 20
#define MAX_OPENINGS 50
#define MAX_BARS 200

typedef struct {
    char key[50];
    char value[50];
} CodeEntry;

typedef struct {
    char serie[50];
    CodeEntry codes[MAX_CODE_ENTRIES];
    int code_count;
    double length;
    double length_width;  // For Screen Shash / U Dvh
    double length_height; // For Screen Shash / U Dvh
    char name[100];
    char spanish_name[100];
    char color[50];
    int quantity;
    int width_quantity;
    int height_quantity;
    double half;
} Frame;

typedef struct {
    int quantity;
    char name[100];
    char serie[50];
    char color[50];
    CodeEntry codes[MAX_CODE_ENTRIES];
    int code_count;
} Bar;

typedef struct {
    double width;
    double height;
    char serie[50];
    char color[50];
    bool dvh;
    bool preframe;
    int quantity;
    Frame frames[MAX_FRAMES_PER_OPENING];
    int frame_count;
} Opening;

void calculate_pieces(Opening* opening);
void framing(Opening* opening);
int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length);

#endif // LOGIC_H
