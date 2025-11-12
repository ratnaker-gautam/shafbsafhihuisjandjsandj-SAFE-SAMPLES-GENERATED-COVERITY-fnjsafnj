//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void count_logs_by_level_recursive(FILE* file, int level_counts[], int current_level) {
    if (current_level >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    char level_str[32];
    char message[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
        int level = parse_log_level(level_str);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }
    
    count_logs_by_level_recursive(file, level_counts, current_level + 1);
}

void print_statistics(int level_counts[]) {
    printf("Log Level Statistics:\n");
    printf("====================\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
    }
    printf("====================\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    int level_counts[5] = {0};
    count_logs_by_level_recursive(file, level_counts, 0);
    
    fclose(file);
    
    print_statistics(level_counts);
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += level_counts[i];
    }
    printf("Total log entries: %d\n", total);
    
    return 0;
}