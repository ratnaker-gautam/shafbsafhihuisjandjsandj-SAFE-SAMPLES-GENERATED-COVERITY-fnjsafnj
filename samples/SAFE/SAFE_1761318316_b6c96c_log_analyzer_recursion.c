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

void process_log_recursive(struct LogEntry* entries, int count, int current, int depth) {
    if (current >= count || current < 0 || depth >= MAX_LEVELS) return;
    
    struct LogEntry* entry = &entries[current];
    int level = parse_log_level(entry->level);
    
    if (level >= 2) {
        print_log_entry(entry, depth);
    }
    
    if (level == 3 || level == 4) {
        for (int i = current + 1; i < count; i++) {
            struct LogEntry* next = &entries[i];
            int next_level = parse_log_level(next->level);
            if (next_level <= level) break;
            process_log_recursive(entries, count, i, depth + 1);
        }
    }
    
    process_log_recursive(entries, count, current + 1, depth);
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(line, "%15s %d: %1023[^\n]", level, &timestamp, message);
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
    
    printf("Enter log entries (format: LEVEL TIMESTAMP: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < 100 && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) >= sizeof(line) - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(line, "%15s %d: %1023[^\n]", level, &timestamp, message);
        if (result == 3) {
            if (parse_log_level(level) >= 0 && timestamp >= 0) {
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                entries[count].level[sizeof(entries[count].level) - 1] = '\0';
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                entries[count].timestamp = timestamp;
                count++;
            } else {
                printf("Invalid level or timestamp, skipping\n");
            }
        } else {
            printf("Invalid format, skipping\n");
        }
    }
    
    if (count == 0) {
        printf("No valid log entries provided\n");
        return 1;
    }
    
    printf("\nFiltered log entries (WARNING and above with hierarchy):\n");
    process_log_recursive(entries, count, 0, 0);
    
    return 0;
}