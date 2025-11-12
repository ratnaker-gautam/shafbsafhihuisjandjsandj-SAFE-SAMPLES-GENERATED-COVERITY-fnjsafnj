//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    char* ptr = temp;
    while (*ptr && part_count < 3) {
        if (*ptr == ' ') {
            *ptr = '\0';
            parts[part_count] = ptr + 1;
            part_count++;
        }
        ptr++;
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 19) return 0;
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    
    if (strlen(parts[1]) >= 9) return 0;
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    
    if (strlen(parts[2]) >= MAX_LINE_LEN - 31) return 0;
    strncpy(entry->message, parts[2], MAX_LINE_LEN - 31);
    entry->message[MAX_LINE_LEN - 31] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_num++;
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (len == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int j = start; j < entry_count; j++) {
        printf("%s [%s] %s\n", entries[j].timestamp, entries[j].level, entries[j].message);
    }
    
    printf("\nError entries:\n");
    int k = 0;
    do {
        if (k < entry_count && strcmp(entries[k].level, "ERROR") == 0) {
            printf("%s %s\n", entries[k].timestamp, entries[k].message);
        }
        k++;
    } while (k < entry_count);
    
    return 0;
}