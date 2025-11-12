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
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_timestamp(const char *str, char *timestamp) {
    int count = 0;
    for (int i = 0; str[i] && count < TIMESTAMP_LEN - 1; i++) {
        if (IS_TIMESTAMP_CHAR(str[i])) {
            timestamp[count++] = str[i];
        } else {
            break;
        }
    }
    if (count > 0) {
        timestamp[count] = '\0';
        return 1;
    }
    return 0;
}

int parse_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strstr(str, LEVEL_NAME(i)) != NULL) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) {
            continue;
        }
        
        int level = parse_level(line);
        if (!IS_VALID_LEVEL(level)) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        
        const char *msg_start = strstr(line, LEVEL_NAME(level));
        if (msg_start) {
            msg_start += strlen(LEVEL_NAME(level));
            while (*msg_start && isspace(*msg_start)) {
                msg_start++;
            }
            strncpy(entries[count].message, msg_start, 
                   sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            
            char *newline = strchr(entries[count].message, '\n');
            if (newline) *newline = '\0';
        } else {
            strcpy(entries[count].message, "");
        }
        
        count++;
    }
    
    return count;
}

void print_summary(const LogEntry entries[], int count) {
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

void print_recent_entries(const LogEntry entries[], int count, int num_recent) {
    if (num_recent <= 0 || num_recent > count) {
        num_recent = count;
    }
    
    printf("Recent %d entries:\n", num_recent);
    int start = count - num_recent;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               LEVEL_NAME(entries[i].level), entries[i].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp, level, message). Press Ctrl+D to finish:\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(entries, count);
    print_recent_entries(entries, count, 5);
    
    return 0;
}