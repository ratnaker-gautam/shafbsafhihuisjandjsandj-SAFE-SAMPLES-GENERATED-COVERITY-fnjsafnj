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

void analyze_log_recursive(FILE* file, struct LogEntry* entries, int* entry_count, int depth) {
    if (depth >= MAX_LEVELS || feof(file)) return;
    
    char buffer[MAX_LINE_LENGTH];
    if (fgets(buffer, sizeof(buffer), file) == NULL) return;
    
    buffer[strcspn(buffer, "\n")] = 0;
    
    if (strlen(buffer) == 0) {
        analyze_log_recursive(file, entries, entry_count, depth + 1);
        return;
    }
    
    int level = parse_log_level(buffer);
    if (level < 0) {
        analyze_log_recursive(file, entries, entry_count, depth + 1);
        return;
    }
    
    int found = 0;
    for (int i = 0; i < *entry_count; i++) {
        if (entries[i].level == level) {
            entries[i].count++;
            found = 1;
            break;
        }
    }
    
    if (!found && *entry_count < MAX_LEVELS) {
        strncpy(entries[*entry_count].line, buffer, sizeof(entries[*entry_count].line) - 1);
        entries[*entry_count].line[sizeof(entries[*entry_count].line) - 1] = '\0';
        entries[*entry_count].level = level;
        entries[*entry_count].count = 1;
        (*entry_count)++;
    }
    
    analyze_log_recursive(file, entries, entry_count, depth + 1);
}

void print_summary(const struct LogEntry* entries, int entry_count) {
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    int total = 0;
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level >= 0 && entries[i].level <= 3) {
            printf("%-6s: %d entries\n", level_names[entries[i].level], entries[i].count);
            total += entries[i].count;
        }
    }
    
    printf("Total: %d entries\n", total);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    struct LogEntry entries[MAX_LEVELS];
    int entry_count = 0;
    
    analyze_log_recursive(file, entries, &entry_count, 0);
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    print_summary(entries, entry_count);
    
    return 0;
}