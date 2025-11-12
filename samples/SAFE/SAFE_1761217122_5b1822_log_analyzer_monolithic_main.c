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
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        struct tm ts;
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]",
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping: %s\\n", line);
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &ts)) {
            printf("Invalid timestamp. Skipping: %s\\n", line);
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            printf("Invalid level. Skipping: %s\\n", line);
            continue;
        }
        
        if (ts.tm_year < 2000 || ts.tm_year > 2100 ||
            ts.tm_mon < 1 || ts.tm_mon > 12 ||
            ts.tm_mday < 1 || ts.tm_mday > 31 ||
            ts.tm_hour < 0 || ts.tm_hour > 23 ||
            ts.tm_min < 0 || ts.tm_min > 59 ||
            ts.tm_sec < 0 || ts.tm_sec > 59) {
            printf("Invalid date/time values. Skipping: %s\\n", line);
            continue;
        }
        
        logs[count] = entry;
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries processed.\\n");
        return 0;
    }
    
    printf("\\nLog Analysis Results:\\n");
    printf("Total entries: %d\\n", count);
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("INFO: %d\\n", info_count);
    printf("WARN: %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    printf("\\nRecent entries (last 5):\\n");
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%s [%s] %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}