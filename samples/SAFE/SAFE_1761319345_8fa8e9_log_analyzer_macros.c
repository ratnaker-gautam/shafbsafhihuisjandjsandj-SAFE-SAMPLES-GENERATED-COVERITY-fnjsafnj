//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define IS_VALID_LEVEL(l) (strcmp(l, "INFO") == 0 || strcmp(l, "WARNING") == 0 || strcmp(l, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        char timestamp[TIMESTAMP_LEN] = {0};
        char level[LEVEL_LEN] = {0};
        char message[MAX_LINE_LEN] = {0};
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp, level, message) == 3) {
            if (parse_timestamp(timestamp) && IS_VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
                strncpy(entries[count].level, level, LEVEL_LEN - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                entries[count].level[LEVEL_LEN - 1] = '\0';
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    return count;
}

#define COUNT_LEVEL(e, l, c) do { \
    if (strcmp((e).level, l) == 0) { \
        (c)++; \
    } \
} while(0)

void analyze_logs(LogEntry entries[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        COUNT_LEVEL(entries[i], "INFO", info_count);
        COUNT_LEVEL(entries[i], "WARNING", warning_count);
        COUNT_LEVEL(entries[i], "ERROR", error_count);
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nFirst timestamp: %s\n", entries[0].timestamp);
        printf("Last timestamp: %s\n", entries[count - 1].timestamp);
    }
}

#define CHECK_ERROR(c, msg) do { \
    if (c) { \
        fprintf(stderr, "Error: %s\n", msg); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    CHECK_ERROR(count == 0, "No valid log entries found");
    
    analyze_logs(entries, count);
    
    return EXIT_SUCCESS;
}