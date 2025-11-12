//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    entry->indent_level = 0;
    while (temp[entry->indent_level] == ' ' && entry->indent_level < MAX_LEVELS) {
        entry->indent_level++;
    }
    
    char* ptr = temp + entry->indent_level;
    if (strlen(ptr) < 20) return 0;
    
    if (sscanf(ptr, "%31s %15s", entry->timestamp, entry->level) != 2) {
        return 0;
    }
    
    char* message_start = strchr(ptr, ' ');
    if (!message_start) return 0;
    message_start = strchr(message_start + 1, ' ');
    if (!message_start) return 0;
    message_start++;
    
    strncpy(entry->message, message_start, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    char* newline = strchr(entry->message, '\n');
    if (newline) *newline = '\0';
    
    return 1;
}

void analyze_log_recursive(FILE* file, int current_level, int* error_count, int* warning_count, int* info_count) {
    if (!file || !error_count || !warning_count || !info_count) return;
    if (current_level >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    long current_pos = ftell(file);
    
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) return;
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
        
        printf("%*s%s %s: %s\n", current_level * 2, "", entry.timestamp, entry.level, entry.message);
        
        long next_pos = ftell(file);
        while (!feof(file)) {
            char next_line[MAX_LINE_LENGTH];
            long check_pos = ftell(file);
            if (fgets(next_line, MAX_LINE_LENGTH, file) == NULL) break;
            
            struct LogEntry next_entry;
            if (parse_log_line(next_line, &next_entry)) {
                if (next_entry.indent_level > current_level) {
                    fseek(file, check_pos, SEEK_SET);
                    analyze_log_recursive(file, current_level + 1, error_count, warning_count, info_count);
                } else {
                    fseek(file, check_pos, SEEK_SET);
                    break;
                }
            }
        }
    }
    
    fseek(file, current_pos, SEEK_SET);
    if (fgets(line, MAX_LINE_LENGTH, file)) {
        long new_pos = ftell(file);
        fseek(file, new_pos, SEEK_SET);
        analyze_log_recursive(file, current_level, error_count, warning_count, info_count);
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
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    fclose(file);
    return 0;
}