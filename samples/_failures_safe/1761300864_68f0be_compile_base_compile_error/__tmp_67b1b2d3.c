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
    if (entry == NULL || indent_level < 0 || indent_level >= MAX_LEVELS) return;
    
    for (int i = 0; i < indent_level * 2; i++) {
        putchar(' ');
    }
    
    printf("[%s] %d: %s\n", entry->level, entry->timestamp, entry->message);
}

void process_log_hierarchy(struct LogEntry* entries, int count, int current_index, int parent_level) {
    if (current_index < 0 || current_index >= count || entries == NULL) return;
    
    struct LogEntry* current = &entries[current_index];
    int current_log_level = parse_log_level(current->level);
    
    if (current_log_level < 0) return;
    
    print_log_entry(current, parent_level);
    
    int next_index = current_index + 1;
    while (next_index < count) {
        struct LogEntry* next = &entries[next_index];
        int next_log_level = parse_log_level(next->level);
        
        if (next_log_level < 0) {
            next_index++;
            continue;
        }
        
        if (next_log_level > current_log_level) {
            process_log_hierarchy(entries, count, next_index, parent_level + 1);
            
            int child_end = next_index + 1;
            while (child_end < count) {
                struct LogEntry* child = &entries[child_end];
                int child_level = parse_log_level(child->level);
                if (child_level <= current_log_level) break;
                child_end++;
            }
            next_index = child_end;
        } else {
            break;
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL TIMESTAMP MESSAGE):\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (count >= max_entries) {
            printf("Maximum entries reached.\n");
            break;
        }
        
        struct LogEntry* entry = &entries[count];
        char level_buf[16];
        int timestamp;
        char message_buf[MAX_LINE_LENGTH];
        
        int parsed = sscanf(line, "%15s %d %1023[^\n]", level_buf, &timestamp, message_buf);
        if (parsed != 3) {
            printf("Invalid format. Skipping: %s\n", line);
            continue;
        }
        
        if (parse_log_level(level_buf) < 0) {
            printf("Invalid log level. Skipping: %s\n", level_buf);
            continue;
        }
        
        if (timestamp < 0) {
            printf("Invalid timestamp. Skipping: %d\n", timestamp);
            continue;
        }
        
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        entry->timestamp = timestamp;
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[50];
    int entry_count = read_log_entries(entries, 50);
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nLog Hierarchy:\n");
    printf("==============\n");
    
    int current_index = 0;
    while (current_index < entry_count) {
        struct LogEntry* current = &entries[current_index];
        int current_level = parse_log_level(current->level);
        
        if (current_level == 0) {
            process_log_hierarchy(entries, entry_count, current_index, 0);
            
            int group_end = current_index + 1;
            while (group_end < entry_count)