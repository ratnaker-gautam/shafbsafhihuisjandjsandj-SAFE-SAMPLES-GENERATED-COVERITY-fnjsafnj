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

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int max_level) {
    if (start >= end || current_level > max_level || current_level < 0) return;
    
    int level_count = 0;
    int error_count = 0;
    
    for (int i = start; i < end; i++) {
        int entry_level = parse_log_level(entries[i].level);
        if (entry_level == current_level) {
            print_log_entry(&entries[i], current_level);
            level_count++;
        }
        if (entry_level >= 3) {
            error_count++;
        }
    }
    
    if (level_count > 0) {
        printf("Found %d entries at level %d\n", level_count, current_level);
    }
    
    analyze_log_recursive(entries, start, end, current_level + 1, max_level);
    
    if (current_level == 0) {
        printf("Total error-level entries: %d\n", error_count);
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (!entries || max_entries <= 0) return 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        if (parsed != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (parse_log_level(level) == -1) {
            printf("Unknown log level: %s. Skipping.\n", level);
            continue;
        }
        
        if (timestamp < 0) {
            printf("Invalid timestamp: %d. Skipping.\n", timestamp);
            continue;
        }
        
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        entries[count].timestamp = timestamp;
        
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = read_log_entries(entries, 100);
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    analyze_log_recursive(entries, 0, entry_count, 0, 4);
    
    return 0;
}