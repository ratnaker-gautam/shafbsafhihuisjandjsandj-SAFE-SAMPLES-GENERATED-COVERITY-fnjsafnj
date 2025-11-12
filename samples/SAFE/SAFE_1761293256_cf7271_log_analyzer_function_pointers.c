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
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strstr(entry->level, "ERROR") != NULL) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strstr(entry->level, "WARN") != NULL) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void process_log_file(LogProcessor processor) {
    if (processor == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entry)) {
            processor(&entry);
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
}

int main() {
    int choice;
    
    printf("Log Analyzer\n");
    printf("1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    const char* names[] = {"Count", "Errors", "Warnings"};
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Processing mode: %s\n", names[choice - 1]);
    process_log_file(processors[choice - 1]);
    
    return 0;
}