//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
    int count;
};

int parse_log_level(const char* line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void process_log_recursive(FILE* file, struct LogEntry* stats, int* total_lines, int depth) {
    if (depth >= MAX_LEVELS || feof(file)) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strlen(buffer) > 0) {
        (*total_lines)++;
        int level = parse_log_level(buffer);
        if (level >= 0 && level < 4) {
            stats[level].count++;
            strncpy(stats[level].line, buffer, sizeof(stats[level].line) - 1);
            stats[level].line[sizeof(stats[level].line) - 1] = '\0';
        }
    }
    
    process_log_recursive(file, stats, total_lines, depth + 1);
}

void print_stats(const struct LogEntry* stats, int total_lines) {
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    printf("Log Analysis Results:\n");
    printf("Total lines processed: %d\n\n", total_lines);
    
    for (int i = 0; i < 4; i++) {
        printf("%s entries: %d\n", level_names[i], stats[i].count);
        if (stats[i].count > 0) {
            printf("Last %s entry: %s\n", level_names[i], stats[i].line);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    struct LogEntry stats[4] = {0};
    int total_lines = 0;
    
    for (int i = 0; i < 4; i++) {
        stats[i].level = i;
        stats[i].count = 0;
        stats[i].line[0] = '\0';
    }
    
    process_log_recursive(file, stats, &total_lines, 0);
    
    fclose(file);
    
    print_stats(stats, total_lines);
    
    return 0;
}