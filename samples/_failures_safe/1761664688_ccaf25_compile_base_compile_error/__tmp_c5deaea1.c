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

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_timestamp(const char* timestamp) {
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    if (tm.tm_year < 2000 || tm.tm_year > 2100) return 0;
    if (tm.tm_mon < 1 || tm.tm_mon > 12) return 0;
    if (tm.tm_mday < 1 || tm.tm_mday > 31) return 0;
    if (tm.tm_hour < 0 || tm.tm_hour > 23) return 0;
    if (tm.tm_min < 0 || tm.tm_min > 59) return 0;
    if (tm.tm_sec < 0 || tm.tm_sec > 59) return 0;
    return 1;
}

int read_log_entry(FILE* file, LogEntry* entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
    
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %[^\n]", timestamp, level, message) != 3) return 0;
    
    if (!is_valid_timestamp(timestamp)) return 0;
    if (!VALID_LEVEL(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN - 1);
    entry->timestamp[TIMESTAMP_LEN - 1] = '\0';
    strncpy(entry->level, level, LEVEL_LEN - 1);
    entry->level[LEVEL_LEN - 1] = '\0';
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(LogEntry entries[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: INFO, WARNING, ERROR\n");
    printf("Enter 'quit' on a new line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (!read_log_entry(stdin, &entries[entry_count])) {
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
                if (strcmp(buffer, "quit") == 0) break;
            }
            printf("Invalid log entry format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");