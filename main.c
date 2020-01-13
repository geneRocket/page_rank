#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE_SIZE 400000
#define MAX_IN_SIZE 500
#define MAX_LINE_LEN 400

int col_indices[MAX_NODE_SIZE][MAX_IN_SIZE];//from_nodes
int in_count[MAX_NODE_SIZE];
double out_count_prob[MAX_NODE_SIZE];

double important_a[MAX_NODE_SIZE];
double important_b[MAX_NODE_SIZE];

double *important_1=important_a;
double *important_2=important_b;

double alpha=0.15;

char str[MAX_NODE_SIZE][MAX_LINE_LEN];

int max(int a,int b){
    if(a>=b)
        return a;
    else
        return b;
}

int min(int a,int b){
    if(a<=b)
        return a;
    else
        return b;
}

double fabs(double x){
    if(x>=0)
        return x;
    else
        return -x;
}

int cmp_vector(double *v1,double *v2,int max_node_size){
    double sum=0;
    for(int i=0;i<max_node_size;i++){
        sum+=fabs(v1[i]-v2[i]);
    }
    double eps=0.0001*max_node_size;
    if(sum<=eps){
        return 1;
    }
    else
        return 0;
}

typedef struct KeyValue{
    int key;
    double value;
}KeyValue;

int cmpfunc (const void * a, const void * b)
{
    return ( (*(KeyValue*)a).value < (*(KeyValue*)b).value );
}

KeyValue keyValues[MAX_NODE_SIZE];

int main() {
    memset(in_count,0, sizeof(in_count));

    FILE* fp=fopen("urlcp.txt","r");
    //FILE* fp=fopen("testcp.txt","r");

    int u_out_count=0;
    int last_u=-1;
    int max_node_size=-1;
    int u,v;
    while (fscanf(fp,"%d - %d",&u,&v)==2){

        max_node_size=max(max_node_size,max(u,v)+1);
        col_indices[v][in_count[v]]=u;
        in_count[v]++;
        if(u!=last_u) {
            if(last_u!=-1){
                out_count_prob[last_u] = (1.0/u_out_count);
            }
            u_out_count = 0;
            last_u = u;
        }
        u_out_count++;
    }
    if(last_u!=-1){
        out_count_prob[last_u] = (1.0/u_out_count);
    }
    fclose(fp);


    for(int i=0;i<max_node_size;i++){
        important_1[i]=1;
    }
    while (1){
        for(int i=0;i<max_node_size;i++){
            important_2[i]=0;
        }
        for(int v=0;v<max_node_size;v++){



            double sum=0;
            for(int j=0;j<in_count[v];j++){
                int u=col_indices[v][j];
                sum+=(1-alpha)*important_1[u]*out_count_prob[u];

            }
            important_2[v]+=sum;
        }

        double sum=0;
        for(int i=0;i<max_node_size;i++){
            sum+=important_1[i];
        }
        double add=alpha*sum/max_node_size;
        for(int i=0;i<max_node_size;i++){
            important_2[i]+=add;
        }

        //归一化
        sum=0;
        for(int i=0;i<max_node_size;i++){
            sum+=important_2[i];
        }
        for(int i=0;i<max_node_size;i++){
            important_2[i]/=sum;
        }



        double *temp=important_1;
        important_1=important_2;
        important_2=temp;



        if(cmp_vector(important_1,important_2,max_node_size))
            break;
    }

    for (int i = 0; i < max_node_size; i++) {
        keyValues[i].key=i;
        keyValues[i].value=important_1[i];
    }

    qsort(keyValues, max_node_size, sizeof(KeyValue), cmpfunc);




    fp=fopen("urllist.txt","r");
    char line[MAX_LINE_LEN];
    int num;

    while (!feof(fp)){


        fgets(line,MAX_LINE_LEN,fp);


        int i=0;
        while(line[i]!=' '){
            i++;
        }
        line[i]='\0';
        i++;

        num=0;
        for(;line[i]>='0' && line[i]<='9';i++){
            num=num*10+(line[i]-'0');
        }

        strcpy(str[num],line);
    }
    fclose(fp);

    fp=fopen("top10.txt","w");
    for(int i=0;i<10;i++){
        fprintf(fp,"%s %f\n",str[keyValues[i].key],keyValues[i].value);
    }
    printf("\n");
}