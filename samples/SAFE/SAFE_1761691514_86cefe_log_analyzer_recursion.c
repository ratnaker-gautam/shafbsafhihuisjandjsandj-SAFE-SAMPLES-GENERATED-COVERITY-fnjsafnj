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
    char message[MAX_LINE_LENGTH];
    int line_number;
};

int parse_log_line(const char* line, struct LogEntry* entry, int line_num) {
    if (line == NULL || entry == NULL || line_num < 0) return 0;
    
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    if (strlen(level) >= 9) return 0;
    if (strlen(message) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    entry->line_number = line_num;
    
    return 1;
}

void analyze_log_recursive(FILE* file, int current_depth, int max_depth, int* error_count, int* warning_count, int* info_count) {
    if (file == NULL || current_depth < 0 || max_depth < 0 || current_depth > max_depth) return;
    if (error_count == NULL || warning_count == NULL || info_count == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    struct LogEntry entry;
    
    if (fgets(line, sizeof(line), file) != NULL) {
        if (parse_log_line(line, &entry, current_depth)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                (*error_count)++;
                printf("Error at line %d: %s\n", entry.line_number, entry.message);
            } else if (strcmp(entry.level, "WARNING") == 0) {
                (*warning_count)++;
                printf("Warning at line %d: %s\n", entry.line_number, entry.message);
            } else if (strcmp(entry.level, "INFO") == 0) {
                (*info_count)++;
                printf("Info at line %d: %s\n", entry.line_number, entry.message);
            }
        }
        
        if (current_depth < max_depth - 1) {
            analyze_log_recursive(file, current_depth + 1, max_depth, error_count, warning_count, info_count);
        }
    }
}

int main(void) {
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARNING High memory usage detected",
        "2024-01-15T10:32:00 INFO User login: john_doe",
        "2024-01-15T10:33:45 ERROR Database connection failed",
        "2024-01-15T10:34:10 INFO Backup process started",
        "2024-01-15T10:35:00 WARNING Disk space below threshold",
        "2024-01-15T10:36:20 ERROR Service unavailable",
        NULL
    };
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file\n");
        return 1;
    }
    
    for (int i = 0; sample_logs[i] != NULL; i++) {
        if (fprintf(temp_file, "%s\n", sample_logs[i]) < 0) {
            fprintf(stderr, "Failed to write to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    if (fseek(temp_file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to rewind temporary file\n");
        fclose(temp_file);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analysis Results:\n");
    printf("=====================\n");
    
    analyze_log_recursive(temp_file, 0, MAX_LEVELS, &error_count, &warning_count, &info_count);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    fclose(temp_file);
    return 0;
}