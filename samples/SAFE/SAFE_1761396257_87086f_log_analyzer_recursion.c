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

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const struct LogEntry* entry, int indent) {
    if (indent < 0 || indent > MAX_LEVELS) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void process_log_recursive(struct LogEntry* logs, int start, int end, int current_level, int* index) {
    if (start > end || current_level < 0 || current_level >= MAX_LEVELS) return;
    if (*index > end) return;
    
    struct LogEntry current = logs[*index];
    int level = parse_log_level(current.level);
    
    if (level < current_level) return;
    
    print_log_entry(&current, current_level);
    (*index)++;
    
    while (*index <= end) {
        struct LogEntry next = logs[*index];
        int next_level = parse_log_level(next.level);
        
        if (next_level < current_level) break;
        
        if (next_level == current_level) {
            print_log_entry(&next, current_level);
            (*index)++;
        } else if (next_level > current_level) {
            process_log_recursive(logs, start, end, current_level + 1, index);
        }
    }
}

int read_log_file(struct LogEntry* logs, int max_entries) {
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int result = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        
        if (result != 3) {
            printf("Invalid format. Use: LEVEL TIMESTAMP MESSAGE\n");
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (timestamp < 0) {
            printf("Timestamp must be non-negative\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Message cannot be empty\n");
            continue;
        }
        
        strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
        logs[count].level[sizeof(logs[count].level) - 1] = '\0';
        logs[count].timestamp = timestamp;
        strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
        logs[count].message[sizeof(logs[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry logs[100];
    int entry_count = read_log_file(logs, 100);
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessed log hierarchy:\n");
    int index = 0;
    process_log_recursive(logs, 0, entry_count - 1, 0, &index);
    
    return 0;
}