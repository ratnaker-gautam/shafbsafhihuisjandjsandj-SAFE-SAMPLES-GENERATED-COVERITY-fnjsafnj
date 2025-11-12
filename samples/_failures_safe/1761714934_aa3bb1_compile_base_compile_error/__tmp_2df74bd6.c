//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
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

    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

    if (strlen(line) == 0 || line[0] == '\n') {
        return parse_log_recursive(file, entries, max_entries, current_depth);
    }

    struct LogEntry *entry = &entries[current_depth];
    char level_buf[16];
    char message_buf[MAX_LINE_LENGTH];
    int timestamp;
    
    int parsed = sscanf(line, "%d %15s %1023[^\n]", &timestamp, level_buf, message_buf);
    
    if (parsed == 3 && timestamp >= 0 && timestamp <= 86400) {
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
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

    printf("Time: %05d | Level: %-7s | Message: %s\n", 
           entries[index].timestamp, entries[index].level, entries[index].message);
    
    display_logs_recursive(entries, count, index + 1);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp LEVEL message)\n");
    printf("Example: 3600 INFO System started\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Timestamp range: 0-86400 (seconds in day)\n");
    printf("Enter empty line to finish input\n\n");
    
    char input[MAX_LINE_LENGTH];
    int line_count = 0;
    
    while (line_count < 100) {
        printf("Log entry %d: ", line_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n' || input[0] == '\0') {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            break;
        }
        
        struct LogEntry *entry = &entries[line_count];
        char level_buf[16];
        char message_buf[MAX_LINE_LENGTH];
        int timestamp;
        
        int parsed = sscanf(input, "%d %15s %1023[^\n]", &timestamp, level_buf, message_buf);
        
        if (parsed != 3) {
            printf("Invalid format. Use: timestamp LEVEL message\n");
            continue;
        }
        
        if (timestamp < 0 || timestamp > 86400) {
            printf("Invalid timestamp. Must be 0-86400\n");
            continue;
        }
        
        if (strcmp(level_buf, "INFO") != 0 && 
            strcmp(level_buf, "WARNING") != 0 && 
            strcmp(level_buf, "ERROR") != 0) {
            printf("Invalid level. Use INFO, WARNING, or ERROR\n");
            continue;
        }
        
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        entry->timestamp = timestamp;
        
        line_count++;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries