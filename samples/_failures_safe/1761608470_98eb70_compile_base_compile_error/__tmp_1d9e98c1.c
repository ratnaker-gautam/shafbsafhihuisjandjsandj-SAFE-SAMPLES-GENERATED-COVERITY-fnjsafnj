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
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) < 19 || strlen(parts[1]) < 1 || strlen(parts[2]) < 1) return 0;
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LENGTH - 30);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int info_count = 0, warning_count = 0, error_count = 0, other_count = 0;
    int i = 0;
    
    do {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else other_count++;
        i++;
    } while (i < count);
    
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void find_errors(struct LogEntry* entries, int count) {
    printf("\nError Messages:\n");
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            printf("[%s] %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

void search_messages(struct LogEntry* entries, int count, const char* keyword) {
    if (keyword == NULL || strlen(keyword) == 0) return;
    
    printf("\nMessages containing '%s':\n", keyword);
    char lower_keyword[MAX_LINE_LENGTH];
    strncpy(lower_keyword, keyword, MAX_LINE_LENGTH - 1);
    lower_keyword[MAX_LINE_LENGTH - 1] = '\0';
    
    for (int i = 0; i < strlen(lower_keyword); i++) {
        lower_keyword[i] = tolower(lower_keyword[i]);
    }
    
    int found_count = 0;
    int index = 0;
    
    while (index < count) {
        char lower_message[MAX_LINE_LENGTH];
        strncpy(lower_message, entries[index].message, MAX_LINE_LENGTH - 1);
        lower_message[MAX_LINE_LENGTH - 1] = '\0';
        
        for (int j = 0; j < strlen(lower_message); j++) {
            lower_message[j] = tolower(lower_message[j]);
        }
        
        if (strstr(lower_message, lower_keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[index].timestamp, entries[index].level, entries[index].message);
            found_count++;
        }
        index++;
    }
    
    if (found_count == 0) {
        printf("No messages found containing '%s'\n", keyword);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS|LEVEL|MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            entries[entry_count] = new_entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);