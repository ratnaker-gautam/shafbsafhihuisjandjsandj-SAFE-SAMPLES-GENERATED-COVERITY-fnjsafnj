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
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strncmp(str, level_names[i], strlen(level_names[i])) == 0) {
            return i;
        }
    }
    return -1;
}

int validate_timestamp(const char *timestamp) {
    if (strlen(timestamp) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (!IS_TIMESTAMP_CHAR(timestamp[i])) return 0;
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        LogEntry *entry = &entries[count];
        
        if (strlen(line) < TIMESTAMP_LEN + 2) continue;
        
        strncpy(entry->timestamp, line, TIMESTAMP_LEN - 1);
        entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
        
        if (!validate_timestamp(entry->timestamp)) continue;
        
        const char *level_start = line + TIMESTAMP_LEN;
        while (*level_start == ' ' && level_start < line + sizeof(line) - 1) {
            level_start++;
        }
        
        entry->level = parse_log_level(level_start);
        if (!IS_VALID_LEVEL(entry->level)) continue;
        
        const char *message_start = level_start + strlen(LEVEL_NAME(entry->level));
        while (*message_start == ' ' && message_start < line + sizeof(line) - 1) {
            message_start++;
        }
        
        size_t message_len = strlen(message_start);
        if (message_len > 0 && message_start[message_len - 1] == '\n') {
            message_len--;
        }
        
        if (message_len >= sizeof(entry->message)) {
            message_len = sizeof(entry->message) - 1;
        }
        
        strncpy(entry->message, message_start, message_len);
        entry->message[message_len] = '\0';
        
        count++;
    }
    
    return count;
}

void generate_summary(const LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (timestamp level message):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\\n\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    generate_summary(entries, count);
    
    return 0;
}