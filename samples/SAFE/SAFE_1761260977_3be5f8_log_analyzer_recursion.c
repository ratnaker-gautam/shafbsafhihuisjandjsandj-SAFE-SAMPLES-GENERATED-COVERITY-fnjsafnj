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
    if (!line || !entry) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    int timestamp_end = -1;
    int level_end = -1;
    
    for (int i = 0; line[i] && i < 19; i++) {
        if (line[i] == ' ') {
            timestamp_end = i;
            break;
        }
        if (!isdigit(line[i]) && line[i] != '-' && line[i] != ':' && line[i] != '.') {
            return 0;
        }
    }
    
    if (timestamp_end == -1) return 0;
    
    strncpy(entry->timestamp, line, timestamp_end);
    entry->timestamp[timestamp_end] = '\0';
    
    const char* level_start = line + timestamp_end + 1;
    for (int i = 0; level_start[i] && i < 9; i++) {
        if (level_start[i] == ' ') {
            level_end = i;
            break;
        }
        if (!isalpha(level_start[i])) {
            return 0;
        }
    }
    
    if (level_end == -1) return 0;
    
    strncpy(entry->level, level_start, level_end);
    entry->level[level_end] = '\0';
    
    const char* message_start = level_start + level_end + 1;
    if (strlen(message_start) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || !file || !error_count || !warning_count || !info_count) {
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return;
    }
    
    line[strcspn(line, "\n")] = '\0';
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
            printf("ERROR: %s - %s\n", entry.timestamp, entry.message);
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
            printf("WARNING: %s - %s\n", entry.timestamp, entry.message);
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
            printf("INFO: %s - %s\n", entry.timestamp, entry.message);
        }
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
}

int main() {
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    FILE* temp_file = tmpfile();
    if (!temp_file) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    char input_line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strlen(input_line) == 0) {
            break;
        }
        
        if (fprintf(temp_file, "%s\n", input_line) < 0) {
            fprintf(stderr, "Error writing to temporary file\n");
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