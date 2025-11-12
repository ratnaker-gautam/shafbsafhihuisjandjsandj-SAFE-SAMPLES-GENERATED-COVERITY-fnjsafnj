//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char line[MAX_LINE_LENGTH];
    int level;
} LogEntry;

int parse_log_level(const char* line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void analyze_log_recursive(LogEntry* entries, int start, int end, int current_depth) {
    if (current_depth >= MAX_LEVELS || start > end) {
        return;
    }

    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;

    for (int i = start; i <= end; i++) {
        if (entries[i].level == 3) error_count++;
        else if (entries[i].level == 2) warn_count++;
        else if (entries[i].level == 1) info_count++;
        else if (entries[i].level == 0) debug_count++;
    }

    for (int i = 0; i < current_depth; i++) {
        printf("  ");
    }
    printf("Depth %d: Errors=%d, Warnings=%d, Info=%d, Debug=%d\n",
           current_depth, error_count, warn_count, info_count, debug_count);

    if (end - start < 2) {
        return;
    }

    int mid = start + (end - start) / 2;
    if (mid > start && mid < end) {
        analyze_log_recursive(entries, start, mid, current_depth + 1);
        analyze_log_recursive(entries, mid + 1, end, current_depth + 1);
    }
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) && count < max_entries) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (len > 1) {
            strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
            entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
            entries[count].level = parse_log_level(buffer);
            count++;
        }
    }

    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    const int max_entries = 1000;
    LogEntry* entries = malloc(max_entries * sizeof(LogEntry));
    if (!entries) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    int entry_count = read_log_file(argv[1], entries, max_entries);
    if (entry_count < 0) {
        fprintf(stderr, "Failed to read log file: %s\n", argv[1]);
        free(entries);
        return 1;
    }

    if (entry_count == 0) {
        printf("No log entries found\n");
        free(entries);
        return 0;
    }

    printf("Log Analysis Summary:\n");
    analyze_log_recursive(entries, 0, entry_count - 1, 0);

    free(entries);
    return 0;
}