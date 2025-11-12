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

void analyze_logs_recursive(struct LogEntry* logs, int start, int end, int current_level, int max_level) {
    if (start >= end || current_level > max_level || current_level < 0) return;
    if (start < 0 || end < 0 || start >= MAX_LEVELS || end >= MAX_LEVELS) return;
    
    int level_count[5] = {0};
    
    for (int i = start; i < end; i++) {
        int level = parse_log_level(logs[i].level);
        if (level >= 0 && level < 5) {
            level_count[level]++;
        }
    }
    
    printf("Level %d Analysis (%d-%d):\n", current_level, start, end);
    printf("  DEBUG: %d\n", level_count[0]);
    printf("  INFO: %d\n", level_count[1]);
    printf("  WARNING: %d\n", level_count[2]);
    printf("  ERROR: %d\n", level_count[3]);
    printf("  CRITICAL: %d\n", level_count[4]);
    
    if (current_level < max_level) {
        int mid = start + (end - start) / 2;
        if (mid > start && mid < end) {
            analyze_logs_recursive(logs, start, mid, current_level + 1, max_level);
            analyze_logs_recursive(logs, mid, end, current_level + 1, max_level);
        }
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    if (!file || !entry) return 0;
    
    char line[MAX_LINE_LENGTH];
    if (!fgets(line, sizeof(line), file)) return 0;
    
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
    
    int result = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
    if (result != 3) return 0;
    
    if (parse_log_level(level) < 0) return 0;
    if (timestamp < 0) return 0;
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->timestamp = timestamp;
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_LEVELS];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE):\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n");
    
    while (count < MAX_LEVELS) {
        printf("Log entry %d: ", count + 1);
        
        struct LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[16];
            if (fgets(buffer, sizeof(buffer), stdin)) {
                if (strncmp(buffer, "END", 3) == 0) break;
            }
            printf("Invalid format. Please use: LEVEL TIMESTAMP MESSAGE\n");
            continue;
        }
        
        logs[count] = entry;
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n=== Log Analysis ===\n");
    analyze_logs_recursive(logs, 0, count, 0, 3);
    
    printf("\n=== All Log Entries ===\n");
    for (int i = 0; i < count; i++) {
        print_log_entry(&logs[i], 0);
    }
    
    return 0;
}