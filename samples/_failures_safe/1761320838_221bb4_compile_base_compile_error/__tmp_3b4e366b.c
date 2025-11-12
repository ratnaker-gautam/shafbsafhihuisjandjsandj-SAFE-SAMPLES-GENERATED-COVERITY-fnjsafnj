//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

#define PARSE_TIMESTAMP(ts, buf) do { \
    if (strlen(buf) < TIMESTAMP_LEN - 1) break; \
    strncpy(ts, buf, TIMESTAMP_LEN - 1); \
    ts[TIMESTAMP_LEN - 1] = '\0'; \
} while(0)

#define PARSE_LEVEL(lvl, buf) do { \
    if (strlen(buf) < LEVEL_LEN - 1) break; \
    strncpy(lvl, buf, LEVEL_LEN - 1); \
    lvl[LEVEL_LEN - 1] = '\0'; \
} while(0)

int validate_timestamp(const char* ts) {
    if (strlen(ts) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (ts[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (ts[i] != ':') return 0;
        } else {
            if (!isdigit(ts[i])) return 0;
        }
    }
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "");
    if (!message) return 0;
    
    if (!validate_timestamp(timestamp)) return 0;
    if (!IS_VALID_LEVEL(level)) return 0;
    
    PARSE_TIMESTAMP(entry->timestamp, timestamp);
    PARSE_LEVEL(entry->level, level);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void generate_sample_logs(LogEntry logs[], int* count) {
    if (!logs || !count || *count >= MAX_ENTRIES) return;
    
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    const char* messages[] = {
        "System startup completed",
        "User login successful",
        "Database connection established",
        "High memory usage detected",
        "Backup process started",
        "Network latency above threshold",
        "Critical error in payment processing",
        "Security audit passed",
        "Disk space running low",
        "Application shutdown initiated"
    };
    
    time_t now = time(NULL);
    struct tm tm_info;
    if (localtime_r(&now, &tm_info) == NULL) return;
    
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < 10 && *count < MAX_ENTRIES; i++) {
        LogEntry* entry = &logs[*count];
        
        tm_info.tm_sec += 5;
        mktime(&tm_info);
        strftime(entry->timestamp, TIMESTAMP_LEN, "%Y-%m-%d %H:%M:%S", &tm_info);
        
        strncpy(entry->level, levels[i % num_levels], LEVEL_LEN - 1);
        entry->level[LEVEL_LEN - 1] = '\0';
        
        strncpy(entry->message, messages[i % num_messages], sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        (*count)++;
    }
}

void analyze_logs(const LogEntry logs[], int count) {
    if (!logs || count <= 0) return;
    
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("