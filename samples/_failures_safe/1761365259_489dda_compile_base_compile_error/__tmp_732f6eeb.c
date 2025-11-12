//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int compare_entries(const void* a, const void* b) {
    const struct LogEntry* entry_a = (const struct LogEntry*)a;
    const struct LogEntry* entry_b = (const struct LogEntry*)b;
    struct tm tm_a, tm_b;
    
    if (parse_timestamp(entry_a->timestamp, &tm_a) && parse_timestamp(entry_b->timestamp, &tm_b)) {
        time_t time_a = mktime(&tm_a);
        time_t time_b = mktime(&tm_b);
        if (time_a != -1 && time_b != -1) {
            if (time_a < time_b) return -1;
            if (time_a > time_b) return 1;
        }
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[32], level[16], message[MAX_LINE_LEN];
        int parsed = sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message);
        
        if (parsed == 3) {
            struct tm tm_temp;
            if (parse_timestamp(timestamp, &tm_temp) && is_valid_level(level)) {
                strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].timestamp[31] = '\0';
                entries[entry_count].level[15] = '\0';
                entries[entry_count].message[MAX_LINE_LEN - 1] = '\0';
                entry_count++;
            } else {
                printf("Invalid timestamp or log level. Skipping: %s", line);
            }
        } else {
            printf("Invalid format. Skipping: %s", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    qsort(entries, entry_count, sizeof(struct LogEntry), compare_entries);
    
    printf("\n=== Log Analysis Report ===\n");
    printf("Total entries: %d\n", entry_count);
    
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("INFO: %d (%.1f%%)\n", info_count, entry_count > 0 ? (100.0 * info_count / entry_count) : 0.0);
    printf("WARN: %d (%.1f%%)\n", warn_count, entry_count > 0 ? (100.0 * warn_count / entry_count) : 0.0);
    printf("ERROR: %d (%.1f%%)\n", error_count, entry_count > 0 ? (100.0 * error_count / entry_count) : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, entry_count > 0 ? (100.0 * debug_count / entry_count) : 0.0);
    
    printf("\n=== Sorted Log Entries ===\n");
    for (int i = 0; i < entry_count; i++) {
        printf