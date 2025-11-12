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
    
    if (strncmp(str, "ERROR", 5) == 0) return LOG_LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LOG_LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LOG_LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LOG_LEVEL_DEBUG;
    
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
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0) continue;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) < TIMESTAMP_LEN + 7) continue;
        
        char timestamp[TIMESTAMP_LEN];
        strncpy(timestamp, line, TIMESTAMP_LEN - 1);
        timestamp[TIMESTAMP_LEN - 1] = 0;
        
        if (!validate_timestamp(timestamp)) continue;
        
        const char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ' && level_start < line + strlen(line)) {
            level_start++;
        }
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char* message_start = level_start;
        while (*message_start && *message_start != ' ' && message_start < line + strlen(line)) {
            message_start++;
        }
        while (*message_start == ' ' && message_start < line + strlen(line)) {
            message_start++;
        }
        
        if (message_start >= line + strlen(line)) continue;
        
        strcpy(entries[count].timestamp, timestamp);
        entries[count].level = level;
        strncpy(entries[count].message, message_start, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = 0;
        
        level_counts[level]++;
        count++;
    }
    
    return count;
}

void print_summary(int total_entries) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", total_entries);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i],
               total_entries > 0 ? (level_counts[i] * 100.0 / total_entries) : 0.0);
    }
}

void print_entries_by_level(LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", level_names[target_level]);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[entries[i].level], entries[i].message);
            found++;
            if (found >= 10) break;
        }
    }
    
    if (found == 0) {
        printf("No %s entries found.\n", level_names[target_level]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");