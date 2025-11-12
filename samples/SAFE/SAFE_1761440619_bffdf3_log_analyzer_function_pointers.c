//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void find_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strstr(entry->level, "ERROR") != NULL) {
        error_count++;
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    static int warning_count = 0;
    if (strstr(entry->level, "WARN") != NULL) {
        warning_count++;
        printf("Warning #%d: %s - %s\n", warning_count, entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len == 0 || len >= MAX_LINE_LENGTH) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line[MAX_LINE_LENGTH - 1] = '\0';
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("============\n\n");
    
    printf("Sample log data:\n");
    printf("2024-01-15T10:30:00 INFO System started\n");
    printf("2024-01-15T10:31:15 WARN High memory usage\n");
    printf("2024-01-15T10:32:00 ERROR Database connection failed\n");
    printf("2024-01-15T10:33:45 INFO Backup completed\n");
    printf("2024-01-15T10:34:20 WARN Disk space low\n");
    printf("2024-01-15T10:35:00 ERROR Service unavailable\n");
    printf("\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        return 1;
    }
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started",
        "2024-01-15T10:31:15 WARN High memory usage",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 WARN Disk space low",
        "2024-01-15T10:35:00 ERROR Service unavailable",
        NULL
    };
    
    for (int i = 0; sample_logs[i] != NULL; i++) {
        fprintf(temp_file, "%s\n", sample_logs[i]);
    }
    
    rewind(temp_file);
    
    LogProcessor processors[] = {count_entries, find_errors, show_warnings};
    const char* processor_names[] = {"Counting entries", "Finding errors", "Showing warnings"};
    
    for (int i = 0; i < 3; i++) {
        printf("%s:\n", processor_names[i]);
        printf("--------------------\n");
        rewind(temp_file);
        process_log_file(temp_file, processors[i]);
        printf("\n");
    }
    
    fclose(temp_file);
    
    return 0;
}