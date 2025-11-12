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
#define LEVEL_DEBUG "DEBUG"

#define IS_VALID_LEVEL(level) \
    (strcmp(level, LEVEL_ERROR) == 0 || \
     strcmp(level, LEVEL_WARN) == 0 || \
     strcmp(level, LEVEL_INFO) == 0 || \
     strcmp(level, LEVEL_DEBUG) == 0)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[MAX_LEVEL_LEN];
    char message[MAX_LINE_LEN];
} LogEntry;

LogEntry logs[MAX_ENTRIES];
int log_count = 0;

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
    }
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';

    char* timestamp_part = strtok(temp, " ");
    if (!timestamp_part) return 0;
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;

    char* level_part = strtok(NULL, " ");
    if (!level_part) return 0;
    if (strlen(level_part) >= MAX_LEVEL_LEN) return 0;
    strncpy(entry->level, level_part, MAX_LEVEL_LEN - 1);
    entry->level[MAX_LEVEL_LEN - 1] = '\0';
    if (!IS_VALID_LEVEL(entry->level)) return 0;

    char* message_part = strtok(NULL, "\n");
    if (!message_part) return 0;
    if (strlen(message_part) >= MAX_LINE_LEN) return 0;
    strncpy(entry->message, message_part, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';

    return 1;
}

void load_logs() {
    char line[MAX_LINE_LEN];
    printf("Enter log entries (empty line to finish):\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
}

void analyze_logs() {
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = 0; i < log_count; i++) {
        if (strcmp(logs[i].level, LEVEL_ERROR) == 0) error_count++;
        else if (strcmp(logs[i].level, LEVEL_WARN) == 0) warn_count++;
        else if (strcmp(logs[i].level, LEVEL_INFO) == 0) info_count++;
        else if (strcmp(logs[i].level, LEVEL_DEBUG) == 0) debug_count++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < log_count; i++) {
            if (strcmp(logs[i].level, LEVEL_ERROR) == 0) {
                printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
}

int main() {
    load_logs();
    if (log_count > 0) {
        analyze_logs();
    } else {
        printf("No valid log entries to analyze.\n");
    }
    return 0;
}