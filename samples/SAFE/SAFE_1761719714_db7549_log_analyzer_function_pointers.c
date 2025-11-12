//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; i < 19; i++) {
        if (entry->timestamp[i] == '\0') break;
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && 
            entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
    }
    
    return 1;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (!entries || count <= 0 || !processor) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2023-10-15-14:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "END", 3) == 0) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessing all entries:\n");
    process_logs(entries, entry_count, count_entries);
    
    printf("\nError entries:\n");
    process_logs(entries, entry_count, show_errors);
    
    printf("\nWarning entries:\n");
    process_logs(entries, entry_count, show_warnings);
    
    return 0;
}