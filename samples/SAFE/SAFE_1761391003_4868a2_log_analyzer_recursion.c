//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

int count_logs_by_level_recursive(FILE* file, int level_counts[], int current_level) {
    char line[MAX_LINE_LENGTH];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    LogEntry entry;
    int scanned = sscanf(line, "%31s %15s %1023[^\n]", 
                        entry.timestamp, entry.level, entry.message);
    
    if (scanned == 3) {
        int level = parse_log_level(entry.level);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }
    
    return count_logs_by_level_recursive(file, level_counts, current_level + 1) + 1;
}

void print_level_stats(int level_counts[]) {
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    printf("Log Level Statistics:\n");
    printf("=====================\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_names[i], level_counts[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    int level_counts[5] = {0};
    
    printf("Analyzing log file: %s\n", argv[1]);
    int total_lines = count_logs_by_level_recursive(file, level_counts, 0);
    
    fclose(file);
    
    printf("Total lines processed: %d\n", total_lines);
    print_level_stats(level_counts);
    
    return 0;
}