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
    char message[MAX_LINE_LENGTH];
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
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        struct LogEntry entry;
        struct tm tm = {0};
        
        if (sscanf(line, "%31s %15s %1023[^\n]",
                   entry.timestamp, entry.level, entry.message) != 3) {
            fprintf(stderr, "Invalid log format. Skipping.\\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &tm)) {
            fprintf(stderr, "Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            fprintf(stderr, "Invalid log level. Skipping.\\n");
            continue;
        }
        
        if (strlen(entry.message) == 0) {
            fprintf(stderr, "Empty message. Skipping.\\n");
            continue;
        }
        
        logs[count] = entry;
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries processed.\\n");
        return 0;
    }
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\\nLog Analysis Summary:\\n");
    printf("Total entries: %d\\n", count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    printf("\\nRecent ERROR entries:\\n");
    int error_shown = 0;
    for (int i = count - 1; i >= 0 && error_shown < 5; i--) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            printf("%s %s %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
            error_shown++;
        }
    }
    
    if (error_shown == 0) {
        printf("No ERROR entries found.\\n");
    }
    
    return 0;
}