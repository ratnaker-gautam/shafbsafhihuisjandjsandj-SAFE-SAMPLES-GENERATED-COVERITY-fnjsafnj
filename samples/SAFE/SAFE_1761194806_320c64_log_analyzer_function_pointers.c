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

void analyze_timestamps(LogEntry* entry) {
    static time_t first_time = 0;
    static time_t last_time = 0;
    
    struct tm tm;
    if (strptime(entry->timestamp, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        time_t current = mktime(&tm);
        if (first_time == 0) {
            first_time = current;
        }
        last_time = current;
        
        if (last_time > first_time) {
            double duration = difftime(last_time, first_time);
            printf("Log duration: %.0f seconds\n", duration);
        }
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level[16];
    char timestamp[64];
    char message[256];
    
    if (sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
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

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_errors, print_warnings, analyze_timestamps};
    int processor_count = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nLog Analysis Results:\n");
    process_logs(entries, entry_count, processors, processor_count);
    
    return 0;
}