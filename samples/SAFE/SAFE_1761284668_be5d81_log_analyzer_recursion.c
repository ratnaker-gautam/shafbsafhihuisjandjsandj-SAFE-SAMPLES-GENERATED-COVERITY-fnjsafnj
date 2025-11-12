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

int parse_log_level(const char* line) {
    if (strncmp(line, "DEBUG", 5) == 0) return 0;
    if (strncmp(line, "INFO", 4) == 0) return 1;
    if (strncmp(line, "WARN", 4) == 0) return 2;
    if (strncmp(line, "ERROR", 5) == 0) return 3;
    if (strncmp(line, "FATAL", 5) == 0) return 4;
    return -1;
}

void analyze_log_recursive(FILE* file, int current_depth, int* level_counts, int* max_depth) {
    if (current_depth >= MAX_LEVELS || file == NULL) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    if (strlen(buffer) >= sizeof(buffer) - 1) {
        while (fgetc(file) != '\n' && !feof(file));
        return;
    }
    
    int level = parse_log_level(buffer);
    if (level >= 0 && level < 5) {
        level_counts[level]++;
    }
    
    if (current_depth > *max_depth) {
        *max_depth = current_depth;
    }
    
    analyze_log_recursive(file, current_depth + 1, level_counts, max_depth);
}

void print_statistics(int* level_counts, int max_depth) {
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-6s: %d entries\n", level_names[i], level_counts[i]);
    }
    
    printf("Maximum depth: %d\n", max_depth);
    printf("Total entries: %d\n", 
        level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3] + level_counts[4]);
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
    int max_depth = 0;
    
    analyze_log_recursive(file, 0, level_counts, &max_depth);
    
    fclose(file);
    
    print_statistics(level_counts, max_depth);
    
    return 0;
}