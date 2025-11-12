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

#define LEVEL_ERROR "ERROR"
#define LEVEL_WARN  "WARN"
#define LEVEL_INFO  "INFO"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LEVEL_ERROR) == 0 || \
     strcmp(level, LEVEL_WARN) == 0 || \
     strcmp(level, LEVEL_INFO) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - MAX_LEVEL_LEN - 3];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_buf[MAX_LEVEL_LEN];
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
    
    strncpy(entry->level, level_buf, MAX_LEVEL_LEN - 1);
    entry->level[MAX_LEVEL_LEN - 1] = '\0';
    
    const char* message_start = strchr(line, ']');
    if (!message_start) return 0;
    message_start++;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) {
        message_len = sizeof(entry->message) - 1;
    }
    
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry entries[], int count) {
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, LEVEL_ERROR) == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, LEVEL_WARN) == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, LEVEL_INFO) == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (int i = count - 1; i >= 0 && error_count > 0; i--) {
            if (strcmp(entries[i].level, LEVEL_ERROR) == 0) {
                printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
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
    
    printf("Enter log entries (format: TIMESTAMP [LEVEL] MESSAGE)\n");
    printf("Supported levels: ERROR, WARN, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Line too long, skipping\n");
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}