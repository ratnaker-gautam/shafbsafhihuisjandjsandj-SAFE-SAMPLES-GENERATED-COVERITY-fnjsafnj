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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* token = strtok(temp, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int* error_count, int* warning_count, int* info_count, int depth) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                (*error_count)++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                (*warning_count)++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                (*info_count)++;
            }
        }
        analyze_log_recursive(file, error_count, warning_count, info_count, depth + 1);
    }
}

int main() {
    FILE* file = fopen("sample.log", "r");
    if (!file) {
        printf("Error: Could not open log file\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, &error_count, &warning_count, &info_count, 0);
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}