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
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
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
        char level_buf[16];
        char message_buf[256];
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]",
                           entry.timestamp, level_buf, message_buf);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &ts)) {
            printf("Invalid timestamp format. Skipping line.\\n");
            continue;
        }
        
        if (!is_valid_level(level_buf)) {
            printf("Invalid log level. Skipping line.\\n");
            continue;
        }
        
        strncpy(entry.level, level_buf, sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        strncpy(entry.message, message_buf, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        logs[count] = entry;
        count++;
        
        if (strcmp(level_buf, "INFO") == 0) info_count++;
        else if (strcmp(level_buf, "WARN") == 0) warn_count++;
        else if (strcmp(level_buf, "ERROR") == 0) error_count++;
        else if (strcmp(level_buf, "DEBUG") == 0) debug_count++;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    if (count > 0) {
        printf("\\n=== Recent Log Entries ===\\n");
        int display_count = count > 5 ? 5 : count;
        for (int i = count - display_count; i < count; i++) {
            printf("[%s] %s: %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
    
    return 0;
}