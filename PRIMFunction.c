#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "TP1Function.h"



node_t* create_node(int id_node,int nb_nodes){
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->id = id_node;
    node->nb_neighbors = 0;
    node->neighbors = (int*) malloc((1+nb_nodes)*sizeof(int));

    for(int i=0;i<(1+nb_nodes);i++){
        node->neighbors[i]=-1;
    }
    return node;
} 

node_t** node_list(FILE* fichier){
    fseek(fichier, 0, SEEK_SET);
    int nb_nodes;
    fscanf(fichier, "%d", &nb_nodes);    

    node_t** list_node = (node_t**) malloc(nb_nodes*sizeof(node_t*));
    for(int i=0;i<nb_nodes;i++){
        list_node[i] = create_node(i,nb_nodes);
    }
    return list_node;
}

node_t** node_list_prim(int nb_nodes){
    node_t** list_node = (node_t**) malloc(nb_nodes*sizeof(node_t*));
    for(int i=0;i<nb_nodes;i++){
        list_node[i] = create_node(i,nb_nodes);
    }
    return list_node;
}

void mod_node(int id1,int id2,node_t** list_node){
    list_node[id1]->neighbors[list_node[id1]->nb_neighbors]=id2;
    list_node[id1]->nb_neighbors++;
}

edge_t* create_edge(int id1,int id2,int poids){
    edge_t* edge =(edge_t*) malloc(sizeof(edge_t));
    edge->id_deb = id1;
    edge->id_fin = id2;
    edge->weight = poids;
    return edge;
}

edge_t** edge_list(FILE* fichier){
    fseek(fichier, 0, SEEK_SET);
    int nb_nodes = 0 , nb_edges =0;
    fscanf(fichier, "%d,%d", &nb_nodes ,&nb_edges);

    edge_t** list_edge = (edge_t**) malloc(nb_edges*sizeof(edge_t*));
    for(int i=0;i<nb_edges;i++){
        list_edge[i] = create_edge(-1,-1,-1);
    }
    return list_edge;
} 

edge_t** edge_list_prim(int nb_nodes){
    edge_t** list_edge_prim = (edge_t**) malloc(nb_nodes*sizeof(edge_t*));
    for(int i=0;i<nb_nodes;i++){
        list_edge_prim[i] = create_edge(-1,-1,-1);
    }
    return list_edge_prim;   
}

void mod_edge(int k,int id1,int id2,int poids,edge_t** edge_list){
    edge_list[k]->id_deb = id1;
    edge_list[k]->id_fin = id2;
    edge_list[k]->weight = poids;
}

int sum_weight(edge_t** list_edges,int nb_edges){
    int sum = 0;
    for(int i=0;i<nb_edges;i++){
        sum += list_edges[i]->weight;
    }
    return sum;
}

int in_graph(int id,node_t** list_node){
    int true = 1;
    if(list_node[id]->nb_neighbors == 0)
        true = 0;
    return true;
}

int get_weight(int id1,int id2,edge_t** list_edge,int nb_edges){
    int poids = -1;
    int k=0;
    while(k<nb_edges){
        if(list_edge[k]->id_deb == id1 && list_edge[k]->id_fin == id2){
            poids = list_edge[k]->weight;
            k = nb_edges;
        }
        k++;
    }
    return poids;
}

int closest(int id,node_t** list_node,edge_t** list_edge,int nb_edges){
    int id_close = -1;
    int poids = INT_MAX;
    for(int i=0;i<list_node[id]->nb_neighbors;i++){
        int dist = get_weight(id,list_node[id]->neighbors[i],list_edge,nb_edges);
        if(dist < poids){
            poids = dist;
            id_close = list_node[id]->neighbors[i];
        }
    }
    return id_close;
}

void close_to_graph(node_t** list_nodes,node_t** list_nodes_prim,edge_t** list_edge_prim,edge_t** list_edges,int nb_nodes,int nb_edges,int tab[3]){
    int id_closest = -1;
    int id_close_to = -1;
    int poids = INT_MAX;

    for(int i=0;i<nb_nodes;i++){
        if(in_graph(i,list_nodes_prim)==1){
            for(int j=0;j<list_nodes[i]->nb_neighbors;j++){
                if(in_graph(list_nodes[i]->neighbors[j],list_nodes_prim)==0){
                    int dist = get_weight(i,list_nodes[i]->neighbors[j],list_edges,nb_edges);
                    if(dist < poids){
                        poids = dist;
                        id_closest = list_nodes[i]->neighbors[j];
                        id_close_to = i;
                    }
                }
            }
        }
    }
    tab[0] = id_closest;
    tab[1] = id_close_to;
    tab[2] = poids;
}


void data_extraction(FILE* fichier,node_t** list_node,edge_t** list_edge){
    fseek(fichier, 0, SEEK_SET);
    int nb_nodes , nb_edges;
    fscanf(fichier, "%d,%d\n", &nb_nodes , &nb_edges);

    for(int k=0;k<nb_edges;k++){
        int id1, id2, poids;
        fscanf(fichier,"%d,%d,%d\n",&id1,&id2,&poids);
        mod_edge(k,id1,id2,poids,list_edge);
        mod_node(id1,id2,list_node);
    }
}

void prim(edge_t** list_edge,edge_t** list_edge_prim,node_t** list_node,node_t** list_node_prim,int node_in,int nb_edges,int nb_nodes){
    int close = closest(node_in,list_node,list_edge,nb_edges);
    int dist = get_weight(node_in,close,list_edge,nb_edges);
    mod_node(node_in,close,list_node_prim);
    mod_node(close,node_in,list_node_prim);

    int k = 0;
    mod_edge(k,node_in,close,dist,list_edge_prim);
    k++;

    int close_graph[3];
    close_graph[0] = -2;
    close_graph[1] = -2;
    close_graph[2] = -1;
    while(close_graph[0] != -1 && k<(nb_nodes-1)){
        close_to_graph(list_node,list_node_prim,list_edge_prim,list_edge,nb_nodes,nb_edges,close_graph);
        mod_node(close_graph[1],close_graph[0],list_node_prim);
        mod_node(close_graph[0],close_graph[1],list_node_prim);
        mod_edge(k,close_graph[1],close_graph[0],close_graph[2],list_edge_prim);
        k++;
    }
}

void free_edge(edge_t** edges,int nb_edges){
    for(int i=0;i<nb_edges;i++){
        free(edges[i]);
    }
    free(edges);
}

void free_node(node_t** nodes,int nb_nodes){
    for(int i=0;i<nb_nodes;i++){
        free(nodes[i]->neighbors);
    }
    for(int i=0;i<nb_nodes;i++){
        free(nodes[i]);
    }
    free(nodes);
}

void free_all(edge_t** edges,node_t** nodes,int nb_nodes , int nb_edges){
    free_edge(edges,nb_edges);
    free_node(nodes,nb_nodes);
}

void affichage_node(int nb_nodes,node_t** list_node){
    printf("\n");
    for(int i=0;i<nb_nodes;i++){
        printf("node : %d , nb_neighbors : %d , neighbors :",list_node[i]->id,list_node[i]->nb_neighbors);
        for(int j=0;j<list_node[i]->nb_neighbors;j++){
            printf(" %d",list_node[i]->neighbors[j]);
        }
        printf("\n");
    }
}

void affichage_edge(int nb_edges,edge_t** list_edge){
    printf("\n");
    for(int i=0;i<nb_edges;i++){
        printf("id1 = %d , id2 = %d , poids = %d\n",list_edge[i]->id_deb,list_edge[i]->id_fin,list_edge[i]->weight);
    }
}

void test_remplissage(){
    FILE* fichier = fopen("TP1instance.csv","r");
    if(fichier != NULL){
        fseek(fichier, 0, SEEK_SET);
        int nb_nodes = 0 , nb_edges =0;
        fscanf(fichier, "%d,%d", &nb_nodes ,&nb_edges);   

        node_t** list_node = node_list(fichier);
        edge_t** list_edge = edge_list(fichier);
        data_extraction(fichier,list_node,list_edge);
        printf("\n");
        affichage_node(nb_nodes,list_node);
        printf("\n");
        affichage_edge(nb_edges,list_edge);
        printf("\n");


        node_t** list_node_prim = node_list_prim(nb_nodes);
        edge_t** list_edge_prim = edge_list_prim(nb_nodes);
        int node_in = 3;
        test_prim(list_edge,list_edge_prim,list_node,list_node_prim,node_in,nb_edges,nb_nodes);
        free_all(list_edge,list_node,nb_nodes,nb_edges);
        free_all(list_edge_prim,list_node_prim,nb_nodes,nb_nodes);
        fclose(fichier);
    }
}

void test_prim(){
    FILE* fichier = fopen("TP1instance.csv","r");
    if(fichier != NULL){
        fseek(fichier, 0, SEEK_SET);
        int nb_nodes = 0 , nb_edges =0;
        fscanf(fichier, "%d,%d", &nb_nodes ,&nb_edges);   

        node_t** list_node = node_list(fichier);
        edge_t** list_edge = edge_list(fichier);
        node_t** list_node_prim = node_list_prim(nb_nodes);
        edge_t** list_edge_prim = edge_list_prim(nb_nodes);

        int node_in = 5;

        data_extraction(fichier,list_node,list_edge);
        prim(list_edge,list_edge_prim,list_node,list_node_prim,node_in,nb_edges,nb_nodes);
        affichage_node(nb_nodes,list_node_prim);
        affichage_edge(nb_nodes-1,list_edge_prim);
        printf("\n");

        int sum = sum_weight(list_edge_prim,nb_nodes-1);
        printf("sum weight : %d\n",sum);

        free_all(list_edge,list_node,nb_nodes,nb_edges);
        free_all(list_edge_prim,list_node_prim,nb_nodes,nb_nodes);
        fclose(fichier);
    }
}
