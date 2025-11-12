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
    char line[MAX_LINE_LEN];
    FILE* file;
    
    file = fopen("sample.log", "r");
    if (file == NULL) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENTRIES) {
        char ts[32], level[16], msg[256];
        struct tm entry_time;
        
        if (sscanf(line, "%31s %15s %255[^\n]", ts, level, msg) == 3) {
            if (parse_timestamp(ts, &entry_time) && is_valid_level(level)) {
                strncpy(logs[count].timestamp, ts, sizeof(logs[count].timestamp) - 1);
                strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
                strncpy(logs[count].message, msg, sizeof(logs[count].message) - 1);
                logs[count].timestamp[sizeof(logs[count].timestamp) - 1] = '\0';
                logs[count].level[sizeof(logs[count].level) - 1] = '\0';
                logs[count].message[sizeof(logs[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nRecent entries:\n");
    int display_count = count > 5 ? 5 : count;
    for (int i = count - display_count; i < count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}