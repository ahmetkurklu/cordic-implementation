#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX 64
#define PI 3.14159265359

double tabTeta[MAX];
double tabA[MAX];

double RtoD(double radian){
    return radian*(180/PI);
}
double DtoR(double degre){
    return degre*(PI/180);
}

void initTabTeta(int n){
    for (int i = 0; i < n; i++)
    {
        tabTeta[i]=RtoD(atan(pow(2,-i)));
    }
}

void initTabA(int n){
    tabA[0] = cos(DtoR(tabTeta[0]));
    for(int i =1;i<n;i++){
        tabA[i]=tabA[i-1]* cos(DtoR(tabTeta[i]));
    }
}

void initialisationTab(){
    initTabTeta(MAX);
    initTabA(MAX);
}

void afficherTab(double *tab,int n){
    int i;
    for(i=0;i<n;i++){
        printf("%f\n",tab[i]);
    }
}


void cordic(double phi, int n,double *x,double*y){
    int i;
    double alpha,teta,n_x,n_y,tmp_x,tmp_y,d;

    n_x= tabA[MAX-1];
    n_y = 0;
    d=1;
    alpha=0;
    tmp_x=0;
    tmp_y=0;
    teta=tabTeta[0];

    for(i=0;i<n;i++){
        if(alpha < phi){
            tmp_x = n_x - n_y * d;
            tmp_y = n_y + n_x * d;

            alpha = alpha + teta;
        }
        else{
            tmp_x = n_x +n_y * d;
            tmp_y = n_y -n_x * d;
            alpha = alpha - teta;
        }
    n_x =tmp_x;
    n_y = tmp_y;
    d=d/2;
    teta = tabTeta[i+1];
    }

    *x = n_x;
    *y = n_y;
}



void test(){
    printf("-----------------------------------------\n");
    printf("------------PROGRAM DE TEST--------------\n");
    printf("-----------------------------------------\n");


    double angle;
    double n,x,y;
    printf("Entree l'angle :");
    scanf("%lf",&angle);

    printf("Entree le nombre d'iteration:");
    scanf("%lf",&n);

    cordic(angle,n,&x,&y);

    printf("x=%f,y=%f \n\n",x,y);

    printf("-----------------------------------------\n");
    printf("------------FIN DU TEST------------------\n");
    printf("-----------------------------------------\n");

}





void quali(int n){
    printf("-----------------------------------------\n");
    printf("------PROGRAM DE TEST QUALI--------------\n");
    printf("-----------------------------------------\n");


    double x;
    double y;
    double angle =0;
    double erreurCos=0;
    double erreurSin=0;

    FILE*f1;
    FILE*f2;
    f1 = fopen("cosinus.txt","w+");
    f2 = fopen("sinus.txt","w+");

    for(angle=0;angle<91;angle++){
        cordic(angle,n,&x,&y);

        //printf("X = %f Y = %f\n",x,y);
        //printf("cosX = %f cosY = %f\n",cos(DtoR(angle)),sin(DtoR(angle)));
        erreurCos = fabs(cos(DtoR(angle))-x);
        erreurSin = fabs(sin(DtoR(angle))-y);

        //printf("errCos = %f errSin = %f\n",erreurCos,erreurSin);

        fprintf(f1,"%.40f\n",erreurCos);
        fprintf(f2,"%.40f\n",erreurSin);

    }
    fclose(f1);
    fclose(f2);


    printf("-----------------------------------------\n");
    printf("------------FIN DU TEST------------------\n");
    printf("-----------------------------------------\n");

}



int main()
{
    //Remplissage des tab const
    initialisationTab();

    //afficherTab(tabTeta,MAX);
    //afficherTab(tabA,MAX);


    test();

    //quali(64);

    return 0;
}
