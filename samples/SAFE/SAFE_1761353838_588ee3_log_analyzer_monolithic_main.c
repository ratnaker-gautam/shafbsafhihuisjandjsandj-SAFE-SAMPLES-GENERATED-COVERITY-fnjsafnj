//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
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

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;

    file = fopen("sample.log", "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open sample.log\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[MAX_LINE_LENGTH] = {0};
        struct tm tm = {0};
        time_t entry_time;

        if (sscanf(line, "%31s %15s %1023[^\n]", timestamp, level, message) != 3) {
            continue;
        }

        if (!parse_timestamp(timestamp, &tm)) {
            continue;
        }

        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        entry_time = mktime(&tm);
        if (entry_time == (time_t)-1) {
            continue;
        }

        if (!is_valid_level(level)) {
            continue;
        }

        strncpy(logs[entry_count].timestamp, timestamp, sizeof(logs[entry_count].timestamp) - 1);
        strncpy(logs[entry_count].level, level, sizeof(logs[entry_count].level) - 1);
        strncpy(logs[entry_count].message, message, sizeof(logs[entry_count].message) - 1);
        logs[entry_count].timestamp[sizeof(logs[entry_count].timestamp) - 1] = '\0';
        logs[entry_count].level[sizeof(logs[entry_count].level) - 1] = '\0';
        logs[entry_count].message[sizeof(logs[entry_count].message) - 1] = '\0';

        if (first_valid) {
            earliest = entry_time;
            latest = entry_time;
            first_valid = 0;
        } else {
            if (entry_time < earliest) earliest = entry_time;
            if (entry_time > latest) latest = entry_time;
        }

        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;

        entry_count++;
    }

    fclose(file);

    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Time range: %s to %s\n", ctime(&earliest), ctime(&latest));
    printf("Level distribution:\n");
    printf("  INFO:  %d\n", info_count);
    printf("  WARN:  %d\n", warn_count);
    printf("  ERROR: %d\n", error_count);
    printf("  DEBUG: %d\n", debug_count);

    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("  %s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }

    return 0;
}