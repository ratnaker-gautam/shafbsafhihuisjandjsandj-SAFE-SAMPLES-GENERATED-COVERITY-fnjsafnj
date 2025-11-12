//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char *level) {
    const char *valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (size_t i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int read_log_entry(FILE *file, struct LogEntry *entry) {
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }

    char *ptr = line;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }

    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL || (timestamp_end - ptr) >= 32) {
        return 0;
    }
    strncpy(entry->timestamp, ptr, timestamp_end - ptr);
    entry->timestamp[timestamp_end - ptr] = '\0';

    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }

    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL || (level_end - ptr) >= 16) {
        return 0;
    }
    strncpy(entry->level, ptr, level_end - ptr);
    entry->level[level_end - ptr] = '\0';

    if (!is_valid_level(entry->level)) {
        return 0;
    }

    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) {
        ptr++;
    }

    size_t msg_len = strlen(ptr);
    if (msg_len == 0 || msg_len >= 256) {
        return 0;
    }
    if (ptr[msg_len - 1] == '\n') {
        ptr[msg_len - 1] = '\0';
        msg_len--;
    }
    strncpy(entry->message, ptr, 255);
    entry->message[255] = '\0';

    return 1;
}

void analyze_logs(struct LogEntry *logs, size_t count) {
    int level_counts[4] = {0};
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};

    for (size_t i = 0; i < count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(logs[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }

    printf("Log Analysis Summary:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t count = 0;

    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Press Ctrl+D to finish input\n");

    while (count < MAX_ENTRIES) {
        struct LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            if (feof(stdin)) {
                break;
            }
            printf("Invalid log entry format. Skipping.\n");
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                break;
            }
            continue;
        }

        struct tm tm;
        if (!parse_timestamp(entry.timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }

        logs[count] = entry;
        count++;
    }

    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }

    analyze_logs(logs, count);

    printf("\nFirst 5 log entries:\n");
    size_t display_count = count < 5 ? count : 5;
    for (size_t i = 0; i < display_count; i++) {
        printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }

    return 0;
}