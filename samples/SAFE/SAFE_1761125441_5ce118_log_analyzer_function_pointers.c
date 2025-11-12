//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN];
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
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 20 || strlen(level) >= 10 || strlen(message) >= MAX_LINE_LEN) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (!entries || !processor || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessing options:\n");
    printf("1. Count all entries\n");
    printf("2. Show only errors\n");
    printf("3. Show only warnings\n");
    printf("Enter choice (1-3): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    const char* descriptions[] = {"Counting entries", "Showing errors", "Showing warnings"};
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    printf("\n%s:\n", descriptions[choice - 1]);
    process_logs(entries, entry_count, processors[choice - 1]);
    
    return 0;
}