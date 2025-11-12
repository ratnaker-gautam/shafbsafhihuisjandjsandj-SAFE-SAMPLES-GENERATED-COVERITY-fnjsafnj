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
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARNING") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_logs(LogEntry* logs, int count, LogProcessor processor) {
    if (logs == NULL || count <= 0 || count > MAX_ENTRIES || processor == NULL) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        processor(&logs[i]);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) {
        return 0;
    }
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return 0;
    }
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
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

int read_logs_from_input(LogEntry* logs, int max_count) {
    if (logs == NULL || max_count <= 0) {
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (timestamp level message), one per line. Empty line to finish.\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    
    while (count < max_count && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &logs[count])) {
            count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    return count;
}

int main() {
    LogEntry logs[MAX_ENTRIES];
    int log_count = read_logs_from_input(logs, MAX_ENTRIES);
    
    if (log_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessing logs with different analyzers:\n\n");
    
    printf("1. Counting all entries:\n");
    process_logs(logs, log_count, count_entries);
    
    printf("\n2. Showing errors only:\n");
    process_logs(logs, log_count, show_errors);
    
    printf("\n3. Showing warnings only:\n");
    process_logs(logs, log_count, show_warnings);
    
    return 0;
}