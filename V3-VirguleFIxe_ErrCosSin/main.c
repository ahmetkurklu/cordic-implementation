#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX 64
#define PI 3.14159265359
#define M_ANGLE 270
#define M_CS 32000


double tabTeta[MAX];
double tabA[MAX];

unsigned short tabTeta_fixe[MAX];
unsigned short tabA_fixe[MAX];





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
void afficherTab_fixe(unsigned short *tab,int n){
    int i;
    for(i=0;i<n;i++){
        printf("%hu\n",tab[i]);
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



void quali(int n,double *MaxC,double *MaxS){
    printf("-----------------------------------------\n");
    printf("------PROGRAM DE TEST QUALI--------------\n");
    printf("-----------------------------------------\n");


    double x;
    double y;
    double angle =0;
    double erreurCos=0;
    double erreurSin=0;

    double MaxErreurCos=0;
    double MaxErreurSin=0;


    for(angle=0;angle<91;angle++){
        cordic(angle,n,&x,&y);

        erreurCos = fabs(cos(DtoR(angle))-x);
        erreurSin = fabs(sin(DtoR(angle))-y);

        if(erreurCos>MaxErreurCos){
            MaxErreurCos = erreurCos;
        }
        if(erreurSin>MaxErreurSin){
            MaxErreurSin = erreurSin;
        }
    }

    *MaxC=MaxErreurCos;
    *MaxS=MaxErreurSin;


    printf("-----------------------------------------\n");
    printf("------------FIN DU TEST------------------\n");
    printf("-----------------------------------------\n");

}

void erreurMax(){

    FILE * f1;
    FILE * f2;
    f1 = fopen("ErrMaxCos.txt","w+");
    f2 = fopen("ErrMaxSin.txt","w+");

    int i;
    double MaxC,MaxS;
    for(i=0;i<64;i++){
        quali(i,&MaxC,&MaxS);
        fprintf(f1,"%.40f\n",MaxC);
        fprintf(f2,"%.40f\n",MaxS);
    }
    fclose(f1);
    fclose(f2);
}






//-----------------------------------------------------
//-----------------------VIRGULE FIXE------------------
//-----------------------------------------------------



short int fp2fix(double M,double x){
    return floor(M*x);
}

double fix2fp(double M, short int x){
    return x/M;
}

void initTabTeta_fixe(int n){
    for (int i = 0; i < n; i++)
    {
        tabTeta_fixe[i] = fp2fix(M_ANGLE , tabTeta[i]);
    }
}

void initTabA_fixe(int n){
    for(int i =0;i<n;i++){
        tabA_fixe[i] = fp2fix(M_CS , tabA[i]);
    }
}

void initialisationTab_fixe(){
    initialisationTab();
    initTabTeta_fixe(MAX);//Ajout des fonciton pour fixe
    initTabA_fixe(MAX);

}

void cordic2(short int phi, int n,short int *x,short int*y){
    int i;
    short int alpha,teta,n_x,n_y,tmp_x,tmp_y;

    n_x= tabA_fixe[MAX-1];
    n_y = 0;
    alpha=0;
    tmp_x=0;
    tmp_y=0;
    teta=tabTeta_fixe[0];

    for(i=0;i<n;i++){
        if(alpha < phi){
            tmp_x = n_x - (n_y>>i);
            tmp_y = n_y + (n_x>>i);

            alpha = alpha + teta;
        }
        else{
            tmp_x = n_x + (n_y>>i);
            tmp_y = n_y -(n_x>>i);
            alpha = alpha - teta;
        }
        n_x =tmp_x;
        n_y = tmp_y;
        teta = tabTeta_fixe[i+1];

        printf("Fixe Pour n =%d | x = %hu y = %hu\n",i,n_x,n_y);
        printf("NONF Pour n =%d | x = %f y = %f\n",i,fix2fp(M_CS,n_x),fix2fp(M_CS,n_y));

    }

    *x = n_x;
    *y = n_y;
}

void test_fixe(){
    printf("-----------------------------------------\n");
    printf("------------PROGRAM DE TEST--------------\n");
    printf("-----------------------------------------\n");


    double angle;
    double n;
    short int x,y;
    printf("Entree l'angle :");
    scanf("%lf",&angle);

    printf("Entree le nombre d'iteration:");
    scanf("%lf",&n);

    short int angle_fixe = fp2fix(M_ANGLE, angle);

    cordic2(angle_fixe,n,&x,&y);

    //printf("x=%f,y=%f \n\n",fix2fp(M_CS,x),fix2fp(M_CS,y));

    printf("-----------------------------------------\n");
    printf("------------FIN DU TEST------------------\n");
    printf("-----------------------------------------\n");

}


void quali2(int n){
    printf("-----------------------------------------\n");
    printf("------PROGRAM DE TEST QUALI--------------\n");
    printf("-----------------------------------------\n");


    short int x;
    short int y;
    double angle =0;
    double erreurCos=0;
    double erreurSin=0;

    FILE*f1;
    FILE*f2;
    f1 = fopen("cosinus.txt","w+");
    f2 = fopen("sinus.txt","w+");

    for(angle=0;angle<91;angle++){
        cordic2(fp2fix(M_ANGLE,angle),n,&x,&y);

        //printf("X = %f Y = %f\n",x,y);
        //printf("cosX = %f cosY = %f\n",cos(DtoR(angle)),sin(DtoR(angle)));
        erreurCos = fabs(cos(DtoR(angle))-fix2fp(M_CS,x));
        erreurSin = fabs(sin(DtoR(angle))-fix2fp(M_CS,y));

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
    //initialisationTab();
    initialisationTab_fixe();

    //afficherTab(tabTeta,MAX);
    //afficherTab(tabA,MAX);
    //afficherTab_fixe(tabTeta_fixe,MAX);
    //printf("-----------------\n");
    //afficherTab_fixe(tabA_fixe,MAX);


    test_fixe();
    //erreurMax();

    //quali2(16);

    return 0;
}
