//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    int indent;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    if (strlen(line) < 30) return 0;
    
    int parsed = sscanf(line, "%19s %9s %[^\n]", 
                       entry->timestamp, entry->level, entry->message);
    
    if (parsed != 3) return 0;
    
    entry->indent = 0;
    for (int i = 0; line[i] && isspace(line[i]); i++) {
        if (line[i] == ' ') entry->indent++;
        else if (line[i] == '\t') entry->indent += 4;
    }
    
    return 1;
}

void analyze_log_recursive(FILE* file, int current_depth, int* error_count, int* warning_count, int* info_count) {
    if (file == NULL || current_depth >= MAX_LEVELS) return;
    
    char buffer[MAX_LINE_LENGTH];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strlen(buffer) >= sizeof(buffer) - 1) {
            continue;
        }
        
        struct LogEntry entry;
        if (!parse_log_line(buffer, &entry)) continue;
        
        for (int i = 0; i < current_depth; i++) {
            printf("  ");
        }
        
        printf("[%s] %s: %s\n", entry.timestamp, entry.level, entry.message);
        
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
        
        if (entry.indent > current_depth * 2) {
            analyze_log_recursive(file, current_depth + 1, error_count, warning_count, info_count);
        } else if (entry.indent < current_depth * 2) {
            break;
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
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("Errors: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    printf("Info: %d\n", info_count);
    
    return 0;
}