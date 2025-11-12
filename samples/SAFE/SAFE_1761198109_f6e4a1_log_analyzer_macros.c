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

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 2];
        
        int scanned = sscanf(line, "%19s %9s %[^\n]", timestamp, level, message);
        if (scanned != 3) continue;
        
        if (!parse_timestamp(timestamp)) continue;
        if (!VALID_LEVEL(level)) continue;
        if (strlen(message) >= sizeof(entries[count].message)) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        
        entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

#define COUNT_LEVEL(entries, count, target_level, counter) \
    do { \
        counter = 0; \
        for (int i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                counter++; \
            } \
        } \
    } while(0)

void analyze_logs(LogEntry entries[], int count) {
    int info_count, warn_count, error_count;
    
    COUNT_LEVEL(entries, count, "INFO", info_count);
    COUNT_LEVEL(entries, count, "WARN", warn_count);
    COUNT_LEVEL(entries, count, "ERROR", error_count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

#define CHECK_ERROR(condition, message) \
    do { \
        if (condition) { \
            fprintf(stderr, "Error: %s\n", message); \
            exit(1); \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    CHECK_ERROR(count == 0, "No valid log entries found");
    
    analyze_logs(entries, count);
    
    return 0;
}