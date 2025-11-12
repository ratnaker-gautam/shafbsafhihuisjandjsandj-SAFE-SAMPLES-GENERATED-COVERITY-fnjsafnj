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
    strcpy(temp, line);
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (!isdigit(temp[i]) && temp[i] != '-' && temp[i] != ':' && temp[i] != ' ') {
            return 0;
        }
        i++;
    }
    if (i < 10) return 0;
    
    strncpy(entry->timestamp, temp, 19);
    entry->timestamp[19] = '\0';
    
    char* rest = temp + 20;
    if (strlen(rest) < 5) return 0;
    
    int j = 0;
    while (rest[j] != '\0' && rest[j] != ' ' && j < 9) {
        entry->level[j] = rest[j];
        j++;
    }
    entry->level[j] = '\0';
    
    if (strlen(rest + j + 1) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, rest + j + 1);
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int k = 0;
    do {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count++;
        }
        k++;
    } while (k < count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries:\n");
    int start = (count > 5) ? count - 5 : 0;
    for (int m = start; m < count; m++) {
        printf("%s [%s] %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
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
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            entries[entry_count] = new_entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}