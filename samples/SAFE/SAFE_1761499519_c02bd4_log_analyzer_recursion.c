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

void analyze_log_recursive(FILE* file, int current_depth, int* level_counts) {
    if (current_depth >= MAX_LEVELS || file == NULL) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    if (strlen(buffer) >= MAX_LINE_LENGTH - 1) return;
    
    int level = parse_log_level(buffer);
    if (level >= 0 && level < MAX_LEVELS) {
        level_counts[level]++;
    }
    
    analyze_log_recursive(file, current_depth + 1, level_counts);
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
    
    int level_counts[MAX_LEVELS] = {0};
    analyze_log_recursive(file, 0, level_counts);
    
    fclose(file);
    
    printf("Log Level Analysis:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO:  %d\n", level_counts[1]);
    printf("WARN:  %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    
    return 0;
}