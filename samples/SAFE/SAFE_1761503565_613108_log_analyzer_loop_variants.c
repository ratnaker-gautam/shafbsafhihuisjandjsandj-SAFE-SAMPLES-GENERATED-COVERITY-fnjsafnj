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
    
    int timestamp_end = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && timestamp_end < 19) {
        entry->timestamp[timestamp_end++] = temp[i++];
    }
    entry->timestamp[timestamp_end] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int level_end = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && level_end < 9) {
        entry->level[level_end++] = temp[i++];
    }
    entry->level[level_end] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int msg_end = 0;
    while (temp[i] != '\0' && msg_end < MAX_LINE_LENGTH - 31) {
        entry->message[msg_end++] = temp[i++];
    }
    entry->message[msg_end] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    int line_num = 0;
    while (line_num < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        }
        line_num++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    do {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    } while (j < entry_count);
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nERROR entries:\n");
    int k;
    for (k = 0; k < entry_count; k++) {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            printf("%s: %s\n", entries[k].timestamp, entries[k].message);
        }
    }
    
    printf("\nRecent entries (last 3):\n");
    int start = (entry_count > 3) ? entry_count - 3 : 0;
    int m = start;
    while (m < entry_count) {
        printf("%s [%s]: %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
        m++;
    }
    
    return 0;
}