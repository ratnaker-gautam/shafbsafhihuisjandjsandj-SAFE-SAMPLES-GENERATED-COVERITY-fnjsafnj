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
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    FILE* input_file;
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int first_valid = 1;
    
    input_file = fopen("logfile.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Cannot open logfile.txt\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), input_file) != NULL && entry_count < MAX_ENTRIES) {
        char ts[32] = {0}, level[16] = {0}, msg[256] = {0};
        struct tm tm_struct = {0};
        time_t entry_time;
        
        if (sscanf(line, "%31s %15s %255[^\n]", ts, level, msg) != 3) {
            continue;
        }
        
        if (!parse_timestamp(ts, &tm_struct)) {
            continue;
        }
        
        tm_struct.tm_year -= 1900;
        tm_struct.tm_mon -= 1;
        entry_time = mktime(&tm_struct);
        if (entry_time == (time_t)-1) {
            continue;
        }
        
        if (!is_valid_level(level)) {
            continue;
        }
        
        if (first_valid) {
            earliest = entry_time;
            latest = entry_time;
            first_valid = 0;
        } else {
            if (entry_time < earliest) earliest = entry_time;
            if (entry_time > latest) latest = entry_time;
        }
        
        strncpy(logs[entry_count].timestamp, ts, sizeof(logs[entry_count].timestamp)-1);
        strncpy(logs[entry_count].level, level, sizeof(logs[entry_count].level)-1);
        strncpy(logs[entry_count].message, msg, sizeof(logs[entry_count].message)-1);
        logs[entry_count].timestamp[sizeof(logs[entry_count].timestamp)-1] = '\0';
        logs[entry_count].level[sizeof(logs[entry_count].level)-1] = '\0';
        logs[entry_count].message[sizeof(logs[entry_count].message)-1] = '\0';
        
        if (strcmp(level, "INFO") == 0) info_count++;
        else if (strcmp(level, "WARN") == 0) warn_count++;
        else if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "DEBUG") == 0) debug_count++;
        
        entry_count++;
    }
    
    if (fclose(input_file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (!first_valid) {
        char time_buf[64];
        struct tm* timeinfo;
        
        timeinfo = localtime(&earliest);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Earliest entry: %s\n", time_buf);
        
        timeinfo = localtime(&latest);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Latest entry: %s\n", time_buf);
        
        double span_hours = difftime(latest, earliest) / 3600.0;
        printf("Time span: %.2f hours\n", span_hours);
    }
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
            }
        }
    }
    
    return 0;
}