//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    struct LogEntry logs[MAX_ENTRIES];
    int count = 0;
    char filename[256];
    FILE* file;
    char line[MAX_LINE_LEN];
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    struct tm earliest = {0}, latest = {0};
    int first_valid = 0;

    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        struct LogEntry entry;
        int parsed = sscanf(line, "%63s %15s %255[^\n]",
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3 && is_valid_level(entry.level)) {
            struct tm current_tm;
            if (parse_timestamp(entry.timestamp, &current_tm)) {
                current_tm.tm_year -= 1900;
                current_tm.tm_mon -= 1;
                time_t current_time = mktime(&current_tm);
                
                if (current_time != (time_t)-1) {
                    if (!first_valid) {
                        earliest = latest = current_tm;
                        first_valid = 1;
                    } else {
                        time_t earliest_time = mktime(&earliest);
                        time_t latest_time = mktime(&latest);
                        if (difftime(current_time, earliest_time) < 0) {
                            earliest = current_tm;
                        }
                        if (difftime(current_time, latest_time) > 0) {
                            latest = current_tm;
                        }
                    }

                    if (strcmp(entry.level, "INFO") == 0) info_count++;
                    else if (strcmp(entry.level, "WARN") == 0) warn_count++;
                    else if (strcmp(entry.level, "ERROR") == 0) error_count++;
                    else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;

                    logs[count] = entry;
                    count++;
                }
            }
        }
    }

    fclose(file);

    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", count);
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);

    if (first_valid) {
        char earliest_str[64], latest_str[64];
        earliest.tm_year += 1900;
        earliest.tm_mon += 1;
        latest.tm_year += 1900;
        latest.tm_mon += 1;
        
        strftime(earliest_str, sizeof(earliest_str), "%Y-%m-%d %H:%M:%S", &earliest);
        strftime(latest_str, sizeof(latest_str), "%Y-%m-%d %H:%M:%S", &latest);
        
        printf("Time range: %s to %s\n", earliest_str, latest_str);
    }

    printf("\nSample entries:\n");
    int samples = count < 5 ? count : 5;
    for (int i = 0; i < samples; i++) {
        printf("%d. [%s] %s: %s\n", i+1, logs[i].timestamp, logs[i].level, logs[i].message);
    }

    return 0;
}