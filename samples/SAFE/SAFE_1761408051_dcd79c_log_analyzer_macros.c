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

int is_valid_log_entry(const LogEntry* entry) {
    struct tm tm = {0};
    if (!parse_timestamp(entry->timestamp, &tm)) return 0;
    if (!VALID_LEVEL(entry->level)) return 0;
    if (strlen(entry->message) == 0) return 0;
    return 1;
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        LogEntry entry = {0};
        int scanned = sscanf(line, "%19s %15s %[^\n]", 
                            entry.timestamp, entry.level, entry.message);
        
        if (scanned == 3 && is_valid_log_entry(&entry)) {
            entries[count++] = entry;
        }
    }
    
    fclose(file);
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
        printf("\nRecent entries:\n");
        int show_count = count > 5 ? 5 : count;
        for (int i = count - show_count; i < count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (count < 0) {
        fprintf(stderr, "Error opening or reading file: %s\n", filename);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, count);
    return 0;
}