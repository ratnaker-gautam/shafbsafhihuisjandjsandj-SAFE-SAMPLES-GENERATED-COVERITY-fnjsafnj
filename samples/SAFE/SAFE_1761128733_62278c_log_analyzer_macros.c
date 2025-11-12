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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    if (strncmp(str, "DEBUG", 5) == 0) return 0;
    if (strncmp(str, "INFO", 4) == 0) return 1;
    if (strncmp(str, "WARNING", 7) == 0) return 2;
    if (strncmp(str, "ERROR", 5) == 0) return 3;
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    int i = 0;
    while (str[i] && i < TIMESTAMP_LEN && IS_TIMESTAMP_CHAR(str[i])) {
        timestamp[i] = str[i];
        i++;
    }
    if (i == 0 || i >= TIMESTAMP_LEN) return 0;
    timestamp[i] = '\0';
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN + 1];
        if (!parse_timestamp(line, timestamp)) continue;
        
        const char *level_start = line + strlen(timestamp);
        while (*level_start == ' ' || *level_start == '\t') level_start++;
        
        int level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(level)) continue;
        
        const char *message_start = level_start;
        while (*message_start && *message_start != ' ') message_start++;
        while (*message_start == ' ') message_start++;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN);
        entries[count].timestamp[TIMESTAMP_LEN] = '\0';
        entries[count].level = level;
        
        size_t msg_len = strlen(message_start);
        if (msg_len > 0 && message_start[msg_len - 1] == '\n') {
            msg_len--;
        }
        if (msg_len >= sizeof(entries[count].message)) {
            msg_len = sizeof(entries[count].message) - 1;
        }
        strncpy(entries[count].message, message_start, msg_len);
        entries[count].message[msg_len] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    int i;
    
    for (i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("Breakdown by level:\n");
    for (i = 0; i < LOG_LEVELS; i++) {
        printf("  %s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nLast 3 entries:\n");
        int start = count > 3 ? count - 3 : 0;
        for (i = start; i < count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, 
                   LEVEL_NAME(entries[i].level), entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Press Ctrl+D (Linux) or Ctrl+Z (Windows) to finish input\n\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}