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
    double V=L*L;
    for(i=0;i<5;i++){
        j=(4*(i-2));
        //printf("%lf\n",j);
        prob[i]=exp((-j)/0.01);
        if(prob[i]>1){
            prob[i]=1;
        }
        //prob[i]=prob[i]/V;
        //printf("%lf\n",prob[i]);
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

void inicializacion_listas(int *lista1,int *lista2,int *lista3, int *lista4,int *lista5,char config[],int *indices,int xp[],int yp[],int xm[],int ym[]){
    int y,x,n,ind,i,j;
    int V=L*L;
    n=0;
    for(j=0;j<5;j++){
        indices[j]=0;
    }
    //primero inicializamos todas las a -1
    //porque no hay ninguna posicion que sea -1
    for(i=0;i<V;i++){
        lista1[i]=-1;
        lista2[i]=-1;
        lista3[i]=-1;
        lista4[i]=-1;
        lista5[i]=-1;

    }
    n=0;
    for(y=0;y<L;y++){
        for(x=0;x<L;x++){
            ind=config[n]*(config[n+xp[x]]+config[n+yp[y]]+config[n+xm[x]]+config[n+ym[y]])/2+2;
            switch (ind){
            case 0:
                indices[0]++;
                lista1[indices[0]]=n;
                //printf("%d\n",lista1[indices[0]]);
                //printf("%d\n",indices[0]);

                break;
            case 1:
                indices[1]++;
                lista2[indices[1]]=n;

                break;
            case 2:
                indices[2]++;
                lista3[indices[2]]=n;

                break;

            case 3:
                indices[3]++;
                lista4[indices[3]]=n;
                //printf("%d\n",config[n]);

                break;

            case 4:
                indices[4]++;
                lista5[indices[4]]=n;

                break;
            }
            //printf("\n");
            n++;

        }
    }


}

void Metropolis(char *config,double prob[],int xp[],int yp[],int xm[],int ym[]){
    int ind,n,y,x,t,i,j,k;
    double w,epsilon;
    double sum_prob;
    double m;
    double V; V=L*L;
    int deltat;
    int tiempo=0;
    int l;

    //Definimos 5 arrays en función del tipo de spin por energia
    //Guardamos en que array los índices de los spines de ese tipo
    int *lista1;//delta E =-8
    int *lista2;//delta E =-4
    int *lista3;//delta E =0
    int *lista4;//delta E =4
    int *lista0;//delta E =8

    lista0=(int*)calloc(V,sizeof(int));
    lista1=(int*)calloc(V,sizeof(int));
    lista2=(int*)calloc(V,sizeof(int));
    lista3=(int*)calloc(V,sizeof(int));
    lista4=(int*)calloc(V,sizeof(int));

    int indices[5];

    double P1, P2, P3, P4, P0,P;

    FILE*f1;
    f1=fopen("Magnetizacion(t).txt","w");

    for(t=0;t<Nt;t++){
        n=0;
        inicializacion_listas(lista0,lista1,lista2, lista3,lista4,config,indices,xp,yp,xm,ym);
        sum_prob=0;
        for (i=0;i<5;i++){
            sum_prob=sum_prob+prob[i]*(double)indices[i];
            //printf("%d\n",indices[i]);
        }
        //printf("\n");
        //printf("%lf\n",sum_prob);
        sum_prob=sum_prob/V;
        epsilon=Random();
        deltat=1+(int)(log(epsilon)/(log(1-sum_prob)));
        //printf("%d\n",tiempo);
        if(sum_prob==0){
            break;
        }

        //printf("%lf     %lf\n",(log(epsilon)/(log(1-sum_prob))),sum_prob);
        tiempo=tiempo+deltat;

        w=Random();
        P0=prob[0]*indices[0];
        P1=prob[1]*indices[1];
        P2=prob[2]*indices[2];
        P3=prob[3]*indices[3];
        P4=prob[4]*indices[4];
        P=P0+P1+P2+P3+P4;
        //printf("%lf\n%lf\n%lf\n%lf\n%lf\n\n",prob[0]*indices[0],prob[1]*indices[1],prob[2]*indices[2],prob[3]*indices[3],prob[4]*indices[4]);
        //l=5;
        if(w<=(P0/P)){
            l=0;
            //printf("%d  %d\n",t,l);
        }
        else if((P0/P)<w<=((P0+P1)/P)){
            l=1;
            //printf("%d  %d\n",t,l);
        }
        else if(((P0+P1)/P)<w<=((P0+P1+P2)/P)){
            l=2;
            //printf("%d  %d\n",t,l);
        }
        else if(((P0+P1+P2)/P)<w<=((P0+P1+P2+P3)/P)){
            l=3;
            //printf("%d  %d\n",t,l);
        }
        else if(((P0+P1+P2+P3)/P)<w<=((P0+P1+P2+P3+P4)/P)){
            l=4;
            //printf("%d  %d\n",t,l);
        }
        //printf("%d\n",l);
        k=1+(int)(Random()*(indices[l]-1));
        //printf("%d\n",k);

        int spin;

        if(l==0){
            spin=lista0[k];
        }
        else if(l==1){
            spin=lista1[k];
        }
        else if(l==2){
            spin=lista2[k];
        }
        else if(l==3){
            spin=lista3[k];
        }
        else if(l==4){
            spin=lista4[k];
        }
        config[spin]=-config[spin];

        magnetizacion(config,&m);
        fprintf(f1,"%d     %lf\n",tiempo,m);

    }
    fclose(f1);
    free(lista1); free(lista2); free(lista3); free(lista4); free(lista0);
}

int main (){
    ini_ran(time(NULL));
    int V;
    V=L*L;
    char *config;
    config=(char*)calloc(V,sizeof(char));

    //Calculamos la configuracion inicial
    configrandom(config,V);
    //Declaramos unos arrays que nos indican el ínidice que hay que añadir para llegar al spin vecino
    int xp[L]={0};int yp[L]={0};int xm[L]={0};int ym[L]={0};
    //printf("%d",ym[5]);
    //Calculamos la probabilidad de transición
    double prob[5]={0};
    probabilidad(prob);

    moverse(xp,yp,xm,ym);
    Metropolis(config,prob,xp,yp,xm,ym);
    fichero(config);
    free (config);

return 0;
}
