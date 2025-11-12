//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;

    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        char ts[32] = {0};
        char level[16] = {0};
        char msg[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", ts, level, msg) == 3) {
            struct tm tm = {0};
            if (parse_timestamp(ts, &tm) && is_valid_level(level)) {
                strncpy(entries[entry_count].timestamp, ts, sizeof(entries[entry_count].timestamp) - 1);
                strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                strncpy(entries[entry_count].message, msg, sizeof(entries[entry_count].message) - 1);
                entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
                entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
                entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                
                if (strcmp(level, "ERROR") == 0) error_count++;
                else if (strcmp(level, "WARN") == 0) warn_count++;
                else if (strcmp(level, "INFO") == 0) info_count++;
                else if (strcmp(level, "DEBUG") == 0) debug_count++;
                
                entry_count++;
            }
        }
    }

    fclose(file);

    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);

    if (entry_count > 0) {
        printf("\nLast 5 entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }

    return 0;
}