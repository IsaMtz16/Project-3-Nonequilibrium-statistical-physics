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

#define L 50

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
        printf("%d\n",config[i]);
    }
}

void fichero(char *config){
    //FICHERO PARA ESCRIBIR D EN FUNCION DE DELTA
    FILE*f;
    f=fopen("Conf_random.txt","w");

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

int main (){
    ini_ran(time(NULL));
    int V;
    V=L*L;
    char *config;
    config=(char*)calloc(V,sizeof(char));


    //Calculamos la configuracion inicial
    configrandom(config,V);
    fichero(config);

    free (config);

return 0;
}
