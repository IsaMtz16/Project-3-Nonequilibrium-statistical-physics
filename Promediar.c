#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int i;
    FILE*f;
    char filename[40];

    int t[5000]={0};
    double m[5000]={0};

    int auxt;
    double auxm;

    int n;

    int mint=10000;

    for(i=0;i<10;i++){
        n=0;
        sprintf(filename, "M(t)%d_c_final.txt", i);
        f=fopen(filename,"r");
        while(!feof(f)){
            fscanf(f,"%d   %lf ",&auxt,&auxm);

            //printf("%lf\n",auxm);
            if(auxm<0){
                auxm=-auxm;
            }
            t[n]=auxt;
            m[n]=m[n]+auxm;
            n++;

        }
        //printf("%d\n",t[n-1]);
        if(auxt<mint){
            mint=auxt;
            printf("%d\n",mint);
        }


    }
    FILE *f1;
    f1=fopen("Promedio_c.txt","w");
    int j;

    for(j=0;j<mint;j++){
        //printf("%d  %lf\n",j,m[j]/(double)10);
        fprintf(f1,"%d  %lf\n",j,m[j]/(double)10);
    }

    fclose(f1);

    return 0;


}
