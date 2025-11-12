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
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    time_t earliest = 0, latest = 0;
    int has_earliest = 0, has_latest = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        struct tm time_struct = {0};
        
        if (sscanf(line, "%63s %15s %255[^\n]",
                   entry.timestamp, entry.level, entry.message) != 3) {
            fprintf(stderr, "Invalid log format. Skipping line.\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &time_struct)) {
            fprintf(stderr, "Invalid timestamp format. Skipping line.\n");
            continue;
        }
        
        time_struct.tm_year -= 1900;
        time_struct.tm_mon -= 1;
        time_t log_time = mktime(&time_struct);
        if (log_time == (time_t)-1) {
            fprintf(stderr, "Invalid timestamp. Skipping line.\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            fprintf(stderr, "Invalid log level. Skipping line.\n");
            continue;
        }
        
        if (!has_earliest || log_time < earliest) {
            earliest = log_time;
            has_earliest = 1;
        }
        if (!has_latest || log_time > latest) {
            latest = log_time;
            has_latest = 1;
        }
        
        if (strcmp(entry.level, "INFO") == 0) info_count++;
        else if (strcmp(entry.level, "WARN") == 0) warn_count++;
        else if (strcmp(entry.level, "ERROR") == 0) error_count++;
        else if (strcmp(entry.level, "DEBUG") == 0) debug_count++;
        
        if (entry_count < MAX_ENTRIES) {
            logs[entry_count] = entry;
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries processed.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    
    if (has_earliest && has_latest) {
        char earliest_str[64], latest_str[64];
        struct tm earliest_tm_buf, latest_tm_buf;
        struct tm* earliest_tm = localtime_r(&earliest, &earliest_tm_buf);
        struct tm* latest_tm = localtime_r(&latest, &latest_tm_buf);
        
        if (earliest_tm && latest_tm) {
            strftime(earliest_str, sizeof(earliest_str), "%Y-%m-%d %H:%M:%S", earliest_tm);
            strftime(latest_str, sizeof(latest_str), "%Y-%m-%d %H:%M:%S", latest_tm);
            printf("Time range: %s to %s\n", earliest_str, latest_str);
            
            double duration = difftime(latest, earliest);
            if (duration >= 0) {
                int hours = (int)(duration / 3600);
                int minutes = (int)((duration - hours * 3600) / 60);
                int seconds = (int)(duration