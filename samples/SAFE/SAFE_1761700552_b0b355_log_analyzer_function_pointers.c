//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
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
    if (strcmp(entry->level, "WARN") == 0) {
        printf("Warning: %s\n", entry->message);
    }
}

void find_keyword(LogEntry* entry) {
    if (strstr(entry->message, "timeout") != NULL) {
        printf("Timeout detected: %s - %s\n", entry->timestamp, entry->message);
    }
}

int validate_log_entry(LogEntry* entry) {
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || strlen(entry->message) == 0) {
        return 0;
    }
    
    for (int i = 0; i < strlen(entry->timestamp); i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && entry->timestamp[i] != ':' && entry->timestamp[i] != ' ') {
            return 0;
        }
    }
    
    if (strcmp(entry->level, "INFO") != 0 && 
        strcmp(entry->level, "WARN") != 0 && 
        strcmp(entry->level, "ERROR") != 0) {
        return 0;
    }
    
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
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
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return validate_log_entry(entry);
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (entries == NULL || processor == NULL || count <= 0 || count > MAX_ENTRIES) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp level message):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Please use: timestamp level message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessing logs...\n\n");
    
    LogProcessor processors[] = {count_errors, print_warnings, find_keyword};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    for (int i = 0; i < num_processors; i++) {
        printf("=== Analysis %d ===\n", i + 1);
        process_logs(entries, entry_count, processors[i]);
        printf("\n");
    }
    
    printf("Total log entries processed: %d\n", entry_count);
    
    return 0;
}