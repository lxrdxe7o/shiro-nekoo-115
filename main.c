#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_DOCTOR_REMARK 200
#define MAX_PASSWORD_LENGTH 50
#define MAX_USER_NAME 30
#define MAX_PATIENTS 100
#define BLOOD_CHAR_MAX 5

typedef struct patientInfo
{
    char patientName[MAX_NAME_LENGTH];
    char patientAddress[MAX_ADDRESS_LENGTH];
    char prescription[MAX_DOCTOR_REMARK];
    char patientPass[MAX_PASSWORD_LENGTH];
    int age;
    int id;
    float height;
    float weight;
    unsigned long int patientContact;
    unsigned long int emergencyContact;
    char patientUserName[MAX_USER_NAME];
    char bloodType[BLOOD_CHAR_MAX];
} patientInfo;

void getPatientInfo(patientInfo *patient)
{
    printf("Input name: ");
    fgets(patient->patientName, sizeof(patient->patientName), stdin);
    strtok(patient->patientName, "\n");

    printf("Input age: ");
    scanf("%d", &patient->age);
    getchar();

    printf("Input height (in centimeters): ");
    scanf("%f", &patient->height);
    getchar();

    printf("Input weight (in kilograms): ");
    scanf("%f", &patient->weight);
    getchar();

    printf("Input blood group: ");
    fgets(patient->bloodType, sizeof(patient->bloodType), stdin);
    strtok(patient->bloodType, "\n");

    printf("Input patient contact number: ");
    scanf("%lu", &patient->patientContact);
    getchar();

    printf("Input address: ");
    fgets(patient->patientAddress, sizeof(patient->patientAddress), stdin);
    strtok(patient->patientAddress, "\n");

    printf("Input emergency contact number: ");
    scanf("%lu", &patient->emergencyContact);
    getchar();
}

void getInfo_patient(patientInfo patients[MAX_PATIENTS])
{
    int numPatients;
    printf("How many patients do you want to add (maximum %d)? ", MAX_PATIENTS);
    scanf("%d", &numPatients);
    getchar();  // Consume newline character left by scanf

    if (numPatients > MAX_PATIENTS)
    {
        printf("You can add a maximum of %d patients.\n", MAX_PATIENTS);
        numPatients = MAX_PATIENTS;
    }

    for (int i = 0; i < numPatients; i++)
    {
        printf("Input details for patient %d:\n", i + 1);
        getPatientInfo(&patients[i]);
    }
}

int searchPatient(patientInfo patients[MAX_PATIENTS], const char* name)
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        if (strcmp(patients[i].patientName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deletePatient(patientInfo patients[MAX_PATIENTS], const char* name)
{
    int index = searchPatient(patients, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_PATIENTS - 1; i++)
        {
            patients[i] = patients[i + 1];
        }
        memset(&patients[MAX_PATIENTS - 1], 0, sizeof(patientInfo));
        printf("Patient '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Patient '%s' not found.\n", name);
    }
}

void displayPatient(const patientInfo* patient)
{
    printf("Name: %s\n", patient->patientName);
    printf("Age: %d\n", patient->age);
    printf("Height: %.2f cm\n", patient->height);
    printf("Weight: %.2f kg\n", patient->weight);
    printf("Blood Type: %s\n", patient->bloodType);
    printf("Contact Number: %lu\n", patient->patientContact);
    printf("Address: %s\n", patient->patientAddress);
    printf("Emergency Contact Number: %lu\n", patient->emergencyContact);
}

void addPatient(patientInfo patients[MAX_PATIENTS])
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        if (strlen(patients[i].patientName) == 0)
        {
            printf("Adding new patient:\n");
            getPatientInfo(&patients[i]);
            printf("Patient '%s' added successfully.\n", patients[i].patientName);
            return;
        }
    }
    printf("Cannot add patient: Maximum number of patients reached.\n");
}

int main()
{
    patientInfo patients[MAX_PATIENTS] = {0};

    getInfo_patient(patients);

    char searchName[MAX_NAME_LENGTH];
    printf("Enter the name of the patient to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    strtok(searchName, "\n");

    int index = searchPatient(patients, searchName);
    if (index != -1)
    {
        printf("Patient found:\n");
        displayPatient(&patients[index]);
    }
    else
    {
        printf("Patient not found.\n");
    }

    char deleteName[MAX_NAME_LENGTH];
    printf("Enter the name of the patient to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    strtok(deleteName, "\n");

    deletePatient(patients, deleteName);

    addPatient(patients);

    return 0;
}
