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
    char codigo[16];
    char nombre[64];
    double cantidad;
    char unidad[16];
} Accesorio;

typedef struct {
    Accesorio cierresLaterales;
    Accesorio ruedasParaHojas;
    Accesorio ruedasParaMosquitero;
    Accesorio tapaMatrizEnganche;
    Accesorio taponDeHermeticidadLateralesDeHoja;
    Accesorio dispositivosDeEstanqueidad;
    Accesorio felpilla;
    Accesorio martillos;
    Accesorio recibidoresDeCierresLaterales;
    Accesorio tapadesagues;
    bool has_accessories;
} ReporteDeAccesorios;

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
    double length_fix_width;
    double length_fix_height;
    int fix_width_quantity;
    int fix_height_quantity;
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
    double width_fix;
    double height_fix;
    int quantity_fix;
    bool has_fixed;
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

    // Accessories
    ReporteDeAccesorios accesorios;
} Opening;

void calculate_pieces(Opening* opening);
int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length);
void get_glass_description(Opening* op, char* buffer);
void init_complex_frame_with_fix(Frame* frame, const char* serie, const char* name, const char* spanish_name, const char* color, double w, int qw, double h, int qh, double fw, int qfw, double fh, int qfh);

#endif // LOGIC_H
