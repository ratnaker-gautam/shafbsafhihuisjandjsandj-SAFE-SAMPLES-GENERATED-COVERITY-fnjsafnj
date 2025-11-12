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

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;

    file = fopen("sample.log", "r");
    if (file == NULL) {
        printf("Error: Cannot open log file\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL && log_count < MAX_ENTRIES) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        struct tm tm = {0};
        time_t log_time;

        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
            continue;
        }

        if (!parse_timestamp(timestamp, &tm)) {
            continue;
        }

        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        log_time = mktime(&tm);
        if (log_time == (time_t)-1) {
            continue;
        }

        if (!is_valid_level(level)) {
            continue;
        }

        if (first_valid) {
            earliest = log_time;
            latest = log_time;
            first_valid = 0;
        } else {
            if (log_time < earliest) earliest = log_time;
            if (log_time > latest) latest = log_time;
        }

        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        strncpy(logs[log_count].level, level, sizeof(logs[log_count].level) - 1);
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\0';
        logs[log_count].level[sizeof(logs[log_count].level) - 1] = '\0';
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\0';

        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;

        log_count++;
    }

    fclose(file);

    printf("Log Analysis Report\n");
    printf("==================\n");
    printf("Total entries processed: %d\n", log_count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);

    if (!first_valid) {
        char time_buf[64];
        struct tm* timeinfo;

        timeinfo = localtime(&earliest);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Earliest: %s\n", time_buf);

        timeinfo = localtime(&latest);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Latest:   %s\n", time_buf);

        double span = difftime(latest, earliest);
        printf("Time span: %.0f seconds\n", span);
    }

    printf("\nRecent entries:\n");
    int start = (log_count > 5) ? log_count - 5 : 0;
    for (int i = start; i < log_count; i++) {
        printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }

    return 0;
}