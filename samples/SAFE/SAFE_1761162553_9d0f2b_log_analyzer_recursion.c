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
    struct LogEntry* next;
};

int parse_log_level(const char* line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void process_log_recursive(FILE* file, int current_depth, int* error_count, int* warn_count, int* info_count, int* debug_count) {
    if (current_depth >= MAX_LEVELS || feof(file)) {
        return;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    int level = parse_log_level(buffer);
    switch (level) {
        case 3: (*error_count)++; break;
        case 2: (*warn_count)++; break;
        case 1: (*info_count)++; break;
        case 0: (*debug_count)++; break;
    }

    process_log_recursive(file, current_depth + 1, error_count, warn_count, info_count, debug_count);
}

int validate_filename(const char* filename) {
    if (filename == NULL || strlen(filename) == 0 || strlen(filename) > 255) {
        return 0;
    }
    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum(filename[i]) && filename[i] != '.' && filename[i] != '-' && filename[i] != '_') {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    if (!validate_filename(argv[1])) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }

    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;

    process_log_recursive(file, 0, &error_count, &warn_count, &info_count, &debug_count);

    fclose(file);

    printf("Log Analysis Results:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total: %d\n", error_count + warn_count + info_count + debug_count);

    return 0;
}