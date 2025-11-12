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

#define IS_VALID_LEVEL(c) ((c) == 'E' || (c) == 'W' || (c) == 'I' || (c) == 'D')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};
LogEntry entries[MAX_ENTRIES];
int entry_count = 0;

int parse_timestamp(const char* str) {
    if (strlen(str) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    return 1;
}

int parse_log_level(char level_char) {
    switch (level_char) {
        case 'E': return LOG_LEVEL_ERROR;
        case 'W': return LOG_LEVEL_WARN;
        case 'I': return LOG_LEVEL_INFO;
        case 'D': return LOG_LEVEL_DEBUG;
        default: return -1;
    }
}

int process_line(const char* line) {
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    
    char timestamp[TIMESTAMP_LEN];
    char level_char;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
    
    if (sscanf(line, "%19s %c %[^\n]", timestamp, &level_char, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp)) return 0;
    if (!IS_VALID_LEVEL(level_char)) return 0;
    
    int level = parse_log_level(level_char);
    if (level == -1) return 0;
    
    if (entry_count >= MAX_ENTRIES) return 0;
    
    strcpy(entries[entry_count].timestamp, timestamp);
    entries[entry_count].level = level;
    strcpy(entries[entry_count].message, message);
    level_counts[level]++;
    entry_count++;
    
    return 1;
}

void generate_sample_logs(void) {
    const char* sample_logs[] = {
        "2024-01-15 10:30:25 E Database connection failed",
        "2024-01-15 10:31:02 W High memory usage detected",
        "2024-01-15 10:32:15 I User login successful",
        "2024-01-15 10:33:40 D Processing request ID 12345",
        "2024-01-15 10:34:22 I Backup completed successfully",
        "2024-01-15 10:35:18 W Cache size approaching limit",
        "2024-01-15 10:36:05 E File not found: config.xml",
        "2024-01-15 10:37:30 D Starting cleanup procedure",
        "2024-01-15 10:38:14 I System health check passed",
        "2024-01-15 10:39:27 W Retrying connection attempt"
    };
    
    int sample_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    for (int i = 0; i < sample_count && entry_count < MAX_ENTRIES; i++) {
        process_line(sample_logs[i]);
    }
}

void display_summary(void) {
    printf("Log Analysis Summary\n");
    printf("===================\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nBreakdown by level:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-6s: %d entries\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void display_entries_by_level(int level) {
    if (level < 0 || level >= LOG_LEVELS) return;
    
    printf("\n%s entries:\n", level_names[level]);
    printf("------------\n");
    
    int count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %c %s\n", entries[i].timestamp, 
                   level_names[level][0], entries[i].message);
            count++;
            if (count >= 10) break;
        }
    }
    if (count == 0) {
        printf("No entries found\n");