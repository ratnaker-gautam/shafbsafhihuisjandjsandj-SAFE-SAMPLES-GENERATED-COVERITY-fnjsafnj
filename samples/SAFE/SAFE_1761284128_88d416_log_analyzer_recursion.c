//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARN") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "FATAL") == 0) return 4;
    return -1;
}

void analyze_log_recursive(FILE* file, int current_depth, int* level_counts, int* total_lines) {
    if (current_depth >= MAX_LEVELS || file == NULL) {
        return;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return;
    }

    (*total_lines)++;

    LogEntry entry;
    int parsed = sscanf(line, "%19s %9s %[^\n]", 
                       entry.timestamp, entry.level, entry.message);
    
    if (parsed >= 2) {
        int level = parse_log_level(entry.level);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }

    analyze_log_recursive(file, current_depth + 1, level_counts, total_lines);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    int level_counts[5] = {0};
    int total_lines = 0;

    analyze_log_recursive(file, 0, level_counts, &total_lines);

    fclose(file);

    if (total_lines == 0) {
        printf("No log entries found.\n");
        return 0;
    }

    printf("Log Analysis Results:\n");
    printf("Total lines: %d\n", total_lines);
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO: %d\n", level_counts[1]);
    printf("WARN: %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("FATAL: %d\n", level_counts[4]);

    return 0;
}