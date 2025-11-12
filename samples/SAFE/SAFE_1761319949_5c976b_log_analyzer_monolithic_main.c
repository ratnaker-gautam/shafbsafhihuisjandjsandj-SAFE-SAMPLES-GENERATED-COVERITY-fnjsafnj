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
    const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    FILE* file;
    
    file = fopen("sample.log", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open log file\n");
        return 1;
    }
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), file) != NULL) {
        char ts[64], level[16], msg[256];
        struct tm time_struct = {0};
        
        if (sscanf(line, "%63s %15s %255[^\n]", ts, level, msg) != 3) {
            continue;
        }
        
        if (!parse_timestamp(ts, &time_struct)) {
            continue;
        }
        
        if (!validate_log_level(level)) {
            continue;
        }
        
        if (strlen(msg) == 0) {
            continue;
        }
        
        strncpy(logs[entry_count].timestamp, ts, sizeof(logs[entry_count].timestamp) - 1);
        logs[entry_count].timestamp[sizeof(logs[entry_count].timestamp) - 1] = '\0';
        strncpy(logs[entry_count].level, level, sizeof(logs[entry_count].level) - 1);
        logs[entry_count].level[sizeof(logs[entry_count].level) - 1] = '\0';
        strncpy(logs[entry_count].message, msg, sizeof(logs[entry_count].message) - 1);
        logs[entry_count].message[sizeof(logs[entry_count].message) - 1] = '\0';
        
        entry_count++;
    }
    
    fclose(file);
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}