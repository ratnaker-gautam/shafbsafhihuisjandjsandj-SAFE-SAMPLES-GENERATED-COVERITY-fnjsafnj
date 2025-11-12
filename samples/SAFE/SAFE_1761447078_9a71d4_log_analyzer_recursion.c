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

void process_log_recursive(struct LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    int i = start;
    while (i < end) {
        if (entries[i].level == current_level) {
            print_indent(current_level);
            printf("%s\n", entries[i].message);
            i++;
        } else if (entries[i].level > current_level) {
            int group_start = i;
            while (i < end && entries[i].level > current_level) {
                i++;
            }
            process_log_recursive(entries, group_start, i, current_level + 1);
        } else {
            i++;
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
            int level = parse_log_level(level_str);
            if (level >= 0) {
                entries[count].level = level;
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    
    printf("Enter log entries (format: LEVEL MESSAGE). End with Ctrl+D:\n");
    
    int count = read_log_entries(entries, 1000);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nStructured log output:\n");
    process_log_recursive(entries, 0, count, 0);
    
    return 0;
}