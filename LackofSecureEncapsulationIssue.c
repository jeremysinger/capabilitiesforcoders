#include<stdio.h>
#include<stdlib.h>

struct Student {
    char name[100];
    int age;
};

struct Student* create_student(char name*, int age) {
    struct Student* student = (struct Student*)malloc(sizeof(struct Student));
    if(student){
        strcpy(student->name, name);
        student->age = age; 
    }
    return student; 
}

int main() {
    struct Student* student1 = create_student("Alice", 20);
    struct Student* student2 = create_student("Bob", 22);

    // unintentional access to students data (no encapsulation)
    printf("student1: %s,%d\n", student2->name, student2->age);

    free(student1);
    free(student2);

    return 0;
}








