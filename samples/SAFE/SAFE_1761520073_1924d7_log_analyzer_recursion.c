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

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= 19) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= 9) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    if (error_count == NULL || warning_count == NULL || info_count == NULL) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, MAX_LINE_LENGTH, file) == NULL) return;
    
    LogEntry entry;
    if (parse_log_line(buffer, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
            printf("Depth %d - ERROR: %s\n", depth, entry.message);
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
            printf("Depth %d - WARNING: %s\n", depth, entry.message);
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
            printf("Depth %d - INFO: %s\n", depth, entry.message);
        }
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Starting log analysis...\n");
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    fclose(file);
    
    printf("\nAnalysis Summary:\n");
    printf("ERRORS: %d\n", error_count);
    printf("WARNINGS: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    return 0;
}