#include "logic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static void safe_copy(char* dest, const char* src, size_t dest_size) {
    if (!dest || dest_size == 0) return;
    if (!src) { dest[0] = '\0'; return; }
    size_t i = 0;
    while (i + 1 < dest_size && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Helper to add a code entry
void add_code(Frame* f, const char* key, const char* value) {
    if (f->code_count < MAX_CODE_ENTRIES) {
        safe_copy(f->codes[f->code_count].key, key, sizeof(f->codes[f->code_count].key));
        safe_copy(f->codes[f->code_count].value, value, sizeof(f->codes[f->code_count].value));
        f->code_count++;
    }
}

void init_frame(Frame* f, const char* serie, const char* name, const char* spanish_name, const char* color, double length, int quantity) {
    memset(f, 0, sizeof(Frame));
    safe_copy(f->serie, serie, sizeof(f->serie));
    safe_copy(f->name, name, sizeof(f->name));
    safe_copy(f->spanish_name, spanish_name, sizeof(f->spanish_name));
    safe_copy(f->color, color, sizeof(f->color));
    f->length = length;
    f->quantity = quantity;
    f->half = length / 2.0;
    f->is_complex = false;
}

void init_complex_frame(Frame* f, const char* serie, const char* name, const char* spanish_name, const char* color, double w_len, int w_qty, double h_len, int h_qty) {
    memset(f, 0, sizeof(Frame));
    safe_copy(f->serie, serie, sizeof(f->serie));
    safe_copy(f->name, name, sizeof(f->name));
    safe_copy(f->spanish_name, spanish_name, sizeof(f->spanish_name));
    safe_copy(f->color, color, sizeof(f->color));
    f->length_width = w_len;
    f->width_quantity = w_qty;
    f->length_height = h_len;
    f->height_quantity = h_qty;
    f->is_complex = true;
}

void init_complex_frame_with_fix(Frame* f, const char* serie, const char* name, const char* spanish_name, const char* color, double w_len, int w_qty, double h_len, int h_qty, double fw_len, int fw_qty, double fh_len, int fh_qty) {
    memset(f, 0, sizeof(Frame));
    safe_copy(f->serie, serie, sizeof(f->serie));
    safe_copy(f->name, name, sizeof(f->name));
    safe_copy(f->spanish_name, spanish_name, sizeof(f->spanish_name));
    safe_copy(f->color, color, sizeof(f->color));
    f->length_width = w_len;
    f->width_quantity = w_qty;
    f->length_height = h_len;
    f->height_quantity = h_qty;
    f->length_fix_width = fw_len;
    f->fix_width_quantity = fw_qty;
    f->length_fix_height = fh_len;
    f->fix_height_quantity = fh_qty;
    f->is_complex = true;
}

void fmt_num(double val, char* out, int max_len) {
    if (out == NULL || max_len <= 0) return;
    double int_part;
    double frac = modf(val, &int_part);
    if (fabs(frac) < 0.0001 || fabs(fabs(frac) - 1.0) < 0.0001) {
        snprintf(out, max_len, "%.0f", round(val));
    } else {
        snprintf(out, max_len, "%.1f", val);
        int len = strlen(out);
        if (len >= 2 && out[len - 2] == '.' && out[len - 1] == '0') {
            out[len - 2] = '\0';
        }
    }
}

bool frame_has_half(const char* name) {
    static const char* const half_names[] = {
        "Horizontal Frame",
        "Inferior Frame",
        "Superior Frame",
        "Lateral Shash",
        "Central Shash",
        "Lateral Shash for DVH",
        "Central Shash for DVH",
        "Lateral Shash for Single Glass",
        "Central Shash for Single Glass",
        "Horizontal Frame Cuatro Rieles",
        "Vertical Frame Cuatro Rieles",
        "Horizontal Frame Triple Riel",
        "Vertical Frame Triple Riel",
        "Lateral Shash for DVH gala",
        "Central Shash for DVH gala",
        "Horizontal Shash for DVH gala (hojas laterales)",
        "Horizontal Shash for DVH gala (hoja central)",
        "Lateral Shash for Single Glass gala",
        "Central Shash for Single Glass gala",
        "Horizontal Shash for Single Glass (Lateral Shashes) gala",
        "Horizontal Shash for Single Glass (Central Shash) gala"
    };
    for (size_t i = 0; i < sizeof(half_names) / sizeof(half_names[0]); i++) {
        if (strcmp(name, half_names[i]) == 0) return true;
    }
    return false;
}

void init_accesorio(Accesorio* a, const char* codigo, const char* nombre, double cantidad, const char* unidad) {
    safe_copy(a->codigo, codigo, sizeof(a->codigo));
    safe_copy(a->nombre, nombre, sizeof(a->nombre));
    a->cantidad = cantidad;
    safe_copy(a->unidad, unidad, sizeof(a->unidad));
}

void init_reporte_accesorios(Opening* op) {
    ReporteDeAccesorios* acc = &op->accesorios;
    memset(acc, 0, sizeof(ReporteDeAccesorios));
    double w = op->width;
    double h = op->height;
    int q = op->quantity;
    
    bool is_probba = (strcmp(op->serie, "probbaCorrediza") == 0 || strcmp(op->serie, "probbaCorredizaTripleRiel") == 0 || strcmp(op->serie, "probbaCorredizaTresHojasEnDosRieles") == 0);
    bool is_gala = (strcmp(op->serie, "galaCorrediza") == 0 || strcmp(op->serie, "galaCorredizaTripleRiel") == 0 || strcmp(op->serie, "galaCorredizaCuatroRieles") == 0);

    if (is_probba || is_gala) {
        acc->has_accessories = true;
        
        // Common for all Probba/Gala (mostly)
        init_accesorio(&acc->cierresLaterales, "NGG2114", "Cierres Laterales Brio", q * 2.0, "unidades");
        init_accesorio(&acc->martillos, "NGG6260", "Martillos para cierres laterales Brio", q * 2.0, "unidades");
        init_accesorio(&acc->recibidoresDeCierresLaterales, "NGG6390", "Recibidor de Cierres Laterales", q * 2.0, "unidades");
        init_accesorio(&acc->tapadesagues, "AUU3094", "Tapa desagües", (w / 250.0) * 2.0 * q, "unidades");
        init_accesorio(&acc->dispositivosDeEstanqueidad, "APD3054", "Dispositivos de estanqueidad", q * 2.0, "unidades");
        init_accesorio(&acc->taponDeHermeticidadLateralesDeHoja, "AGG5600", "Tapones de hermeticidad para laterales de hoja", q * 4.0, "unidades");

        if (strcmp(op->serie, "probbaCorrediza") == 0 || strcmp(op->serie, "galaCorrediza") == 0) {
            init_accesorio(&acc->ruedasParaHojas, "AGF1020", "Ruedas regulables para hojas", q * 4.0, "unidades");
            init_accesorio(&acc->ruedasParaMosquitero, "AGG1060", "Ruedas regulables para mosquitero", q * 2.0, "unidades");
            init_accesorio(&acc->tapaMatrizEnganche, "AGU3574", "Tapamatrices para enganche", q * 4.0, "unidades");
            
            double felp;
            if (strcmp(op->serie, "probbaCorrediza") == 0)
                felp = ((((w / 2.0 - 23.0) * 8.0) + ((h - 65.0) * 6.0) + ((w / 2.0 + 11.0) * 4.0) + ((h - 62.0) * 4.0)) / 1000.0) * q;
            else // galaCorrediza
                felp = ((((w / 4.0 + 21.0) * 16.0) + ((h - 65.0) * 10.0) + ((w / 4.0 + 11.0) * 4.0) + ((h - 62.0) * 4.0)) / 1000.0) * q;
            
            init_accesorio(&acc->felpilla, "AMS7227", "Felpilla", felp, "metros");
        }
        else if (strcmp(op->serie, "probbaCorredizaTripleRiel") == 0 || strcmp(op->serie, "galaCorredizaTripleRiel") == 0) {
            init_accesorio(&acc->ruedasParaHojas, "AGF1020", "Ruedas regulables para hojas", q * 6.0, "unidades");
            init_accesorio(&acc->ruedasParaMosquitero, "AGG1060", "Ruedas regulables para mosquitero", q * 2.0, "unidades");
            init_accesorio(&acc->tapaMatrizEnganche, "AGU3574", "Tapamatrices para enganche", q * 8.0, "unidades");
            acc->dispositivosDeEstanqueidad.cantidad = q * 4.0;
            
            double felp;
            if (strcmp(op->serie, "probbaCorredizaTripleRiel") == 0)
                felp = ((((w / 2.0 - 23.0) * 8.0) + ((h - 65.0) * 6.0) + ((w / 3.0 + 11.0) * 4.0) + ((h - 62.0) * 4.0)) / 1000.0) * q;
            else // galaTriple
                felp = ((((w / 3.0 + 12.0) * 12.0) + ((h - 65.0) * 8.0) + ((w / 3.0 + 11.0) * 4.0) + ((h - 62.0) * 4.0)) / 1000.0) * q;
            
            init_accesorio(&acc->felpilla, "AMS7227", "Felpilla", felp, "metros");
        }
        else if (strcmp(op->serie, "probbaCorredizaTresHojasEnDosRieles") == 0) {
            init_accesorio(&acc->ruedasParaHojas, "AGF1020", "Ruedas regulables para hojas", q * 6.0, "unidades");
            init_accesorio(&acc->ruedasParaMosquitero, "AGG1060", "Ruedas regulables para mosquitero", q * 4.0, "unidades");
            init_accesorio(&acc->tapaMatrizEnganche, "AGU3574", "Tapamatrices para enganche", q * 8.0, "unidades");
            acc->dispositivosDeEstanqueidad.cantidad = q * 4.0;
            
            double felp = ((((w / 3.0 + 12.0) * 12.0) + ((h - 65.0) * 8.0) + ((w / 4.0 + 11.0) * 8.0) + ((h - 62.0) * 8.0)) / 1000.0) * q;
            init_accesorio(&acc->felpilla, "AMS7227", "Felpilla", felp, "metros");
        }
        else if (strcmp(op->serie, "galaCorredizaCuatroRieles") == 0) {
            init_accesorio(&acc->ruedasParaHojas, "AGF1020", "Ruedas regulables para hojas", q * 8.0, "unidades");
            init_accesorio(&acc->ruedasParaMosquitero, "AGG1060", "Ruedas regulables para mosquitero", q * 2.0, "unidades");
            init_accesorio(&acc->tapaMatrizEnganche, "AGU3574", "Tapamatrices para enganche", q * 12.0, "unidades");
            acc->dispositivosDeEstanqueidad.cantidad = q * 6.0;
            
            double felp = ((((w / 2.0 - 23.0) * 8.0) + ((h - 65.0) * 6.0) + ((w / 2.0 + 11.0) * 4.0) + ((h - 62.0) * 4.0)) / 1000.0) * q;
            init_accesorio(&acc->felpilla, "AMS7227", "Felpilla", felp, "metros");
        }
    }
}

void calculate_pieces(Opening* op) {
    double w = op->width;
    double h = op->height;
    int q = op->quantity;
    op->frame_count = 0;
    
    init_reporte_accesorios(op);

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

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E4821");
        add_code(&op->frames[op->frame_count], "urualum", "213");
        add_code(&op->frames[op->frame_count], "juan", "213");
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 0213");
        op->frame_count++;

        op->glass.is_complex = false;
        op->glass.width_central = w / 2.0 - 58;
        op->glass.height = h - 112;
        op->glass.quantity_central = q * 2;
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

            op->glass.is_complex = false;
            op->glass.width_central = w / 2.0 - 111;
            op->glass.height = h - 168;
            op->glass.quantity_central = q * 2;
        } else {
            op->glass.is_complex = false;
             op->glass.width_central = w / 2.0 - 80;
            op->glass.height = h - 137;
            op->glass.quantity_central = q * 2;
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

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame Triple Riel", "Lateral de Marco Triple Riel", op->color, h, q * 2);
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

            op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 92;
            op->glass.height = h - 168;
            op->glass.quantity_central = q * 3;
        } else {
             op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 60;
            op->glass.height = h - 137;
            op->glass.quantity_central = q * 3;
        }
    }
    else if (strcmp(op->serie, "s25TresHojasEnDosRieles") == 0) {
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

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja", op->color, h - 48, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash Fix", "Lateral de Hoja Fijo", op->color, h - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche", op->color, h - 48, q * 3);
        add_code(&op->frames[op->frame_count], "abasur", "E2861");
        add_code(&op->frames[op->frame_count], "urualum", "4507");
        add_code(&op->frames[op->frame_count], "juan", "155 o 4507");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash Fix", "Enganche Fijo", op->color, h - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2861");
        add_code(&op->frames[op->frame_count], "urualum", "4507");
        add_code(&op->frames[op->frame_count], "juan", "155 o 4507");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big", "Horizontal de Hoja Grueso", op->color, w / 3.0 - 77, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Small", "Horizontal de Hoja Fino", op->color, w / 3.0 - 77, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2863");
        add_code(&op->frames[op->frame_count], "urualum", "4504");
        add_code(&op->frames[op->frame_count], "juan", "4504");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big Fix", "Horizontal de Hoja Grueso Fijo", op->color, w / 3.0 - 49, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Small Fix", "Horizontal de Hoja Fino Fijo", op->color, w / 3.0 - 49, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2863");
        add_code(&op->frames[op->frame_count], "urualum", "4504");
        add_code(&op->frames[op->frame_count], "juan", "4504");
        op->frame_count++;

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 - 9, q * 2, h - 40, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2860");
        add_code(&op->frames[op->frame_count], "urualum", "4506");
        add_code(&op->frames[op->frame_count], "juan", "4506");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 L", "Gua de Mosquitero S25 L", op->color, w - 5, q);
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
            init_complex_frame_with_fix(&op->frames[op->frame_count], op->serie, "U Dvh", "U Dvh", op->color, w / 3.0 - 81, q * 4, h - 155, q * 4, w / 3.0 - 53, q * 2, h - 112, q * 2);
            add_code(&op->frames[op->frame_count], "abasur", "E4886");
            add_code(&op->frames[op->frame_count], "juan", "4590");
            op->frame_count++;
            
            op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 92;
            op->glass.height = h - 168;
            op->glass.quantity_central = q * 2;
            op->glass.width_fix = w / 3.0 - 64;
            op->glass.height_fix = h - 125;
            op->glass.quantity_fix = q;
            op->glass.has_fixed = true;
        } else {
            op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 61;
            op->glass.height = h - 137;
            op->glass.quantity_central = q * 2;
            op->glass.width_fix = w / 3.0 - 33;
            op->glass.height_fix = h - 94;
            op->glass.quantity_fix = q;
            op->glass.has_fixed = true;
        }
    }
    else if (strcmp(op->serie, "s25ReforzadaTresHojasEnDosRieles") == 0) {
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

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash", "Lateral de Hoja", op->color, h - 48, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash Fix", "Lateral de Hoja Fijo", op->color, h - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E2862");
        add_code(&op->frames[op->frame_count], "urualum", "4505");
        add_code(&op->frames[op->frame_count], "juan", "4505");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash", "Enganche", op->color, h - 48, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2861");
        add_code(&op->frames[op->frame_count], "urualum", "4507");
        add_code(&op->frames[op->frame_count], "juan", "155 o 4507");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash Reinforced", "Enganche Reforzado", op->color, h - 48, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4496");
        add_code(&op->frames[op->frame_count], "urualum", "4537");
        add_code(&op->frames[op->frame_count], "juan", "4537");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Central Shash Fix Reinforced", "Enganche Fijo Reforzado", op->color, h - 5, q);
        add_code(&op->frames[op->frame_count], "abasur", "E4496");
        add_code(&op->frames[op->frame_count], "urualum", "4537");
        add_code(&op->frames[op->frame_count], "juan", "4537");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big", "Horizontal de Hoja Grueso", op->color, w / 3.0 - 77, q * 4);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash Big Fix", "Horizontal de Hoja Grueso Fijo", op->color, w / 3.0 - 49, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2859");
        add_code(&op->frames[op->frame_count], "urualum", "4503");
        add_code(&op->frames[op->frame_count], "juan", "4503");
        op->frame_count++;

        init_complex_frame(&op->frames[op->frame_count], op->serie, "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 - 9, q * 2, h - 40, q * 2);
        add_code(&op->frames[op->frame_count], "abasur", "E2860");
        add_code(&op->frames[op->frame_count], "urualum", "4506");
        add_code(&op->frames[op->frame_count], "juan", "4506");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Screen Guide S25 L", "Guia de Mosquitero S25 L", op->color, w - 5, q);
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
            init_complex_frame_with_fix(&op->frames[op->frame_count], op->serie, "U Dvh", "U Dvh", op->color, w / 3.0 - 81, q * 4, h - 170, q * 4, w / 3.0 - 53, q * 2, h - 127, q * 2);
            add_code(&op->frames[op->frame_count], "abasur", "E4886");
            add_code(&op->frames[op->frame_count], "juan", "4590");
            op->frame_count++;
            
            op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 92;
            op->glass.height = h - 183;
            op->glass.quantity_central = q * 2;
            op->glass.width_fix = w / 3.0 - 64;
            op->glass.height_fix = h - 140;
            op->glass.quantity_fix = q;
            op->glass.has_fixed = true;
        } else {
            op->glass.is_complex = false;
            op->glass.width_central = w / 3.0 - 61;
            op->glass.height = h - 152;
            op->glass.quantity_central = q * 2;
            op->glass.width_fix = w / 3.0 - 33;
            op->glass.height_fix = h - 109;
            op->glass.quantity_fix = q;
            op->glass.has_fixed = true;
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
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash for DVH", "Lateral de Hoja para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash for DVH", "Enganche para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90030");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash for DVH", "Horizontal de Hoja para DVH", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            op->glass.is_complex = false;
            op->glass.width_central = w / 2.0 - 86;
            op->glass.height = h - 165;
            op->glass.quantity_central = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], op->serie, "Lateral Shash for Single Glass", "Lateral de Hoja para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90027");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Central Shash for Single Glass", "Enganche para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90028");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Shash for Single Glass", "Horizontal de Hoja para Vidrio Simple", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            op->glass.is_complex = false;
            op->glass.width_central = w / 2.0 - 94;
            op->glass.height = h - 172;
            op->glass.quantity_central = q * 2;
        }

        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 2.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "probbaCorredizaTripleRiel") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame Triple Riel", "Horizontal de Marco triple Riel", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93054");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame Triple Riel", "Lateral de Marco Triple Riel", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93073");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Lateral Shash for DVH", "Lateral de Hoja para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Central Shash for DVH", "Enganche para DVH", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90030");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for DVH (Lateral Shashes)", "Horizontal de Hoja para DVH (Hojas Laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for DVH (Central Shash)", "Horizontal de Hoja para DVH (Hoja Central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            op->glass.is_complex = true;
            op->glass.width_central = w / 3.0 - 61;
            op->glass.width_lateral = w / 3.0 - 70;
            op->glass.height = h - 165;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Lateral Shash for Single Glass", "Lateral de Hoja para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90027");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Central Shash for Single Glass", "Enganche para Vidrio Simple", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90028");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for Single Glass (Lateral Shashes)", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for Single Glass (Central Shash)", "Horizontal de Hoja para Vidrio Simple (Hoja Central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

             op->glass.is_complex = true;
            op->glass.width_central = w / 3.0 - 68;
            op->glass.width_lateral = w / 3.0 - 77;
            op->glass.height = h - 173;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        }
        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "probbaCorredizaTresHojasEnDosRieles") == 0) {
        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Frame", "Horizontal de Marco", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93150");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Vertical Frame", "Lateral de Marco", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90021");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Lateral Shash for DVH", "Lateral de Hoja para DVH", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Central Shash for DVH", "Enganche para DVH", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90030");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for DVH (Lateral Shashes)", "Horizontal de Hoja para DVH (Hojas Laterales)", op->color, w / 4.0 - 11, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for DVH (Central Shash)", "Horizontal de Hoja para DVH (Hoja Central)", op->color, w / 2.0 + 21, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90026");
            op->frame_count++;

            op->glass.is_complex = true;
            op->glass.width_central = w / 2.0 - 53;
            op->glass.width_lateral = w / 4.0 - 73;
            op->glass.height = h - 165;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Lateral Shash for Single Glass", "Lateral de Hoja para Vidrio Simple", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90027");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Central Shash for Single Glass", "Enganche para Vidrio Simple", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90028");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for Single Glass (Lateral Shashes)", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales)", op->color, w / 4.0 - 11, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Shash for Single Glass (Central Shash)", "Horizontal de Hoja para Vidrio Simple (Hoja Central)", op->color, w / 2.0 + 21, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90025");
            op->frame_count++;

             op->glass.is_complex = true;
            op->glass.width_central = w / 2.0 - 60;
            op->glass.width_lateral = w / 4.0 - 81;
            op->glass.height = h - 173;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        }
        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 4.0 + 37, q * 4, h - 62, q * 4);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "galaCorredizaCuatroRieles") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame Cuatro Rieles", "Horizontal de Marco cuatro Rieles", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93067");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame Cuatro Rieles", "Lateral de Marco cuatro Rieles", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93063");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for DVH gala", "Lateral de Hoja para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for DVH gala", "Enganche para DVH gala", op->color, h - 65, q * 6);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45033");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for DVH gala (hojas laterales)", "Horizontal de Hoja para DVH gala (hojas laterales)", op->color, w / 4.0 + 2, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for DVH gala (hoja central)", "Horizontal de Hoja para DVH gala (hoja central)", op->color, w / 4.0 + 21, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

             // Glass DVH
            op->glass.is_complex = true;
            op->glass.width_central = w / 4.0 - 52;
            op->glass.width_lateral = w / 4.0 - 61;
            op->glass.height = h - 165;
            op->glass.quantity_central = q * 2;
            op->glass.quantity_lateral = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for Single Glass gala", "Lateral de Hoja para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93007");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for Single Glass gala", "Enganche para Vidrio Simple gala", op->color, h - 65, q * 6);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93009");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for Single Glass (Lateral Shashes) gala", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales) gala", op->color, w / 4.0 + 2, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for Single Glass (Central Shash) gala", "Horizontal de Hoja para Vidrio Simple (Hoja Central) gala", op->color, w / 4.0 + 21, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            // Glass Simple
            op->glass.is_complex = true;
            op->glass.width_central = w / 4.0 - 59;
            op->glass.width_lateral = w / 4.0 - 67;
            op->glass.height = h - 171;
            op->glass.quantity_central = q * 2;
            op->glass.quantity_lateral = q * 2;
        }
        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 4.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "galaCorredizaTripleRiel") == 0) {
        init_frame(&op->frames[op->frame_count], op->serie, "Horizontal Frame Triple Riel", "Horizontal de Marco triple Riel", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93054");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], op->serie, "Vertical Frame Triple Riel", "Lateral de Marco Triple Riel", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93073");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for DVH gala", "Lateral de Hoja para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for DVH gala", "Enganche para DVH gala", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45033");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for DVH gala (hojas laterales)", "Horizontal de Hoja para DVH gala (hojas laterales)", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for DVH gala (hoja central)", "Horizontal de Hoja para DVH gala (hoja central)", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

             // Glass DVH
            op->glass.is_complex = true;
            op->glass.width_central = w / 3.0 - 61;
            op->glass.width_lateral = w / 3.0 - 70;
            op->glass.height = h - 165;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for Single Glass gala", "Lateral de Hoja para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93007");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for Single Glass gala", "Enganche para Vidrio Simple gala", op->color, h - 65, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93009");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for Single Glass (Lateral Shashes) gala", "Horizontal de Hoja para Vidrio Simple (Hojas Laterales) gala", op->color, w / 3.0 - 7, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for Single Glass (Central Shash) gala", "Horizontal de Hoja para Vidrio Simple (Hoja Central) gala", op->color, w / 3.0 + 12, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            // Glass Simple
            op->glass.is_complex = true;
            op->glass.width_central = w / 3.0 - 68;
            op->glass.width_lateral = w / 3.0 - 77;
            op->glass.height = h - 173;
            op->glass.quantity_central = q;
            op->glass.quantity_lateral = q * 2;
        }
        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 3.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
    else if (strcmp(op->serie, "galaCorrediza") == 0) {
        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Horizontal Frame", "Horizontal de Marco", op->color, w - 36, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93150");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Vertical Frame", "Lateral de Marco", op->color, h, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 90021");
        op->frame_count++;

        if (op->dvh) {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for DVH gala", "Lateral de Hoja para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45031");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for DVH gala", "Enganche para DVH gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45033");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for DVH gala", "Horizontal de Hoja para DVH gala", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 45032");
            op->frame_count++;

            op->glass.is_complex = false;
            op->glass.width_central = w / 2.0 - 86;
            op->glass.height = h - 165;
            op->glass.quantity_central = q * 2;
        } else {
            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Lateral Shash for Single Glass gala", "Lateral de Hoja para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93007");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Central Shash for Single Glass gala", "Enganche para Vidrio Simple gala", op->color, h - 65, q * 2);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93009");
            op->frame_count++;

            init_frame(&op->frames[op->frame_count], "galaCorrediza", "Horizontal Shash for Single Glass gala", "Horizontal de Hoja para Vidrio Simple gala", op->color, w / 2.0 - 23, q * 4);
            add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93005");
            op->frame_count++;

            op->glass.is_complex = false;
            op->glass.width_central = w / 2.0 - 94;
            op->glass.height = h - 172;
            op->glass.quantity_central = q * 2;
        }

        init_complex_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Shash", "Hoja de Mosquitero", op->color, w / 2.0 + 11, q * 2, h - 62, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 41043");
        op->frame_count++;

        init_frame(&op->frames[op->frame_count], "probbaCorrediza", "Screen Guide", "Guía de Mosquitero", op->color, w - 5, q * 2);
        add_code(&op->frames[op->frame_count], "aluminiosDelUruguay", "PN 93074");
        op->frame_count++;
    }
}

void get_glass_description(Opening* op, char* buffer) {
    char s_wc[32], s_wl[32], s_h[32], s_ql[32], s_qc[32], s_wf[32], s_hf[32], s_qf[32];
    fmt_num(op->glass.width_central, s_wc, sizeof(s_wc));
    fmt_num(op->glass.width_lateral, s_wl, sizeof(s_wl));
    fmt_num(op->glass.height, s_h, sizeof(s_h));
    fmt_num((double)op->glass.quantity_lateral, s_ql, sizeof(s_ql));
    fmt_num((double)op->glass.quantity_central, s_qc, sizeof(s_qc));
    fmt_num(op->glass.width_fix, s_wf, sizeof(s_wf));
    fmt_num(op->glass.height_fix, s_hf, sizeof(s_hf));
    fmt_num((double)op->glass.quantity_fix, s_qf, sizeof(s_qf));

    if (op->glass.has_fixed && op->glass.is_complex) {
        sprintf(buffer, 
            "Ancho de Vidrio Hoja Central = %s\r\n"
            "Ancho de Vidrios Hojas Laterales = %s\r\n"
            "Alto de Vidrios = %s\r\n"
            "Cantidad de Vidrios Laterales = %s\r\n"
            "Cantidad de vidrios centrales = %s\r\n"
            "Ancho de Vidrio Fijo = %s\r\n"
            "Alto de Vidrio Fijo = %s\r\n"
            "Cantidad de Vidrios Fijos = %s",
            s_wc, s_wl, s_h, s_ql, s_qc, s_wf, s_hf, s_qf
        );
    } else if (op->glass.has_fixed && !op->glass.is_complex) {
        sprintf(buffer, 
            "Ancho de Vidrio = %s\r\n"
            "Alto de Vidrio = %s\r\n"
            "Cantidad de Vidrios = %s\r\n"
            "Ancho de Vidrio Fijo = %s\r\n"
            "Alto de Vidrio Fijo = %s\r\n"
            "Cantidad de Vidrios Fijos = %s",
            s_wc, s_h, s_qc, s_wf, s_hf, s_qf
        );
    } else if (op->glass.is_complex) {
        sprintf(buffer, 
            "Ancho de Vidrio Hoja Central = %s\r\n"
            "Ancho de Vidrios Hojas Laterales = %s\r\n"
            "Alto de Vidrios = %s\r\n"
            "Cantidad de Vidrios Laterales = %s\r\n"
            "Cantidad de vidrios centrales = %s",
            s_wc, s_wl, s_h, s_ql, s_qc
        );
    } else {
        sprintf(buffer, 
            "Ancho de Vidrio = %s\r\n"
            "Alto de Vidrio = %s\r\n"
            "Cantidad de Vidrios = %s",
            s_wc, s_h, s_qc
        );
    }
}

// Bin Packing Helpers
int sort_desc(const void* a, const void* b) {
    double fa = *(const double*)a;
    double fb = *(const double*)b;
    return (fa < fb) - (fa > fb);
}

// Helper to format map
void format_cutting_map(char* buffer, int count, double* pieces, int* assignments, int bin_count, double bar_length) {
    buffer[0] = '\0';
    for (int b = 0; b < bin_count; b++) {
        char line[1024];
        sprintf(line, "Barra %d: [", b+1);
        strcat(buffer, line);
        
        bool first = true;
        double used = 0;
        for (int i = 0; i < count; i++) {
            if (assignments[i] == b) {
                if (!first) strcat(buffer, ", ");
                char p_str[32];
                fmt_num(pieces[i], p_str, sizeof(p_str));
                strcat(buffer, p_str);
                used += pieces[i] + 4; // piece + cut
                first = false;
            }
        }
        char r_str[32];
        fmt_num(bar_length - used, r_str, sizeof(r_str));
        sprintf(line, "] (Resto: %s)\r\n", r_str);
        strcat(buffer, line);
    }
}

// Branch and Bound / DFS Globals
int g_best_solution = 999999;
double* g_pieces_ref = NULL;
int g_count_ref = 0;
double g_bar_len_ref = 0;
double g_slice_ref = 0;
int g_best_assignments[1000];
int g_current_assignments[1000];
long g_nodes_visited = 0;

int greedy_bin_packing(double* pieces, int count, double bar_length, double slice_val, int* assignments_out) {
    double bins[1000]; 
    int bin_count = 0;
    
    // Reset assignments
    if (assignments_out) {
        for(int i=0; i<count; i++) assignments_out[i] = -1;
    }

    for (int i = 0; i < count; i++) {
        bool placed = false;
        double piece_size = pieces[i] + slice_val;
        for (int j = 0; j < bin_count; j++) {
            if (bins[j] >= piece_size) {
                bins[j] -= piece_size;
                if (assignments_out) assignments_out[i] = j;
                placed = true;
                break;
            }
        }
        if (!placed) {
            if (assignments_out) assignments_out[i] = bin_count;
            bins[bin_count++] = bar_length - piece_size;
        }
    }
    return bin_count;
}

void dfs_bnb(int current_piece_idx, double* current_bins, int bin_count) {
    g_nodes_visited++;
    if (g_nodes_visited > 500000) return; // Node limit for safety

    // Pruning 1: If current bins already >= best found, stop.
    if (bin_count >= g_best_solution) return;

    // Base case: All pieces placed
    if (current_piece_idx >= g_count_ref) {
        if (bin_count < g_best_solution) {
            g_best_solution = bin_count;
            // Save assignments
            for(int i=0; i<g_count_ref; i++) g_best_assignments[i] = g_current_assignments[i];
        }
        return;
    }

    // Pruning 2: Theoretical Lower Bound
    // Sum of remaining pieces
    double remaining_sum = 0;
    for (int i = current_piece_idx; i < g_count_ref; i++) {
        remaining_sum += (g_pieces_ref[i] + g_slice_ref);
    }
    
    // Calculate total free space in current bins
    double current_free_space = 0;
    for (int i = 0; i < bin_count; i++) {
        current_free_space += current_bins[i]; // current_bins stores remaining space
    }
    
    double needed_volume = remaining_sum - current_free_space;
    int min_additional = 0;
    if (needed_volume > 0) {
        min_additional = (int)ceil(needed_volume / g_bar_len_ref);
    }
    
    if (bin_count + min_additional >= g_best_solution) return;

    double piece_size = g_pieces_ref[current_piece_idx] + g_slice_ref;

    // Try to place into existing bins
    for (int i = 0; i < bin_count; i++) {
        // Pruning 3: Symmetry Breaking
        bool symmetric = false;
        for (int k = 0; k < i; k++) {
            if (fabs(current_bins[k] - current_bins[i]) < 0.001) {
                symmetric = true;
                break;
            }
        }
        if (symmetric) continue;

        if (current_bins[i] >= piece_size) {
            current_bins[i] -= piece_size;
            g_current_assignments[current_piece_idx] = i;
            dfs_bnb(current_piece_idx + 1, current_bins, bin_count);
            current_bins[i] += piece_size; // Backtrack
            
            if (g_best_solution <= bin_count) return; 
        }
    }

    // Try to place into a new bin
    if (bin_count + 1 < g_best_solution) {
        current_bins[bin_count] = g_bar_len_ref - piece_size;
        g_current_assignments[current_piece_idx] = bin_count;
        dfs_bnb(current_piece_idx + 1, current_bins, bin_count + 1);
    }
}

int calculate_bars_for_group(double* pieces, int count, double bar_length, double slice_val, char* method_out, char* map_out) {
    if (count == 0) return 0;

    // Sort Descending
    qsort(pieces, count, sizeof(double), sort_desc);

    int assignments[1000];

    // Initial Solution: Greedy
    int greedy_res = greedy_bin_packing(pieces, count, bar_length, slice_val, assignments);
    
    // Default to greedy map
    format_cutting_map(map_out, count, pieces, assignments, greedy_res, bar_length);

    // Threshold decision for Super Fast performance
    if (count > 50) {
        strcpy(method_out, "Greedy");
        return greedy_res;
    }

    // Setup for Branch and Bound
    g_best_solution = greedy_res;
    g_nodes_visited = 0;
    // Initialize best assignments with greedy result as baseline
    for(int i=0; i<count; i++) g_best_assignments[i] = assignments[i];
    
    g_pieces_ref = pieces;
    g_count_ref = count;
    g_bar_len_ref = bar_length;
    g_slice_ref = slice_val;

    double bins[100]; 
    dfs_bnb(0, bins, 0);

    // If new solution found, update map
    if (g_best_solution < greedy_res) {
         format_cutting_map(map_out, count, pieces, g_best_assignments, g_best_solution, bar_length);
         strcpy(method_out, "Optimal");
    } else {
         // Even if not better count, we could stick to greedy or use the found one. 
         // Since we init g_best_assignments with greedy, we are safe.
         // But logic above: if dfs_bnb doesn't improve, it might not populate g_best_assignments? 
         // Actually, I added logic: if (bin_count < g_best_solution) update.
         // If equal, it won't update. So we keep greedy assignments if count is same.
         strcpy(method_out, "Optimal (Greedy Fallback)");
         if (greedy_res == g_best_solution) strcpy(method_out, "Optimal");
    }

    return g_best_solution;
}

int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length) {
    (void)default_bar_length;
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
            
            double bar_len_for_f = 6750.0;
            if (strcmp(f->serie, "s20") == 0) {
                bar_len_for_f = 5750.0;
            } else if (strcmp(f->serie, "s25") == 0 || strcmp(f->serie, "s25Reforzada") == 0 ||
                       strcmp(f->serie, "s25TripleRiel") == 0 || strcmp(f->serie, "s25TresHojasEnDosRieles") == 0 ||
                       strcmp(f->serie, "s25ReforzadaTresHojasEnDosRieles") == 0) {
                bar_len_for_f = 5950.0;
            }

            // Group by color, bar length, and profile codes (or spanish_name if no codes)
            int found = -1;
            for (int k = 0; k < group_count; k++) {
                if (strcmp(groups[k].color, f->color) == 0 &&
                    fabs(groups[k].bar_length - bar_len_for_f) < 0.1) {
                    
                    if (f->code_count > 0 && groups[k].code_count == f->code_count) {
                        bool match = true;
                        for (int m = 0; m < f->code_count; m++) {
                            bool code_found = false;
                            for (int n = 0; n < groups[k].code_count; n++) {
                                if (strcmp(groups[k].codes[n].key, f->codes[m].key) == 0 &&
                                    strcmp(groups[k].codes[n].value, f->codes[m].value) == 0) {
                                    code_found = true;
                                    break;
                                }
                            }
                            if (!code_found) { match = false; break; }
                        }
                        if (match) { found = k; break; }
                    } else if (f->code_count == 0 && groups[k].code_count == 0) {
                        if (strcmp(groups[k].spanish_name, f->spanish_name) == 0) {
                            found = k;
                            break;
                        }
                    }
                }
            }

            if (found == -1) {
                if (group_count >= 100) break;
                found = group_count++;
                safe_copy(groups[found].color, f->color, sizeof(groups[found].color));
                groups[found].code_count = f->code_count;
                memcpy(groups[found].codes, f->codes, sizeof(CodeEntry) * f->code_count);
                groups[found].piece_count = 0;
                groups[found].bar_length = bar_len_for_f;
                safe_copy(groups[found].spanish_name, f->spanish_name, sizeof(groups[found].spanish_name));
                safe_copy(groups[found].serie, f->serie, sizeof(groups[found].serie));
            }

            if (f->is_complex) {
                for (int m = 0; m < f->width_quantity && groups[found].piece_count < 1000; m++) {
                    groups[found].pieces[groups[found].piece_count++] = f->length_width;
                }
                for (int m = 0; m < f->height_quantity && groups[found].piece_count < 1000; m++) {
                    groups[found].pieces[groups[found].piece_count++] = f->length_height;
                }
                if (f->fix_width_quantity > 0) {
                    for (int m = 0; m < f->fix_width_quantity && groups[found].piece_count < 1000; m++) {
                        groups[found].pieces[groups[found].piece_count++] = f->length_fix_width;
                    }
                }
                if (f->fix_height_quantity > 0) {
                    for (int m = 0; m < f->fix_height_quantity && groups[found].piece_count < 1000; m++) {
                        groups[found].pieces[groups[found].piece_count++] = f->length_fix_height;
                    }
                }
            } else {
                for (int m = 0; m < f->quantity && groups[found].piece_count < 1000; m++) {
                    groups[found].pieces[groups[found].piece_count++] = f->length;
                }
            }
        }
    }

    int result_count = 0;
    for (int i = 0; i < group_count; i++) {
        char method[32] = "";
        char map[12800] = "";
        int qty = calculate_bars_for_group(groups[i].pieces, groups[i].piece_count, groups[i].bar_length, 4.0, method, map);
        if (qty > 0) {
            result_bars[result_count].quantity = qty;
            safe_copy(result_bars[result_count].name, groups[i].spanish_name, sizeof(result_bars[result_count].name));
            safe_copy(result_bars[result_count].serie, groups[i].serie, sizeof(result_bars[result_count].serie));
            safe_copy(result_bars[result_count].color, groups[i].color, sizeof(result_bars[result_count].color));
            result_bars[result_count].code_count = groups[i].code_count;
            memcpy(result_bars[result_count].codes, groups[i].codes, sizeof(CodeEntry) * groups[i].code_count);
            result_bars[result_count].bar_length = groups[i].bar_length;
            safe_copy(result_bars[result_count].calculation_method, method, sizeof(result_bars[result_count].calculation_method));
            safe_copy(result_bars[result_count].cutting_map, map, sizeof(result_bars[result_count].cutting_map));
            result_count++;
        }
    }

    return result_count;
}
