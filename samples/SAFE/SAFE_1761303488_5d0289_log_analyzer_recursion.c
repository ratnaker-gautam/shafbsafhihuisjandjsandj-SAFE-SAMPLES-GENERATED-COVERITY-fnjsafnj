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

void analyze_log_recursive(FILE* file, int current_level, int* line_count) {
    char line[MAX_LINE_LENGTH];
    struct LogEntry entry;
    
    if (current_level >= MAX_LEVELS || *line_count >= 1000) {
        return;
    }
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return;
    }
    
    (*line_count)++;
    
    char level_str[32];
    char message[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
        int level = parse_log_level(level_str);
        if (level >= 0 && level >= current_level) {
            print_indent(level - current_level);
            printf("%s: %s\n", level_str, message);
            
            if (level > current_level) {
                analyze_log_recursive(file, level, line_count);
            }
        }
    }
    
    analyze_log_recursive(file, current_level, line_count);
}

int main(void) {
    printf("Enter log entries (format: LEVEL MESSAGE). Type 'END' to finish:\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file\n");
        return 1;
    }
    
    char input[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) > 0) {
            fprintf(temp_file, "%s\n", input);
        }
    }
    
    rewind(temp_file);
    
    printf("\nStructured log analysis:\n");
    int line_count = 0;
    analyze_log_recursive(temp_file, 0, &line_count);
    
    fclose(temp_file);
    return 0;
}