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

void print_indent(int level) {
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}

void process_log_recursive(struct LogEntry* entries, int count, int current, int parent_level) {
    if (current >= count || current < 0) return;
    
    struct LogEntry entry = entries[current];
    
    if (entry.level <= parent_level) return;
    
    print_indent(entry.level - 1);
    printf("%s: %s\n", 
           entry.level == 1 ? "DEBUG" :
           entry.level == 2 ? "INFO" :
           entry.level == 3 ? "WARNING" :
           entry.level == 4 ? "ERROR" : "CRITICAL",
           entry.message);
    
    process_log_recursive(entries, count, current + 1, entry.level);
    
    int next = current + 1;
    while (next < count) {
        if (entries[next].level <= entry.level) break;
        if (entries[next].level == entry.level + 1) {
            process_log_recursive(entries, count, next, entry.level);
        }
        next++;
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
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
        
        entries[count].level = level + 1;
        strncpy(entries[count].message, message, MAX_LINE_LENGTH - 1);
        entries[count].message[MAX_LINE_LENGTH - 1] = '\0';
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    int count = read_log_entries(entries, MAX_LEVELS);
    
    if (count == 0) {
        printf("No log entries to process.\n");
        return 0;
    }
    
    printf("\nStructured Log Analysis:\n");
    printf("=======================\n");
    
    int i = 0;
    while (i < count) {
        if (entries[i].level == 1) {
            process_log_recursive(entries, count, i, 0);
        }
        i++;
    }
    
    return 0;
}