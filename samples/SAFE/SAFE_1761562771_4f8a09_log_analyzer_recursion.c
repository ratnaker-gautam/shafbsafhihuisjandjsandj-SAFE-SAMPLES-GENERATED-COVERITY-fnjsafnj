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
    if (level <= 0) return;
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
}

void analyze_log_recursive(struct LogEntry* entries, int count, int current, int parent_level) {
    if (current >= count || current < 0) return;
    
    struct LogEntry entry = entries[current];
    
    if (entry.level <= parent_level) {
        return;
    }
    
    print_indent(entry.level - 1);
    printf("%d: %s\n", current + 1, entry.message);
    
    analyze_log_recursive(entries, count, current + 1, entry.level);
    
    int next_sibling = current + 1;
    while (next_sibling < count) {
        if (entries[next_sibling].level == entry.level) {
            analyze_log_recursive(entries, count, next_sibling, parent_level);
            break;
        }
        if (entries[next_sibling].level < entry.level) {
            break;
        }
        next_sibling++;
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
        
        if (sscanf(line, "%31s %[^\n]", level_str, message) != 2) {
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level < 0) {
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        count++;
    }
    
    return count;
}

int main(void) {
    struct LogEntry entries[MAX_LEVELS];
    
    printf("Enter log entries (format: LEVEL MESSAGE). Maximum %d entries.\n", MAX_LEVELS);
    printf("Valid levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input.\n\n");
    
    int count = 0;
    char input[MAX_LINE_LENGTH];
    
    while (count < MAX_LEVELS) {
        printf("Log entry %d: ", count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(input, "%31s %[^\n]", level_str, message) != 2) {
            printf("Invalid format. Use: LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level < 0) {
            printf("Invalid level. Valid: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nStructured Log Analysis:\n");
    printf("=======================\n");
    
    int current = 0;
    while (current < count) {
        analyze_log_recursive(entries, count, current, -1);
        current++;
        while (current < count && entries[current].level > 0) {
            current++;
        }
    }
    
    return 0;
}