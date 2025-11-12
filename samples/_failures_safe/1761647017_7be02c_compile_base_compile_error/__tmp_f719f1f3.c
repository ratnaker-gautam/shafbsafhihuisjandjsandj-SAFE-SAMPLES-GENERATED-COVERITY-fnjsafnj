//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_DEBUG 3

#define LEVEL_NAME(level) (level == LOG_INFO ? "INFO" : \
                          level == LOG_WARN ? "WARN" : \
                          level == LOG_ERROR ? "ERROR" : "DEBUG")

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char* level_str) {
    if (strncmp(level_str, "INFO", 4) == 0) return LOG_INFO;
    if (strncmp(level_str, "WARN", 4) == 0) return LOG_WARN;
    if (strncmp(level_str, "ERROR", 5) == 0) return LOG_ERROR;
    if (strncmp(level_str, "DEBUG", 5) == 0) return LOG_DEBUG;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else if (!isdigit(timestamp[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int level_counts[LOG_LEVELS] = {0};
    char input_line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = 0;
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) >= MAX_LINE_LEN - 1) {
            printf("Error: Line too long\n");
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        int parsed = sscanf(input_line, "%19s %9s %255[^\n]", 
                           timestamp, level_str, message);
        
        if (parsed != 3) {
            printf("Error: Invalid format\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Error: Invalid timestamp format\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Error: Invalid log level\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Error: Empty message\n");
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[entry_count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, 
                sizeof(entries[entry_count].message) - 1);
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
        
        level_counts[level]++;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed\n");
        return 0;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nBreakdown by level:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i],
               (level_counts[i] * 100.0) / entry_count);
    }
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               LEVEL_NAME(entries[i].level), entries[i].message);
    }
    
    int error_count = level_counts[LOG_ERROR];
    if (error_count > 0) {
        printf("\nALERT: %d error(s) detected in logs\n", error_count);
    }
    
    return