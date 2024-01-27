#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../resources/school.h"

FILE *read_file(char CSV_PATH[]);
int file_size(FILE *stream);

FILE *studentsTable;
int header_sizes_num_elements = 0;

char **read_value(char CSV_PATH[], int row)
{
    row = row + 1;
    FILE *stream = read_file(CSV_PATH);
    int size = file_size(stream);
    char buffer[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    int actual_row = 0;

    while (fgets(line, sizeof(line), stream) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        actual_row++;
        if (strlen(line) == 0 || (strspn(line, " \t") == strlen(line)))
        {
            printf("Empty line at line number %d\n", actual_row);
            return NULL;
        }
        if (actual_row == row)
        {
            strcpy(buffer, line);
            break;
        }
    }

    if (feof(stream))
    {
        printf("Desired line does not exist (end of file reached)\n");
        return NULL;
    }
    else if (ferror(stream))
    {
        perror("Error reading file");
        return NULL;
    }

    printf("%s", buffer);
    fclose(stream);

    char **headers = (char **)malloc(sizeof(char *) * header_sizes_num_elements);

    if (!headers)
        return NULL;
    for (int i = 0; i < header_sizes_num_elements; i++)
    {
        headers[i] = (char *)malloc(sizeof(buffer) + 1);
        if (!headers[i])
        {
            free(headers);
            return NULL;
        }
    }

    int header_index = 0;
    int cur_index = 0;
    char *str = (char *)malloc(sizeof(buffer) + sizeof(char));
    for (int i = 0; i < size; i++)
    {
        char ch = buffer[i];
        if (ch == '\n' || ch == '\r')
        {
            strncpy(headers[header_index], str, sizeof(buffer));
            headers[header_index][cur_index] = '\0';
            free(str);
            break;
        }
        if (ch == ',')
        {
            strncpy(headers[header_index], str, sizeof(buffer));
            headers[header_index][cur_index] = '\0';
            strcpy(str, "");

            cur_index = 0;
            header_index++;
        }
        else
        {
            str[cur_index] = ch;
            cur_index++;
        }
    }

    return headers;
}

FILE *read_file(char CSV_PATH[])
{
    FILE *stream;
    stream = fopen(CSV_PATH, "r+");
    if (stream == NULL)
    {
        return NULL;
    }
    return stream;
}

int file_size(FILE *stream)
{

    fseek(stream, 0, SEEK_END);
    int size = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    return size;
}

char **read_headers(char CSV_PATH[])
{
    FILE *stream = read_file(CSV_PATH);
    int size = file_size(stream);

    char buffer[size];
    int count = fread(buffer, sizeof(char), size, stream);
    fclose(stream);

    int header_ammt = 0;

    for (int i = 0; i < size; i++)
    {
        char ch = buffer[i];
        if (ch == '\n' || ch == '\r')
        {
            break;
        }
        if (ch == ',')
        {
            header_ammt++;
        }
    }

    int header_sizes[header_ammt + 1]; // Increased by 1 to account for the last header
    int header_size = 0;
    int header_sizes_index = 0;

    for (int i = 0; i < size; i++)
    {
        char ch = buffer[i];
        if (ch == '\n' || ch == '\r')
        {
            header_sizes[header_sizes_index] = header_size;
            break;
        }
        if (ch == ',')
        {
            header_sizes[header_sizes_index] = header_size;
            header_size = 0;
            header_sizes_index++;
        }
        else
        {
            header_size++;
        }
    }

    header_sizes_num_elements = sizeof(header_sizes) / sizeof(header_sizes[0]);
    int larger_header = 0;

    for (int i = 0; i < header_sizes_num_elements; i++)
    {
        if (i == 0 || larger_header < header_sizes[i])
        {
            larger_header = header_sizes[i];
        }
    }

    char **headers = (char **)malloc(sizeof(char *) * header_sizes_num_elements);

    if (!headers)
        return NULL;
    for (int i = 0; i < header_sizes_num_elements; i++)
    {
        headers[i] = (char *)malloc(larger_header + 1);
        if (!headers[i])
        {
            free(headers);
            return NULL;
        }
    }

    int header_index = 0;
    int cur_index = 0;
    char *str = (char *)malloc(larger_header + sizeof(char));
    for (int i = 0; i < size; i++)
    {
        char ch = buffer[i];
        if (ch == '\n' || ch == '\r')
        {
            strncpy(headers[header_index], str, larger_header);
            headers[header_index][cur_index] = '\0';
            free(str);
            break;
        }
        if (ch == ',')
        {
            strncpy(headers[header_index], str, larger_header);
            headers[header_index][cur_index] = '\0';
            strcpy(str, "");

            cur_index = 0;
            header_index++;
        }
        else
        {
            str[cur_index] = ch;
            cur_index++;
        }
    }

    return headers;
}

int main()
{

    char **headers = read_headers(SCHOOL_CSV_PATH);
    char **values = read_value(SCHOOL_CSV_PATH, 2);

    if (values == NULL)
    {
        printf("ERROR: Unable to allocate array!\n");
        return 1;
    }
    for (int i = 0; i < header_sizes_num_elements; i++)
    {
        printf("%d: %s\n", i, values[i]);
    }
    free(values);
    free(headers);

    return 0;
}
