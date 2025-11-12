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

void process_log_hierarchy(struct LogEntry* entries, int count, int current, int parent_level, int depth) {
    if (current >= count || depth >= MAX_LEVELS) return;
    
    struct LogEntry* entry = &entries[current];
    int current_level = parse_log_level(entry->level);
    
    if (current_level < 0) return;
    
    if (current_level >= parent_level) {
        print_log_entry(entry, depth);
        process_log_hierarchy(entries, count, current + 1, current_level, depth + 1);
    }
    
    if (current + 1 < count) {
        struct LogEntry* next_entry = &entries[current + 1];
        int next_level = parse_log_level(next_entry->level);
        if (next_level >= 0 && next_level <= parent_level) {
            process_log_hierarchy(entries, count, current + 1, parent_level, depth);
        }
    }
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file) != NULL) {
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
    struct LogEntry entries[1000];
    int entry_count = 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char input[MAX_LINE_LENGTH];
    
    while (entry_count < 1000) {
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strlen(input) >= sizeof(input) - 1) {
            printf("Input too long, skipping\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) break;
        
        if (strlen(input) == 0) continue;
        
        char level[16];
        char message[MAX_LINE_LENGTH];
        int timestamp;
        
        int result = sscanf(input, "%15s %d: %1023[^\n]", level, &timestamp, message);
        if (result == 3) {
            if (parse_log_level(level) >= 0 && timestamp >= 0) {
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                entries[entry_count].timestamp = timestamp;
                entry_count++;
            } else {
                printf("Invalid level or timestamp\n");
            }
        } else {
            printf("Invalid format. Use: LEVEL TIMESTAMP: MESSAGE\n");
        }
    }
    
    if (entry_count >