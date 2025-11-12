//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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
    return strptime(ts_str, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int validate_log_level(const char* level) {
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int count = 0;
    char line[MAX_LINE_LEN];
    FILE* file;
    
    file = fopen("sample.log", "r");
    if (!file) {
        printf("Error: Cannot open log file\n");
        return 1;
    }
    
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), file)) {
        char ts[64], level[16], msg[256];
        struct tm time_struct;
        
        if (sscanf(line, "%63s %15s %255[^\n]", ts, level, msg) == 3) {
            if (parse_timestamp(ts, &time_struct) && validate_log_level(level)) {
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
    
    printf("\nRecent ERROR entries:\n");
    int error_shown = 0;
    for (int i = count - 1; i >= 0 && error_shown < 3; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("%s: %s\n", logs[i].timestamp, logs[i].message);
            error_shown++;
        }
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found\n");
    }
    
    return 0;
}