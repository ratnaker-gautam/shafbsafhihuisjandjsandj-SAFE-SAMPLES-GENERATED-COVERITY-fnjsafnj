//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
    int indent_level;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    if (strlen(line) < 30) return 0;
    
    int parsed = sscanf(line, "%19s %9s %[^\n]", 
                       entry->timestamp, entry->level, entry->message);
    
    if (parsed != 3) return 0;
    
    entry->indent_level = 0;
    for (int i = 0; i < 3; i++) {
        if (strcmp(entry->level, "INFO") == 0) entry->indent_level = 1;
        else if (strcmp(entry->level, "WARN") == 0) entry->indent_level = 2;
        else if (strcmp(entry->level, "ERROR") == 0) entry->indent_level = 3;
    }
    
    return 1;
}

void print_indented_entry(const struct LogEntry* entry, int current_depth) {
    if (!entry || current_depth < 0 || current_depth > MAX_LEVELS) return;
    
    for (int i = 0; i < current_depth; i++) {
        printf("  ");
    }
    
    printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
}

void process_log_recursive(FILE* file, int current_depth, int* line_count) {
    if (!file || current_depth < 0 || current_depth > MAX_LEVELS || !line_count) return;
    
    char line[MAX_LINE_LENGTH];
    struct LogEntry entry;
    
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    (*line_count)++;
    
    if (parse_log_line(line, &entry)) {
        print_indented_entry(&entry, current_depth);
        
        if (strstr(entry.message, "Starting") != NULL || 
            strstr(entry.message, "Begin") != NULL) {
            process_log_recursive(file, current_depth + 1, line_count);
        } else if (strstr(entry.message, "Completed") != NULL || 
                   strstr(entry.message, "End") != NULL) {
            return;
        } else {
            process_log_recursive(file, current_depth, line_count);
        }
    } else {
        process_log_recursive(file, current_depth, line_count);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    printf("Structured Log Analysis:\n");
    printf("========================\n");
    
    int line_count = 0;
    process_log_recursive(file, 0, &line_count);
    
    printf("========================\n");
    printf("Total lines processed: %d\n", line_count);
    
    fclose(file);
    return 0;
}