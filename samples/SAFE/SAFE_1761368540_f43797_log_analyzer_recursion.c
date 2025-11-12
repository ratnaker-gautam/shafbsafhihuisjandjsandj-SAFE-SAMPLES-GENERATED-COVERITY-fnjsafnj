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

void process_log_tree(struct LogEntry* logs, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    int i = start;
    while (i < end) {
        if (logs[i].level == current_level) {
            print_indent(current_level);
            printf("%s\n", logs[i].message);
            int next_start = i + 1;
            int next_end = next_start;
            
            while (next_end < end && logs[next_end].level > current_level) {
                next_end++;
            }
            
            if (next_end > next_start) {
                process_log_tree(logs, next_start, next_end, current_level + 1);
            }
            
            i = next_end;
        } else {
            i++;
        }
    }
}

int read_log_file(const char* filename, struct LogEntry* logs, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) == 0) continue;
        
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
            int level = parse_log_level(level_str);
            if (level >= 0) {
                logs[count].level = level;
                strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
                logs[count].message[sizeof(logs[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry logs[1000];
    int entry_count = read_log_file(argv[1], logs, 1000);
    
    if (entry_count < 0) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Structured Log Analysis:\n");
    printf("=======================\n");
    process_log_tree(logs, 0, entry_count, 0);
    
    return 0;
}