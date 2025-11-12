//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[64];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void process_error(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("CRITICAL: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_warning(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("Attention: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_info(LogEntry* entry) {
    if (strcmp(entry->level, "INFO") == 0) {
        printf("Note: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 64 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry* entries, int count, LogProcessor* processors, int processor_count) {
    if (!entries || !processors || count <= 0 || processor_count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < processor_count; j++) {
            processors[j](&entries[i]);
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
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
    
    LogProcessor processors[] = {process_error, process_warning, process_info};
    int processor_count = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing results:\n");
    process_logs(entries, entry_count, processors, processor_count);
    
    printf("\nSummary: Processed %d log entries\n", entry_count);
    
    return 0;
}