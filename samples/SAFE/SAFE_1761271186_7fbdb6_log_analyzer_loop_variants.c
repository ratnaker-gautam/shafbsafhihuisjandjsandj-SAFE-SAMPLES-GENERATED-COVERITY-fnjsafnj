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
                while (j < len && isspace(temp[j])) j++;
                i = j;
                pos = 0;
                if (j >= len) break;
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

void analyze_log_levels(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    int k = 0;
    while (k < count) {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count++;
        } else {
            other_count++;
        }
        k++;
    }
    
    printf("Log Level Analysis:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    printf("OTHER: %d\n", other_count);
}

void find_errors(struct LogEntry* entries, int count) {
    printf("\nRecent ERROR entries:\n");
    int found = 0;
    
    for (int i = count - 1; i >= 0 && found < 5; i--) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[i].timestamp, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No ERROR entries found.\n");
    }
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
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    analyze_log_levels(entries, entry_count);
    find_errors(entries, entry_count);
    
    return 0;
}