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
        
        char timestamp[32], level[16], message[256];
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        struct tm tm = {0};
        if (!parse_timestamp(timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping line.\\n");
            continue;
        }
        
        if (!is_valid_level(level)) {
            printf("Invalid log level. Skipping line.\\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message. Skipping line.\\n");
            continue;
        }
        
        strncpy(logs[count].timestamp, timestamp, sizeof(logs[count].timestamp) - 1);
        logs[count].timestamp[sizeof(logs[count].timestamp) - 1] = '\0';
        strncpy(logs[count].level, level, sizeof(logs[count].level) - 1);
        logs[count].level[sizeof(logs[count].level) - 1] = '\0';
        strncpy(logs[count].message, message, sizeof(logs[count].message) - 1);
        logs[count].message[sizeof(logs[count].message) - 1] = '\0';
        
        count++;
    }
    
    if (count == 0) {
        printf("No valid log entries provided.\\n");
        return 1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\\nLog Analysis Results:\\n");
    printf("Total entries: %d\\n", count);
    printf("INFO:  %d\\n", info_count);
    printf("WARN:  %d\\n", warn_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    printf("\\nRecent entries (last 5):\\n");
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%s [%s] %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    if (error_count > 0) {
        printf("\\nERROR entries:\\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(logs[i].level, "ERROR") == 0) {
                printf("%s %s\\n", logs[i].timestamp, logs[i].message);
            }
        }
    }
    
    return 0;
}