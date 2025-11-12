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

int parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int level_counts[], int current_depth) {
    if (current_depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    LogEntry entry;
    if (parse_log_line(line, &entry)) {
        int level = parse_log_level(entry.level);
        if (level >= 0 && level < 5) {
            level_counts[level]++;
        }
    }
    
    analyze_log_recursive(file, level_counts, current_depth + 1);
}

int main(void) {
    FILE* file = fopen("sample.log", "r");
    if (!file) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    int level_counts[5] = {0};
    analyze_log_recursive(file, level_counts, 0);
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("DEBUG: %d\n", level_counts[0]);
    printf("INFO:  %d\n", level_counts[1]);
    printf("WARN:  %d\n", level_counts[2]);
    printf("ERROR: %d\n", level_counts[3]);
    printf("FATAL: %d\n", level_counts[4]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3] + level_counts[4];
    printf("Total entries: %d\n", total);
    
    return 0;
}