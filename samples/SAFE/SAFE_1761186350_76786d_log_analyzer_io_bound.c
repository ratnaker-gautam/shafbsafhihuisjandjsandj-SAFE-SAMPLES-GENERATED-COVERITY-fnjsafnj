//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 ||
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int read_log_entries(FILE* file, struct LogEntry* entries, int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        struct LogEntry entry;
        char ts[32], lvl[16], msg[MAX_LINE_LEN];
        
        if (sscanf(line, "%31s %15s %1023[^\n]", ts, lvl, msg) == 3) {
            if (strlen(ts) < sizeof(entry.timestamp) &&
                strlen(lvl) < sizeof(entry.level) &&
                strlen(msg) < sizeof(entry.message)) {
                
                strcpy(entry.timestamp, ts);
                strcpy(entry.level, lvl);
                strcpy(entry.message, msg);
                
                struct tm tm;
                if (parse_timestamp(entry.timestamp, &tm) && is_valid_level(entry.level)) {
                    entries[count] = entry;
                    count++;
                }
            }
        }
    }
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (count > 0) {
        printf("\nSample entries:\n");
        for (int i = 0; i < (count < 3 ? count : 3); i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_entries(file, entries, MAX_ENTRIES);
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found or file is empty\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    return 0;
}