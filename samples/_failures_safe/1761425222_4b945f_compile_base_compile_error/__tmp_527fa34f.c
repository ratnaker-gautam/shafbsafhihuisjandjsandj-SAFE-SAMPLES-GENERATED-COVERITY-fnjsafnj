//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_WARN "WARN"
#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LOG_LEVEL_ERROR) == 0 || \
     strcmp(level, LOG_LEVEL_WARN) == 0 || \
     strcmp(level, LOG_LEVEL_INFO) == 0 || \
     strcmp(level, LOG_LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[LEVEL_LEN];
    char timestamp_buf[TIMESTAMP_LEN];
    
    if (sscanf(line, "%19s %9s", timestamp_buf, level_buf) != 2) {
        return 0;
    }
    
    if (!IS_VALID_LEVEL(level_buf)) {
        return 0;
    }
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (timestamp_buf[i] == '\0') break;
        if (!isdigit(timestamp_buf[i]) && timestamp_buf[i] != '-' && timestamp_buf[i] != ':' && timestamp_buf[i] != '.') {
            return 0;
        }
    }
    
    strncpy(entry->timestamp, timestamp_buf, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    
    strncpy(entry->level, level_buf, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    
    const char* message_start = line + strlen(timestamp_buf) + strlen(level_buf) + 2;
    if (*message_start == ' ') message_start++;
    
    strncpy(entry->message, message_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    size_t len = strlen(entry->message);
    if (len > 0 && entry->message[len - 1] == '\n') {
        entry->message[len - 1] = '\0';
    }
    
    return 1;
}

void analyze_logs(LogEntry entries[], int count) {
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
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = count - 1; i >= 0 && error_count > 0; i--) {
            if (strcmp(entries[i].level, LOG_LEVEL_ERROR) == 0) {
                printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                error_count--;
                if (error_count == 0) break;
            }
        }
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        if (strlen(line) == 0 || line[0] == '\n') {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %