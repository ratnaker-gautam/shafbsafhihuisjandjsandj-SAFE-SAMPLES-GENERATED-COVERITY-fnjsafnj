//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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

void find_keyword(LogEntry* entry) {
    static const char* keyword = "failed";
    if (strstr(entry->message, keyword) != NULL) {
        printf("Found '%s' in log: %s\n", keyword, entry->timestamp);
    }
}

int validate_log_entry(LogEntry* entry) {
    if (strlen(entry->timestamp) == 0 || strlen(entry->level) == 0 || strlen(entry->message) == 0) {
        return 0;
    }
    
    if (strlen(entry->timestamp) >= 64 || strlen(entry->level) >= 16 || strlen(entry->message) >= 256) {
        return 0;
    }
    
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if (strcmp(entry->level, valid_levels[i]) == 0) {
            valid = 1;
            break;
        }
    }
    
    return valid;
}

void process_logs(LogEntry* entries, int count, LogProcessor processor) {
    if (entries == NULL || count <= 0 || count > MAX_ENTRIES || processor == NULL) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (validate_log_entry(&entries[i])) {
            processor(&entries[i]);
        }
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
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) {
        return 0;
    }
    
    if (strlen(timestamp) < 63 && strlen(level) < 15 && strlen(message) < 255) {
        strncpy(entry->timestamp, timestamp, 63);
        strncpy(entry->level, level, 15);
        strncpy(entry->message, message, 255);
        entry->timestamp[63] = '\0';
        entry->level[15] = '\0';
        entry->message[255] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
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
    
    LogProcessor processors[] = {count_errors, print_warnings, find_keyword};
    const char* processor_names[] = {"Error Counter", "Warning Printer", "Keyword Finder"};
    int num_processors = sizeof(processors) / sizeof(processors[0]);
    
    printf("\nProcessing logs with %d entries:\n", entry_count);
    
    for (int i = 0; i < num_processors; i++) {
        printf("\n--- %s ---\n", processor_names[i]);
        process_logs(entries, entry_count, processors[i]);
    }
    
    return 0;
}