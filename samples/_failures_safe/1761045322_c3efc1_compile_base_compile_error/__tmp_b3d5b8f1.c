//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(ts_str, ts) \
    do { \
        struct tm tm_parse = {0}; \
        if (strptime(ts_str, "%Y-%m-%d %H:%M:%S", &tm_parse) != NULL) { \
            ts = mktime(&tm_parse); \
        } else { \
            ts = -1; \
        } \
    } while(0)

typedef struct {
    time_t timestamp;
    char level[16];
    char message[MAX_LINE_LEN - 40];
} LogEntry;

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    int level_counts[LOG_LEVELS] = {0};
    const char *level_names[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && isspace(line[3])) {
            break;
        }
        
        char timestamp_str[TIMESTAMP_LEN + 1];
        char level[16];
        char message[MAX_LINE_LEN - 40];
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp_str, level, message) != 3) {
            fprintf(stderr, "Invalid log format. Skipping line.\n");
            continue;
        }
        
        if (!IS_VALID_LEVEL(level)) {
            fprintf(stderr, "Invalid log level: %s. Skipping line.\n", level);
            continue;
        }
        
        time_t timestamp;
        PARSE_TIMESTAMP(timestamp_str, timestamp);
        
        if (timestamp == -1) {
            fprintf(stderr, "Invalid timestamp: %s. Skipping line.\n", timestamp_str);
            continue;
        }
        
        if (strlen(message) >= sizeof(entries[entry_count].message)) {
            fprintf(stderr, "Message too long. Truncating.\n");
            message[sizeof(entries[entry_count].message) - 1] = '\0';
        }
        
        entries[entry_count].timestamp = timestamp;
        strcpy(entries[entry_count].level, level);
        strcpy(entries[entry_count].message, message);
        
        for (int i = 0; i < LOG_LEVELS; i++) {
            if (strcmp(level, level_names[i]) == 0) {
                level_counts[i]++;
                break;
            }
        }
        
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nLevel distribution:\n");
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = (double)level_counts[i] / entry_count * 100.0;
        printf("%-6s: %3d (%5.1f%%)\n", level_names[i], level_counts[i], percentage);
    }
    
    time_t earliest = entries[0].timestamp;
    time_t latest = entries[0].timestamp;
    
    for (int i = 1; i < entry_count; i++) {
        if (entries[i].timestamp < earliest) {
            earliest = entries[i].timestamp;
        }
        if (entries[i].timestamp > latest) {
            latest = entries[i].timestamp;
        }
    }
    
    printf("\nTime range:\n");
    char time_buf[64];
    struct tm tm_early;
    struct tm *tm_info = localtime_r(&earliest, &tm_early);
    if (tm_info != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Earliest: %s\n", time_buf);
    }
    
    struct tm tm_late;
    tm_info = localtime_r(&latest, &tm_late);
    if (tm_info != NULL) {
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Latest:   %s\n", time_buf);
    }
    
    printf("\nSample entries:\n");
    int samples = (entry_count < 3) ? entry_count : 3;
    for (int i = 0; i < samples; i++) {
        struct tm tm_sample;
        tm_info = localtime_r(&entries[i].timestamp, &tm