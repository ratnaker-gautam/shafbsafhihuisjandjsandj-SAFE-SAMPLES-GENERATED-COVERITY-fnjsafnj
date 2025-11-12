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

void process_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* index) {
    if (start >= end || *index >= end) return;
    if (current_level < 0 || current_level > MAX_LEVELS) return;
    
    while (*index < end) {
        struct LogEntry* current = &entries[*index];
        int entry_level = parse_log_level(current->level);
        
        if (entry_level < current_level) {
            return;
        }
        
        if (entry_level == current_level) {
            print_log_entry(current, current_level);
            (*index)++;
        } else if (entry_level > current_level) {
            print_log_entry(current, current_level);
            (*index)++;
            process_log_recursive(entries, start, end, current_level + 1, index);
        }
    }
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    if (!file || !entries || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(line, "%15s %d: %1023[^\n]", level, &timestamp, message);
        if (result == 3) {
            if (parse_log_level(level) >= 0) {
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                entries[count].level[sizeof(entries[count].level) - 1] = '\0';
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                entries[count].timestamp = timestamp;
                count++;
            }
        }
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char input[MAX_LINE_LENGTH];
    while (entry_count < 100 && fgets(input, sizeof(input), stdin)) {
        if (strlen(input) >= sizeof(input) - 1) {
            printf("Input too long, skipping\n");
            continue;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(input, "%15s %d: %1023[^\n]", level, &timestamp, message);
        if (result == 3) {
            if (parse_log_level(level) >= 0) {
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                entries[entry_count].timestamp = timestamp;
                entry_count++;
            } else {
                printf("Invalid log level: %s\n", level);
            }
        } else {
            printf("Invalid format. Use: LEVEL TIMESTAMP: MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nProcessing log entries hierarchically:\n");
    int index = 0;
    process_log_recursive(entries,