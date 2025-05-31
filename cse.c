make this code support both uppercase and lower case as input: #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Length definitions
#define MAX_PATIENTS 100
#define MAX_APPOINTMENTS 100
#define MAX_MEDICINE 100
#define MAX_EMPLOYES 30
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_BLOOD_GROUP_LENGTH 10
#define MAX_DEPARTMENT_LENGTH 50
#define MAX_MEDICAL_HISTORY_LENGTH 500
#define MAX_REASON_LENGTH 100
#define MAX_DATE_LENGTH 20
#define MAX_TIME_LENGTH 10
#define MAX_TIME_SLOT 16
#define MAX_TESTS 100

// Define structures
typedef struct {
    char name[100];
    float price;
} pathology;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int quantity;
    float price;
} Medicine;

typedef struct {
    long id;
    char name[MAX_NAME_LENGTH];
    int age;
    char gender[MAX_NAME_LENGTH];
    long contact_info;
    char medical_history[MAX_MEDICAL_HISTORY_LENGTH];
    char occupation[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char department[MAX_DEPARTMENT_LENGTH];
    char blood_group[MAX_BLOOD_GROUP_LENGTH];
    float weight;
    float height;
    int diastolic_Pressure;
    int systolic_Pressure;
} Patient;

typedef struct {
    long id;
    char name[MAX_NAME_LENGTH];
    int age;
    char gender[MAX_NAME_LENGTH];
    long contact_info;
    char medical_history[MAX_MEDICAL_HISTORY_LENGTH];
    char post[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char department[MAX_DEPARTMENT_LENGTH];
    char blood_group[MAX_BLOOD_GROUP_LENGTH];
    double salary;
} Employe;

typedef struct {
    char name[50];
    long patient_id;
    long doctor_id;
    char date[MAX_DATE_LENGTH];
    char time[MAX_TIME_LENGTH];
    char reason[MAX_REASON_LENGTH];
} Appointment;

// Global variables
pathology tests[MAX_TESTS];
int test_count = 0;

Medicine medicine[MAX_MEDICINE];
int medicine_count = 0;

Patient patients[MAX_PATIENTS];
int patient_count = 0;

Employe employes[MAX_EMPLOYES];
int employe_count = 0;

long doctor_List[MAX_EMPLOYES];
int doctor_count = 0;

Appointment appointments[MAX_APPOINTMENTS];
int appointment_count = 0;

long donation_id[MAX_PATIENTS];
int donation_count = 0;

const char *time_slots[MAX_TIME_SLOT] = {
    "09:00 AM", "09:30 AM", "10:00 AM", "10:30 AM", "11:00 AM", "11:30 AM",
    "12:00 PM", "12:30 PM", "01:00 PM", "01:30 PM", "02:00 PM", "02:30 PM",
    "03:00 PM", "03:30 PM", "04:00 PM", "04:30 PM"
};

// File paths
const char *PATIENTS_FILE = "patients.dat";
const char *EMPLOYEES_FILE = "employees.dat";
const char *APPOINTMENTS_FILE = "appointments.dat";
const char *MEDICINES_FILE = "medicines.dat";
const char *TESTS_FILE = "tests.dat";
const char *DONATIONS_FILE = "donations.dat";

// Function prototypes
void save_data();
void load_data();
void add_patient();
void view_patients();
void add_employe();
void view_employes();
void add_appointment();
void view_appointments();
void Add_Medicine();
void View();
void add_Test();
void search_Test();
void test_billing();
void login_menu();
void suggest_alternative_slots(long doctor_id, const char *date);
bool is_slot_available(long doctor_id, const char *date, const char *time);
int search_patient(long id);
void remove_patient_asPer_employe(long id);
void remove_doctor_asPer_employe(long id);

// Update the save_data() function
void save_data() {
    FILE *file;

    // Save patients
    file = fopen(PATIENTS_FILE, "wb");
    if (file) {
        fwrite(&patient_count, sizeof(int), 1, file);
        fwrite(patients, sizeof(Patient), patient_count, file);
        fclose(file);
    }

    // Save employees
    file = fopen(EMPLOYEES_FILE, "wb");
    if (file) {
        fwrite(&employe_count, sizeof(int), 1, file);
        fwrite(employes, sizeof(Employe), employe_count, file);
        fclose(file);
    }

    // Save appointments
    file = fopen(APPOINTMENTS_FILE, "wb");
    if (file) {
        fwrite(&appointment_count, sizeof(int), 1, file);
        fwrite(appointments, sizeof(Appointment), appointment_count, file);
        fclose(file);
    }

    // Save medicines
    file = fopen(MEDICINES_FILE, "wb");
    if (file) {
        fwrite(&medicine_count, sizeof(int), 1, file);
        fwrite(medicine, sizeof(Medicine), medicine_count, file);
        fclose(file);
    }

    // Save tests
    file = fopen(TESTS_FILE, "wb");
    if (file) {
        fwrite(&test_count, sizeof(int), 1, file);
        fwrite(tests, sizeof(pathology), test_count, file);
        fclose(file);
    }

    // Save donations - fixed this part
    file = fopen(DONATIONS_FILE, "wb");
    if (file) {
        fwrite(&donation_count, sizeof(int), 1, file);
        fwrite(donation_id, sizeof(long), donation_count, file);
        fclose(file);
    }
}

// Update the load_data() function
void load_data() {
    FILE *file;

    // Load patients
    file = fopen(PATIENTS_FILE, "rb");
    if (file) {
        fread(&patient_count, sizeof(int), 1, file);
        fread(patients, sizeof(Patient), patient_count, file);
        fclose(file);
    }

    // Load employees and rebuild doctor list
    file = fopen(EMPLOYEES_FILE, "rb");
    if (file) {
        fread(&employe_count, sizeof(int), 1, file);
        fread(employes, sizeof(Employe), employe_count, file);
        fclose(file);
        
        // Rebuild doctor list after loading employees
        doctor_count = 0;
        for (int i = 0; i < employe_count; i++) {
            if (strcmp(employes[i].post, "doctor") == 0) {
                doctor_List[doctor_count++] = employes[i].id;
            }
        }
    }

    // Load appointments
    file = fopen(APPOINTMENTS_FILE, "rb");
    if (file) {
        fread(&appointment_count, sizeof(int), 1, file);
        fread(appointments, sizeof(Appointment), appointment_count, file);
        fclose(file);
    }

    // Load medicines
    file = fopen(MEDICINES_FILE, "rb");
    if (file) {
        fread(&medicine_count, sizeof(int), 1, file);
        fread(medicine, sizeof(Medicine), medicine_count, file);
        fclose(file);
    }

    // Load tests
    file = fopen(TESTS_FILE, "rb");
    if (file) {
        fread(&test_count, sizeof(int), 1, file);
        fread(tests, sizeof(pathology), test_count, file);
        fclose(file);
    }

    // Load donations - fixed this part
    file = fopen(DONATIONS_FILE, "rb");
    if (file) {
        fread(&donation_count, sizeof(int), 1, file);
        fread(donation_id, sizeof(long), donation_count, file);
        fclose(file);
    }
}


// Function to check if a time slot is available
bool is_slot_available(long doctor_id, const char *date, const char *time) {
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].doctor_id == doctor_id &&
            strcmp(appointments[i].date, date) == 0 &&
            strcmp(appointments[i].time, time) == 0) {
            return false;
        }
    }
    return true;
}

// Function to suggest alternative time slots
void suggest_alternative_slots(long doctor_id, const char *date) {
    printf("\nSuggested alternative time slots for %s:\n", date);
    for (int i = 0; i < MAX_TIME_SLOT; i++) {
        if (is_slot_available(doctor_id, date, time_slots[i])) {
            printf("%d. %s\n", i + 1, time_slots[i]);
        }
    }
}

int search_employe(long id) {
    for (int i = 0; i < employe_count; i++) {
        if (employes[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Function to add a patient from an employee
void add_patient_from_Employe(long id) {
    if (patient_count >= MAX_PATIENTS) {
        printf("Max patient limit reached.\n");
        return;
    }

    int index = search_employe(id);
    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }

    Patient new_patient;
    strcpy(new_patient.name, employes[index].name);
    new_patient.id = employes[index].id;

    if (search_patient(new_patient.id) != -1) {
        printf("This ID already exists.\n");
        return;
    }

    strcpy(new_patient.department, employes[index].department);
    strcpy(new_patient.occupation, employes[index].post);
    strcpy(new_patient.blood_group, employes[index].blood_group);
    new_patient.age = employes[index].age;
    strcpy(new_patient.gender, employes[index].gender);
    new_patient.contact_info = employes[index].contact_info;
    strcpy(new_patient.email, employes[index].email);
    strcpy(new_patient.address, employes[index].address);
    strcpy(new_patient.medical_history, employes[index].medical_history);

    patients[patient_count++] = new_patient;
    printf("Employe added successfully as patient!\n");
}

// Function to add an employee
void add_employe() {
    if (employe_count >= MAX_EMPLOYES) {
        printf("Max employee limit reached.\n");
        return;
    }

    Employe new_employe;

    printf("\nEnter name: ");
    scanf(" %49[^\n]", new_employe.name);
    printf("Enter ID: ");
    scanf("%ld", &new_employe.id);

    if (search_employe(new_employe.id) != -1) {
        printf("This ID already exists.\n");
        return;
    }
 
    // Initialize medical_history to empty string
    strcpy(new_employe.medical_history, "");
    printf("Enter department: ");
    scanf(" %49[^\n]", new_employe.department);
    printf("Enter post: ");
    scanf(" %49[^\n]", new_employe.post);
    printf("Enter blood group: ");
    scanf("%9s", new_employe.blood_group);
    printf("Enter age: ");
    scanf("%d", &new_employe.age);
    printf("Enter gender: ");
    scanf("%9s", new_employe.gender);
    printf("Enter contact info: ");
    scanf("%ld", &new_employe.contact_info);
    printf("Enter email: ");
    scanf("%49s", new_employe.email);
    printf("Enter address: ");
    scanf(" %99[^\n]", new_employe.address);
    printf("Enter salary: ");
    scanf("%lf", &new_employe.salary);

    employes[employe_count++] = new_employe;
    if (strcmp(new_employe.post, "doctor") == 0) {
        doctor_List[doctor_count++] = new_employe.id;
    }
    add_patient_from_Employe(new_employe.id);
}

// Function to view all employees
void view_employes() {
    if (employe_count == 0) {
        printf("No employees found.\n");
        return;
    }

    printf("\nEmployee Records:\n");
    printf("Total Employees: %d\n\n", employe_count);

    for (int i = 0; i < employe_count; i++) {
        printf("ID: %ld\n", employes[i].id);
        printf("Name: %s\n", employes[i].name);
        printf("Department: %s\n", employes[i].department);
        printf("Post: %s\n", employes[i].post);
        printf("Salary: %.2lf\n", employes[i].salary);
        printf("Blood Group: %s\n", employes[i].blood_group);
        printf("Age: %d\n", employes[i].age);
        printf("Gender: %s\n", employes[i].gender);
        printf("Contact Info: %ld\n", employes[i].contact_info);
        printf("Email: %s\n", employes[i].email);
        printf("Address: %s\n\n", employes[i].address);
    }
}

// Function to search for an employee by ID and print details
void search_employe_and_print() {
    long id;
    printf("Enter Employee ID to search: ");
    scanf("%ld", &id);

    int index = search_employe(id);
    if (index != -1) {
        printf("Employee found:\n\n");
        printf("ID: %ld\n", employes[index].id);
        printf("Name: %s\n", employes[index].name);
        printf("Department: %s\n", employes[index].department);
        printf("Post: %s\n", employes[index].post);
        printf("Salary: %.2lf\n", employes[index].salary);
        printf("Blood Group: %s\n", employes[index].blood_group);
        printf("Age: %d\n", employes[index].age);
        printf("Gender: %s\n", employes[index].gender);
        printf("Contact Info: %ld\n", employes[index].contact_info);
        printf("Email: %s\n", employes[index].email);
        printf("Address: %s\n", employes[index].address);
    } else {
        printf("Employee not found.\n");
    }
}

// Function to remove an employee
void remove_employe() {
    long id;
    printf("Enter Employee ID to remove: ");
    scanf("%ld", &id);

    int index = search_employe(id);
    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }
    
    // Remove employee from patient list
    remove_patient_asPer_employe(id);
    // Remove employee from doctor list
    remove_doctor_asPer_employe(id);
    
    for (int i = index; i < employe_count - 1; i++) {
        employes[i] = employes[i + 1];
    }

    employe_count--;
    printf("Employee removed successfully.\n");
}

// Function to view all doctors
void doctor_view() {
    if (doctor_count == 0) {
        printf("No doctors found.\n");
        return;
    }

    for (int j = 0; j < doctor_count; j++) {
        for (int i = 0; i < employe_count; i++) {
            if (employes[i].id == doctor_List[j]) {
                printf("\nName: %s\n", employes[i].name);
                printf("ID: %ld\n", employes[i].id);
                printf("Department: %s\n\n", employes[i].department);
            }
        }
    }
}

// Update the doctor_view_appointmentView function
void doctor_view_appointmentView(long id) {
    int found = 0;
    for (int i = 0; i < employe_count; i++) {
        if (employes[i].id == id && strcmp(employes[i].post, "doctor") == 0) {
            printf("Doctor Name: %s\n", employes[i].name);
            printf("Doctor ID: %ld\n", employes[i].id);
            printf("Department: %s\n", employes[i].department);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Doctor details not found for ID: %ld\n", id);
    }
}

// Function to remove a patient based on employee ID
void remove_patient_asPer_employe(long id) {
    int index = search_patient(id);
    if (index == -1) {
        return;
    }

    for (int i = index; i < patient_count - 1; i++) {
        patients[i] = patients[i + 1];
    }

    patient_count--;
    for(int i=0;i<donation_count;i++){
        if(donation_id[i]==id){ 
         for(int j=i;j<donation_count-1;j++){
             donation_id[j]=donation_id[j+1];
         }
         donation_count--;
        }
    }
}

// Function to remove a doctor based on employee ID
void remove_doctor_asPer_employe(long id) {
    for (int i = 0; i < doctor_count; i++) {
        if (doctor_List[i] == id) {
            for (int j = i; j < doctor_count - 1; j++) {
                doctor_List[j] = doctor_List[j + 1];
            }
            doctor_count--;
            return;
        }
    }
}

// Function to search for a patient by ID
int search_patient(long id) {
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Function to add a patient
void add_patient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("Max patient limit reached.\n");
        return;
    }

    Patient new_patient;

    printf("Enter name: ");
    scanf(" %49[^\n]", new_patient.name);
    printf("Enter ID: ");
    scanf("%ld", &new_patient.id);

    if (search_patient(new_patient.id) != -1) {
        printf("This ID already exists.\n");
        return;
    }

    printf("Enter department: ");
    scanf(" %49[^\n]", new_patient.department);
    printf("Enter occupation: ");
    scanf(" %49[^\n]", new_patient.occupation);
    printf("Enter blood group: ");
    scanf("%9s", new_patient.blood_group);
    printf("Enter age: ");
    scanf("%d", &new_patient.age);
    printf("Enter gender: ");
    scanf("%9s", new_patient.gender);
    printf("Enter contact info: ");
    scanf("%ld", &new_patient.contact_info);
    printf("Enter email: ");
    scanf("%49s", new_patient.email);
    printf("Enter address: ");
    scanf(" %99[^\n]", new_patient.address);

    // Initialize medical_history to empty string
    strcpy(new_patient.medical_history, "");
    patients[patient_count++] = new_patient;
    printf("Patient added successfully!\n");
}

// Function to view all patients
void view_patients() {
    if (patient_count == 0) {
        printf("No patients found.\n");
        return;
    }

    printf("\nPatient Records:\n");
    printf("Total Patients: %d\n\n", patient_count);

    for (int i = 0; i < patient_count; i++) {
        printf("ID: %ld\n", patients[i].id);
        printf("Name: %s\n", patients[i].name);
        printf("Department: %s\n", patients[i].department);
        printf("Occupation: %s\n", patients[i].occupation);
        printf("Blood Group: %s\n", patients[i].blood_group);
        printf("Age: %d\n", patients[i].age);
        printf("Gender: %s\n", patients[i].gender);
        printf("Contact Info: %ld\n", patients[i].contact_info);
        printf("Email: %s\n", patients[i].email);
        printf("Address: %s\n\n", patients[i].address);
    }
}

// Function to search for a patient by ID and print details
void search_patient_and_print() {
    long id;
    printf("Enter Patient ID to search: ");
    scanf("%ld", &id);

    int index = search_patient(id);
    if (index != -1) {
        printf("Patient found:\n\n");
        printf("ID: %ld\n", patients[index].id);
        printf("Name: %s\n", patients[index].name);
        printf("Department: %s\n", patients[index].department);
        printf("Occupation: %s\n", patients[index].occupation);
        printf("Blood Group: %s\n", patients[index].blood_group);
        printf("Age: %d\n", patients[index].age);
        printf("Gender: %s\n", patients[index].gender);
        printf("Contact Info: %ld\n", patients[index].contact_info);
        printf("Email: %s\n", patients[index].email);
        printf("Address: %s\n", patients[index].address);
    } else {
        printf("Patient not found.\n");
    }
}

// Function to remove a patient
void remove_patient() {
    long id;
    printf("Enter Patient ID to remove: ");
    scanf("%ld", &id);

    int index = search_patient(id);
    if (index == -1) {
        printf("Patient not found.\n");
        return;
    }

    for (int i = index; i < patient_count - 1; i++) {
        patients[i] = patients[i + 1];
    }

    patient_count--;
    for(int i=0;i<donation_count;i++){
       if(donation_id[i]==id){ 
        for(int j=i;j<donation_count-1;j++){
            donation_id[j]=donation_id[j+1];
        }
        donation_count--;
       }
    }
    printf("Patient removed successfully.\n");
}

// Function to search for patients by blood group
void search_patient_and_print_forBlood(char blood_group[]) {
    for (int i = 0; i < patient_count; i++) {
        if (strcmp(patients[i].blood_group, blood_group) == 0) {
            printf("\nID: %ld\n", patients[i].id);
            printf("Name: %s\n", patients[i].name);
            printf("Department: %s\n", patients[i].department);
            printf("Occupation: %s\n", patients[i].occupation);
            printf("Blood Group: %s\n", patients[i].blood_group);
            printf("Age: %d\n", patients[i].age);
            printf("Gender: %s\n", patients[i].gender);
            printf("Contact Info: %ld\n", patients[i].contact_info);
            printf("Email: %s\n", patients[i].email);
            printf("Address: %s\n\n", patients[i].address);
        }
    }
}

// Function to search for blood group
void search_blood() {
    char blood_group[MAX_BLOOD_GROUP_LENGTH];
    printf("Enter Blood Group: ");
    scanf("%9s", blood_group);

    search_patient_and_print_forBlood(blood_group);
}

// Function to handle blood donation
void blood_donation() {
    long id;
    printf("Enter ID: ");
    scanf("%ld", &id);
    int index = search_patient(id);
    if(index==-1){
        printf("\nInvalid id\n");
        return;
    }

    for(int i=0;i<donation_count;i++){
        if(donation_id[i]==id){ 
          printf("\nDonation Already Exist\n");
          return;
        }
    }

    donation_id[donation_count++] = id;
    printf("Donation request complete.\n");
}

// Function to handle blood request
void blood_request() {
    long id;
    char blood_group[MAX_BLOOD_GROUP_LENGTH];

    printf("Enter ID: ");
    scanf("%ld", &id);
    int index = search_patient(id);
    if(index==-1){
        printf("\nInvalid id\n");
        return;
    }
    printf("Require Blood: ");
    scanf("%9s", blood_group);
    
    int blood_found = 0;

    for (int i = 0; i < donation_count; i++) {
        int index = search_patient(donation_id[i]);
        if (index != -1) {
            if (strcmp(patients[index].blood_group, blood_group) == 0) {
                printf("Blood found:\n\n");
                printf("ID: %ld\n", patients[index].id);
                printf("Name: %s\n", patients[index].name);
                printf("Department: %s\n", patients[index].department);
                printf("Occupation: %s\n", patients[index].occupation);
                printf("Blood Group: %s\n", patients[index].blood_group);
                printf("Age: %d\n", patients[index].age);
                printf("Gender: %s\n", patients[index].gender);
                printf("Contact Info: %ld\n", patients[index].contact_info);
                printf("Email: %s\n", patients[index].email);
                printf("Address: %s\n", patients[index].address);
                printf("Medical History: %s\n\n", patients[index].medical_history);

                blood_found = 1;
                char dicision;
                printf("If Donor accepts, enter (y/Y): ");
                scanf(" %c", &dicision);
                if (dicision == 'y' || dicision == 'Y') {
                    for (int j = i; j < donation_count - 1; j++) {
                        donation_id[j] = donation_id[j + 1];
                    }
                    donation_count--;
                    printf("Transaction Complete\n");
                }
                return;
            }
        }
    }

    if (!blood_found) {
        printf("Blood is not available.\n");
    }
}

// Function to add an appointment
void add_appointment() {
    if (appointment_count >= MAX_APPOINTMENTS) {
        printf("Max appointment limit reached.\n");
        return;
    }

    Appointment new_appointment;

    printf("Enter Patient ID: ");
    scanf("%ld", &new_appointment.patient_id);
    int index = search_patient(new_appointment.patient_id);
    if (index == -1) {
        printf("No patient found.\n");
        return;
    }
    strcpy(new_appointment.name,patients[index].name);

    doctor_view();
    printf("Enter Doctor ID: ");
    scanf("%ld", &new_appointment.doctor_id);

    int find_doctor = 0;
    for (int i = 0; i < doctor_count; i++) {
        if (doctor_List[i] == new_appointment.doctor_id) {
            find_doctor = 1;
            break;
        }
    }

    if (find_doctor == 1) {
        printf("Enter Date (DD/MM/YYYY): ");
        scanf("%19s", new_appointment.date);
        
        printf("Available time slots:\n");
        for (int i = 0; i < MAX_TIME_SLOT; i++) {
            printf("%d. %s\n", i+1, time_slots[i]);
        }
        
        printf("Enter preferred time slot number (1-%d): ", MAX_TIME_SLOT);
        int slot_choice;
        scanf("%d", &slot_choice);
        
        if (slot_choice < 1 || slot_choice > MAX_TIME_SLOT) {
            printf("Invalid time slot choice.\n");
            return;
        }
        
        strcpy(new_appointment.time, time_slots[slot_choice-1]);
        
        // Check if slot is available
        if (is_slot_available(new_appointment.doctor_id, new_appointment.date, new_appointment.time)) {
            printf("Enter reason for appointment: ");
            scanf(" %99[^\n]", new_appointment.reason);
        } else {
            printf("\nThis Time Slot is not Available\n");
            suggest_alternative_slots(new_appointment.doctor_id, new_appointment.date);
            printf("Enter preferred time slot number (1-%d): ", MAX_TIME_SLOT);
            scanf("%d", &slot_choice);
        
            if (slot_choice < 1 || slot_choice > MAX_TIME_SLOT) {
                printf("Invalid time slot choice.\n");
                return;
            }
        
            strcpy(new_appointment.time, time_slots[slot_choice-1]);
            printf("Enter reason for appointment: ");
            scanf(" %99[^\n]", new_appointment.reason);
        }

        appointments[appointment_count++] = new_appointment;
        printf("Appointment added successfully!\n");
    } else {
        printf("Invalid Doctor ID.\n");
    }
}

// Update the view_appointments function
void view_appointments() {
    if (appointment_count == 0) {
        printf("No appointments found.\n");
        return;
    }

    printf("\nAppointment Records:\n");
    printf("Total Appointments: %d\n\n", appointment_count);

    for (int i = 0; i < appointment_count; i++) {
        printf("Patient Name: %s\n", appointments[i].name);
        printf("Patient ID: %ld\n", appointments[i].patient_id);
        
        // Find and display doctor information
        int doc_found = 0;
        for (int j = 0; j < employe_count; j++) {
            if (employes[j].id == appointments[i].doctor_id && strcmp(employes[j].post, "doctor") == 0) {
                printf("Doctor Name: %s\n", employes[j].name);
                printf("Doctor ID: %ld\n", employes[j].id);
                printf("Department: %s\n", employes[j].department);
                doc_found = 1;
                break;
            }
        }
        if (!doc_found) {
            printf("Doctor information not available for ID: %ld\n", appointments[i].doctor_id);
        }
        
        printf("Date: %s\n", appointments[i].date);
        printf("Time: %s\n", appointments[i].time);
        printf("Reason: %s\n\n", appointments[i].reason);
    }
}

// Function to search for appointments by patient ID
void search_appointment_by_patient() {
    int found=0;
    long patient_id;
    printf("Enter Patient ID: ");
    scanf("%ld", &patient_id);
    int index = search_patient(patient_id);
    if(index ==-1){
        printf("Invalid patient id\n");
        return;
    }

    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].patient_id == patient_id) {
            printf("Appointment found:\n\n");
            printf("Patient ID: %ld\n", appointments[i].patient_id);
            printf("Patient Name: %s\n", appointments[i].name);
            int index = search_employe(appointments[i].doctor_id);
            printf("Doctor Name: %s\n", employes[index].name);
            printf("Doctor ID: %ld\n", appointments[i].doctor_id);
            printf("Date: %s\n", appointments[i].date);
            printf("Time: %s\n", appointments[i].time);
            printf("Reason: %s\n\n", appointments[i].reason);
            found = 1;
        }
    }
    if(found==0){
        printf("No appointments found for the given Patient ID.\n");
    }
}

// Function to search for appointments by doctor ID
void search_appointment_by_doctor() {
    int found=0;
    doctor_view();
    long doctor_id;
    printf("Enter Doctor ID: ");
    scanf("%ld", &doctor_id);
    int index = search_employe(doctor_id);
    if(index ==-1){
        printf("Invalid Doctor id\n");
        return;
    }

    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].doctor_id == doctor_id) {
            printf("Appointment found:\n\n");
            printf("Patient ID: %ld\n", appointments[i].patient_id);
            printf("Patient Name: %s\n", appointments[i].name);
            printf("Doctor Name: %s\n", employes[index].name);
            printf("Doctor ID: %ld\n", appointments[i].doctor_id);
            printf("Date: %s\n", appointments[i].date);
            printf("Time: %s\n", appointments[i].time);
            printf("Reason: %s\n\n", appointments[i].reason);
            found=1;
        }
    }
    if(found==0){
        printf("No appointments found for the given Doctor ID.\n");
    }
}

// Function to remove an appointment
void remove_appointment() {
    doctor_view();
    long patient_id, doctor_id;
    printf("Enter Patient ID: ");
    scanf("%ld", &patient_id);
    printf("Enter Doctor ID: ");
    scanf("%ld", &doctor_id);

    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].patient_id == patient_id && appointments[i].doctor_id == doctor_id) {
            for (int j = i; j < appointment_count - 1; j++) {
                appointments[j] = appointments[j + 1];
            }
            appointment_count--;
            printf("Appointment removed successfully!\n");
            return;
        }
    }
    printf("No matching appointment found to remove.\n");
}

// Function to calculate BMI
void cal_BMI(float weight, float height) {
    float bmi = weight / (height * height);
    printf("BMI: %.2f - ", bmi);
    if (bmi < 18.5) {
        printf("Underweight\n");
    } else if (bmi < 25) {
        printf("Normal Weight\n");
    } else if (bmi < 30) {
        printf("Overweight\n");
    } else if (bmi < 35) {
        printf("Obesity Class 1 (Moderate)\n");
    } else if (bmi < 40) {
        printf("Obesity Class 2 (Severe)\n");
    } else {
        printf("Obesity Class 3 (Morbid)\n");
    }
}

// Function to comment on blood pressure
void comment_pressure(float diastolic, float systolic) {
    if (diastolic < 60 && systolic < 90) {
        printf("Low Pressure\n");
    } else if (diastolic <= 80 && systolic <= 120) {
        printf("Normal Pressure\n");
    } else if (diastolic <= 80 && systolic < 130) {
        printf("Elevated Blood Pressure\n");
    } else if (diastolic < 90 && systolic < 140) {
        printf("Hypertension\n");
        printf("High Pressure Stage 1\n");
    } else if (diastolic < 120 && systolic < 180) {
        printf("Hypertension\n");
        printf("High Pressure Stage 2\n");
    } else {
        printf("Hypertensive Crisis (Emergency Care Needed)\n");
    }
}

// Function to record vitals
void vitals(int index) {
    char date[50];
    printf("Enter Date: ");
    scanf("%49s", date);

    char weight_str[20], height_str[20], systolic_str[20], diastolic_str[20];

    printf("\nEnter weight in KG: ");
    scanf("%f", &patients[index].weight);
    printf("Enter height in meters: ");
    scanf("%f", &patients[index].height);
    printf("Enter diastolic pressure: ");
    scanf("%d", &patients[index].diastolic_Pressure);
    printf("Enter systolic pressure: ");
    scanf("%d", &patients[index].systolic_Pressure);

    sprintf(weight_str, "%.2f", patients[index].weight);
    sprintf(height_str, "%.2f", patients[index].height);
    sprintf(systolic_str, "%d", patients[index].systolic_Pressure);
    sprintf(diastolic_str, "%d", patients[index].diastolic_Pressure);
    char pati_ent_id[20]; 
    sprintf(pati_ent_id, "%ld", patients[index].id);
    
    strcat(patients[index].medical_history, "\t\t\tPatient Name: ");
    strcat(patients[index].medical_history, patients[index].name);
    strcat(patients[index].medical_history, "\n");
    strcat(patients[index].medical_history, "\t\t\tPatient id: ");
    strcat(patients[index].medical_history, pati_ent_id);
    strcat(patients[index].medical_history, "\n\n");

    strcat(patients[index].medical_history, "Date: ");
    strcat(patients[index].medical_history, date);
    strcat(patients[index].medical_history, "\n\n");

    strcat(patients[index].medical_history, "Weight: ");
    strcat(patients[index].medical_history, weight_str);
    strcat(patients[index].medical_history, " kg\n");

    strcat(patients[index].medical_history, "Height: ");
    strcat(patients[index].medical_history, height_str);
    strcat(patients[index].medical_history, " m\n");

    strcat(patients[index].medical_history, "Blood Pressure: ");
    strcat(patients[index].medical_history, systolic_str);
    strcat(patients[index].medical_history, "/");
    strcat(patients[index].medical_history, diastolic_str);
    strcat(patients[index].medical_history, " mmHg\n\n");

    cal_BMI(patients[index].weight, patients[index].height);
    comment_pressure(patients[index].diastolic_Pressure, patients[index].systolic_Pressure);
}

// Function to add a prescription
void prescription(int index) {
    char medicine[100];
    char test[100];
    char comment[200];
    char id_str[20];
    long doctor_id;
    int found_doc = 0;

    printf("\nEnter Prescription for Patient: %s\n", patients[index].name);
    printf("\nPatient id: %ld\n", patients[index].id);

    printf("Enter Doctor id: ");
    scanf("%ld", &doctor_id);

    // Search for doctor ID in the doctor list
    for (int i = 0; i < doctor_count; i++) {
        if (doctor_List[i] == doctor_id) {
            found_doc = 1;
            break;
        }
    }

    // Clear the input buffer
    while(getchar() != '\n');

    if (found_doc == 0) {
        printf("\nDoctor id not found !\n");
        return;
    }

    printf("Enter All Medicine Names (separated by commas): ");
    scanf(" %99[^\n]", medicine);
    while(getchar() != '\n');

    // Append the medicine to the patient's medical history
    strcat(patients[index].medical_history, "Medicine: ");
    strcat(patients[index].medical_history, medicine);
    strcat(patients[index].medical_history, "\n\n");

    printf("Enter All Test Names (separated by commas): ");
    scanf(" %99[^\n]", test);
    while(getchar() != '\n');

    // Append the tests to the patient's medical history
    strcat(patients[index].medical_history, "Test: ");
    strcat(patients[index].medical_history, test);
    strcat(patients[index].medical_history, "\n\n");

    printf("Enter Comment: ");
    scanf(" %199[^\n]", comment);
    while(getchar() != '\n');

    strcat(patients[index].medical_history, "Comment: ");
    strcat(patients[index].medical_history, comment);
    strcat(patients[index].medical_history, "\n\n");

    // Append doctor information
    sprintf(id_str, "%ld", doctor_id);
    strcat(patients[index].medical_history, "Doctor: ");
    int emp_index = search_employe(doctor_id);
    if(emp_index != -1) {
        strcat(patients[index].medical_history, employes[emp_index].name);
    }
    strcat(patients[index].medical_history, "\n");
    strcat(patients[index].medical_history, "Doctor id: ");
    strcat(patients[index].medical_history, id_str);
    strcat(patients[index].medical_history, "\n");

    printf("\nPrescription added successfully!\n");
}

// Function to view patient history
void patient_history(int index) {
    if (strlen(patients[index].medical_history) == 0) {
        printf("No medical history found for patient: %s\n", patients[index].name);
        return;
    }

    printf("---------------------------------\n");
    printf("%s\n", patients[index].medical_history);
    printf("---------------------------------\n");
}

// Function to add a new medicine
void Add_Medicine() {
    if (medicine_count >= MAX_MEDICINE) {
        printf("Medicine list is full.\n");
        return;
    }

    Medicine new_medicine;
    printf("Enter Medicine Name: ");
    scanf(" %49[^\n]", new_medicine.name);
    printf("Enter Quantity: ");
    scanf("%d", &new_medicine.quantity);
    printf("Enter Price: ");
    scanf("%f", &new_medicine.price);
    
    for(int i=0;i<medicine_count;i++){
        if(strcmp(medicine[i].name, new_medicine.name) == 0) {
            medicine[i].quantity += new_medicine.quantity;
            medicine[i].price = new_medicine.price;
            printf("Medicine quantity updated successfully!\n");
            return;
        }
    }

    medicine[medicine_count++] = new_medicine;
    printf("Medicine added successfully!\n");
}

// Function to view all medicines
void View() {
    if (medicine_count == 0) {
        printf("No medicines found.\n");
        return;
    }

    printf("\nList of Medicines:\n");
    for (int i = 0; i < medicine_count; i++) {
        printf("Name: %s\n", medicine[i].name);
        printf("Quantity: %d\n", medicine[i].quantity);
        printf("Price: %.2f\n", medicine[i].price);
        printf("---------------------\n");
    }
}

// Function to search for a medicine by name
void Search() {
    char search_name[MAX_NAME_LENGTH];
    printf("Enter Medicine Name: ");
    scanf(" %49[^\n]", search_name);

    for (int i = 0; i < medicine_count; i++) {
        if (strcmp(medicine[i].name, search_name) == 0) {
            printf("Medicine Found:\n\n");
            printf("Name: %s\n", medicine[i].name);
            printf("Quantity: %d\n", medicine[i].quantity);
            printf("Price: %.2f\n", medicine[i].price);
            return;
        }
    }
    printf("Medicine not found.\n");
}

// Function to handle billing
void Billing() {
    char billing_name[MAX_NAME_LENGTH];
    int billing_quantity;
    float total_price = 0.0;

    while (1) {
        printf("\nEnter Medicine Name (type 'STOP' to finish): ");
        scanf("%s", billing_name);

        if (strcmp(billing_name, "STOP") == 0) {
            break;
        }

        printf("Enter Quantity: ");
        scanf("%d", &billing_quantity);

        int found = 0;
        float price = 0.0;

        for (int i = 0; i < medicine_count; i++) {
            if (strcmp(medicine[i].name, billing_name) == 0) {
                found = 1;
                if (medicine[i].quantity >= billing_quantity) {
                    price = billing_quantity * (medicine[i].price + (0.25 * medicine[i].price));
                    medicine[i].quantity -= billing_quantity;
                    total_price += price;

                    printf("Item Added:\n");
                    printf("Name: %s\n", medicine[i].name);
                    printf("Quantity: %d\n", billing_quantity);
                    printf("Price: %.2f\n", price);
                } else {
                    printf("Insufficient stock for %s. Available: %d\n", medicine[i].name, medicine[i].quantity);
                }
                break;
            }
        }

        if (!found) {
            printf("Medicine not found.\n");
        }
    }

    printf("\n---- Final Billing Summary ----\n");
    printf("Total Bill Amount: %.2f\n", total_price);
    printf("-------------------------------\n");
}

// Function to add a test
void add_Test() {
    if (test_count >= MAX_TESTS) {
        printf("Cannot add more tests.\n");
        return;
    }
    pathology new_test;
    printf("Enter test name: ");
    scanf(" %99[^\n]", new_test.name);
    printf("Enter test price: ");
    scanf("%f", &new_test.price);
    
    for(int i=0;i<test_count;i++){
        if(strcmp(tests[i].name, new_test.name)==0){
            tests[i].price = new_test.price;
            printf("Test price updated successfully.\n");
            return;
        }
    }

    tests[test_count++]=new_test;
    printf("Test added successfully.\n");
}

// Function to search for a test
void search_Test() {
    char search_name[100];
    int found = 0;

    printf("Enter test name to search: ");
    scanf(" %99[^\n]", search_name);

    for (int i = 0; i < test_count; i++) {
        if (strcmp(tests[i].name, search_name) == 0) {
            printf("Test found:\n\n");
            printf("Name: %s\n", tests[i].name);
            printf("Price: %.2f\n", tests[i].price);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Test not found.\n");
    }
}

// Function to bill tests
void test_billing() {
    char billing_name[100];
    float total_price = 0.0;

    while (1) {
        printf("\nBilling Name (type 'STOP' to finish): ");
        scanf("%s", billing_name);

        if (strcmp(billing_name, "STOP") == 0) {
            break;
        }

        int found = 0;
        float price = 0.0;

        for (int i = 0; i < test_count; i++) {
            if (strcmp(tests[i].name, billing_name) == 0) {
                found = 1;
                price = tests[i].price;
                total_price += price;

                printf("Item Added:\n");
                printf("Name: %s\n", tests[i].name);
                printf("Price: %.2f\n", price);
                break;
            }
        }

        if (!found) {
            printf("Test not found.\n");
        }
    }

    printf("\n---- Final Billing Summary ----\n");
    printf("Total Bill Amount: %.2f\n", total_price);
    printf("-------------------------------\n");
}

// Function to manage patient operations
void patientManagement_menu() {
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Add Patient\n");
        printf("2. View Patients\n");
        printf("3. Search Patient\n");
        printf("4. Remove Patient\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_patient();
                break;
            case 2:
                view_patients();
                break;
            case 3:
                search_patient_and_print();
                break;
            case 4:
                remove_patient();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to manage appointment operations
void appointmentManagement_menu() {
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Add Appointment\n");
        printf("2. View Appointments\n");
        printf("3. Search Appointment by Patient\n");
        printf("4. Search Appointment by Doctor\n");
        printf("5. Remove Appointment\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_appointment();
                break;
            case 2:
                view_appointments();
                break;
            case 3:
                search_appointment_by_patient();
                break;
            case 4:
                search_appointment_by_doctor();
                break;
            case 5:
                remove_appointment();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to manage clinical operations
void clinicalManagement_menu() {
    int choice;
    long id;
    printf("Enter Patient ID: ");
    scanf("%ld", &id);

    int index = search_patient(id);
    if (index == -1) {
        printf("Invalid ID.\n");
        return;
    }

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Vitals\n");
        printf("2. Prescription\n");
        printf("3. Patient History\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                vitals(index);
                break;
            case 2:
                prescription(index);
                break;
            case 3:
                patient_history(index);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to manage pharmacy operations
void pharmacyManagement_menu() {
    int choice;

    while (1) {
        printf("\nPharmacist Menu\n");
        printf("1. Add Medicine\n");
        printf("2. View Medicines\n");
        printf("3. Search Medicine\n");
        printf("4. Billing\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Add_Medicine();
                break;
            case 2:
                View();
                break;
            case 3:
                Search();
                break;
            case 4:
                Billing();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to manage pathology operations
void pathologyManagement_menu() {
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. ADD Test\n");
        printf("2. Search Test\n");
        printf("3. Patient test procedure with billing\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_Test();
                break;
            case 2:
                search_Test();
                break;
            case 3:
                test_billing();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to manage blood bank operations
void bloodBankManagement_menu() {
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Blood Donation\n");
        printf("2. Blood Request\n");
        printf("3. Search Blood\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                blood_donation();
                break;
            case 2:
                blood_request();
                break;
            case 3:
                search_blood();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to display emergency contact information
void emargancyContact_menu() {
    printf("\t\tEmergency Contact\n");
    printf("Northern University Hotline : \n\n");
    printf("\t01799994363\n");
    printf("\t01305280410\n");
    printf("\t01970070090\n");
    printf("\t01916438801\n");
    printf("\t01919296363\n\n");
    printf("Emergency Ambulance : \n\n");
    printf("\tBSMMU : 8614001\n");
    printf("\tDay-Night Ambulance : 9132023\n");
    printf("\tFire Service Ambulance : 9555555\n");
    printf("\tRed Crescent : 9330188\n\n");
    printf("Blood Bank : \n\n");
    printf("\tSandhani(DMCH) : 9668690\n");
    printf("\tQuantum Foundation : 9351989\n");
    printf("\tRed Crescent : 8121497\n\n");
    printf("Hospital & Clinic : \n\n");
    printf("\tAd-Din Hospital : 9350932\n");
    printf("\tIBN Sina Hospital : 8113709\n");
    printf("\tUnited Hospital : 8836000\n\n");
    printf("Police Hotline : \n\n");
    printf("\tDMP Control Room : 01713-398311\n");
    printf("\tOC(Airport Zone) : 01713-398585\n\n");
    printf("Fire Service Hotline : \n\n");
    printf("\tCentral Control Room : 9556666\n\n");
}

// Function to manage employee operations
void employeManagement_menu() {
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Add Employee\n");
        printf("2. View Employees\n");
        printf("3. Search Employee\n");
        printf("4. Remove Employee\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_employe();
                break;
            case 2:
                view_employes();
                break;
            case 3:
                search_employe_and_print();
                break;
            case 4:
                remove_employe();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to handle doctor operations
void doctor() {
    char storedUsername[30] = "mashrafi";
    int storedPassword = 1773;

    char username[30];
    int pass;
    printf("Enter Username : ");
    scanf("%29s", username);

    printf("Enter Password : ");
    scanf("%d", &pass);

    if(strcmp(storedUsername, username) != 0 || storedPassword != pass) {
        printf("Login Failed! Invalid Username or Password.\n");
        return;
    }
    printf("Login Successful! Welcome, %s.\n", username);
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Patient Management\n");
        printf("2. Clinical Management\n");
        printf("3. Pathology\n");
        printf("4. Blood Bank\n");
        printf("5. Emergency Contact\n");
        printf("6. Back\n");
        printf("7. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientManagement_menu();
                break;
            case 2:
                clinicalManagement_menu();
                break;
            case 3:
                pathologyManagement_menu();
                break;
            case 4:
                bloodBankManagement_menu();
                break;
            case 5:
                emargancyContact_menu();
                break;
            case 6:
                return;
            case 7:
                save_data();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to handle nurse operations
void nurse() {
    char storedUsername[30] = "mashrafi";
    int storedPassword = 1773;

    char username[30];
    int pass;
    printf("Enter Username : ");
    scanf("%29s", username);

    printf("Enter Password : ");
    scanf("%d", &pass);

    if(strcmp(storedUsername, username) != 0 || storedPassword != pass) {
        printf("Login Failed! Invalid Username or Password.\n");
        return;
    }
    printf("Login Successful! Welcome, %s.\n", username);
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Patient Management\n");
        printf("2. Appointment\n");
        printf("3. Clinical Management\n");
        printf("4. Pathology\n");
        printf("5. Blood Bank\n");
        printf("6. Emergency Contact\n");
        printf("7. Back\n");
        printf("8. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientManagement_menu();
                break;
            case 2:
                appointmentManagement_menu();
                break;
            case 3:
                clinicalManagement_menu();
                break;
            case 4:
                pathologyManagement_menu();
                break;
            case 5:
                bloodBankManagement_menu();
                break;
            case 6:
                emargancyContact_menu();
                break;    
            case 7:
                return;
            case 8:
                save_data();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to handle pharmacist operations
void pharmacist() {
    char storedUsername[30] = "mashrafi";
    int storedPassword = 1773;

    char username[30];
    int pass;
    printf("Enter Username : ");
    scanf("%29s", username);

    printf("Enter Password : ");
    scanf("%d", &pass);

    if(strcmp(storedUsername, username) != 0 || storedPassword != pass) {
        printf("Login Failed! Invalid Username or Password.\n");
        return;
    }
    printf("Login Successful! Welcome, %s.\n", username);
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("\nPharmacist Menu\n\n");
        printf("1. Add Medicine\n");
        printf("2. View Medicines\n");
        printf("3. Search Medicine\n");
        printf("4. Billing\n");
        printf("5. Emergency Contact\n");
        printf("6. Back\n");
        printf("7. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Add_Medicine();
                break;
            case 2:
                View();
                break;
            case 3:
                Search();
                break;
            case 4:
                Billing();
                break;
            case 5:
                emargancyContact_menu();
                break;
            case 6:
                return;
            case 7:
                save_data();
                exit(0);    
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }   
}

// Function to handle pathologist operations
void pathologist() {
    char storedUsername[30] = "mashrafi";
    int storedPassword = 1773;

    char username[30];
    int pass;
    printf("Enter Username : ");
    scanf("%29s", username);

    printf("Enter Password : ");
    scanf("%d", &pass);

    if(strcmp(storedUsername, username) != 0 || storedPassword != pass) {
        printf("Login Failed! Invalid Username or Password.\n");
        return;
    }
    printf("Login Successful! Welcome, %s.\n", username);
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("\nPathologist Menu\n\n");
        printf("1. ADD Test\n");
        printf("2. Search Test\n");
        printf("3. Patient test procedure with billing\n");
        printf("4. Emergency Contact\n");
        printf("5. Back\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_Test();
                break;
            case 2:
                search_Test();
                break;
            case 3:
                test_billing();
                break;
            case 4:
                emargancyContact_menu();
                break;
            case 5:
                return;
            case 6:
                save_data();
                exit(0);    
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to handle admin operations
void admin() {
    char storedUsername[30] = "mashrafi";
    int storedPassword = 1773;

    char username[30];
    int pass;
    printf("Enter Username : ");
    scanf("%29s", username);

    printf("Enter Password : ");
    scanf("%d", &pass);

    if(strcmp(storedUsername, username) != 0 || storedPassword != pass) {
        printf("Login Failed! Invalid Username or Password.\n");
        return;
    }
    printf("Login Successful! Welcome, %s.\n", username);
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("1. Patient Management\n");
        printf("2. Appointment\n");
        printf("3. Clinical Management\n");
        printf("4. Pharmacy\n");
        printf("5. Pathology\n");
        printf("6. Blood Bank\n");
        printf("7. Emergency Contact\n");
        printf("8. Employee Management\n");
        printf("9. Back\n");
        printf("10. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                patientManagement_menu();
                break;
            case 2:
                appointmentManagement_menu();
                break;
            case 3:
                clinicalManagement_menu();
                break;
            case 4:
                pharmacyManagement_menu();
                break;
            case 5:
                pathologyManagement_menu();
                break;
            case 6:
                bloodBankManagement_menu();
                break;
            case 7:
                emargancyContact_menu();
                break;
            case 8:
                employeManagement_menu();
                break;
            case 9:
                return;
            case 10:
                save_data();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to handle login menu
void login_menu() {
    load_data(); // Load data at startup
    int choice;

    while (1) {
        printf("\nNorthern University Healthcare System\n");
        printf("\nChoose your Post : \n\n");
        printf("1. Doctor\n");
        printf("2. Nurse\n");
        printf("3. Pharmacist\n");
        printf("4. Pathologist\n");
        printf("5. Admin\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                doctor();
                break;
            case 2:
                nurse();
                break;
            case 3:
                pharmacist();
                break;
            case 4:
                pathologist();
                break;
            case 5:
                admin();
                break;
            case 6:
                save_data();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Main function
int main() {
    login_menu();
    printf("Author Kabbo");
    return 0;
}