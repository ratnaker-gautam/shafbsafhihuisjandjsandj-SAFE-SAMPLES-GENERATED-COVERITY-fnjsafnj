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
    
    if (field == 2 && start < i) {
        int len = i - start;
        if (len >= MAX_LINE_LEN - 1) len = MAX_LINE_LEN - 2;
        strncpy(entry->message, temp + start, len);
        entry->message[len] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    int line_num = 0;
    while (line_num < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
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
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    while (j < entry_count) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR entries:\n");
    int k = 0;
    do {
        if (k < entry_count && strcmp(entries[k].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[k].timestamp, entries[k].message);
        }
        k++;
    } while (k < entry_count);
    
    return 0;
}