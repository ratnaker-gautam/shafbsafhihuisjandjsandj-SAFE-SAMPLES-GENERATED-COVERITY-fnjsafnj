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
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int count, int current, int* level_counts) {
    if (current >= count || current < 0) return;
    
    if (entries[current].level >= 0 && entries[current].level < 4) {
        level_counts[entries[current].level]++;
    }
    
    analyze_log_recursive(entries, count, current + 1, level_counts);
}

struct LogEntry* read_log_file(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    struct LogEntry* entries = malloc(MAX_LEVELS * sizeof(struct LogEntry));
    if (!entries) {
        fclose(file);
        return NULL;
    }
    
    char buffer[MAX_LINE_LENGTH];
    *count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && *count < MAX_LEVELS) {
        if (strlen(buffer) > 0) {
            strncpy(entries[*count].line, buffer, sizeof(entries[*count].line) - 1);
            entries[*count].line[sizeof(entries[*count].line) - 1] = '\0';
            entries[*count].level = parse_log_level(buffer);
            (*count)++;
        }
    }
    
    fclose(file);
    return entries;
}

void print_statistics(int* level_counts) {
    const char* levels[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    printf("Log Level Statistics:\n");
    printf("=====================\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%-6s: %d entries\n", levels[i], level_counts[i]);
    }
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    printf("Total : %d entries\n", total);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    int count;
    struct LogEntry* entries = read_log_file(argv[1], &count);
    
    if (!entries) {
        printf("Error: Could not read log file '%s'\n", argv[1]);
        return 1;
    }
    
    if (count == 0) {
        printf("No log entries found in file '%s'\n", argv[1]);
        free(entries);
        return 1;
    }
    
    int level_counts[4] = {0};
    analyze_log_recursive(entries, count, 0, level_counts);
    
    print_statistics(level_counts);
    
    free(entries);
    return 0;
}