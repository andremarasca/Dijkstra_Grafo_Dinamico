//Desvio de rota URI
#include <stdio.h>
#include <stdlib.h>
#define infinito 2147483647

typedef struct aresta
{
    int id;
    int peso;
    struct aresta*proximo;
} aresta;

typedef struct vertices
{
    aresta*primeiro;
    aresta*ultimo;
} Vertices;

typedef struct fila_prioridade
{
    int id,distancia;
} fila_prioridade;

void inicializagrafo(Vertices*g,int n);
void inserearesta(Vertices*v,int pai,int id,int peso);
void inicializa_fila_prioridade(fila_prioridade*f,int tam);
void DIJKSTRA(Vertices *G,int tam,int s,int parent[],int dist[]);
void INSERIRFILAPRIORIDADE(fila_prioridade PQ[],int *fim,int u,int dist);
int RETORNAMENOR(fila_prioridade PQ[],int *fim);
void RELAXAMENTO(fila_prioridade PQ[],int *fim,int u,int dist);
void corrige_descendo (fila_prioridade vet[], int fim, int indice);
void corrige_subindo (fila_prioridade vet[], int indice);
void heap_remove(fila_prioridade vet[], int *fim);

int main (void)
{
    int i, troca, qn, qa, pai, filho, peso, C, K;
    int dist[250], parent[250];
    Vertices grafo[250];
    scanf("%d %d %d %d", &qn, &qa, &C, &K);
    while(qn || qa || C || K)
    {
        inicializagrafo(grafo,qn);
        for(i = 0; i < qa; i++)
        {
            scanf("%d %d %d",&pai,&filho,&peso);
            if(pai < filho)
            {
                troca = pai;
                pai = filho;
                filho = troca;
            }
            if(filho == (pai - 1) || (pai >= C)) inserearesta(grafo, pai, filho, peso);
            if(filho == (pai - 1) || (filho >= C)) inserearesta(grafo, filho, pai, peso);
        }
        DIJKSTRA(grafo, qn, K, parent, dist);
        printf("%d\n", dist[C - 1]);
        scanf("%d %d %d %d", &qn, &qa, &C, &K);
    }
    return 0;
}

void inicializagrafo(Vertices*g,int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        g[i].primeiro = NULL;
        g[i].ultimo = NULL;
    }
}

void inserearesta(Vertices*v, int pai, int id, int peso)
{
    aresta*novo = (aresta*)malloc(sizeof(aresta));
    novo->id = id;
    novo->peso = peso;
    novo->proximo = NULL;
    if(v[pai].primeiro == NULL)
    {
        v[pai].primeiro = novo;
        v[pai].ultimo = novo;
    }
    else
    {
        v[pai].ultimo->proximo = novo;
        v[pai].ultimo = novo;
        v[pai].ultimo->proximo = NULL;
    }
}

void DIJKSTRA(Vertices *G, int tam, int s, int parent[], int dist[])
{
    int v0, i, frj[tam], fim_fila = 0;
    fila_prioridade PQ[tam];
    aresta* a;
    for(i = 0; i < tam; i++) parent[i] = frj[i] = -1, dist[i] = infinito;
    parent[s] = s;
    dist[s] = 0;
    for(a = G[s].primeiro; a != NULL; a = a->proximo)
    {
        dist[a->id] = a->peso;
        INSERIRFILAPRIORIDADE(PQ, &fim_fila, a->id, dist[a->id]);
        frj[a->id] = s;
    }
    while(fim_fila)
    {
        v0 = RETORNAMENOR(PQ, &fim_fila);
        parent[v0] = frj[v0];
        a = G[v0].primeiro;
        for(a = G[v0].primeiro; a != NULL; a = a->proximo)
        {
            if(parent[a->id]!= -1) continue;
            if(frj[a->id] == -1)
            {
                dist[a->id] = dist[v0] + a->peso;
                INSERIRFILAPRIORIDADE(PQ, &fim_fila, a->id, dist[a->id]);
                frj[a->id]=v0;
            }
            else if (dist[a->id] > dist[v0] + a->peso)
            {
                dist[a->id] = dist[v0] + a->peso;
                RELAXAMENTO(PQ, &fim_fila, a->id, dist[a->id]);
                frj[a->id] = v0;
            }
        }
    }
}

void inicializa_fila_prioridade(fila_prioridade*f, int tam)
{
    int i;
    for(i = 0; i < tam; i++)
    {
        f[i].distancia = infinito;
        f[i].id = i;
    }
}

void INSERIRFILAPRIORIDADE(fila_prioridade vet[], int *fim, int u, int dist)
{
    vet[*fim].id = u;
    vet[*fim].distancia = dist;
    corrige_subindo(vet, (*fim)++);
}

int RETORNAMENOR(fila_prioridade PQ[], int *fim)
{
    int menor = PQ[0].id;
    heap_remove(PQ,fim);
    return menor;
}

void RELAXAMENTO(fila_prioridade PQ[],int *fim,int u,int dist)
{
    int i;
    for(i=0;i<(*fim) && PQ[i].id!=u;i++);
    if(PQ[i].distancia < dist)
    {
        PQ[i].distancia = dist;
        corrige_descendo(PQ, *fim, i);
    }
    else
    {
        PQ[i].distancia = dist;
        corrige_subindo(PQ, i);
    }
}

void corrige_descendo (fila_prioridade vet[], int fim, int indice)
{
    int i = indice;
    fila_prioridade tmp;
    while((2*i + 1)< fim)
    {
        if(vet[2*i+1].distancia < vet[i].distancia && (2*i + 2 >= fim || vet[2*i+1].distancia < vet[2*i+2].distancia))
        {
            tmp = vet[2*i+1];
            vet[2*i+1] = vet[i];
            vet[i] = tmp;
            i = 2*i + 1;
        }
        else if (2*i + 2 < fim && vet[2*i+2].distancia <= vet[i].distancia)
        {
            tmp = vet[2*i  + 2];
            vet[2*i + 2] = vet[i];
            vet[i] = tmp;
            i = 2*i + 2;
        }
        else
        {
            break;
        }
    }
}

void corrige_subindo (fila_prioridade vet[], int indice)
{
    fila_prioridade tmp;
    while(indice)
    {
        if(vet[indice].distancia < vet[(indice - 1)/2].distancia)
        {
            tmp = vet[indice];
            vet[indice] = vet[(indice - 1)/2];
            vet[(indice - 1)/2] = tmp;
        }
        indice = (indice - 1)/2;
    }
}

void heap_remove(fila_prioridade vet[], int *fim)
{
    fila_prioridade tmp;
    (*fim)--;
    tmp = vet[0];
    vet[0] = vet[*fim];
    vet[*fim] = tmp;
    corrige_descendo(vet, *fim, 0);
}
