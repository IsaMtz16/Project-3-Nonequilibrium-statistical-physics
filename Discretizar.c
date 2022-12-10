#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int i;
    FILE*f;
    char filename[40];

    for (i=0;i<10;i++){
        int t[1000]={0};
        double m[1000]={0};
        int n=0;
        sprintf(filename, "M(t)%d_c.txt", i);
        f=fopen(filename,"r");
        while(!feof(f)){
            fscanf(f,"%d   %lf ",&t[n],&m[n]);
            n++;
            //printf("%d\n",t[n-1]);
        }
        t[n]=0;
        fclose(f);
        //printf("%d\n",t[n]);
        int tamano;
        tamano=t[n-1];
        int j;
        double *m_final; int *t_final;
        m_final=(double*)calloc(tamano,sizeof(double));
        t_final=(int*)calloc(tamano,sizeof(int));

        int aux;
        aux=t[0];
        int k=1;
        int l;
        int diferencia;
        t_final[0]=t[0];
        m_final[0]=m[0];
        for(j=1;t[j]!=0;j++){
            diferencia=t[j]-t[j-1];
            if(diferencia>1){
                for(l=0;l<(diferencia-1);l++){
                   t_final[k]=t_final[k-1]+1;
                    m_final[k]=m_final[k-1];
                    k++;
                }

            }
            t_final[k]=t[j];
            m_final[k]=m[j];
            k++;
        }
        FILE*f1;
        char filename1[40];
        sprintf(filename1, "M(t)%d_c_final.txt", i);
        f1=fopen(filename1,"w");

        for(j=0;j<tamano;j++){
            fprintf(f1,"%d  %lf\n",t_final[j],m_final[j]);

        }
        fclose(f1);
        free(m_final);free(t_final);

    }

    return 0;
}
