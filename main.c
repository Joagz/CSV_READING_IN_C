#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../resources/school.h"

typedef struct
{
    char *students_amount;
    char *name;
    char *id;
    char *address;
    char *jurisdiction;
    char *city;
    char *country_code;
} School;

void initSchool(School *school, const char *students_amount, const char *name, const char *id, const char *address, const char *jurisdiction, const char *city, const char *country_code)
{
    school->students_amount = strdup(students_amount);
    if (school->students_amount == NULL)
    {
        perror("Memory allocation error");
        exit(1); // Terminate the program on memory allocation error
    }

    school->name = strdup(name);
    if (school->name == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    school->id = strdup(id);
    if (school->id == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    school->address = strdup(address);
    if (school->address == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    school->jurisdiction = strdup(jurisdiction);
    if (school->jurisdiction == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    school->city = strdup(city);
    if (school->city == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }

    school->country_code = strdup(country_code);
    if (school->country_code == NULL)
    {
        perror("Memory allocation error");
        exit(1);
    }
}

int readFromCSV(School *schools)
{
    FILE *file;
    file = fopen(SCHOOL_CSV_PATH, "rb");

    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(size + 1);

    if (buffer == NULL)
    {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }

    fread(buffer, sizeof(char), size, file);
    fclose(file);

    int line_amount = 0;
    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == '\n')
            line_amount += 1;
    }

    int school_index = 0;

    for (int i = 0; i < 7; ++i)
    {
        schools[i].students_amount = (char *)malloc(20 * sizeof(char));
        if (schools[i].students_amount == NULL)
        {
            perror("Memory allocation error");
            exit(1);
        }
    }

    char *token = strtok(buffer, ",\n\r");
    while (token != NULL && school_index <= line_amount)
    {
        initSchool(&schools[school_index], token, strtok(NULL, ",\n\r"), strtok(NULL, ",\n\r"), strtok(NULL, ",\n\r"), strtok(NULL, ",\n\r"), strtok(NULL, ",\n\r"), strtok(NULL, ",\n\r"));
        school_index++;
        token = strtok(NULL, ",\n\r");
    }

    for (int i = 0; i < school_index; i++)
    {
        printf("School %d\n", i + 1);
        printf("Students Amount: %s\n", schools[i].students_amount);
        printf("Name: %s\n", schools[i].name);
        printf("ID: %s\n", schools[i].id);
        printf("Address: %s\n", schools[i].address);
        printf("Jurisdiction: %s\n", schools[i].jurisdiction);
        printf("City: %s\n", schools[i].city);
        printf("Country Code: %s\n", schools[i].country_code);
        printf("\n");
    }

    free(buffer);

    for (int i = 0; i < school_index; ++i)
    {
        free(schools[i].students_amount);
        free(schools[i].address);
        free(schools[i].id);
        free(schools[i].name);
        free(schools[i].city);
        free(schools[i].country_code);
        free(schools[i].jurisdiction);
    }

    return school_index;
}

int main()
{
    School *schools = (School *)malloc(sizeof(School) * 1000);
    int num_schools = readFromCSV(schools);
    return 0;
}
