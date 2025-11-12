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

void print_log_entry(const struct LogEntry* entry, int indent_level) {
    if (indent_level < 0 || indent_level > MAX_LEVELS) return;
    
    for (int i = 0; i < indent_level * 2; i++) {
        putchar(' ');
    }
    
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int target_level) {
    if (start >= end || current_level > MAX_LEVELS) return;
    
    for (int i = start; i < end; i++) {
        int entry_level = parse_log_level(entries[i].level);
        if (entry_level < 0) continue;
        
        if (entry_level >= target_level) {
            print_log_entry(&entries[i], current_level);
        }
        
        if (i + 1 < end) {
            int next_level = parse_log_level(entries[i + 1].level);
            if (next_level > entry_level) {
                analyze_log_recursive(entries, i + 1, end, current_level + 1, target_level);
            }
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0) continue;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        if (result == 3) {
            if (parse_log_level(level) >= 0 && timestamp >= 0) {
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
    int entry_count;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE):\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter empty line to finish.\n");
    
    entry_count = read_log_entries(entries, 100);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nEnter minimum log level to display (0=DEBUG, 1=INFO, 2=WARNING, 3=ERROR, 4=CRITICAL): ");
    
    int min_level;
    if (scanf("%d", &min_level) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (min_level < 0 || min_level > 4) {
        printf("Invalid log level. Must be between 0 and 4.\n");
        return 1;
    }
    
    printf("\nFiltered log entries (indentation shows hierarchy):\n");
    analyze_log_recursive(entries, 0, entry_count, 0, min_level);
    
    return 0;
}