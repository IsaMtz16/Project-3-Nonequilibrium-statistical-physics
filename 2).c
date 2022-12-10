#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// ALGORITMO P-R
extern float Random(void);
#define NormRANu (2.3283063671E-10)
extern void ini_ran(int SEMILLA);
unsigned int irr[256];
unsigned int ir1;
unsigned char ind_ran, ig1, ig2, ig3;

#define L 10
#define T 0
#define Nt 1000

// GENERACION DE NUMEROS
float Random(void)	// generar numero aleatorio
{
    float r;
    ig1=ind_ran-24;
    ig2=ind_ran-55;
    ig3=ind_ran-61;
    irr[ind_ran]=irr[ig1]+irr[ig2];
    ir1=(irr[ind_ran]^irr[ig3]);
    ind_ran++;
    r=ir1*NormRANu;
    return r;
}

void ini_ran(int SEMILLA)
{
    int INI,FACTOR,SUM,i;
    srand(SEMILLA);
    INI=SEMILLA;
    FACTOR=67397;
    SUM=7364893;
    for(i=0; i<256; i++)
    {
        INI=(INI*FACTOR+SUM);
        irr[i]=INI;
    }
    ind_ran=ig1=ig2=ig3=0;
}

void configrandom(char *config,int V){

    int i,k;
    float x;

    for(i=0;i<V;i++){
        x=Random();
        if(x<0.5){
            config[i]=-1;
        }
        else{
            config[i]=1;
        }
        //printf("%d\n",config[i]);
    }
}

void fichero(char *config){
    //FICHERO PARA ESCRIBIR D EN FUNCION DE DELTA
    FILE*f;
    f=fopen("Conf_final.txt","w");

    int i, j,n;
    n=0;
    for(i=0;i<L;i++){
        for(j=0;j<L;j++){
            fprintf(f,"%d   %d  %d\n",i,j,config[n]);
            n++;
        }
    }
    fclose(f);

}

//CALCULAMOS LOS VALORES DE X E Y PARA CUANDO NOS MOVEMOS EN LA CONFIGURACION
void moverse(int *xp,int *yp, int *xm, int *ym){
    int y, x;
    for(y=0;y<L;y++){

        yp[y]=L;

        ym[y]=-L;

    }
    yp[L-1]=-L*(L-1);
    ym[0]=L*(L-1);

    for(x=0;x<L;x++){

        xp[x]=1;
                    //printf("%i",xp[x]);
        xm[x]=-1;

    }
    xp[L-1]=(-(L-1));
    xm[0]=L-1;

}

void probabilidad(double *prob){
    int i;
    double j;
    for(i=0;i<5;i++){
        j=(4*(i-2));
        printf("%lf\n",j);
        prob[i]=exp((-j)/0.001);
        if(prob[i]>1){
            prob[i]=1;
        }
        printf("%lf\n",prob[i]);
    }

}

void magnetizacion (char config[],double *m){
    int i;
    int V;
    V=L*L;

    double valor=0;
    for (i=0;i<V;i++){
        valor=valor+config[i];
    }
    *m=valor/(double)V;
}

void Metropolis(char *config,double prob[],int xp[],int yp[],int xm[],int ym[]){
    int ind,n,j,y,x,t;
    double w;
    double m;

    FILE*f1;
    f1=fopen("Magnetizacion(t).txt","w");

    for(t=0;t<Nt;t++){
        n=0;

        for(y=0;y<L;y++){
            for(x=0;x<L;x++){

                ind=config[n]*(config[n+xp[x]]+config[n+yp[y]]+config[n+xm[x]]+config[n+ym[y]])/2+2;
                    //printf("%i ",ind);
                    //printf("%i ",n);
                w=Random();
                if(w<prob[ind]){
                    //printf("%i \n",n);
                    config[n]=-config[n];
                }
                n++;

            }
        }
        magnetizacion(config,&m);
        fprintf(f1,"%d     %lf\n",t,m);

    }
    fclose(f1);
}


int main (){
    ini_ran(time(NULL));
    int V;
    V=L*L;
    char *config;
    config=(char*)calloc(V,sizeof(char));


    //Calculamos la configuracion inicial
    configrandom(config,V);
    //fichero(config);
    //Declaramos unos arrays que nos indican el ínidice que hay que añadir para llegar al spin vecino
    int xp[L]={0};int yp[L]={0};int xm[L]={0};int ym[L]={0};


    //Calculamos la probabilidad de transición
    double prob[5]={0};
    probabilidad(prob);

    moverse(xp,yp,xm,ym);
    Metropolis(config,prob,xp,yp,xm,ym);
    fichero(config);

    free (config);

return 0;
}
