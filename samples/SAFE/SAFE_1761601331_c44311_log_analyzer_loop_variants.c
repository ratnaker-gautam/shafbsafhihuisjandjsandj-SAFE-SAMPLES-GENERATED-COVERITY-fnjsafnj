//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    char level_buf[16];
    char message_buf[256];
    char timestamp_buf[32];
    
    if (strlen(line) >= MAX_LINE_LEN - 1) {
        return 0;
    }
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                       timestamp_buf, level_buf, message_buf);
    
    if (parsed != 3) {
        return 0;
    }
    
    if (strlen(timestamp_buf) >= 32 || strlen(level_buf) >= 16 || strlen(message_buf) >= 256) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp_buf);
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    
    return 1;
}

void analyze_logs(struct LogEntry* logs, int count) {
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        }
        i++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("INFO entries: %d\n", info_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("ERROR entries: %d\n", error_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    for (int j = count - 1; j >= 0 && found_errors < 5; j--) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[j].timestamp, logs[j].message);
            found_errors++;
        }
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}