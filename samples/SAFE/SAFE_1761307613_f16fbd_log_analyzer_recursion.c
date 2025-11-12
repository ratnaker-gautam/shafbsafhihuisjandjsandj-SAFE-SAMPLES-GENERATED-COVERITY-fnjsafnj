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

int parse_log_level(const char* line) {
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = start; i < end; i++) {
        if (entries[i].level == 3) error_count++;
        else if (entries[i].level == 2) warn_count++;
        else if (entries[i].level == 1) info_count++;
        else if (entries[i].level == 0) debug_count++;
    }
    
    printf("Level %d analysis (%d-%d): ERRORS=%d, WARNINGS=%d, INFO=%d, DEBUG=%d\n",
           current_level, start, end, error_count, warn_count, info_count, debug_count);
    
    if (end - start > 1) {
        int mid = start + (end - start) / 2;
        analyze_log_recursive(entries, start, mid, current_level + 1);
        analyze_log_recursive(entries, mid, end, current_level + 1);
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = 0;
            int level = parse_log_level(line);
            if (level >= 0) {
                entries[count].level = level;
                strncpy(entries[count].message, line, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[1000];
    int entry_count = read_log_file(argv[1], entries, 1000);
    
    if (entry_count < 0) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found in %s\n", argv[1]);
        return 0;
    }
    
    printf("Analyzing %d log entries from %s\n", entry_count, argv[1]);
    analyze_log_recursive(entries, 0, entry_count, 0);
    
    int total_errors = 0;
    int total_warnings = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == 3) total_errors++;
        else if (entries[i].level == 2) total_warnings++;
    }
    
    printf("\nSummary: %d total entries, %d errors, %d warnings\n", 
           entry_count, total_errors, total_warnings);
    
    return 0;
}