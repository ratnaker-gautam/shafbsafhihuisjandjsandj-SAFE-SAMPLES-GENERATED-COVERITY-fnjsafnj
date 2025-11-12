//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

int read_log_entries(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) {
            printf("Warning: Line too long, skipping\n");
            continue;
        }

        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};

        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed < 3) {
            printf("Warning: Invalid log format, skipping line\n");
            continue;
        }

        struct tm tm = {0};
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Warning: Invalid timestamp format, skipping line\n");
            continue;
        }

        if (!is_valid_level(level)) {
            printf("Warning: Invalid log level, skipping line\n");
            continue;
        }

        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
        entries[count].level[sizeof(entries[count].level) - 1] = '\0';
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        count++;
    }

    if (ferror(file)) {
        printf("Error: File read error\n");
        fclose(file);
        return -1;
    }

    fclose(file);
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
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);

    if (count > 0) {
        printf("\nLast 5 entries:\n");
        int start = count > 5 ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_entries(argv[1], entries, MAX_ENTRIES);

    if (count < 0) {
        return 1;
    }

    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    analyze_logs(entries, count);
    return 0;
}