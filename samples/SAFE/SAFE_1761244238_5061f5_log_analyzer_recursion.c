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
    if (current_level >= MAX_LEVELS || current_level < 0) return;
    
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

int read_log_file(const char* filename, int level_counts[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    for (int i = 0; i < 5; i++) {
        level_counts[i] = 0;
    }
    
    count_logs_by_level_recursive(file, level_counts, 0);
    
    fclose(file);
    return 1;
}

void generate_sample_log(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    const char* messages[] = {
        "Application started",
        "User logged in",
        "Low disk space",
        "Database connection failed",
        "System shutdown initiated"
    };
    
    for (int i = 0; i < 20; i++) {
        int level = rand() % 5;
        fprintf(file, "%s %s\n", levels[level], messages[level]);
    }
    
    fclose(file);
}

int main() {
    const char* filename = "sample.log";
    int level_counts[5];
    
    printf("Generating sample log file...\n");
    generate_sample_log(filename);
    
    printf("Analyzing log file: %s\n", filename);
    
    if (!read_log_file(filename, level_counts)) {
        return 1;
    }
    
    printf("\nLog Level Summary:\n");
    printf("==================\n");
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d entries\n", level_to_string(i), level_counts[i]);
        total += level_counts[i];
    }
    
    printf("==================\n");
    printf("Total: %d entries\n", total);
    
    return 0;
}