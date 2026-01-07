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
    strncpy(f->serie, serie, 49);
    strncpy(f->name, name, 99);
    strncpy(f->spanish_name, spanish_name, 99);
    strncpy(f->color, color, 49);
    f->length = length;
    f->quantity = quantity;
    f->half = length / 2.0;
}

void calculate_pieces(Opening* opening) {
    double w = opening->width;
    double h = opening->height;
    int q = opening->quantity;

    opening->frame_count = 0;

    if (strcmp(opening->serie, "s20") == 0) {
        init_frame(&opening->frames[0], opening->serie, "Horizontal Frame", "Horizontal de Marco", opening->color, w - 25, q * 2);
        add_code(&opening->frames[0], "abasur", "N1749");
        add_code(&opening->frames[0], "urualum", "190");

        init_frame(&opening->frames[1], opening->serie, "Vertical Frame", "Lateral de Marco", opening->color, h, q * 2);
        add_code(&opening->frames[1], "abasur", "N1753");
        add_code(&opening->frames[1], "urualum", "191");

        init_frame(&opening->frames[2], opening->serie, "Lateral Shash", "Lateral de Hoja", opening->color, h - 48, q * 2);
        add_code(&opening->frames[2], "abasur", "N1751");
        add_code(&opening->frames[2], "urualum", "193");

        init_frame(&opening->frames[3], opening->serie, "Central Shash", "Enganche", opening->color, h - 48, q * 2);
        add_code(&opening->frames[3], "abasur", "E2864");
        add_code(&opening->frames[3], "urualum", "189");

        init_frame(&opening->frames[4], opening->serie, "Horizontal Shash", "Horizontal de Hoja", opening->color, w / 2.0 - 74, q * 4);
        add_code(&opening->frames[4], "abasur", "N1752");
        add_code(&opening->frames[4], "urualum", "192");

        // Screen Shash - complex
        init_frame(&opening->frames[5], opening->serie, "Screen Shash", "Hoja de Mosquitero", opening->color, 0, 0);
        opening->frames[5].length_width = w / 2.0 + 10;
        opening->frames[5].length_height = h - 30;
        opening->frames[5].width_quantity = q * 2;
        opening->frames[5].height_quantity = q * 2;
        add_code(&opening->frames[5], "abasur", "E4436");
        add_code(&opening->frames[5], "urualum", "2314");

        init_frame(&opening->frames[6], opening->serie, "Screen Guide S20", "Guía de Mosquitero", opening->color, w - 5, q * 2);
        add_code(&opening->frames[6], "abasur", "E4821");
        add_code(&opening->frames[6], "urualum", "213");

        opening->frame_count = 7;
    }
    // TODO: Add other series (s25, probba, gala) if needed. 
    // For now, focusing on s20 as a prototype.
}

// Minimal memoization for optimal bin packing
typedef struct {
    int index;
    double remains[MAX_BARS];
    int remains_count;
    int result;
} MemoEntry;

#define MEMO_SIZE 10000
MemoEntry memo[MEMO_SIZE];
int memo_count = 0;

int find_memo(int index, double* remains, int remains_count) {
    // Very simple linear search for demo, could be hashed
    for (int i = 0; i < memo_count; i++) {
        if (memo[i].index == index && memo[i].remains_count == remains_count) {
            bool match = true;
            for (int k = 0; k < remains_count; k++) {
                if (fabs(memo[i].remains[k] - remains[k]) > 0.001) {
                    match = false;
                    break;
                }
            }
            if (match) return memo[i].result;
        }
    }
    return -1;
}

void add_memo(int index, double* remains, int remains_count, int result) {
    if (memo_count < MEMO_SIZE) {
        memo[memo_count].index = index;
        memo[memo_count].remains_count = remains_count;
        memo[memo_count].result = result;
        memcpy(memo[memo_count].remains, remains, sizeof(double) * remains_count);
        memo_count++;
    }
}

int sort_desc(const void* a, const void* b) {
    double fa = *(const double*)a;
    double fb = *(const double*)b;
    return (fa < fb) - (fa > fb);
}

int optimal_helper(double* pieces, int piece_count, int index, double* remains, int remains_count, double bar_length, double slice_val) {
    if (index >= piece_count) return 0;

    // Filter remains to only those that can fit at least the smallest remaining piece
    // (This is an optimization from the Python code)
    double min_piece = pieces[piece_count - 1]; // sorted desc, so last is min
    double filtered_remains[MAX_BARS];
    int filtered_count = 0;
    for (int i = 0; i < remains_count; i++) {
        if (remains[i] >= min_piece + slice_val) {
            filtered_remains[filtered_count++] = remains[i];
        }
    }
    // Sort filtered remains to improve memoization hits (canonical state)
    qsort(filtered_remains, filtered_count, sizeof(double), sort_desc);

    int cached = find_memo(index, filtered_remains, filtered_count);
    if (cached != -1) return cached;

    int min_bars = 999999;
    double piece = pieces[index];

    // Try to put in existing bin
    for (int i = 0; i < filtered_count; i++) {
        if (filtered_remains[i] >= piece + slice_val) {
            double next_remains[MAX_BARS];
            memcpy(next_remains, filtered_remains, sizeof(double) * filtered_count);
            next_remains[i] -= (piece + slice_val);
            
            // Re-add non-filtered remains if any? Actually the helper in Python 
            // only passes the filtered ones to the key, but maintains the rest in the state.
            // Let's stick to the Python logic: key uses filtered, but call uses full remains.
            
            // Wait, if we use filtered for key, we must use full for the next call to be consistent,
            // or just use filtered and add one bar if we can't fit.
            
            // Python: res = helper(index + 1, new_remains)
            // It modifies the list in place (or copy).
            
            int res = optimal_helper(pieces, piece_count, index + 1, next_remains, filtered_count, bar_length, slice_val);
            if (res < min_bars) min_bars = res;
        }
    }

    // Try to start new bin
    double next_remains_new[MAX_BARS];
    memcpy(next_remains_new, filtered_remains, sizeof(double) * filtered_count);
    next_remains_new[filtered_count] = bar_length - (piece + slice_val);
    int res_new = 1 + optimal_helper(pieces, piece_count, index + 1, next_remains_new, filtered_count + 1, bar_length, slice_val);
    if (res_new < min_bars) min_bars = res_new;

    add_memo(index, filtered_remains, filtered_count, min_bars);
    return min_bars;
}

int calculate_bars_for_group(double* pieces, int count, double bar_length, double slice_val) {
    if (count == 0) return 0;
    qsort(pieces, count, sizeof(double), sort_desc);
    memo_count = 0; // Clear memo for new group
    double empty_remains[MAX_BARS];
    return optimal_helper(pieces, count, 0, empty_remains, 0, bar_length, slice_val);
}

// Logic for calculating materials across multiple openings
int calculate_materials(Opening* openings, int opening_count, Bar* result_bars, double default_bar_length) {
    // This is a simplified version of classify_frames + calculate_frame_bars
    // For now, it groups by name and color (in a real app, use codes)
    
    // Grouping structure
    typedef struct {
        char name[100];
        char color[50];
        double pieces[500];
        int piece_count;
        double bar_length;
        Frame ref_frame;
    } MaterialGroup;

    MaterialGroup groups[50];
    int group_count = 0;

    for (int i = 0; i < opening_count; i++) {
        calculate_pieces(&openings[i]);
        for (int j = 0; j < openings[i].frame_count; j++) {
            Frame* f = &openings[i].frames[j];
            
            // Find group
            int found = -1;
            for (int k = 0; k < group_count; k++) {
                if (strcmp(groups[k].name, f->name) == 0 && strcmp(groups[k].color, f->color) == 0) {
                    found = k;
                    break;
                }
            }

            if (found == -1) {
                found = group_count++;
                strncpy(groups[found].name, f->name, 99);
                strncpy(groups[found].color, f->color, 49);
                groups[found].piece_count = 0;
                groups[found].bar_length = default_bar_length; // TODO: custom length
                groups[found].ref_frame = *f;
            }

            if (strcmp(f->name, "Screen Shash") == 0 || strcmp(f->name, "U Dvh") == 0) {
                for (int m = 0; m < (int)f->width_quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length_width;
                for (int m = 0; m < (int)f->height_quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length_height;
            } else {
                for (int m = 0; m < f->quantity; m++) groups[found].pieces[groups[found].piece_count++] = f->length;
            }
        }
    }

    int bar_result_count = 0;
    for (int i = 0; i < group_count; i++) {
        int qty = calculate_bars_for_group(groups[i].pieces, groups[i].piece_count, groups[i].bar_length, 4.0);
        if (qty > 0) {
            result_bars[bar_result_count].quantity = qty;
            strncpy(result_bars[bar_result_count].name, groups[i].ref_frame.spanish_name, 99);
            strncpy(result_bars[bar_result_count].serie, groups[i].ref_frame.serie, 49);
            strncpy(result_bars[bar_result_count].color, groups[i].ref_frame.color, 49);
            memcpy(result_bars[bar_result_count].codes, groups[i].ref_frame.codes, sizeof(CodeEntry) * MAX_CODE_ENTRIES);
            result_bars[bar_result_count].code_count = groups[i].ref_frame.code_count;
            bar_result_count++;
        }
    }

    return bar_result_count;
}
