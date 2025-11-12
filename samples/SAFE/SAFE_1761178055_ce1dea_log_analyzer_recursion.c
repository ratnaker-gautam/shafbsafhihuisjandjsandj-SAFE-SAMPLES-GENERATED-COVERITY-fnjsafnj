//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    int level;
    char message[MAX_LINE_LENGTH];
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
    if (entry == NULL || indent < 0 || indent >= MAX_LEVELS) return;
    
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    if (entry->level >= 0 && entry->level <= 4) {
        printf("[%s] %s\n", level_names[entry->level], entry->message);
    }
}

void process_log_hierarchy(struct LogEntry* entries, int count, int current_index, int current_level) {
    if (current_index >= count || current_index < 0 || current_level < 0 || current_level >= MAX_LEVELS) {
        return;
    }
    
    print_log_entry(&entries[current_index], current_level);
    
    int next_index = current_index + 1;
    while (next_index < count) {
        if (entries[next_index].level > entries[current_index].level) {
            process_log_hierarchy(entries, count, next_index, current_level + 1);
            
            int child_level = entries[next_index].level;
            next_index++;
            while (next_index < count && entries[next_index].level > child_level) {
                next_index++;
            }
        } else {
            break;
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(message) >= MAX_LINE_LENGTH) {
            printf("Message too long\n");
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[100];
    int count = read_log_entries(entries, 100);
    
    if (count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nLog Hierarchy:\n");
    printf("==============\n");
    
    int current_index = 0;
    while (current_index < count) {
        if (entries[current_index].level == 0) {
            process_log_hierarchy(entries, count, current_index, 0);
            
            int root_level = entries[current_index].level;
            current_index++;
            while (current_index < count && entries[current_index].level > root_level) {
                current_index++;
            }
        } else {
            current_index++;
        }
    }
    
    return 0;
}