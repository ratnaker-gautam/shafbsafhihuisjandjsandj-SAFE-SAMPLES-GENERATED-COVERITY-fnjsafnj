//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
};

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(FILE* file, int current_depth, int* error_count, int* warn_count, int* info_count, int* debug_count) {
    if (current_depth >= MAX_LEVELS || feof(file)) {
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    if (strlen(buffer) == 0) {
        analyze_log_recursive(file, current_depth + 1, error_count, warn_count, info_count, debug_count);
        return;
    }

    int level = parse_log_level(buffer);
    switch (level) {
        case 3: (*error_count)++; break;
        case 2: (*warn_count)++; break;
        case 1: (*info_count)++; break;
        case 0: (*debug_count)++; break;
    }

    analyze_log_recursive(file, current_depth + 1, error_count, warn_count, info_count, debug_count);
}

int main(void) {
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");

    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }

    char input[MAX_LINE_LENGTH];
    int line_count = 0;

    while (line_count < 1000) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) {
            break;
        }

        if (fprintf(temp_file, "%s\n", input) < 0) {
            fprintf(stderr, "Error writing to temporary file\n");
            fclose(temp_file);
            return 1;
        }
        line_count++;
    }

    rewind(temp_file);

    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;

    analyze_log_recursive(temp_file, 0, &error_count, &warn_count, &info_count, &debug_count);

    printf("\nLog Analysis Results:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total: %d\n", error_count + warn_count + info_count + debug_count);

    fclose(temp_file);
    return 0;
}