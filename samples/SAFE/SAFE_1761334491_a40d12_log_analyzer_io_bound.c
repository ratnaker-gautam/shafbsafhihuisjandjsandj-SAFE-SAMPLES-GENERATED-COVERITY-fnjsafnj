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
    char message[256];
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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    line[strcspn(line, "\n")] = 0;
    
    char ts[32], lvl[16], msg[256];
    if (sscanf(line, "%31s %15s %255[^\n]", ts, lvl, msg) != 3) {
        return 0;
    }
    
    struct tm tm;
    if (!parse_timestamp(ts, &tm)) {
        return 0;
    }
    
    if (!is_valid_level(lvl)) {
        return 0;
    }
    
    strncpy(entry->timestamp, ts, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    strncpy(entry->level, lvl, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, msg, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
            }
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
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    while (entry_count < MAX_ENTRIES) {
        if (!read_log_entry(file, &entries[entry_count])) {
            break;
        }
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}