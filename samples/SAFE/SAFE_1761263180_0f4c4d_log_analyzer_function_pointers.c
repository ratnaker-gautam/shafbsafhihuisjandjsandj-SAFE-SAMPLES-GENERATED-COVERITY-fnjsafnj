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
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries so far...\n", count);
}

void find_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void find_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING found: %s - %s\n", entry->timestamp, entry->message);
    }
}

void parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp && level && message) {
        if (strlen(timestamp) < sizeof(entry->timestamp) &&
            strlen(level) < sizeof(entry->level) &&
            strlen(message) < sizeof(entry->message)) {
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            
            entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
            entry->level[sizeof(entry->level) - 1] = '\0';
            entry->message[sizeof(entry->message) - 1] = '\0';
        }
    }
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int read_log_file(LogEntry entries[]) {
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) > 0) {
            parse_log_line(line, &entries[entry_count]);
            entry_count++;
        }
    }
    
    return entry_count;
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(entries);
    
    if (entry_count == 0) {
        printf("No log entries to process.\n");
        return 1;
    }
    
    printf("\n=== Processing Logs ===\n");
    
    LogProcessor processors[] = {count_entries, find_errors, find_warnings};
    const char* processor_names[] = {"Counting entries", "Finding errors", "Finding warnings"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        process_logs(entries, entry_count, processors[i]);
    }
    
    printf("\nLog analysis complete. Processed %d entries.\n", entry_count);
    return 0;
}