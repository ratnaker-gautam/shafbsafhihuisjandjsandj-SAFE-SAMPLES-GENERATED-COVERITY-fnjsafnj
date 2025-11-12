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

void process_log_recursive(struct LogEntry* entries, int count, int current_level, int* index) {
    if (*index >= count || current_level >= MAX_LEVELS) {
        return;
    }

    while (*index < count) {
        struct LogEntry* current = &entries[*index];
        
        if (current->level < current_level) {
            return;
        }
        
        if (current->level == current_level) {
            print_indent(current_level);
            printf("Level %d: %s\n", current->level, current->message);
            (*index)++;
            
            if (*index < count && entries[*index].level > current_level) {
                process_log_recursive(entries, count, current_level + 1, index);
            }
        } else {
            return;
        }
    }
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31[^:]: %1023[^\n]", level_str, message) == 2) {
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
    int entry_count = 0;
    
    printf("Enter log entries (format: LEVEL: message). Enter 'END' to finish:\n");
    
    char input[MAX_LINE_LENGTH];
    while (entry_count < 1000) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) > 0 && input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(input, "%31[^:]: %1023[^\n]", level_str, message) == 2) {
            int level = parse_log_level(level_str);
            if (level >= 0) {
                entries[entry_count].level = level;
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                entry_count++;
            } else {
                printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            }
        } else {
            printf("Invalid format. Use: LEVEL: message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to process.\n");
        return 1;
    }
    
    printf("\nStructured log analysis:\n");
    int index = 0;
    process_log_recursive(entries, entry_count, 0, &index);
    
    return 0;
}