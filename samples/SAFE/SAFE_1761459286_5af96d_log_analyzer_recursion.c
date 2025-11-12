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

const char* level_to_string(int level) {
    switch (level) {
        case 0: return "DEBUG";
        case 1: return "INFO";
        case 2: return "WARNING";
        case 3: return "ERROR";
        case 4: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void count_logs_by_level_recursive(FILE* file, int level_counts[], int current_level) {
    if (current_level >= MAX_LEVELS || current_level < 0) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return;
    
    char level_str[32];
    char message[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
        int level = parse_log_level(level_str);
        if (level >= 0 && level < MAX_LEVELS) {
            level_counts[level]++;
        }
    }
    
    count_logs_by_level_recursive(file, level_counts, current_level + 1);
}

int read_log_file(const char* filename, struct LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char level_str[32];
        char message[MAX_LINE_LENGTH];
        
        if (sscanf(line, "%31s %1023[^\n]", level_str, message) == 2) {
            int level = parse_log_level(level_str);
            if (level >= 0) {
                entries[count].level = level;
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void print_log_summary_recursive(int level_counts[], int current_level) {
    if (current_level >= 5) return;
    
    printf("%-10s: %d entries\n", level_to_string(current_level), level_counts[current_level]);
    print_log_summary_recursive(level_counts, current_level + 1);
}

void filter_logs_by_level_recursive(struct LogEntry entries[], int count, int target_level, int index) {
    if (index >= count) return;
    
    if (entries[index].level == target_level) {
        printf("[%s] %s\n", level_to_string(entries[index].level), entries[index].message);
    }
    
    filter_logs_by_level_recursive(entries, count, target_level, index + 1);
}

int main() {
    printf("Log Analyzer\n");
    printf("Enter log filename: ");
    
    char filename[256];
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename input\n");
        return 1;
    }
    
    struct LogEntry entries[1000];
    int entry_count = read_log_file(filename, entries, 1000);
    
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("\nFound %d log entries\n\n", entry_count);
    
    int level_counts[MAX_LEVELS] = {0};
    FILE* file = fopen(filename, "r");
    if (file) {
        count_logs_by_level_recursive(file, level_counts, 0);
        fclose(file);
    }
    
    printf("Log Level Summary:\n");
    print_log_summary_recursive(level_counts, 0);
    
    printf("\nEnter log level to filter (DEBUG=0, INFO=1, WARNING=2, ERROR=3, CRITICAL=4): ");
    int filter_level;
    if (scanf("%d", &filter_level) != 1 || filter_level < 0 || filter_level > 4) {
        printf("Invalid log level\n");
        return 1;
    }
    
    printf("\nFiltered %s logs:\n", level_to_string(filter_level));
    filter_logs_by_level_recursive(entries, entry_count, filter_level, 0);
    
    return 0;
}