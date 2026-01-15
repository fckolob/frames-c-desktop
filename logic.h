#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>

#define MAX_CODE_ENTRIES 10
#define MAX_FRAMES_PER_OPENING 30
#define MAX_OPENINGS 100
#define MAX_BARS 500

typedef struct {
    char key[50];
    char value[50];
} CodeEntry;

typedef struct {
    char serie[64];
    CodeEntry codes[MAX_CODE_ENTRIES];
    int code_count;
    double length;
    double length_width;  // For Screen Shash / U Dvh
    double length_height; // For Screen Shash / U Dvh
    char name[128];
    char spanish_name[128];
    char color[64];
    int quantity;
    int width_quantity;
    int height_quantity;
    double half;
    bool is_complex; // True for Screen Shash / U Dvh
} Frame;

typedef struct {
    int quantity;
    char name[128];
    char serie[64];
    char color[64];
    CodeEntry codes[MAX_CODE_ENTRIES];
    int code_count;
    double bar_length;
    char calculation_method[32];
    char cutting_map[12800];
} Bar;

typedef struct {
    double width_lateral;
    double width_central;
    double height;
    int quantity_lateral;
    int quantity_central;
    bool is_complex; // True if uses lateral/central distinct widths
} Glass;

typedef struct {
    double width;
    double height;
    char serie[64];
    char color[64];
    bool dvh;
    bool preframe;
    int quantity;
    Frame frames[MAX_FRAMES_PER_OPENING];
    int frame_count;
    
    // Glass info structured
    Glass glass;
} Opening;

void calculate_pieces(Opening* opening);
int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length);
void get_glass_description(Opening* op, char* buffer);

#endif // LOGIC_H
