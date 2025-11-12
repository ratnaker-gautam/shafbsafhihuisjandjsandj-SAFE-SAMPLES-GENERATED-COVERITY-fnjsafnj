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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_timestamp(const char* str, char* timestamp) {
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
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
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        if (!parse_timestamp(line, timestamp)) {
            continue;
        }
        
        char* level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ' && level_start < line + sizeof(line) - 1) {
            level_start++;
        }
        
        int level = parse_level(level_start);
        if (level == -1) {
            continue;
        }
        
        char* message_start = level_start;
        while (*message_start != ' ' && *message_start != '\0' && 
               message_start < line + sizeof(line) - 1) {
            message_start++;
        }
        while (*message_start == ' ' && message_start < line + sizeof(line) - 1) {
            message_start++;
        }
        
        if (message_start >= line + sizeof(line) - 1) {
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message_start, 
                sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        char* newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void print_summary(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

void print_errors(const LogEntry entries[], int count) {
    printf("\nError entries:\n");
    for (int i = 0; i < count; i++) {
        if (entries[i].level == 3) {
            printf("%s ERROR %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(entries, count);
    print_errors(entries, count);
    
    return 0;
}