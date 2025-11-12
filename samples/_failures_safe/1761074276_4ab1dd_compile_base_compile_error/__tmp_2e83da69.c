//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
#define LEVEL_TO_IDX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 3];
} LogEntry;

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

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Line too long\n");
            continue;
        }
        
        if (strlen(line) < TIMESTAMP_LEN + 3) {
            fprintf(stderr, "Invalid log format\n");
            continue;
        }
        
        char* timestamp = line;
        char* level_ptr = line + TIMESTAMP_LEN;
        char* message = level_ptr + 2;
        
        if (level_ptr[0] != ' ' || level_ptr[1] == ' ' || !IS_VALID_LEVEL(level_ptr[1])) {
            fprintf(stderr, "Invalid log level\n");
            continue;
        }
        
        if (message[0] != ' ') {
            fprintf(stderr, "Invalid message format\n");
            continue;
        }
        message++;
        
        if (!parse_timestamp(timestamp)) {
            fprintf(stderr, "Invalid timestamp\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level_ptr[1];
        strncpy(entries[count].message, message, MAX_LINE_LEN - TIMESTAMP_LEN - 3);
        entries[count].message[MAX_LINE_LEN - TIMESTAMP_LEN - 4] = '\0';
        
        char* newline = strchr(entries[count].message, '\n');
        if (newline) *newline = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    char level_names[LOG_LEVELS] = {'E', 'W', 'I', 'D'};
    char* level_descriptions[LOG_LEVELS] = {"Error", "Warning", "Info", "Debug"};
    
    for (int i = 0; i < count; i++) {
        int idx = LEVEL_TO_IDX(entries[i].level);
        if (idx >= 0 && idx < LOG_LEVELS) {
            level_counts[idx]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s (%c): %d (%.1f%%)\n", 
               level_descriptions[i], level_names[i], 
               level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s %c %s\n", 
               entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry:  %s %c %s\n", 
               entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS L message)\n");
    printf("Supported levels: E=Error, W=Warning, I=Info, D=Debug\n");
    printf("Press Ctrl+D to finish input\n\n");
    
    int count =