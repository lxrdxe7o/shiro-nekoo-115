#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_DOCTOR_REMARK 200
#define MAX_PASSWORD_LENGTH 50
#define MAX_USER_NAME 30
#define MAX_PATIENTS 100
#define MAX_DOCTORS 10
#define BLOOD_CHAR_MAX 5
#define MAX_CONTACT_LENGTH 15
#define DAYS_IN_WEEK 7
#define SLOTS_PER_DAY 5

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

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
    char patientContact[MAX_CONTACT_LENGTH];
    char emergencyContact[MAX_CONTACT_LENGTH];
    char patientUserName[MAX_USER_NAME];
    char bloodType[BLOOD_CHAR_MAX];
} patientInfo;

typedef struct doctorInfo
{
    char doctorName[MAX_NAME_LENGTH];
    char doctorAddress[MAX_ADDRESS_LENGTH];
    char specialty[MAX_DOCTOR_REMARK];
    char doctorPass[MAX_PASSWORD_LENGTH];
    int id;
    int experience;
    char doctorContact[MAX_CONTACT_LENGTH];
    char doctorUserName[MAX_USER_NAME];
    int calendar[DAYS_IN_WEEK][SLOTS_PER_DAY];
} doctorInfo;

void clear_terminal();

void getPatientInfo(patientInfo *patient);
void getInfo_patient(patientInfo patients[MAX_PATIENTS], int *lastAssignedID);
void patientAccountCreation(patientInfo *patient, int *lastAssignedID);
int searchPatient(patientInfo patients[MAX_PATIENTS], const char* name);
void deletePatient(patientInfo patients[MAX_PATIENTS], const char* name);
void displayPatient(const patientInfo* patient);

void getDoctorInfo(doctorInfo *doctor);
void doctorAccountCreation(doctorInfo *doctor, int *lastAssignedID);
void getInfo_doctor(doctorInfo doctors[MAX_DOCTORS], int *lastAssignedID);
int searchDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name);
void deleteDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name);
void displayDoctor(const doctorInfo* doctor);
void initializeCalendar(doctorInfo *doctors);
void displayCalendar(const doctorInfo *doctors);
void updateCalendar(doctorInfo *doctors);
int docLogIn(const doctorInfo *doctors, int maxCount, const char *inputUser, const char *pass);
void docLogInPanel();
void displayDoctorSubMenu();
void editDoctorInfo(doctorInfo *doctor);

void displayPatientMenu();
void displayDoctorMenu();
void displayMainMenu();

void nextPage();
void notFound(char k);
void inputString(char* buffer, int length);
int inputInt();

void savePatientsToFile(const patientInfo patients[MAX_PATIENTS], const char *filename);
void loadPatientsFromFile(patientInfo patients[MAX_PATIENTS], const char *filename);

void saveDoctorsToFile(const doctorInfo doctors[MAX_DOCTORS], const char *filename);
void loadDoctorsFromFile(doctorInfo doctors[MAX_DOCTORS], const char *filename);

void saveIDs(int patientID, int doctorID);
void loadIDs(int *patientID, int *doctorID);

int main() {
    static int lastAssignedPatientID = 0;
    static int lastAssignedDoctorID = 0;

    loadIDs(&lastAssignedPatientID, &lastAssignedDoctorID);

    patientInfo patients[MAX_PATIENTS] = {0};
    doctorInfo doctors[MAX_DOCTORS] = {0};

    loadPatientsFromFile(patients, "patients.dat");
    loadDoctorsFromFile(doctors, "doctors.dat");

    int choice;
    while (1) {
        clear_terminal();
        displayMainMenu();
        scanf("%d", &choice);
        getchar();
        clear_terminal();

        switch (choice) {
            case 1: {
                int patientChoice;
                while (1) {
                    displayPatientMenu();
                    scanf("%d", &patientChoice);
                    getchar();
                    clear_terminal();

                    switch (patientChoice) {
                        case 1:
                            clear_terminal();
                            getInfo_patient(patients, &lastAssignedPatientID);
                            break;
                        case 2:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char searchName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to search: ");
                            fgets(searchName, sizeof(searchName), stdin);
                            strtok(searchName, "\n");

                            int index = searchPatient(patients, searchName);
                            if (index != -1) {
                                clear_terminal();
                                printf("Patient found:\n");
                                displayPatient(&patients[index]);
                                nextPage();
                            } else {
                                clear_terminal();
                                notFound('p');
                                nextPage();
                            }
                            break;
                        }
                        case 3:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char deleteName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to delete: ");
                            fgets(deleteName, sizeof(deleteName), stdin);
                            strtok(deleteName, "\n");

                            clear_terminal();
                            deletePatient(patients, deleteName);
                            break;
                        }
                        case 4:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char displayName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to display: ");
                            fgets(displayName, sizeof(displayName), stdin);
                            strtok(displayName, "\n");

                            int index = searchPatient(patients, displayName);
                            if (index != -1) {
                                clear_terminal();
                                displayPatient(&patients[index]);
                                nextPage();
                            } else {
                                clear_terminal();
                                notFound('p');
                                nextPage();
                            }
                            break;
                        }
                        case 5:
                            clear_terminal();
                            goto mainMenu;
                        default:
                            clear_terminal();
                            printf("Invalid choice. Please try again.\n");
                            nextPage();
                            break;
                    }
                }
                break;
            }
            case 2: {
                int doctorChoice;
                while (1) {
                    docMenu:;
                    displayDoctorMenu();
                    scanf("%d", &doctorChoice);
                    getchar();

                    switch (doctorChoice) {
                        case 1:
                            clear_terminal();
                            getInfo_doctor(doctors, &lastAssignedDoctorID);
                            break;
                        case 2:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char searchName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to search: ");
                            fgets(searchName, sizeof(searchName), stdin);
                            strtok(searchName, "\n");

                            int index = searchDoctor(doctors, searchName);
                            if (index != -1) {
                                clear_terminal();
                                printf("Doctor found:\n");
                                displayDoctor(&doctors[index]);
                                nextPage();
                            } else {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 3:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char deleteName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to delete: ");
                            fgets(deleteName, sizeof(deleteName), stdin);
                            strtok(deleteName, "\n");

                            deleteDoctor(doctors, deleteName);
                            break;
                        }
                        case 4:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char displayName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to display: ");
                            fgets(displayName, sizeof(displayName), stdin);
                            strtok(displayName, "\n");

                            int index = searchDoctor(doctors, displayName);
                            if (index != -1) {
                                clear_terminal();
                                displayDoctor(&doctors[index]);
                                printf("\n\n");
                                displayCalendar(&doctors[index]);
                                nextPage();
                            } else {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 5:
                        {
                            clear_terminal();
                            
                            char inputUser[MAX_USER_NAME];
                            char pass[MAX_PASSWORD_LENGTH];
    
                            printf("Enter Doctor Username:");
                            fgets(inputUser, MAX_USER_NAME, stdin);
                            strtok(inputUser, "\n");

                            printf("Enter password:");
                            fgets(pass, MAX_PASSWORD_LENGTH, stdin);
                            strtok(pass, "\n");

                            int index = docLogIn(doctors, MAX_DOCTORS, inputUser, pass);
                            if (index != -1) 
                            {
                                while (1)
                                {
                                    displayDoctorSubMenu();
                                    int subChoiceDoc;
                                    scanf("%d", &subChoiceDoc);
                                    getchar();
                                    clear_terminal();

                                    switch (subChoiceDoc)
                                    {
                                    case 1:
                                        updateCalendar(&doctors[index]);
                                        break;
                                    case 2:
                                        editDoctorInfo(&doctors[index]);
                                        break;
                                    case 3:
                                        // Prescribe patient
                                        break;
                                    case 4:
                                        goto docMenu;
                                    default:
                                        printf("Invalid choice. Please try again.\n");
                                    }
                                
                                }
                                clear_terminal();
                                break;
                            } 
                            else 
                            {
                                clear_terminal();
                                notFound('d');
                                nextPage();
                                break;
                            }
                        }
                        case 6:
                            goto mainMenu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;
            }
            case 3:
                // Save data to files
                savePatientsToFile(patients, "patients.dat");
                saveDoctorsToFile(doctors, "doctors.dat");
                saveIDs(lastAssignedPatientID, lastAssignedDoctorID);
                
                clear_terminal();
                printf("Exiting the program..\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                mainMenu:;
        }
    }

    return 0;
}

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
    fgets(patient->patientContact, sizeof(patient->patientContact), stdin);
    strtok(patient->patientContact, "\n");

    printf("Input address: ");
    fgets(patient->patientAddress, sizeof(patient->patientAddress), stdin);
    strtok(patient->patientAddress, "\n");

    printf("Input emergency contact number: ");
    fgets(patient->emergencyContact, sizeof(patient->emergencyContact), stdin);
    strtok(patient->emergencyContact, "\n");
}


void patientAccountCreation(patientInfo *patient, int *lastAssignedID)
{
    patient->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", patient->id);

    printf("Input patient username: ");
    fgets(patient->patientUserName, sizeof(patient->patientUserName), stdin);
    strtok(patient->patientUserName, "\n");

    printf("Input patient password: ");
    fgets(patient->patientPass, sizeof(patient->patientPass), stdin);
    strtok(patient->patientPass, "\n");
}

void getInfo_patient(patientInfo patients[MAX_PATIENTS], int *lastAssignedID)
{
    int numPatients;
    printf("How many patients do you want to add (maximum %d)? ", MAX_PATIENTS);
    scanf("%d", &numPatients);
    getchar();

    if ((*lastAssignedID) + numPatients > MAX_PATIENTS)
    {
        printf("You can add a maximum of %d patients.\n", MAX_PATIENTS - (*lastAssignedID));
        numPatients = MAX_PATIENTS - (*lastAssignedID);
    }

    for (int i = 0; i < numPatients; i++)
    {
        printf("\nInput details for patient %d:\n", i + 1);
        getPatientInfo(&patients[(*lastAssignedID) + i]);
        patientAccountCreation(&patients[(*lastAssignedID) + i], lastAssignedID);
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

void getDoctorInfo(doctorInfo *doctor)
{
    printf("Input name: ");
    fgets(doctor->doctorName, sizeof(doctor->doctorName), stdin);
    strtok(doctor->doctorName, "\n");

    printf("Input address: ");
    fgets(doctor->doctorAddress, sizeof(doctor->doctorAddress), stdin);
    strtok(doctor->doctorAddress, "\n");

    printf("Input specialty: ");
    fgets(doctor->specialty, sizeof(doctor->specialty), stdin);
    strtok(doctor->specialty, "\n");

    printf("Input years of experience: ");
    scanf("%d", &doctor->experience);
    getchar();

    printf("Input contact number: ");
    fgets(doctor->doctorContact, sizeof(doctor->doctorContact), stdin);
    strtok(doctor->doctorContact, "\n");
}

void doctorAccountCreation(doctorInfo *doctor, int *lastAssignedID)
{
    doctor->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", doctor->id);

    printf("Input doctor username: ");
    fgets(doctor->doctorUserName, sizeof(doctor->doctorUserName), stdin);
    strtok(doctor->doctorUserName, "\n");

    printf("Input doctor password: ");
    fgets(doctor->doctorPass, sizeof(doctor->doctorPass), stdin);
    strtok(doctor->doctorPass, "\n");
}

void getInfo_doctor(doctorInfo doctors[MAX_DOCTORS], int *lastAssignedID)
{
    int numDoctors;
    printf("How many doctors do you want to add (maximum %d)? ", MAX_DOCTORS);
    scanf("%d", &numDoctors);
    getchar();

    if (numDoctors > MAX_DOCTORS)
    {
        printf("You can add a maximum of %d doctors.\n", MAX_DOCTORS);
        numDoctors = MAX_DOCTORS;
    }

    for (int i = 0; i < numDoctors; i++)
    {
        printf("\nInput details for doctor %d:\n", i + 1);
        getDoctorInfo(&doctors[i]);
        doctorAccountCreation(&doctors[i], lastAssignedID);
        initializeCalendar(&doctors[i]);
    }
}

int searchDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name)
{
    for (int i = 0; i < MAX_DOCTORS; i++)
    {
        if (strcmp(doctors[i].doctorName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deleteDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name)
{
    int index = searchDoctor(doctors, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_DOCTORS - 1; i++)
        {
            doctors[i] = doctors[i + 1];
        }
        memset(&doctors[MAX_DOCTORS - 1], 0, sizeof(doctorInfo));
        printf("Doctor '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Doctor '%s' not found.\n", name);
    }
}

void displayPatient(const patientInfo* patient)
{
    printf("\n          %s╔═════════════════════════════════════════════════════════════════╗%s\n", CYAN, RESET);
    printf("          %s║                      Patient Information                        ║%s\n", CYAN, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", CYAN, RESET);
    printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", CYAN, RESET);
    printf("          %s║ Name: %-57s ║%s\n", CYAN, patient->patientName, RESET);
    printf("          %s║ Age: %-58d ║%s\n", CYAN, patient->age, RESET);
    printf("          %s║ Height: %-55.2f ║%s\n", CYAN, patient->height, RESET);
    printf("          %s║ Weight: %-55.2f ║%s\n", CYAN, patient->weight, RESET);
    printf("          %s║ Blood Type: %-52s║%s\n", CYAN, patient->bloodType, RESET);
    printf("          %s║ Contact Number: %-48s║%s\n", CYAN, patient->patientContact, RESET);
    printf("          %s║ Address: %-53s  ║%s\n", CYAN, patient->patientAddress, RESET);
    printf("          %s║ Emergency Contact: %-43s  ║%s\n", CYAN, patient->emergencyContact, RESET);
    printf("          %s║ Username: %-49s     ║%s\n", CYAN, patient->patientUserName, RESET);
    printf("          %s║ Password: %-49s     ║%s\n", CYAN, patient->patientPass, RESET);
    printf("          %s║ ID: %-57d   ║%s\n", CYAN, patient->id, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", CYAN, RESET);
}



void displayDoctor(const doctorInfo* doctor)
{
    printf("\n          %s╔═════════════════════════════════════════════════════════════════╗%s\n", GREEN, RESET);
    printf("          %s║                      Doctor Information                         ║%s\n", GREEN, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", GREEN, RESET);
    printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", GREEN, RESET);
    printf("          %s║ Name: %-57s ║%s\n", GREEN, doctor->doctorName, RESET);
    printf("          %s║ Address: %-53s  ║%s\n", GREEN, doctor->doctorAddress, RESET);
    printf("          %s║ Specialty: %-51s  ║%s\n", GREEN, doctor->specialty, RESET);
    printf("          %s║ Experience: %-50d  ║%s\n", GREEN, doctor->experience, RESET);
    printf("          %s║ Contact Number: %-46s  ║%s\n", GREEN, doctor->doctorContact, RESET);
    printf("          %s║ Username: %-49s     ║%s\n", GREEN, doctor->doctorUserName, RESET);
    printf("          %s║ Password: %-49s     ║%s\n", GREEN, doctor->doctorPass, RESET);
    printf("          %s║ ID: %-57d   ║%s\n", GREEN, doctor->id, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", GREEN, RESET);
}

void displayPatientMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║              Patient Panel             ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Add patients                        ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Search patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Delete patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Display patient information         ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s5.%s Back to main menu                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayDoctorMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", MAGENTA, RESET);
    printf("                    %s║              Doctor Panel              ║%s\n", MAGENTA, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", MAGENTA, RESET);
    printf("                    %s║ %s1.%s Add doctors                         ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s2.%s Search doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s3.%s Delete doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s4.%s Display doctor schedule             ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s5.%s Doctor login                        ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s6.%s Back to main menu                   ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", MAGENTA, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayMainMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("                    %s║                 Main Menu              ║%s\n", BLUE, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", BLUE, RESET);
    printf("                    %s║ %s1.%s Patient Panel                       ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s║ %s2.%s Doctor Panel                        ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s║ %s3.%s Exit                                ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", BLUE, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void clear_terminal()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
} 
 
void nextPage()
{
  char choice;
  printf("Go to previous panel (Y/N) ?");
  scanf("%c", &choice);

  while(1)
    if((choice == 'Y') || (choice == 'y') || (choice == 'n') || (choice == 'N'))
      break;

  clear_terminal();
}

void savePatientsToFile(const patientInfo patients[MAX_PATIENTS], const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(patients, sizeof(patientInfo), MAX_PATIENTS, file);
    fclose(file);
}

void loadPatientsFromFile(patientInfo patients[MAX_PATIENTS], const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(patients, sizeof(patientInfo), MAX_PATIENTS, file);
    fclose(file);
}

void saveDoctorsToFile(const doctorInfo doctors[MAX_DOCTORS], const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(doctors, sizeof(doctorInfo), MAX_DOCTORS, file);
    fclose(file);
}

void loadDoctorsFromFile(doctorInfo doctors[MAX_DOCTORS], const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(doctors, sizeof(doctorInfo), MAX_DOCTORS, file);
    fclose(file);
}


void initializeCalendar(doctorInfo *doctors) {
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            doctors->calendar[i][j] = 0; // All slots are initially unavailable
        }
    }
}

void displayCalendar(const doctorInfo *doctors) {
    const char *daysOfWeek[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    
    printf("══════════════════════════════════════════════════════════════════════════════\n");
    printf(" Doctor %s's Weekly Calendar:                                     \n", doctors->doctorName);
    printf("══════════════════════════════════════════════════════════════════════════════\n");
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%-9s: ", daysOfWeek[i]);
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            printf("%-12s", doctors->calendar[i][j] ? "[Available]" : "[Unavailable]");
        }
        printf("\n");
    }
    printf("══════════════════════════════════════════════════════════════════════════════\n");
}


void updateCalendar(doctorInfo *doctors) {
    int day, slot;
    printf("Enter the day you want to update (0 for Monday, 1 for Tuesday, ..., 6 for Sunday): ");
    scanf("%d", &day);
    printf("Enter the slot you want to update (0 to 4): ");
    scanf("%d", &slot);

    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) {
        printf("Invalid day or slot. Please try again.\n");
        return;
    }

    printf("Enter 1 for available or 0 for unavailable: ");
    int status;
    scanf("%d", &status);
    
    if (status != 0 && status != 1) {
        printf("Invalid status. Please try again.\n");
        return;
    }

    doctors->calendar[day][slot] = status;
    printf("Calendar updated successfully.\n");
    printf("Updated schedule:\n");
    displayCalendar(doctors);
}

void notFound(char k)
{
    if(k == 'p')
    {
        printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                       PATIENT NOT FOUND                         ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", RED, RESET);
    }
    else if(k == 'd')
    {
        printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                        DOCTOR NOT FOUND                         ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", RED, RESET);
    
    }
}

int docLogIn(const doctorInfo *doctors, int maxCount, const char *inputUser, const char *pass)
{
    for (int i = 0; i < maxCount; i++)
    {
        if (doctors[i].id != -1 && strcmp(doctors[i].doctorUserName, inputUser) == 0 && strcmp(doctors[i].doctorPass, pass) == 0)
        {
            return i;
        }
    }
    return -1;
}

void displayDoctorSubMenu() {
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║           Doctor Sub-Panel             ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Update schedule                     ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Edit information                    ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Prescribe patient                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Back to doctor menu                 ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void editDoctorInfo(doctorInfo *doctor) {
    printf("Enter new name: ");
    inputString(doctor->doctorName, MAX_NAME_LENGTH);

    printf("Enter new address: ");
    inputString(doctor->doctorAddress, MAX_ADDRESS_LENGTH);

    printf("Enter new specialty: ");
    inputString(doctor->specialty, MAX_DOCTOR_REMARK);

    printf("Enter new password: ");
    inputString(doctor->doctorPass, MAX_PASSWORD_LENGTH);

    printf("Enter new contact: ");
    inputString(doctor->doctorContact, MAX_CONTACT_LENGTH);

    printf("Enter new username: ");
    inputString(doctor->doctorUserName, MAX_USER_NAME);

    printf("Enter new experience: ");
    doctor->experience = inputInt();
}

void inputString(char* buffer, int length) {
    fgets(buffer, length, stdin);

    // Remove trailing newline character, if there is one
    if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n')) {
        buffer[strlen (buffer) - 1] = '\0';
    }
}

int inputInt() {
    int value;
    scanf("%d", &value);

    // Clear the input buffer
    while(getchar() != '\n');

    return value;
}

// Function to save IDs to a file
void saveIDs(int patientID, int doctorID) {
    FILE *file = fopen("ids.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d\n%d", patientID, doctorID);
    fclose(file);
}

// Function to load IDs from a file
void loadIDs(int *patientID, int *doctorID) {
    FILE *file = fopen("ids.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(file, "%d\n%d", patientID, doctorID);
    fclose(file);
}
