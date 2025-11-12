//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void print_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void print_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
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
    int line_count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && line_count < MAX_ENTRIES) {
        line_count++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        }
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("Generating sample log data...\n");
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 WARN Disk usage at 85%",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 ERROR Service unavailable",
        "2024-01-15T10:35:00 WARN Memory usage high",
        "2024-01-15T10:36:10 INFO User login successful"
    };
    
    int num_logs = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Error creating temporary file\n");
        return 1;
    }
    
    for (int i = 0; i < num_logs; i++) {
        if (fprintf(temp_file, "%s\n", sample_logs[i]) < 0) {
            printf("Error writing to file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    printf("\nProcessing all entries:\n");
    process_log_file(temp_file, count_entries);
    
    rewind(temp_file);
    printf("\nError entries:\n");
    process_log_file(temp_file, print_errors);
    
    rewind(temp_file);
    printf("\nWarning entries:\n");
    process_log_file(temp_file, print_warnings);
    
    fclose(temp_file);
    
    return 0;
}