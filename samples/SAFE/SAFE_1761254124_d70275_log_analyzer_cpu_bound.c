//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int response_time;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char message_buf[256];
    int year, month, day, hour, min, sec, resp_time;
    
    int parsed = sscanf(line, "%d-%d-%d %d:%d:%d %15s %255[^|]| Response time: %dms",
                       &year, &month, &day, &hour, &min, &sec, 
                       level_buf, message_buf, &resp_time);
    
    if (parsed != 9) return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;
    if (sec < 0 || sec > 59) return 0;
    if (resp_time < 0 || resp_time > 60000) return 0;
    
    snprintf(entry->timestamp, sizeof(entry->timestamp), 
             "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
    strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    entry->response_time = resp_time;
    
    return 1;
}

void analyze_logs(LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    long total_response_time = 0;
    int max_response_time = 0;
    int min_response_time = 1000000;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
        
        total_response_time += logs[i].response_time;
        if (logs[i].response_time > max_response_time) {
            max_response_time = logs[i].response_time;
        }
        if (logs[i].response_time < min_response_time) {
            min_response_time = logs[i].response_time;
        }
    }
    
    double avg_response_time = count > 0 ? (double)total_response_time / count : 0.0;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Average response time: %.2f ms\n", avg_response_time);
    printf("Max response time: %d ms\n", max_response_time);
    printf("Min response time: %d ms\n", min_response_time);
}

int main(void) {
    LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE | Response time: Nms):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}