//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    if (i == 0 || i >= 19) return 0;
    entry->timestamp[i] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    int j = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    if (j == 0) return 0;
    entry->level[j] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LEN - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    if (j == 0) return 0;
    entry->message[j] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int k = 0;
    do {
        if (strcmp(logs[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[k].level, "INFO") == 0) {
            info_count++;
        }
        k++;
    } while (k < count);
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    for (int m = count - 1; m >= 0 && found_errors < 3; m--) {
        if (strcmp(logs[m].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[m].timestamp, logs[m].message);
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
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    char input_line[MAX_LINE_LEN];
    while (log_count < MAX_ENTRIES) {
        if (fgets(input_line, MAX_LINE_LEN, stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        struct LogEntry new_entry;
        if (parse_log_line(input_line, &new_entry)) {
            logs[log_count] = new_entry;
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", input_line);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries provided.\n");
    }
    
    return 0;
}