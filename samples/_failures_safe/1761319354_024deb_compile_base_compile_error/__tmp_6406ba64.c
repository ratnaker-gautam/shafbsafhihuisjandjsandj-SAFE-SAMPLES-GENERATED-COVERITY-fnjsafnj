//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LEN 10
#define TIMESTAMP_LEN 20

#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_WARN  "WARN"
#define LOG_LEVEL_INFO  "INFO"
#define LOG_LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LOG_LEVEL_ERROR) == 0 || \
     strcmp(level, LOG_LEVEL_WARN) == 0 || \
     strcmp(level, LOG_LEVEL_INFO) == 0 || \
     strcmp(level, LOG_LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN + 1];
    char level[MAX_LEVEL_LEN + 1];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - MAX_LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    size_t len = strlen(line);
    if (len < TIMESTAMP_LEN + 3 || len >= MAX_LINE_LEN) return 0;
    
    if (line[TIMESTAMP_LEN] != ' ') return 0;
    
    strncpy(entry->timestamp, line, TIMESTAMP_LEN);
    entry->timestamp[TIMESTAMP_LEN] = '\0';
    
    for (int i = 0; i < TIMESTAMP_LEN; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) {
            if (entry->timestamp[i] != '-' && entry->timestamp[i] != ' ' && 
                entry->timestamp[i] != ':') return 0;
        } else if (!isdigit((unsigned char)entry->timestamp[i])) {
            return 0;
        }
    }
    
    const char* level_start = line + TIMESTAMP_LEN + 1;
    const char* level_end = strchr(level_start, ' ');
    if (!level_end) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len == 0 || level_len > MAX_LEVEL_LEN) return 0;
    
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    if (!IS_VALID_LEVEL(entry->level)) return 0;
    
    const char* message_start = level_end + 1;
    if (*message_start == '\0') return 0;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) error_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_WARN) == 0) warn_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_INFO) == 0) info_count++;
        else if (strcmp(entries[i].level, LOG_LEVEL_DEBUG) == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nLast 3 entries:\n");
        int start = (count > 3) ? count - 3 : 0;
        for (int i = start; i < count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Example: 2023-10-15 14:30:25 ERROR Database connection failed\n");
    printf("Enter 'quit' on a separate line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';