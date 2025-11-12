//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int field = 0;
    int pos = 0;
    int len = strlen(temp);
    
    for (int j = i; j < len && field < 3; j++) {
        if (temp[j] == ' ' || temp[j] == '\t') {
            if (pos > 0) {
                switch (field) {
                    case 0:
                        if (pos >= 20) return 0;
                        strncpy(entry->timestamp, temp + i, pos);
                        entry->timestamp[pos] = '\0';
                        break;
                    case 1:
                        if (pos >= 10) return 0;
                        strncpy(entry->level, temp + i, pos);
                        entry->level[pos] = '\0';
                        break;
                }
                field++;
                i = j + 1;
                pos = 0;
            }
        } else {
            pos++;
        }
    }
    
    if (field == 2 && pos > 0) {
        if (pos >= MAX_LINE_LENGTH - 30) return 0;
        strncpy(entry->message, temp + i, pos);
        entry->message[pos] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int idx = 0;
    while (idx < entry_count) {
        if (strcmp(entries[idx].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[idx].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[idx].level, "INFO") == 0) {
            info_count++;
        }
        idx++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR entries:\n");
    for (int j = 0; j < entry_count; j++) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[j].timestamp, entries[j].message);
        }
    }
    
    printf("\nWARNING entries:\n");
    int k = 0;
    do {
        if (k < entry_count && strcmp(entries[k].level, "WARNING") == 0) {
            printf("- %s: %s\n", entries[k].timestamp, entries[k].message);
        }
        k++;
    } while (k < entry_count);
    
    return 0;
}