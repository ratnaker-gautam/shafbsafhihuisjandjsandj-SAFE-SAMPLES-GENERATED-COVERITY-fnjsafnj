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
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19 && strlen(level) < 10) {
        strncpy(entry->timestamp, timestamp, 19);
        entry->timestamp[19] = '\0';
        strncpy(entry->level, level, 9);
        entry->level[9] = '\0';
        strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
        entry->message[MAX_LINE_LENGTH - 1] = '\0';
        return 1;
    }
    
    return 0;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || !file || !error_count || !warning_count || !info_count) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, MAX_LINE_LENGTH, file)) {
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                (*error_count)++;
                printf("Error: %s\n", entry.message);
            } else if (strcmp(entry.level, "WARNING") == 0) {
                (*warning_count)++;
                printf("Warning: %s\n", entry.message);
            } else if (strcmp(entry.level, "INFO") == 0) {
                (*info_count)++;
                printf("Info: %s\n", entry.message);
            }
        }
        analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
    }
}

int main() {
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        printf("Error creating temporary file\n");
        return 1;
    }
    
    char input[MAX_LINE_LENGTH];
    while (1) {
        if (!fgets(input, MAX_LINE_LENGTH, stdin)) break;
        
        if (input[0] == '\n' || input[0] == '\0') break;
        
        if (fputs(input, temp_file) == EOF) {
            printf("Error writing to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("\nAnalysis Results:\n");
    analyze_log_recursive(temp_file, 0, &error_count, &warning_count, &info_count);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info messages: %d\n", info_count);
    
    fclose(temp_file);
    return 0;
}