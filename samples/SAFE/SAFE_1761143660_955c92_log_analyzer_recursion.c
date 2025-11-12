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
    for (int i = 0; i < level * 2; i++) {
        putchar(' ');
    }
}

void analyze_log_recursive(struct LogEntry* entries, int count, int current_level, int* index) {
    if (*index >= count || current_level >= MAX_LEVELS) {
        return;
    }

    while (*index < count) {
        struct LogEntry entry = entries[*index];
        
        if (entry.level < current_level) {
            break;
        }
        
        if (entry.level == current_level) {
            print_indent(current_level);
            printf("Level %d: %s\n", entry.level, entry.message);
            (*index)++;
            
            if (*index < count && entries[*index].level > current_level) {
                analyze_log_recursive(entries, count, current_level + 1, index);
            }
        } else {
            break;
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL MESSAGE), empty line to finish:\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            break;
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
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
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nStructured Log Analysis:\n");
    int index = 0;
    analyze_log_recursive(entries, count, 0, &index);
    
    return 0;
}