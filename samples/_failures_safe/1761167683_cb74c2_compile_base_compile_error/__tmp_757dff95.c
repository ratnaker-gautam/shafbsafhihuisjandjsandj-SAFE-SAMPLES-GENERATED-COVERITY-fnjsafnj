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
#define LEVEL_TO_IDX(c) ((c) == 'E' ? 0 : (c) == 'W' ? 1 : (c) == 'I' ? 2 : 3)
#define LEVEL_NAME(i) ((i) == 0 ? "ERROR" : (i) == 1 ? "WARN" : (i) == 2 ? "INFO" : "DEBUG")

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
    int count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS L message)\n");
    printf("L = E(ERROR), W(WARN), I(INFO), D(DEBUG)\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) break;
        
        if (len < TIMESTAMP_LEN + 3) {
            printf("Line too short, skipping.\n");
            continue;
        }
        
        if (len >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        char* timestamp_part = line;
        char* level_part = line + TIMESTAMP_LEN;
        char* message_part = level_part + 2;
        
        if (level_part[0] != ' ') {
            printf("Invalid format, missing space after timestamp.\n");
            continue;
        }
        
        if (!IS_VALID_LEVEL(level_part[1])) {
            printf("Invalid log level.\n");
            continue;
        }
        
        if (message_part[0] != ' ') {
            printf("Invalid format, missing space after level.\n");
            continue;
        }
        
        message_part++;
        
        if (!parse_timestamp(timestamp_part)) {
            printf("Invalid timestamp format.\n");
            continue;
        }
        
        LogEntry* entry = &entries[count];
        strncpy(entry->timestamp, timestamp_part, TIMESTAMP_LEN - 1);
        entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
        entry->level = level_part[1];
        strncpy(entry->message, message_part, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        int level_idx = LEVEL_TO_IDX(entries[i].level);
        if (level_idx >= 0 && level_idx < LOG_LEVELS) {
            level_counts[level_idx]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
    
    if (count > 0) {
        printf("\nMost recent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("[%s] %c: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int