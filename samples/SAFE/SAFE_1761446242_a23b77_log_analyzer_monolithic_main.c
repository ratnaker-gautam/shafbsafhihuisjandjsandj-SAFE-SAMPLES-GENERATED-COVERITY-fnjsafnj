//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
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
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char filename[256];
    char line[MAX_LINE_LENGTH];
    FILE* file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;

    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL && entry_count < MAX_ENTRIES) {
        char timestamp[64], level[16], message[256];
        struct tm tm = {0};
        
        if (sscanf(line, "%63s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (parse_timestamp(timestamp, &tm) && is_valid_level(level)) {
                tm.tm_year -= 1900;
                tm.tm_mon -= 1;
                time_t entry_time = mktime(&tm);
                
                if (entry_time != -1) {
                    strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
                    strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
                    strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
                    entries[entry_count].timestamp[sizeof(entries[entry_count].timestamp) - 1] = '\0';
                    entries[entry_count].level[sizeof(entries[entry_count].level) - 1] = '\0';
                    entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';
                    
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
            }
        }
    }

    fclose(file);

    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("DEBUG entries: %d\n", debug_count);
    
    if (!first_valid) {
        double time_span = difftime(latest, earliest);
        int hours = (int)(time_span / 3600);
        int minutes = (int)((time_span - hours * 3600) / 60);
        printf("Time span: %d hours, %d minutes\n", hours, minutes);
    }

    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }

    return 0;
}