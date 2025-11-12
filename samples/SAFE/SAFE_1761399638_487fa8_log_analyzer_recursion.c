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
    if (entry == NULL || indent < 0 || indent > MAX_LEVELS) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void process_log_hierarchy(struct LogEntry* entries, int count, int current_index, int parent_level, int depth) {
    if (current_index >= count || depth >= MAX_LEVELS) return;
    
    struct LogEntry* current = &entries[current_index];
    int current_level = parse_log_level(current->level);
    
    if (current_level < 0) return;
    
    if (current_level >= parent_level) {
        print_log_entry(current, depth);
        
        int next_index = current_index + 1;
        while (next_index < count) {
            struct LogEntry* next = &entries[next_index];
            int next_level = parse_log_level(next->level);
            
            if (next_level < 0) {
                next_index++;
                continue;
            }
            
            if (next_level > current_level) {
                process_log_hierarchy(entries, count, next_index, current_level, depth + 1);
                next_index++;
            } else {
                break;
            }
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE):\n");
    printf("Enter 'END' to finish input.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry* entry = &entries[count];
        char level[16];
        int timestamp;
        char message[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%15s %d %1023[^\n]", level, &timestamp, message);
        
        if (parsed == 3 && timestamp >= 0) {
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            entry->level[sizeof(entry->level) - 1] = '\0';
            entry->timestamp = timestamp;
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->message[sizeof(entry->message) - 1] = '\0';
            count++;
        } else {
            printf("Invalid format. Use: LEVEL TIMESTAMP MESSAGE\n");
        }
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = read_log_entries(entries, 100);
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nLog Hierarchy:\n");
    printf("==============\n");
    
    int i = 0;
    while (i < entry_count) {
        process_log_hierarchy(entries, entry_count, i, -1, 0);
        i++;
        
        if (i < entry_count) {
            int prev_level = parse_log_level(entries[i-1].level);
            int curr_level = parse_log_level(entries[i].level);
            
            if (curr_level <= prev_level) {
                continue;
            }
        }
    }
    
    return 0;
}