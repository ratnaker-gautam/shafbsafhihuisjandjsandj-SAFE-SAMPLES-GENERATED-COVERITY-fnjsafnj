//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void process_info(LogEntry* entry) {
    if (entry == NULL) return;
    printf("INFO: %s - %s\n", entry->timestamp, entry->message);
}

void process_warning(LogEntry* entry) {
    if (entry == NULL) return;
    printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
}

void process_error(LogEntry* entry) {
    if (entry == NULL) return;
    printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
}

void process_unknown(LogEntry* entry) {
    if (entry == NULL) return;
    printf("UNKNOWN: %s - %s\n", entry->timestamp, entry->message);
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char timestamp[64] = {0};
    char level[16] = {0};
    char message[256] = {0};
    
    int result = sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    if (strlen(timestamp) > 0 && strlen(level) > 0 && strlen(message) > 0) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        entry->level[sizeof(entry->level) - 1] = '\0';
        entry->message[sizeof(entry->message) - 1] = '\0';
        return 1;
    }
    
    return 0;
}

LogProcessor get_processor(const char* level) {
    if (level == NULL) return process_unknown;
    
    if (strcmp(level, "INFO") == 0) return process_info;
    if (strcmp(level, "WARNING") == 0) return process_warning;
    if (strcmp(level, "ERROR") == 0) return process_error;
    
    return process_unknown;
}

void generate_sample_logs(void) {
    printf("2024-01-15T10:30:00 INFO System started successfully\n");
    printf("2024-01-15T10:31:15 WARNING High memory usage detected\n");
    printf("2024-01-15T10:32:30 ERROR Database connection failed\n");
    printf("2024-01-15T10:33:45 INFO Backup completed\n");
    printf("2024-01-15T10:34:20 ERROR File not found\n");
    printf("2024-01-15T10:35:10 WARNING Disk space low\n");
    printf("2024-01-15T10:36:25 INFO User login successful\n");
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Generating sample log entries...\n\n");
    generate_sample_logs();
    printf("\nProcessing log entries:\n\n");
    
    for (int i = 0; i < 7; i++) {
        switch (i) {
            case 0: strncpy(line, "2024-01-15T10:30:00 INFO System started successfully", sizeof(line) - 1); break;
            case 1: strncpy(line, "2024-01-15T10:31:15 WARNING High memory usage detected", sizeof(line) - 1); break;
            case 2: strncpy(line, "2024-01-15T10:32:30 ERROR Database connection failed", sizeof(line) - 1); break;
            case 3: strncpy(line, "2024-01-15T10:33:45 INFO Backup completed", sizeof(line) - 1); break;
            case 4: strncpy(line, "2024-01-15T10:34:20 ERROR File not found", sizeof(line) - 1); break;
            case 5: strncpy(line, "2024-01-15T10:35:10 WARNING Disk space low", sizeof(line) - 1); break;
            case 6: strncpy(line, "2024-01-15T10:36:25 INFO User login successful", sizeof(line) - 1); break;
        }
        line[sizeof(line) - 1] = '\0';
        
        if (entry_count < MAX_ENTRIES) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            }
        }
    }
    
    for (int i = 0; i < entry_count; i++) {
        LogProcessor processor = get_processor(entries[i].level);
        if (processor != NULL) {