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
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char ts[TIMESTAMP_LEN], lvl[LEVEL_LEN], msg[MAX_LINE_LEN];
        int parsed = sscanf(line, "%19s %9s %[^\n]", ts, lvl, msg);
        
        if (parsed == 3 && parse_timestamp(ts) && IS_VALID_LEVEL(lvl)) {
            strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
            entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
            strncpy(entries[count].level, lvl, LEVEL_LEN - 1);
            entries[count].level[LEVEL_LEN - 1] = '\0';
            strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            count++;
        }
    }
    
    return count;
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

int main() {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (count >= MAX_ENTRIES) {
        printf("Warning: Reached maximum entry limit (%d)\n", MAX_ENTRIES);
    }
    
    analyze_logs(entries, count);
    
    return 0;
}