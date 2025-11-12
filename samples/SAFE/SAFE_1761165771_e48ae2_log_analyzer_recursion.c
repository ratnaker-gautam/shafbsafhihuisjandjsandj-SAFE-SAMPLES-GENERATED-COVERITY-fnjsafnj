//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_file(const char* filename, LogEntry* entries, int max_entries, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;

    while (fgets(line, sizeof(line), file) && *count < max_entries) {
        if (strlen(line) < 30) continue;

        char* timestamp_end = strchr(line, ' ');
        if (!timestamp_end) continue;

        size_t timestamp_len = timestamp_end - line;
        if (timestamp_len >= sizeof(entries[*count].timestamp)) continue;

        strncpy(entries[*count].timestamp, line, timestamp_len);
        entries[*count].timestamp[timestamp_len] = '\0';

        char* level_start = timestamp_end + 1;
        char* level_end = strchr(level_start, ' ');
        if (!level_end) continue;

        size_t level_len = level_end - level_start;
        if (level_len >= sizeof(entries[*count].level)) continue;

        strncpy(entries[*count].level, level_start, level_len);
        entries[*count].level[level_len] = '\0';

        char* message_start = level_end + 1;
        size_t message_len = strlen(message_start);
        if (message_len > 0 && message_start[message_len - 1] == '\n') {
            message_start[message_len - 1] = '\0';
        }
        if (message_len >= sizeof(entries[*count].message)) continue;

        strncpy(entries[*count].message, message_start, sizeof(entries[*count].message) - 1);
        entries[*count].message[sizeof(entries[*count].message) - 1] = '\0';

        (*count)++;
    }

    fclose(file);
    return 0;
}

void recursive_level_count(const LogEntry* entries, int index, int count, int* error_count, int* warning_count, int* info_count) {
    if (index >= count) {
        return;
    }

    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }

    recursive_level_count(entries, index + 1, count, error_count, warning_count, info_count);
}

void recursive_search(const LogEntry* entries, int index, int count, const char* search_term) {
    if (index >= count) {
        return;
    }

    if (strstr(entries[index].message, search_term) != NULL) {
        printf("%s %s: %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
    }

    recursive_search(entries, index + 1, count, search_term);
}

int main() {
    printf("Log Analyzer\n");
    printf("Enter log filename: ");

    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading filename\n");
        return 1;
    }

    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }

    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }

    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;

    if (parse_log_file(filename, entries, MAX_ENTRIES, &entry_count) != 0) {
        printf("Error opening or parsing log file: %s\n", filename);
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }

    printf("\nFound %d log entries\n", entry_count);

    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;

    recursive_level_count(entries, 0, entry_count, &error_count, &warning_count, &info_count);

    printf("\nLog Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);

    printf("\nEnter search term (or press Enter to skip): ");
    char search_term[MAX_LINE_LENGTH];
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        len = strlen(search_term);
        if (len > 0 && search_term[len - 1] == '\n') {
            search_term[len - 1] = '\0';
        }

        if (strlen(search_term) > 0) {
            printf("\nSearch results for '%s':\n", search_term);
            recursive_search(entries, 0, entry_count, search_term);
        }
    }

    return 0;
}