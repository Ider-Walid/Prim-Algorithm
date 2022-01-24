typedef struct node{	
    int id;
    int nb_neighbors;
    int* neighbors;
} node_t;

typedef struct edge{
    int id_deb;
    int id_fin;
    int weight;
} edge_t;


node_t* create_node(int id_node,int voisins);
node_t** node_list(FILE* fichier);
node_t** node_list_prim(int nb_nodes);
void mod_node(int id1,int id2,node_t** list_node);

edge_t* create_edge(int id1,int id2,int poids);
edge_t** edge_list(FILE* fichier);
edge_t** edge_list_prim(int nb_nodes);
void mod_edge(int k,int id1,int id2,int poids,edge_t** edge_list);

int sum_weight(edge_t** list_edges,int nb_edges);
int in_graph(int id,node_t** list_node);
int get_weight(int id1,int id2,edge_t** list_edge,int nb_edges);
int closest(int id,node_t** list_node,edge_t** list_edge,int nb_edges);
void close_to_graph(node_t** list_nodes,node_t** list_nodes_prim,edge_t** list_edge_prim,edge_t** list_edges,int nb_nodes,int nb_edges,int tab[3]);

void data_extraction(FILE* fichier,node_t** list_node,edge_t** list_edge);
void prim(edge_t** list_edge,edge_t** list_edge_prim,node_t** list_node,node_t** list_node_prim,int node_in,int nb_edges,int nb_nodes);

void free_edge(edge_t** edges,int nb_edges);
void free_node(node_t** nodes,int nb_nodes);
void free_all(edge_t** edges,node_t** nodes,int a , int b);

void affichage_node(int nb_nodes,node_t** list_node);
void affichage_edge(int nb_edges,edge_t** list_edge);

void test_remplissage();
void test_prim();