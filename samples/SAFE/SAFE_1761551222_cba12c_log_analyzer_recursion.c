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

void analyze_log_recursive(LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = start; i < end; i++) {
        if (entries[i].level == current_level) {
            switch (entries[i].level) {
                case 3: error_count++; break;
                case 2: warn_count++; break;
                case 1: info_count++; break;
                case 0: debug_count++; break;
            }
        }
    }
    
    if (error_count > 0 || warn_count > 0 || info_count > 0 || debug_count > 0) {
        printf("Level %d: Errors=%d, Warnings=%d, Info=%d, Debug=%d\n",
               current_level, error_count, warn_count, info_count, debug_count);
    }
    
    analyze_log_recursive(entries, start, end, current_level + 1);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && count < max_entries) {
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strlen(buffer) > 0) {
            strncpy(entries[count].line, buffer, sizeof(entries[count].line) - 1);
            entries[count].line[sizeof(entries[count].line) - 1] = '\0';
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
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    analyze_log_recursive(entries, 0, entry_count, 0);
    
    free(entries);
    return 0;
}