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

void analyze_timestamps(LogEntry* entry) {
    static int entry_count = 0;
    entry_count++;
    printf("Processed %d entries, latest: %s\n", entry_count, entry->timestamp);
}

int validate_log_entry(LogEntry* entry) {
    if (strlen(entry->timestamp) == 0 || strlen(entry->timestamp) >= 64) {
        return 0;
    }
    if (strlen(entry->level) == 0 || strlen(entry->level) >= 16) {
        return 0;
    }
    if (strlen(entry->message) == 0 || strlen(entry->message) >= 256) {
        return 0;
    }
    
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    return valid;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (strlen(timestamp) < 63) {
        strncpy(entry->timestamp, timestamp, 63);
        entry->timestamp[63] = '\0';
    } else {
        return 0;
    }
    
    if (strlen(level) < 15) {
        strncpy(entry->level, level, 15);
        entry->level[15] = '\0';
    } else {
        return 0;
    }
    
    if (strlen(message) < 255) {
        strncpy(entry->message, message, 255);
        entry->message[255] = '\0';
    } else {
        return 0;
    }
    
    return validate_log_entry(entry);
}

void process_logs(LogEntry* entries, int count, LogProcessor* processors, int processor_count) {
    if (entries == NULL || processors == NULL || count <= 0 || processor_count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < processor_count; j++) {
            processors[j](&entries[i]);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started",
        "2024-01-15T10:31:15 WARNING High memory usage",
        "2024-01-15T10:32:00 ERROR Database connection failed",
        "2024-01-15T10:33:45 INFO Backup completed",
        "2024-01-15T10:34:20 ERROR File not found",
        "2024-01-15T10:35:00 DEBUG Processing request",
        "2024-01-15T10:36:10 WARNING Slow response time"
    };
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    LogProcessor processors[] = {count_errors, print_warnings, analyze_timestamps};
    int processor_count = sizeof(processors) / sizeof(processors[0]);
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    process_logs(entries, entry_count, processors, processor_count);
    
    return 0;
}