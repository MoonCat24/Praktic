#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char LICENSE_KEY[] = "01AFC567";

// Структура для збереження контакту
typedef struct Contact {
    char name[50];
    char birthdate[50];
    char phone[20];
    int hasTelegram;
    struct Contact* next;
} Contact;

// Головний зв'язок для списку контактів
Contact* contacts = NULL;

// Зчитування налаштувань з конфігураційного файлу
void readConfig() {
    FILE* file = fopen("config.txt", "r");
    if (file == NULL) {
        printf("Помилка читання конфігураційного файлу.\n");
        return;
    }

    // Зчитування налаштувань
    fclose(file);
}

// Зчитування даних з файлу
void loadData() {
    FILE* file = fopen("data.bin", "rb");
    if (file == NULL) {
        printf("Помилка читання файлу з даними.\n");
        return;
    }

    Contact* current = NULL;
    Contact* previous = NULL;

    while (1) {
        Contact* contact = (Contact*)malloc(sizeof(Contact));
        size_t elementsRead = fread(contact, sizeof(Contact), 1, file);
        if (elementsRead != 1) {
            free(contact);
            break;
        }

        contact->next = NULL;

        if (previous != NULL) {
            previous->next = contact;
        } else {
            contacts = contact;
        }

        previous = contact;
    }

    fclose(file);
}

// Збереження даних в файл
void saveData() {
    FILE* file = fopen("data.bin", "wb");
    if (file == NULL) {
        printf("Помилка запису до файлу з даними.\n");
        return;
    }

    Contact* current = contacts;

    while (current != NULL) {
        fwrite(current, sizeof(Contact), 1, file);
        current = current->next;
    }

    fclose(file);
}

// Додавання нового контакту
void addContact() {
    Contact* contact = (Contact*)malloc(sizeof(Contact));

    printf("Прізвище та ім'я: ");
    scanf("%s", contact->name);

    printf("Дата народження (ДД.ММ.РРРР): ");
    scanf("%s", contact->birthdate);

    printf("Номер телефону: ");
    scanf("%s", contact->phone);

    printf("Telegram (так - 1, ні - 0): ");
    scanf("%d", &contact->hasTelegram);

    contact->next = NULL;

    if (contacts == NULL) {
        contacts = contact;
    } else {
        Contact* current = contacts;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = contact;
    }

    printf("Контакт успішно доданий.\n");
}

// Видалення контакту за ідентифікаційним номером
void deleteContact(int id) {
    Contact* current = contacts;
    Contact* previous = NULL;

    while (current != NULL) {
        if (id == 1) {
            contacts = current->next;
        } else if (id > 1) {
            for (int i = 1; i < id; i++) {
                previous = current;
                current = current->next;
            }
            previous->next = current->next;
        }

        free(current);
        printf("Контакт успішно видалений.\n");
        return;
    }

    printf("Контакт з ідентифікаційним номером %d не знайдений.\n", id);
}

// Виведення усіх контактів
void displayContacts() {
    if (contacts == NULL) {
        printf("База даних порожня.\n");
        return;
    }

    Contact* current = contacts;
    int id = 1;

    while (current != NULL) {
        printf("ID: %d\n", id);
        printf("Прізвище та ім'я: %s\n", current->name);
        printf("Дата народження: %s\n", current->birthdate);
        printf("Номер телефону: %s\n", current->phone);
        printf("Telegram: %s\n", current->hasTelegram ? "так" : "ні");
        printf("\n");

        current = current->next;
        id++;
    }
}

// Зміна полів контакту за ідентифікаційним номером
void editContact(int id) {
    Contact* current = contacts;
    int currentId = 1;

    while (current != NULL) {
        if (currentId == id) {
            printf("Прізвище та ім'я: ");
            scanf("%s", current->name);

            printf("Дата народження (ДДММРР): ");
            scanf("%s", current->birthdate);

            printf("Номер телефону: ");
            scanf("%s", current->phone);

            printf("Telegram (так - 1, ні - 0): ");
            scanf("%d", &current->hasTelegram);

            printf("Контакт успішно змінений.\n");
            return;
        }

        current = current->next;
        currentId++;
    }

    printf("Контакт з ідентифікаційним номером %d не знайдений.\n", id);
}
// Виведення контактів, у яких немає Telegram
void displayContactsWithoutTelegram() {
    if (contacts == NULL) {
        printf("База даних порожня.\n");
        return;
    }

    Contact* current = contacts;

    while (current != NULL) {
        if (current->hasTelegram == 0) {
            printf("Прізвище та ім'я: %s\n", current->name);
            printf("Дата народження: %s\n", current->birthdate);
            printf("Номер телефону: %s\n", current->phone);
            printf("Telegram: %s\n", current->hasTelegram ? "так" : "ні");
            printf("\n");
        }

        current = current->next;
    }
}

// Головне меню
void displayMenu() {
    printf("1. Додати запис\n");
    printf("2. Видалити запис\n");
    printf("3. Переглянути записи\n");
    printf("4. Змінити запис\n");
    printf("5. Вивести контакти без Telegram\n");
    printf("0. Вийти\n");
}

int checkLicenseKey(const char* key) {
    return strcmp(key, LICENSE_KEY) == 0;
}

int main() {
    
    // Перевірка ліцензійного ключа
    char enteredKey[100];
    printf("Введіть ліцензійний ключ: ");
    scanf("%s", enteredKey);

    if (!checkLicenseKey(enteredKey)) {
        printf("Неправильний ліцензійний ключ. Програма завершується.\n");
        return 1;
    }
    
    readConfig();
    loadData();

    int choice;

    do {
        displayMenu();
        printf("Виберіть опцію: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2: {
                int id;
                printf("Введіть ідентифікаційний номер запису: ");
                scanf("%d", &id);
                deleteContact(id);
                break;
            }
            case 3:
                displayContacts();
                break;
            case 4: {
                int id;
                printf("Введіть ідентифікаційний номер запису: ");
                scanf("%d", &id);
                editContact(id);
                break;
            }
            case 5:
                displayContactsWithoutTelegram();
                break;
            case 0:
                printf("Дякую за використання програми!\n");
                break;
            default:
                printf("Невірний вибір. Спробуйте ще раз.\n");
                break;
        }
    } while (choice != 0);

    saveData();

    return 0;
}
