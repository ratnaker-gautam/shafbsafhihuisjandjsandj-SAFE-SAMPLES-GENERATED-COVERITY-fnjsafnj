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

void process_log_recursive(struct LogEntry* entries, int count, int current, int depth) {
    if (current >= count || depth >= MAX_LEVELS) {
        return;
    }

    print_indent(depth);
    printf("[%d] %s\n", entries[current].level, entries[current].message);

    if (current + 1 < count) {
        if (entries[current + 1].level > entries[current].level) {
            process_log_recursive(entries, count, current + 1, depth + 1);
        } else {
            process_log_recursive(entries, count, current + 1, depth);
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') {
            continue;
        }

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
    
    printf("Enter log entries (format: LEVEL MESSAGE). Empty line to finish:\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    
    int count = read_log_entries(entries, 1000);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    printf("\nStructured log hierarchy:\n");
    process_log_recursive(entries, count, 0, 0);
    
    return 0;
}