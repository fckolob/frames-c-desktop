#include "logic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Helper to add a code entry
void add_code(Frame* f, const char* key, const char* value) {
    if (f->code_count < MAX_CODE_ENTRIES) {
        strncpy(f->codes[f->code_count].key, key, 49);
        strncpy(f->codes[f->code_count].value, value, 49);
        f->code_count++;
    }
}

void init_frame(Frame* f, const char* serie, const char* name, const char* spanish_name, const char* color, double length, int quantity) {
    memset(f, 0, sizeof(Frame));
    strncpy(f->serie, serie, 63);
    strncpy(f->name, name, 127);
    strncpy(f->spanish_name, spanish_name, 127);
    strncpy(f->color, color, 63);
    f->length = length;
    f->quantity = quantity;
    f->half = length / 2.0;
    f->is_complex = false;
}

void init_complex_frame(Frame* f, const char* serie, const char* name, const char* spanish_name, const char* color, double w_len, int w_qty, double h_len, int h_qty) {
    memset(f, 0, sizeof(Frame));
    strncpy(f->serie, serie, 63);
    strncpy(f->name, name, 127);
    strncpy(f->spanish_name, spanish_name, 127);
    strncpy(f->color, color, 63);
    f->length_width = w_len;
    f->width_quantity = w_qty;
    f->length_height = h_len;
    f->height_quantity = h_qty;
    f->is_complex = true;
}

void calculate_pieces(Opening* op) {
    double w = op->width;
    double h = op->height;
    int q = op->quantity;
    op->frame_count = 0;
    op->glass_info[0] = '\0';

    if (strcmp(op->serie, "s20") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame", "Horizontal de Marco", op->color, w - 25, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "N1749");
        add_code(&op->frames[op->frame_count], "urualum", "190");
        add_code(&op->frames[op->frame_count], "juan", "204 o 190");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0190");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "N1753");
        add_code(&op->frames[op->frame_count], "urualum", "191");
        add_code(&op->frames[op->frame_count], "juan", "205 o 191");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0191");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "N1751");
        add_code(&op->frames[op->frame_count], "urualum", "193");
        add_code(&op->frames[op->frame_count], "juan", "202 o 193");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0193");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2864");
        add_code(&op->frames[op->frame_count], "urualum", "189");
        add_code(&op->frames[op->frame_count], "juan", "216 o 189");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0189");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash", "Horizontal de Hoja", op->color, w / 2.0 - 74, q * 4);
        add_code(&op->frames[op->frame_count], "abasur", "N1752");
        add_code(&op->frames[op->frame_count], "urualum", "192");
        add_code(&op->frames[op->frame_count], "juan", "192");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0192");
        op->frame_count++;

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 2.0 + 10, q * 2, h - 30, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E4436");
        add_code(&op->frames[op->frame_count], "urualum", "2314");
        add_code(&op->frames[op->frame_count], "juan", "214");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 2314");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S20", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E4821");
        add_code(&op->frames[op->frame_count], "urualum", "213");
        add_code(&op->frames[op->frame_count], "juan", "213");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 2.0 - 58, h - 112, q * 2);
    }
    else if (strcmp(op->serie, "s25") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Inferior Frame", "Inferior de Marco", op->color, w - 51, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2857");
        add_code(&op->frames[op->frame_count], "urualum", "2500");
        add_code(&op->frames[op->frame_count], "juan", "150 o 2500");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Superior Frame", "Superior de Marco", op->color, w - 51, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2858");
        add_code(&op->frames[op->frame_count], "urualum", "2528");
        add_code(&op->frames[op->frame_count], "juan", "151 o 2528");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E3513");
        add_code(&op->frames[op->frame_count], "urualum", "2501");
        add_code(&op->frames[op->frame_count], "juan", "2501");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2861");
        add_code(&op->frames[op->frame_count], "urualum", "4507");
        add_code(&op->frames[op->frame_count], "juan", "155 o 4507");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big", "Horizontal de Hoja Grueso", op->color, w / 2.0 - 96, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Small", "Horizontal de Hoja Fino", op->color, w / 2.0 - 96, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2863");
        add_code(&op->frames[op->frame_count], "urualum", "4504");
        add_code(&op->frames[op->frame_count], "juan", "4504");
        op->frame_count++;

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 2.0 + 10, q * 2, h - 40, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E3514");
        add_code(&op->frames[op->frame_count], "urualum", "2343");
        add_code(&op->frames[op->frame_count], "juan", "2343");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 2343");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 L", "Guía de Mosquitero S25 L", op->color, w - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4678");
        add_code(&op->frames[op->frame_count], "urualum", "213");
        add_code(&op->frames[op->frame_count], "juan", "213");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 P", "Guía de Mosquitero S25 P", op->color, w - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4677");
        add_code(&op->frames[op->frame_count], "urualum", "2344");
        add_code(&op->frames[op->frame_count], "juan", "2344");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        if (op->dvh) {
            init_complex_frame(&op->frames[op->frame_count], op->serie, "U Dvh", "U Dvh", op->color, w / 2.0 - 100, q * 4, h - 155, q * 4);
            add_code(&op->frames[op->frame_count], "abasur", "E4886");
            add_code(&op->frames[op->frame_count], "juan", "4590");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 2.0 - 111, h - 168, q * 2);
        } else {
            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 2.0 - 80, h - 137, q * 2);
        }
    }
    else if (strcmp(op->serie, "s25TripleRiel") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Inferior Frame Triple Riel", "Inferior de Marco Triple Riel", op->color, w - 51, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4940");
        add_code(&op->frames[op->frame_count], "juan", "2538");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Superior Frame Triple Riel", "Superior de Marco Triple Riel", op->color, w - 51, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4674");
        add_code(&op->frames[op->frame_count], "juan", "2532");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco Triple Riel", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E4676");
        add_code(&op->frames[op->frame_count], "juan", "2534");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche", op->color, h - 48, q * 4);
        add_code(&op->frames[op->frame_count], "abasur", "E2861");
        add_code(&op->frames[op->frame_count], "urualum", "4507");
        add_code(&op->frames[op->frame_count], "juan", "155 o 4507");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big", "Horizontal de Hoja Grueso", op->color, w / 3.0 - 77, q * 3);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Small", "Horizontal de Hoja Fino", op->color, w / 3.0 - 77, q * 3);
        add_code(&op->frames[op->frame_count], "abasur", "E2863");
        add_code(&op->frames[op->frame_count], "urualum", "4504");
        add_code(&op->frames[op->frame_count], "juan", "4504");
        op->frame_count++;

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 - 9, q * 2, h - 40, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2860");
        add_code(&op->frames[op->frame_count], "urualum", "4506");
        add_code(&op->frames[op->frame_count], "juan", "4506");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 L", "Guía de Mosquitero S25 L", op->color, w - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4678");
        add_code(&op->frames[op->frame_count], "urualum", "213");
        add_code(&op->frames[op->frame_count], "juan", "213");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 P", "Guia de Mosquitero S25 P", op->color, w - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4677");
        add_code(&op->frames[op->frame_count], "urualum", "2344");
        add_code(&op->frames[op->frame_count], "juan", "2344");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        if (op->dvh) {
            init_complex_frame(&op->frames[op->frame_count], op->serie, "U Dvh", "U Dvh", op->color, w / 3.0 - 81, q * 6, h - 155, q * 6);
            add_code(&op->frames[op->frame_count], "abasur", "E4886");
            add_code(&op->frames[op->frame_count], "juan", "4590");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 3.0 - 92, h - 168, q * 3);
        } else {
            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 3.0 - 60, h - 137, q * 3);
        }
    }
    else if (strcmp(op->serie, "probbaCorrediza") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame", "Horizontal de Marco", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93150");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90021");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90030");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash", "Horizontal de Hoja para DVH", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 2.0 - 86, h - 165, q * 2);
        } else {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90027");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90028");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash", "Horizontal de Hoja para Vidrio Simple", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho de Vidrio = %.1f\nAlto de Vidrio = %.1f\nCantidad de Vidrios = %d\n", w / 2.0 - 94, h - 172, q * 2);
        }

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 2.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "probbaCorredizaTripleRiel") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame", "Horizontal de Marco triple Riel", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93054");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco Triple Riel", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93073");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para DVH", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90030");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para DVH (Hojas Laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para DVH (Hoja Central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 3.0 - 61, w / 3.0 - 70, h - 165, q * 2, q);
        } else {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90027");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para Vidrio Simple", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90028");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para Vidrio Simple (Hoja Central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 3.0 - 68, w / 3.0 - 77, h - 173, q * 2, q);
        }
        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "galaCorredizaCuatroRieles") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame", "Horizontal de Marco cuatro Rieles", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93067");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco cuatro Rieles", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93063");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para DVH gala", op->color, h - 65, q * 6);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45033");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para DVH gala (hojas laterales)", op->color, w / 4.0 + 2, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para DVH gala (hoja central)", op->color, w / 4.0 + 21, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 4.0 - 52, w / 4.0 - 61, h - 165, q * 2, q * 2);
        } else {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93007");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para Vidrio Simple gala", op->color, h - 65, q * 6);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93009");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales) gala", op->color, w / 4.0 + 2, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para Vidrio Simple (Hoja Central) gala", op->color, w / 4.0 + 21, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 4.0 - 59, w / 4.0 - 67, h - 171, q * 2, q * 2);
        }
        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 4.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "galaCorredizaTripleRiel") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame", "Horizontal de Marco triple Riel", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93054");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame", "Lateral de Marco Triple Riel", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93073");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para DVH gala", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45033");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para DVH gala (hojas laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para DVH gala (hoja central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 3.0 - 61, w / 3.0 - 70, h - 165, q * 2, q);
        } else {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93007");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche para Vidrio Simple gala", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93009");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Lateral", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales) gala", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Central", "Horizontal de Hoja para Vidrio Simple (Hoja Central) gala", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            sprintf(op->glass_info, "Ancho H. Central = %.1f\nAncho H. Laterales = %.1f\nAlto = %.1f\nCant Laterales = %d\nCant Central = %d\n", w / 3.0 - 68, w / 3.0 - 77, h - 173, q * 2, q);
        }
        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
}

// Bin Packing Helpers
int sort_desc(const void* a, const void* b) {
    double fa = *(const double*)a;
    double fb = *(const double*)b;
    return (fa < fb) - (fa > fb);
}

typedef struct {
    char key[512];
    int result;
} MemoEntry;

MemoEntry memo[5000];
int memo_count = 0;

int find_cached(const char* key) {
    for (int i = 0; i < memo_count; i++) {
        if (strcmp(memo[i].key, key) == 0) return memo[i].result;
    }
    return -1;
}

void add_cache(const char* key, int result) {
    if (memo_count < 5000) {
        strncpy(memo[memo_count].key, key, 511);
        memo[memo_count].result = result;
        memo_count++;
    }
}

int helper(double* pieces, int count, int index, double* remains, int remains_count, double bar_length, double slice_val) {
    if (index >= count) return 0;

    // Filter remains
    double min_piece = pieces[count - 1]; // sorted desc
    double filtered[MAX_BARS];
    int f_count = 0;
    for (int i = 0; i < remains_count; i++) {
        if (remains[i] >= min_piece + slice_val) filtered[f_count++] = remains[i];
    }
    qsort(filtered, f_count, sizeof(double), sort_desc);

    char key[512];
    char part[64];
    sprintf(key, "%d|", index);
    for (int i = 0; i < f_count; i++) {
        sprintf(part, "%.1f,", filtered[i]);
        strcat(key, part);
    }

    int cached = find_cached(key);
    if (cached != -1) return cached;

    int min_bars = 999999;
    double piece = pieces[index];

    // Try put in existing
    for (int i = 0; i < f_count; i++) {
        if (filtered[i] >= piece + slice_val) {
            double* next_remains = malloc(sizeof(double) * f_count);
            if (next_remains) {
                memcpy(next_remains, filtered, sizeof(double) * f_count);
                next_remains[i] -= (piece + slice_val);
                int res = helper(pieces, count, index + 1, next_remains, f_count, bar_length, slice_val);
                if (res < min_bars) min_bars = res;
                free(next_remains);
            }
        }
    }

    // New bin
    double* next_remains_new = malloc(sizeof(double) * (f_count + 1));
    if (next_remains_new) {
        memcpy(next_remains_new, filtered, sizeof(double) * f_count);
        next_remains_new[f_count] = bar_length - (piece + slice_val);
        int res_new = 1 + helper(pieces, count, index + 1, next_remains_new, f_count + 1, bar_length, slice_val);
        if (res_new < min_bars) min_bars = res_new;
        free(next_remains_new);
    }

    add_cache(key, min_bars);
    return min_bars;
}

int greedy_bin_packing(double* pieces, int count, double bar_length, double slice_val) {
    double bins[MAX_BARS];
    int bin_count = 0;
    for (int i = 0; i < count; i++) {
        bool placed = false;
        for (int j = 0; j < bin_count; j++) {
            if (bins[j] >= pieces[i] + slice_val) {
                bins[j] -= (pieces[i] + slice_val);
                placed = true;
                break;
            }
        }
        if (!placed) {
            bins[bin_count++] = bar_length - (pieces[i] + slice_val);
        }
    }
    return bin_count;
}

int calculate_bars_for_group(double* pieces, int count, double bar_length, double slice_val) {
    if (count == 0) return 0;
    qsort(pieces, count, sizeof(double), sort_desc);
    if (count > 18) return greedy_bin_packing(pieces, count, bar_length, slice_val);
    memo_count = 0;
    double empty[MAX_BARS];
    return helper(pieces, count, 0, empty, 0, bar_length, slice_val);
}

int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length) {
    typedef struct {
        char color[64];
        CodeEntry codes[MAX_CODE_ENTRIES];
        int code_count;
        double pieces[1000];
        int piece_count;
        double bar_length;
        char spanish_name[128];
        char serie[64];
    } Group;

    Group groups[100];
    int group_count = 0;

    for (int i = 0; i < opening_count; i++) {
        calculate_pieces(&openings[i]);
        for (int j = 0; j < openings[i].frame_count; j++) {
            Frame* f = &openings[i].frames[j];
            
            // Group by color and codes
            int found = -1;
            for (int k = 0; k < group_count; k++) {
                if (strcmp(groups[k].color, f->color) == 0 && groups[k].code_count == f->code_count) {
                    bool match = true;
                    for (int m = 0; m < f->code_count; m++) {
                        bool code_found = false;
                        for (int n = 0; n < groups[k].code_count; n++) {
                            if (strcmp(groups[k].codes[n].key, f->codes[m].key) == 0 && strcmp(groups[k].codes[n].value, f->codes[m].value) == 0) {
                                code_found = true;
                                break;
                            }
                        }
                        if (!code_found) { match = false; break; }
                    }
                    if (match) { found = k; break; }
                }
            }

            if (found == -1) {
                found = group_count++;
                strncpy(groups[found].color, f->color, 63);
                groups[found].code_count = f->code_count;
                memcpy(groups[found].codes, f->codes, sizeof(CodeEntry) * f->code_count);
                groups[found].piece_count = 0;
                groups[found].bar_length = (strcmp(f->serie, "s20") == 0 || strcmp(f->serie, "s25") == 0 || strcmp(f->serie, "s25TripleRiel") == 0) ? 5900 : 6700;
                strncpy(groups[found].spanish_name, f->spanish_name, 127);
                strncpy(groups[found].serie, f->serie, 63);
            }

            if (f->is_complex) {
                for (int m = 0; m < f->width_quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length_width;
                for (int m = 0; m < f->height_quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length_height;
            } else {
                for (int m = 0; m < f->quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length;
            }
        }
    }

    int result_count = 0;
    for (int i = 0; i < group_count; i++) {
        int qty = calculate_bars_for_group(groups[i].pieces, groups[i].piece_count, groups[i].bar_length, 4.0);
        if (qty > 0) {
            result_bars[result_count].quantity = qty;
            strncpy(result_bars[result_count].name, groups[i].spanish_name, 127);
            strncpy(result_bars[result_count].serie, groups[i].serie, 63);
            strncpy(result_bars[result_count].color, groups[i].color, 63);
            result_bars[result_count].code_count = groups[i].code_count;
            memcpy(result_bars[result_count].codes, groups[i].codes, sizeof(CodeEntry) * groups[i].code_count);
            result_bars[result_count].bar_length = groups[i].bar_length;
            result_count++;
        }
    }

    return result_count;
}
