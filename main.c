#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

typedef struct student{
    char* lname; // need to DMA;
    int assignment;
    int finalExam;
    int total; // sum of scores from M quizzes + score in assignment + final exam
    int* quizzes; // need to DMA
}student;

student** readCourses(int* C, int* N, int* M);

void printHighestTotal(student** courseList, int C, int N, int M);

void release_memroy(student** st, int C, int N, int M);

int main(){
    atexit(report_mem_leak); //memory leak detector
    student** courseList;
    int i, j, C, N, M;

    courseList = readCourses(&C, &N, &M);

    printHighestTotal(courseList, C, N, M);

    release_memroy(courseList, C, N, M);

    return 0;
}

//Read in user input
student** readCourses(int* C, int* N, int* M){
    scanf("%d %d %d", C, N, M);
    student** courses = (student**)malloc(*C * sizeof(student*));  //DMA for C courses

    for (int i = 0; i<*C; i++){ //loop through each course + allocate space for N students
        courses[i] = (student*)malloc(*N * sizeof(student));
    }

    for (int i = 0; i<*C; i++){ //loop through each course
        for(int j = 0; j<*N; j++){ //loop through student's in course
            courses[i][j].quizzes = (int*)malloc(*M * sizeof(int)); //allocate M quizzes
            courses[i][j].lname = (char*)malloc( 51 * sizeof(char)); //allocate last name
        }
    }

    for (int i = 0; i<*C; i++){
        for(int j = 0; j<*N; j++){
            int quizScores = 0;
            scanf("%s", courses[i][j].lname); //collecting last name
            scanf("%d", &courses[i][j].assignment); //collecting exam score
            for(int k = 0; k<*M; k++){
                scanf("%d", &courses[i][j].quizzes[k]); //collecting quiz scores
                quizScores = quizScores + courses[i][j].quizzes[k]; //assigning to variable for total
            }
            scanf("%d", &courses[i][j].finalExam); //collecting final exam score
            courses[i][j].total = courses[i][j].assignment + quizScores + courses[i][j].finalExam;//calculating total
        }
    }
    return courses;
}

//prints the highest total of all students
void printHighestTotal(student** courseList, int C, int N, int M){
    student* highScoreStudent = NULL;
    int highScore = 0, courseNum = 0;
    for(int i = 0; i<C; i++){
        for (int j = 0; j<N; j++){
            if(courseList[i][j].total > highScore){
                highScore = courseList[i][j].total;
                highScoreStudent = &courseList[i][j];
                courseNum = i;
            }
        }
    }
    if(highScoreStudent != NULL){
        printf("Name: %s\n", highScoreStudent->lname);
        printf("Assignment: %d\n", highScoreStudent->assignment);
        printf("Quizzes: ");
        for(int k = 0; k<M; k++){
            printf("%d ", highScoreStudent->quizzes[k]);
        }
        printf("\nFinal exam: %d", highScoreStudent->finalExam);
        printf("\nTotal: %d", highScoreStudent->total);
        printf("\nCourse number: %d", courseNum + 1);
    }
}

//Free Memory
void release_memroy(student** courseList, int C, int N, int M){
    for(int i = 0; i< C; i++){
        for(int j= 0; j< N; j++){
            free(courseList[i][j].quizzes);
            free(courseList[i][j].lname);
        }
        free(courseList[i]);
    }
    free(courseList);
}