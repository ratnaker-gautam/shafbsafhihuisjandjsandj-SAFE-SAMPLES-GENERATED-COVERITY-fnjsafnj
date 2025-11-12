//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
    int indent_level;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    entry->indent_level = 0;
    while (temp[entry->indent_level] == ' ' && entry->indent_level < MAX_LEVELS) {
        entry->indent_level++;
    }
    
    const char* ptr = temp + entry->indent_level;
    
    if (sscanf(ptr, "%31s %15s", entry->timestamp, entry->level) != 2) {
        return 0;
    }
    
    const char* msg_start = strchr(ptr, ' ');
    if (msg_start == NULL) return 0;
    msg_start = strchr(msg_start + 1, ' ');
    if (msg_start == NULL) return 0;
    msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= MAX_LINE_LENGTH) return 0;
    strncpy(entry->message, msg_start, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    char* newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int current_level, int* line_count, int* error_count, int* warning_count) {
    if (file == NULL || line_count == NULL || error_count == NULL || warning_count == NULL) return;
    if (current_level >= MAX_LEVELS) return;
    
    long start_pos = ftell(file);
    if (start_pos == -1L) return;
    
    char line[MAX_LINE_LENGTH];
    struct LogEntry entry;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        (*line_count)++;
        
        if (!parse_log_line(line, &entry)) continue;
        
        if (entry.indent_level < current_level) {
            fseek(file, start_pos, SEEK_SET);
            return;
        }
        
        if (entry.indent_level == current_level) {
            if (strcmp(entry.level, "ERROR") == 0) {
                (*error_count)++;
                printf("ERROR at level %d: %s\n", current_level, entry.message);
            } else if (strcmp(entry.level, "WARNING") == 0) {
                (*warning_count)++;
                printf("WARNING at level %d: %s\n", current_level, entry.message);
            }
            
            long next_pos = ftell(file);
            analyze_log_recursive(file, current_level + 1, line_count, error_count, warning_count);
            fseek(file, next_pos, SEEK_SET);
        }
    }
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
    
    int total_lines = 0;
    int error_count = 0;
    int warning_count = 0;
    
    printf("Analyzing log file: %s\n", argv[1]);
    printf("====================\n");
    
    analyze_log_recursive(file, 0, &total_lines, &error_count, &warning_count);
    
    fclose(file);
    
    printf("====================\n");
    printf("Summary:\n");
    printf("Total lines processed: %d\n", total_lines);
    printf("Errors found: %d\n", error_count);
    printf("Warnings found: %d\n", warning_count);
    
    return 0;
}