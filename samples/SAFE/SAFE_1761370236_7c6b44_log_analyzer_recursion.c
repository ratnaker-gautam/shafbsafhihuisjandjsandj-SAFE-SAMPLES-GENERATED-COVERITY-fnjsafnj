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

void process_log_hierarchy(struct LogEntry* entries, int start, int end, int current_level) {
    if (start > end || current_level >= MAX_LEVELS) {
        return;
    }

    int i = start;
    while (i <= end) {
        if (entries[i].level == current_level) {
            print_indent(current_level);
            printf("%s\n", entries[i].message);
            
            int next_start = i + 1;
            int next_end = next_start - 1;
            
            while (next_end + 1 <= end && entries[next_end + 1].level > current_level) {
                next_end++;
            }
            
            if (next_end >= next_start) {
                process_log_hierarchy(entries, next_start, next_end, current_level + 1);
            }
            
            i = next_end + 1;
        } else {
            i++;
        }
    }
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: LEVEL: MESSAGE), empty line to finish:\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        char* colon = strchr(line, ':');
        if (colon == NULL || colon == line) {
            printf("Invalid format. Use: LEVEL: MESSAGE\n");
            continue;
        }
        
        *colon = '\0';
        char* level_str = line;
        char* message = colon + 1;
        
        while (*message == ' ') message++;
        
        if (strlen(level_str) == 0 || strlen(message) == 0) {
            printf("Empty level or message\n");
            continue;
        }
        
        for (char* p = level_str; *p; p++) {
            *p = toupper((unsigned char)*p);
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Unknown log level: %s\n", level_str);
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
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nStructured Log Hierarchy:\n");
    process_log_hierarchy(entries, 0, count - 1, 0);
    
    return 0;
}