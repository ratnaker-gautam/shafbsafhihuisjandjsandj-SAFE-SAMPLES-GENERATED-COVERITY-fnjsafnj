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

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_message(const char* msg) {
    if (strlen(msg) == 0) return 0;
    for (size_t i = 0; msg[i]; i++) {
        if (!isprint((unsigned char)msg[i])) return 0;
    }
    return 1;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_entries) {
        if (line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 2];
        
        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        struct tm tm;
        if (!parse_timestamp(timestamp, &tm)) continue;
        if (!VALID_LEVEL(level)) continue;
        if (!is_valid_message(message)) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        strncpy(entries[count].level, level, LEVEL_LEN - 1);
        entries[count].level[LEVEL_LEN - 1] = '\0';
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

#define COUNT_LEVEL(entries, count, target_level, result) do { \
    result = 0; \
    for (int i = 0; i < count; i++) { \
        if (strcmp(entries[i].level, target_level) == 0) { \
            result++; \
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

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Linux) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}