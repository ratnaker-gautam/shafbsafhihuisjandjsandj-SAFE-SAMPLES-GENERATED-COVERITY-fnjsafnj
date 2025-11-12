//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_EMPTY_LINE(s) ((s)[0] == '\0' || (s)[0] == '\n')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char *str, char *timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (str[i] == ' ') break;
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, level_names[i]) != NULL) return i;
    }
    return -1;
}

int read_log_entries(LogEntry *entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (IS_EMPTY_LINE(line)) continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) continue;
        
        int level = parse_level(line);
        if (!IS_VALID_LEVEL(level)) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        
        const char *msg_start = strstr(line, level_names[level]);
        if (msg_start != NULL) {
            msg_start += strlen(level_names[level]);
            while (*msg_start == ' ') msg_start++;
            strncpy(entries[count].message, msg_start, 
                   sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            char *newline = strchr(entries[count].message, '\n');
            if (newline != NULL) *newline = '\0';
        } else {
            strcpy(entries[count].message, "No message");
        }
        
        count++;
    }
    
    return count;
}

void print_summary(const LogEntry *entries, int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", LEVEL_NAME(i), level_counts[i]);
    }
    printf("\n");
}

void print_entries_by_level(const LogEntry *entries, int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("%s entries:\n", LEVEL_NAME(target_level));
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
            found++;
        }
    }
    if (found == 0) {
        printf("No %s entries found.\n", LEVEL_NAME(target_level));
    }
    printf("\n");
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message). Press Ctrl+D to finish.\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(entries, count);
    
    for (int level = 0; level < LOG_LEVELS; level++) {
        print_entries_by_level(entries, count, level);
    }
    
    return 0;
}