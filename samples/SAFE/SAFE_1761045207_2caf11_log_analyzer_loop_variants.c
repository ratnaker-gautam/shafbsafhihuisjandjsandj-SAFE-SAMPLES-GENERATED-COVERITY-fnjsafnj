//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    entry->timestamp[i] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    int j = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LEN - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    entry->message[j] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    int k = 0;
    while (k < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
        
        k++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int m = 0;
    do {
        if (strcmp(entries[m].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[m].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[m].level, "INFO") == 0) {
            info_count++;
        }
        m++;
    } while (m < entry_count);
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR messages:\n");
    int n;
    for (n = 0; n < entry_count; n++) {
        if (strcmp(entries[n].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[n].timestamp, entries[n].message);
        }
    }
    
    printf("\nRecent entries (last 5):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    int p;
    for (p = start_idx; p < entry_count; p++) {
        printf("%s [%s] %s\n", entries[p].timestamp, entries[p].level, entries[p].message);
    }
    
    return 0;
}