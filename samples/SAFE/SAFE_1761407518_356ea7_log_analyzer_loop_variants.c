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
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    entry->timestamp[i] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int j = 0;
    while (temp[i] != '\0' && !isspace(temp[i]) && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LEN - 1) {
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
    
    printf("Enter log entries (empty line to finish):\n");
    
    int k = 0;
    while (k < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
        k++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int m = 0;
    while (m < entry_count) {
        if (strcmp(entries[m].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[m].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[m].level, "INFO") == 0) {
            info_count++;
        }
        m++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nError messages:\n");
    int n = 0;
    do {
        if (n >= entry_count) break;
        if (strcmp(entries[n].level, "ERROR") == 0) {
            printf("- %s\n", entries[n].message);
        }
        n++;
    } while (n < entry_count);
    
    printf("\nRecent entries (last 3):\n");
    int start = entry_count - 3;
    if (start < 0) start = 0;
    
    for (int p = start; p < entry_count; p++) {
        printf("[%s] %s: %s\n", entries[p].timestamp, entries[p].level, entries[p].message);
    }
    
    return 0;
}