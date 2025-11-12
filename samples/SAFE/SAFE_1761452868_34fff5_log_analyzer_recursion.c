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

void process_log_hierarchy(struct LogEntry* entries, int count, int current_level, int* index) {
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
            printf("%s\n", current->message);
            (*index)++;
            
            if (*index < count && entries[*index].level > current_level) {
                process_log_hierarchy(entries, count, current_level + 1, index);
            }
        } else {
            return;
        }
    }
}

int read_log_entries(FILE* file, struct LogEntry* entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_LEVELS) {
        if (strlen(line) == 0) continue;
        
        line[strcspn(line, "\n")] = 0;
        
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    struct LogEntry entries[MAX_LEVELS];
    int count = read_log_entries(file, entries);
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Log hierarchy:\n");
    int index = 0;
    process_log_hierarchy(entries, count, 0, &index);
    
    return 0;
}