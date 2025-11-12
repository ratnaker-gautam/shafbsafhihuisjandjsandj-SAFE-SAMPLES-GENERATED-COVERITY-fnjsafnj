//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
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
    return strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || 
           strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    int info_count = 0, warning_count = 0, error_count = 0, debug_count = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\\n' || line[3] == '\\0')) {
            break;
        }
        
        char timestamp[64], level[16], message[256];
        struct tm time_struct;
        
        int parsed = sscanf(line, "%63s %15s %255[^\\n]", timestamp, level, message);
        
        if (parsed != 3) {
            printf("Invalid log format. Skipping.\\n");
            continue;
        }
        
        if (!parse_timestamp(timestamp, &time_struct)) {
            printf("Invalid timestamp format. Skipping.\\n");
            continue;
        }
        
        if (!validate_log_level(level)) {
            printf("Invalid log level. Skipping.\\n");
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message. Skipping.\\n");
            continue;
        }
        
        strncpy(logs[log_count].timestamp, timestamp, sizeof(logs[log_count].timestamp) - 1);
        logs[log_count].timestamp[sizeof(logs[log_count].timestamp) - 1] = '\\0';
        strncpy(logs[log_count].level, level, sizeof(logs[log_count].level) - 1);
        logs[log_count].level[sizeof(logs[log_count].level) - 1] = '\\0';
        strncpy(logs[log_count].message, message, sizeof(logs[log_count].message) - 1);
        logs[log_count].message[sizeof(logs[log_count].message) - 1] = '\\0';
        
        log_count++;
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\\n");
        return 0;
    }
    
    for (int i = 0; i < log_count; i++) {
        if (strcmp(logs[i].level, "INFO") == 0) info_count++;
        else if (strcmp(logs[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(logs[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(logs[i].level, "DEBUG") == 0) debug_count++;
    }
    
    printf("\\nLog Analysis Summary:\\n");
    printf("Total entries: %d\\n", log_count);
    printf("INFO: %d\\n", info_count);
    printf("WARNING: %d\\n", warning_count);
    printf("ERROR: %d\\n", error_count);
    printf("DEBUG: %d\\n", debug_count);
    
    printf("\\nRecent log entries (last 5):\\n");
    int start = log_count - 5;
    if (start < 0) start = 0;
    for (int i = start; i < log_count; i++) {
        printf("%s [%s] %s\\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}