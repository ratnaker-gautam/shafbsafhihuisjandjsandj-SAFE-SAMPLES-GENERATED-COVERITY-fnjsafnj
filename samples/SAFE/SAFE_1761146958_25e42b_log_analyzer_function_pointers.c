//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_errors(LogEntry* entry) {
    static int error_count = 0;
    if (strcmp(entry->level, "ERROR") == 0) {
        error_count++;
        printf("Error #%d: %s - %s\n", error_count, entry->timestamp, entry->message);
    }
}

void find_warnings(LogEntry* entry) {
    static int warning_count = 0;
    if (strcmp(entry->level, "WARN") == 0) {
        warning_count++;
        printf("Warning #%d: %s - %s\n", warning_count, entry->timestamp, entry->message);
    }
}

void print_all(LogEntry* entry) {
    printf("%s [%s] %s\n", entry->timestamp, entry->level, entry->message);
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    int scanned = sscanf(line, "%19s %9s %1023[^\n]", 
                        entry->timestamp, entry->level, entry->message);
    
    if (scanned != 3) return 0;
    
    for (int i = 0; i < 10; i++) {
        if (strcmp(entry->level, "DEBUG") == 0 || 
            strcmp(entry->level, "INFO") == 0 ||
            strcmp(entry->level, "WARN") == 0 ||
            strcmp(entry->level, "ERROR") == 0 ||
            strcmp(entry->level, "FATAL") == 0) {
            return 1;
        }
    }
    
    return 0;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0 || count > MAX_ENTRIES) return;
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (len <= 1) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nChoose processing mode:\n");
    printf("1. Show all logs\n");
    printf("2. Count errors\n");
    printf("3. Find warnings\n");
    printf("Enter choice (1-3): ");
    
    char choice_str[10];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }
    
    int choice = atoi(choice_str);
    LogProcessor processor = NULL;
    
    switch (choice) {
        case 1:
            processor = print_all;
            printf("\nAll log entries:\n");
            break;
        case 2:
            processor = count_errors;
            printf("\nError entries:\n");
            break;
        case 3:
            processor = find_warnings;
            printf("\nWarning entries:\n");
            break;
        default:
            printf("Invalid choice. Showing all logs.\n");
            processor = print_all;
            printf("\nAll log entries:\n");
            break;
    }
    
    process_logs(entries, entry_count, processor);
    
    return 0;
}