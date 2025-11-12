//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

void analyze_logs(struct LogEntry* logs, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int i = 0;
    
    while (i < count) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    int j;
    for (j = 0; j < count; j++) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            printf("ERROR: %s - %s\n", logs[j].timestamp, logs[j].message);
        }
    }
    
    int k = 0;
    do {
        if (k < count && strcmp(logs[k].level, "WARNING") == 0) {
            printf("WARNING: %s - %s\n", logs[k].timestamp, logs[k].message);
        }
        k++;
    } while (k < count);
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char input[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(input, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(input, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", input);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}