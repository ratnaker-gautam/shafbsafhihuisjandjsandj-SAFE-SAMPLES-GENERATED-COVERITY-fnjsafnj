//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strcasecmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcasecmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcasecmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcasecmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (timestamp == NULL) return 0;
    if (strlen(timestamp) != TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
        
        if (parsed == 3) {
            if (!validate_timestamp(timestamp)) {
                continue;
            }
            
            int level = parse_log_level(level_str);
            if (!IS_VALID_LEVEL(level)) {
                continue;
            }
            
            strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
            entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
            entries[count].level = level;
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            level_counts[level]++;
            count++;
        }
    }
    
    return count;
}

void print_summary(int total_entries) {
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d entries (%.1f%%)\n", 
               level_names[i], 
               level_counts[i],
               total_entries > 0 ? (level_counts[i] * 100.0 / total_entries) : 0.0);
    }
}

void print_entries_by_level(LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", level_names[target_level]);
    printf("-------------\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s [%s] %s\n", entries[i].timestamp, level_names[target_level], entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No %s entries found.\n", level_names[target_level]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        level_counts[i] = 0;
    }
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0)