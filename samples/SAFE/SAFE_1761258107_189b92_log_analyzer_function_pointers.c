//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LOG_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*, int);

void count_errors(LogEntry* entries, int count) {
    int error_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
    }
    printf("Total ERROR entries: %d\n", error_count);
}

void list_warnings(LogEntry* entries, int count) {
    printf("WARNING entries:\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "WARNING") == 0) {
            printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

void show_all_entries(LogEntry* entries, int count) {
    printf("All log entries:\n");
    for (int i = 0; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0) return;
    processor(entries, count);
}

int main() {
    LogEntry entries[MAX_LOG_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_LOG_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
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
    
    LogProcessor processors[] = {
        count_errors,
        list_warnings,
        show_all_entries
    };
    
    const char* processor_names[] = {
        "Count ERROR entries",
        "List WARNING entries", 
        "Show all entries"
    };
    
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nAvailable analysis options:\n");
    for (int i = 0; i < num_processors; i++) {
        printf("%d. %s\n", i + 1, processor_names[i]);
    }
    
    printf("Select analysis (1-%d): ", num_processors);
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_processors) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("\n");
    process_logs(entries, entry_count, processors[choice - 1]);
    
    return 0;
}