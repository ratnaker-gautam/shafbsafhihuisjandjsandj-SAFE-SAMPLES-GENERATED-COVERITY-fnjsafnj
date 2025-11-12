//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error count: %d\n", error_count);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Warning: %s\n", entry->message);
    }
}

void find_keyword(LogEntry* entry) {
    static const char* keyword = "failed";
    if (strstr(entry->message, keyword) != NULL) {
        printf("Found '%s' in: %s\n", keyword, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void process_log_file(FILE* file, LogProcessor processor) {
    if (file == NULL || processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
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
    printf("Log Analyzer - Processing sample log data\n\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:35:22 WARNING High memory usage detected",
        "2024-01-15T10:40:15 ERROR Database connection failed",
        "2024-01-15T10:45:30 INFO Backup completed",
        "2024-01-15T10:50:05 WARNING Disk space low",
        "2024-01-15T10:55:40 ERROR Authentication failed for user",
        "2024-01-15T11:00:00 INFO Scheduled task executed",
        NULL
    };
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        return 1;
    }
    
    for (int i = 0; sample_logs[i] != NULL; i++) {
        fprintf(temp_file, "%s\n", sample_logs[i]);
    }
    
    rewind(temp_file);
    
    LogProcessor processors[] = {count_errors, print_warnings, find_keyword};
    const char* processor_names[] = {"Error Counter", "Warning Display", "Keyword Search"};
    
    for (int i = 0; i < 3; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        rewind(temp_file);
        process_log_file(temp_file, processors[i]);
    }
    
    fclose(temp_file);
    
    return 0;
}