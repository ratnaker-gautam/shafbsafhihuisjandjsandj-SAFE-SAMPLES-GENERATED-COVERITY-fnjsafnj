//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_recursive(FILE *file, struct LogEntry *entries, int max_entries, int current_depth) {
    if (current_depth >= max_entries || feof(file)) {
        return 0;
    }

    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
    }

    if (strlen(buffer) == 0 || buffer[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
    
    int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level, message);
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;
        
        return 1 + parse_log_recursive(file, entries, max_entries, current_depth + 1);
    }
    
    return parse_log_recursive(file, entries, max_entries, current_depth);
}

void analyze_logs_recursive(struct LogEntry *entries, int count, int index, int *error_count, int *warning_count, int *info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    analyze_logs_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void display_logs_recursive(struct LogEntry *entries, int count, int index) {
    if (index >= count) {
        return;
    }

    printf("Time: %d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);

    display_logs_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Example: 3600 INFO System started\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'done' on a separate line to finish input\n\n");
    
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (line_count < 100) {
        printf("Log entry %d: ", line_count + 1);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "done") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        struct LogEntry *entry = &entries[line_count];
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(buffer, "%d %15s %1023[^\n]", &timestamp, level, message);
        
        if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
            if (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || strcmp(level, "ERROR") == 0) {
                strncpy(entry->level, level, sizeof(entry->level) - 1);
                entry->level[sizeof(entry->level) - 1] = '\0';
                strncpy(entry->message, message, sizeof(entry->message) - 1);
                entry->message[sizeof(entry->message) - 1] = '\0';
                entry->timestamp = timestamp;
                line_count++;
            } else {
                printf("Invalid log level. Use INFO, WARNING, or ERROR.\n");
            }
        } else {
            printf("Invalid format. Use: timestamp LEVEL message\n");
        }
    }
    
    entry_count = line_count;
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", entry_count);
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_logs_recursive(