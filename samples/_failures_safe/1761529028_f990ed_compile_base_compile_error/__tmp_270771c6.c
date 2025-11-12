//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[MAX_LINE_LEN];
};

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

enum LogLevel parse_log_level(const char* str) {
    if (strcmp(str, "INFO") == 0) return LOG_INFO;
    if (strcmp(str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

int parse_timestamp(const char* str, time_t* timestamp) {
    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return 0;
    }
    *timestamp = mktime(&tm);
    return 1;
}

int read_log_entries(const char* filename, struct LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    char line[MAX_LINE_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        char timestamp_str[32];
        char level_str[16];
        char message[MAX_LINE_LEN];

        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%31s %15s %255[^\n]", timestamp_str, level_str, message) == 3) {
            time_t timestamp;
            if (parse_timestamp(timestamp_str, &timestamp)) {
                entries[count].timestamp = timestamp;
                entries[count].level = parse_log_level(level_str);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }

    fclose(file);
    return count;
}

void analyze_log_levels(struct LogEntry entries[], int count) {
    int level_counts[5] = {0};

    for (int i = 0; i < count; i++) {
        switch (entries[i].level) {
            case LOG_INFO: level_counts[0]++; break;
            case LOG_WARNING: level_counts[1]++; break;
            case LOG_ERROR: level_counts[2]++; break;
            case LOG_DEBUG: level_counts[3]++; break;
            case LOG_UNKNOWN: level_counts[4]++; break;
        }
    }

    printf("Log Level Analysis:\n");
    printf("INFO: %d\n", level_counts[0]);
    printf("WARNING: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    printf("UNKNOWN: %d\n", level_counts[4]);
}

void filter_by_level(struct LogEntry entries[], int count, enum LogLevel filter_level) {
    printf("\nEntries with level %s:\n", level_to_string(filter_level));
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            char time_buf[32];
            struct tm* tm_info = localtime(&entries[i].timestamp);
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
            printf("%s %s %s\n", time_buf, level_to_string(entries[i].level), entries[i].message);
            found = 1;
        }
    }

    if (!found) {
        printf("No entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[64];

    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';

    int count = read_log_entries(filename, entries, MAX_ENTRIES);
    if (count < 0) {
        fprintf(stderr, "Error reading log file\n");
        return 1;
    }

    if (count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }

    printf("Successfully read %d log entries.\n", count);

    analyze_log_levels(entries, count);

    int choice;
    printf("\nFilter by level (0=INFO, 1=WARNING, 2=ERROR, 3=DEBUG, 4=UNKNOWN, -1