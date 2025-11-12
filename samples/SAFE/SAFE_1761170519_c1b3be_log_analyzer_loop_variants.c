//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int field = 0;
    int pos = 0;
    int start = i;
    
    while (temp[i] != '\0' && field < 3) {
        if (temp[i] == ' ' && field < 2) {
            int len = i - start;
            if (len > 0) {
                if (field == 0) {
                    if (len >= 19) len = 19;
                    strncpy(entry->timestamp, temp + start, len);
                    entry->timestamp[len] = '\0';
                } else if (field == 1) {
                    if (len >= 9) len = 9;
                    strncpy(entry->level, temp + start, len);
                    entry->level[len] = '\0';
                }
            }
            field++;
            while (temp[i] != '\0' && isspace(temp[i])) i++;
            start = i;
        } else {
            i++;
        }
    }
    
    if (field == 2 && temp[start] != '\0') {
        int len = strlen(temp + start);
        if (len >= MAX_LINE_LEN) len = MAX_LINE_LEN - 1;
        strncpy(entry->message, temp + start, len);
        entry->message[len] = '\0';
        return 1;
    }
    
    return 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    while (j < count) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int k = 0;
    int printed = 0;
    for (k = 0; k < count && printed < 3; k++) {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[k].timestamp, entries[k].message);
            printed++;
        }
    }
    if (printed == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    while (entry_count < MAX_ENTRIES && line_num < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count] = entry;
                entry_count++;
            }
        } else {
            printf("Invalid log format on line %d\n", line_num + 1);
        }
        
        line_num++;
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}