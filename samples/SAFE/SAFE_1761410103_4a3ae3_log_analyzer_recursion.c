//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; entry->timestamp[i]; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(buffer, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
            printf("Error: %s - %s\n", entry.timestamp, entry.message);
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
            printf("Warning: %s - %s\n", entry.timestamp, entry.message);
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
            printf("Info: %s - %s\n", entry.timestamp, entry.message);
        }
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
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
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    return 0;
}