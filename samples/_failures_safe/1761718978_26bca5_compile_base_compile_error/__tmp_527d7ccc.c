//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* tokens[3];
    tokens[0] = strtok(temp_line, " ");
    tokens[1] = strtok(NULL, " ");
    tokens[2] = strtok(NULL, "\n");
    
    if (tokens[0] == NULL || tokens[1] == NULL || tokens[2] == NULL) {
        return 0;
    }
    
    if (strlen(tokens[0]) >= 20 || strlen(tokens[1]) >= 10 || strlen(tokens[2]) >= (MAX_LINE_LENGTH - 30)) {
        return 0;
    }
    
    strncpy(entry->timestamp, tokens[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, tokens[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, tokens[2], MAX_LINE_LENGTH - 31);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 0;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[count] = entry;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_log_levels(const struct LogEntry entries[], int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("\nLog Level Analysis:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", count);
}

void search_log_messages(const struct LogEntry entries[], int count) {
    char search_term[100];
    printf("\nEnter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) == NULL) {
        return;
    }
    
    search_term[strcspn(search_term, "\n")] = '\0';
    
    if (strlen(search_term) == 0) {
        return;
    }
    
    printf("\nMatching log entries:\n");
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, search_term) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No entries found containing '%s'\n", search_term);
    }
}

void display_recent_entries(const struct LogEntry entries[], int count) {
    int num_to_show;
    printf("\nNumber of recent entries to display: ");
    if (scanf("%d", &num_to_show) != 1) {
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    if (num_to_show <= 0 || num_to_show > count) {
        num_to_show = count;
    }
    
    printf("\nLast %d log entries:\n", num_to_show);
    int start = count - num_to_show;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s