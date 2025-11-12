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

int count_logs_by_level_recursive(FILE* file, int target_level, int* counts, long pos) {
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    
    if (fseek(file, pos, SEEK_SET) != 0) {
        return 0;
    }
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    long current_pos = ftell(file);
    if (current_pos == -1L) {
        return 0;
    }
    
    int parsed = sscanf(line, "%31s %15s %[^\n]", entry.timestamp, entry.level, entry.message);
    if (parsed == 3) {
        int level = parse_log_level(entry.level);
        if (level >= 0 && level < 5) {
            counts[level]++;
            if (level == target_level) {
                printf("%s %s %s\n", entry.timestamp, entry.level, entry.message);
            }
        }
    }
    
    return count_logs_by_level_recursive(file, target_level, counts, current_pos) + 1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <logfile> <level>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    const char* target_level_str = argv[2];
    
    int target_level = parse_log_level(target_level_str);
    if (target_level == -1) {
        fprintf(stderr, "Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    int counts[5] = {0};
    printf("Log entries with level %s:\n", target_level_str);
    
    int total_lines = count_logs_by_level_recursive(file, target_level, counts, 0L);
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("Total lines processed: %d\n", total_lines);
    printf("DEBUG: %d\n", counts[0]);
    printf("INFO: %d\n", counts[1]);
    printf("WARNING: %d\n", counts[2]);
    printf("ERROR: %d\n", counts[3]);
    printf("CRITICAL: %d\n", counts[4]);
    
    return 0;
}