#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
//#include <pbl.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

//#include "common.h"
//#include "parallel.h"
//mpicc -I pbl_1_04/ coloring.c -o coloring

#define TAG 42

#define POS(x, y, side) ((x) + (y) * (side))

using namespace std;

//vector<vector <int> > matrix_out;

class Set{
public:
	vector<int> s;
	//int size;
public:
	Set(){}
	void add_set(int e){
		bool aux=false;
		for(int i=0;i<s.size();++i)
		{
			if(s[i]==e) return;
		}
		s.push_back(e);
	}
	void clear_set(){
		s.clear();
	}

	bool color_in_set(int e){
		for(int i=0;i<s.size();++i)
		{
			if(s[i]==e) return true;
		}
		return false;
	}	
};



void error(char * msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}


// devuele la posicion de la arista i -> j
/*
int POS(int node1, int node2, int n_nodes)
{
	for(int i=0;i<n_nodes;++i)
		for(int j=0;j<n_nodes;++j)
		{
			if(node1==i && node2==j)
				return (i*n_nodes) + j;
		}

}*/



// lee de archivo un grafo -> matriz adiacencia
void read_file_to_matrix_ady(FILE * f, int ** pmatrix, int ** other_matrix,int * pnodes) {
	//printf("entro a red graph \n");
	int arista, i;
	fscanf(f, " %d", pnodes);
	fscanf(f, " %d", &arista);
	////////////////////////////
	//int * other_matrix;
	*other_matrix = (int *) calloc(*pnodes * *pnodes, sizeof(int)); 
	////////////////////////
	*pmatrix = (int *) calloc(*pnodes * *pnodes, sizeof(int));


	//printf("node size %d \n",*pnodes);
		
	/* lee las aristas */
	for (i = 0; i < arista; i++) {
		int node1, node2;
		int n = fscanf(f, " %d %d", &node1, &node2);
		// verfica que sean dos nodos y una arista
		if (n != 2 || node1 < 0 || node1 >= *pnodes || node2 < 0 || node2 >= *pnodes) {
			fprintf(stderr, "error arista%d in grafo de archivo\n", i);
			exit(1);
		}
		(*pmatrix)[POS(node1, node2, *pnodes)] = 1;
		(*pmatrix)[POS(node2, node1, *pnodes)] = 1;
		(*other_matrix)[POS(node2, node1, *pnodes)] = 1;
		(*other_matrix)[POS(node1, node2, *pnodes)] = 1;
		
	}
	//printf("termino read graph\n");
}//

//imprime matriz de adyasencia
void print_adjacency_matrix(int pid, int * matrix_ady, int my_nodes, int nodes) {
	int i, j;
	//vector<vector<int > > rpta;
	//printf("[rank %d] adjacency %d:\n", pid, my_nodes*nodes);
	for ( i = 0; i < my_nodes; i++ ) {
		//vector<int> aux;
		for ( j = 0; j < nodes; j++ ){	
			printf( "%d ", matrix_ady[i*nodes + j]);
			//aux.push_back(matrix_ady[i*nodes + j]);
		}
		//printf( "[rank %d]\n",pid);
		//matrix_out.push_back(aux);
	}
	
	fflush(stdout);
}

string string_color(int color)
    {
        //cout<<"entro"<<endl;

            if(color==1) return "red";
            else if(color==2) return "green";
            else if(color==3) return "blue";
            else if(color==4) return "orange";
            else if(color==5) return "purple";
            else if(color==6) return "yellow";
            else if(color==7) return "pink";
            else if(color==8) return "brown";
            else if(color==9) return "gray";
            else if(color==10) return "black";

  
}



int find_color(Set psetcolors, int nodes) {
	int color;
	for (color = 1; psetcolors.color_in_set(color) && color < nodes; color++);
	return color;
}

int get_random(int a, int b) {

	return rand() % (b - a + 1) + a;
}



void coloring_parallel(int pid) {

	int nodes, node, i, tmp;
	int color, othernode;
	MPI_Status status;
	MPI_Request req;
	int * indexes, *colors, *matrix_ady;

	//PblSet * psetcolors = (PblSet *) pblSetNewHashSet();
	
	//psetcolors = (int*) malloc(nodes*sizeof(int));
	Set psetcolors;
	MPI_Recv( &nodes, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status );

	//printf("[rank %d] NODES: %d\n", pid, nodes);

	srand( time(NULL) / pid );

	indexes = (int*) malloc(nodes*sizeof(int));
	colors =  (int*) malloc(nodes*sizeof(int));
	matrix_ady  = (int*) malloc(nodes*nodes*sizeof(int));

	MPI_Recv(matrix_ady, nodes*nodes, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

	for (i = 0; i < nodes; i++)
		indexes[i] = i;

	for (i = nodes-1; i >= 0; i--) {
		tmp = get_random(0, i);
		
		node = indexes[tmp];

		indexes[tmp] = indexes[i];

		//pblSetClear(psetcolors);
		psetcolors.clear_set();

		for (othernode = 0; othernode < nodes; othernode++) {
			if (othernode != node) {
				if (matrix_ady[POS(othernode, node, nodes)] == 1) {
					if (colors[othernode] != 0){
					// the neighbor already has a color 
						//pblSetAdd(psetcolors, (void *) colors[othernode]);
						psetcolors.add_set(colors[othernode]);
					}
				}
				
			}
		}

		colors[node] = find_color(psetcolors, nodes);
	}

	//printf("[rank %d] colors = ", pid);
	//for (i = 0; i < nodes; i++)
	//	printf(" %d", colors[i]);
	//printf("\n");

	MPI_Send(colors, nodes, MPI_INT, 0, TAG, MPI_COMM_WORLD);
}


int main(int argc, char * argv[]) {
	

	//printf("ya fuimos \n");
	int * matrix, * colors, * conflicts, *other_matrix;
	int nbrnodes, nbrcolors,best_solution_pos,best_solution_max_colors;
	int i, j, err, blocks;
	int  numtasks, rank, conflict_pos = 0, read_items;
	static int excl_ranks[1] = {0};
	clock_t ticks1, ticks2;
	MPI_Request * reqs;
	MPI_Status * status;

	MPI_Group orig_group, workers_group;
	MPI_Comm comm_workers;

	Set psetcolors;
    
	
	//printf("mpi funcionando\n" );

	colors = (int*)malloc(nbrnodes*sizeof(int));

	err = MPI_Init(&argc, &argv); /* Initialize MPI */
	if (err != MPI_SUCCESS) {
		printf("MPI_init failed!\n"); return 1;
	}

	err = MPI_Comm_size(MPI_COMM_WORLD, &numtasks);	/* Get nr of tasks */
	if (err != MPI_SUCCESS) {
		printf("MPI_Comm_size failed!\n"); return 1;
	}

	err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);    /* Get id of this process */
	if (err != MPI_SUCCESS) {
		printf("MPI_Comm_rank failed!\n"); return 1;
	}
	


	reqs = (MPI_Request*)malloc((numtasks-1)*sizeof(MPI_Request));
	status = (MPI_Status*)malloc((numtasks-1)*sizeof(MPI_Status));

	if (rank == 0) {
		//string name_file;

		

		//cout<<"nombre del archivo: ";
		//cin>>name_file;
		//printf("dentro de rank 0\n");
		//char * file = name_file;
		FILE * f ;
		//f = fopen(name_file.c_str(),"r");
		f = fopen(argv[1],"r");
		//printf("archivo leyendo\n");

		//int * matrix_ini;
////////////////////////////////////tomar el tiempo

		ticks1 = clock();
		read_file_to_matrix_ady(f, &matrix, &other_matrix, &nbrnodes);
		//read_file_to_matrix_ady(f, &matrix_ini, &nbrnodes);
		//int * matrix_2 = matrix;

		//print_adjacency_matrix(0, matrix, nbrnodes, nbrnodes);

	    //printf("nbrnodes = %d\n", nbrnodes);
	    
	   
        for(i = 1; i < numtasks; i++) {
            MPI_Send( &nbrnodes, 1, MPI_INT, i, TAG, MPI_COMM_WORLD );
    		MPI_Send( matrix, nbrnodes*nbrnodes, MPI_INT, i, TAG, MPI_COMM_WORLD );   
        }
        
        
        for (i = 1; i < numtasks; i++){
			MPI_Irecv( &matrix[(i-1)*nbrnodes], nbrnodes, MPI_INT, i, TAG, MPI_COMM_WORLD, &reqs[i-1] );
		}
		MPI_Waitall(numtasks-1, reqs, status);
        
		
        best_solution_pos = 0;
        best_solution_max_colors = 0;
		// FIND MAX COLOR and best solucion
        for(i = 0; i < numtasks-1; i++) {
			nbrcolors = 0;
		    for (j = 0; j < nbrnodes; j++) {
			    if (matrix[i*nbrnodes + j] > nbrcolors) {   
				    nbrcolors = matrix[i*nbrnodes + j];
			    }
			}
			
			if(best_solution_max_colors > nbrcolors || best_solution_max_colors == 0) {
                best_solution_pos = i * nbrnodes;
                best_solution_max_colors = nbrcolors;
			}
			
				    
		}
		ticks2	= clock();
		printf("--------------------------\n");
		printf("numero de nodes: %d\n numero max de colores: %d\n", nbrnodes, best_solution_max_colors);
        for (i = best_solution_pos; i < best_solution_pos+nbrnodes; i++)
			printf("nodo :%d , color :%d\n", i,matrix[i]);
        printf("Tiempo estimado = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
	float t_estimado = 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC ;
        //////////////////////////////////////////////////////
        //vector<int> colores_f;
///////////////////////////////////////////////////////////////////////
        ofstream Esc("grafo.dot");

    	Esc<<"strict graph A {"<<endl;
    	int k=0;
        for (i = best_solution_pos; i < best_solution_pos+nbrnodes; i++) {
			//printf("best solucion :%d\n", matrix[i]);
			//colores_f.push_back(matrix[i]);
			////////////////////////////
			Esc<<k<<" [shape=circle,sides=4,distortion=\"0.0\",orientation=\"0.0\",skew =\"0.0\",fontcolor=white,style=filled,fillcolor="<<string_color(matrix[i])<<"] "<<";"<<endl; 
			k++;

		}
        

        
////////////////////////////////////////////////////

        //print_adjacency_matrix(0, matrix, nbrnodes, nbrnodes);
        //print_adjacency_matrix(0, other_matrix, nbrnodes, nbrnodes);
        //dot_adjacency_matrix(0, other_matrix, nbrnodes, nbrnodes,colores_f);

        ///////////////////////////////////////////////////////////////////

        for ( i = 0; i < nbrnodes; i++ ) {
			
			for ( j = 0; j < nbrnodes; j++ ){	
				if(i!=j){
					if(other_matrix[i*nbrnodes + j]==1){
					
					Esc<<i<<"--"<<j<<";"<<endl;
					
					}
				}
				else break;
		}
		
	}

///////////////////////////////////////////////////////////////////////////
	stringstream ss (stringstream::in | stringstream::out);
	ss<<t_estimado;	
	string label = ss.str();
	Esc<<label<<endl;
        Esc<<"}"<<endl;
  		Esc.close();
   		system("dot grafo.dot -Tpdf -o grafo.pdf");
		system("dot grafo.dot -Tpng -o grafo.png");
   		//system("evince grafo.pdf");
   		cout<<endl<<"VER EL PDF y PNG: grafo"<<endl;

   		

   		

	} else {	
		coloring_parallel(rank);
	}
	

	err = MPI_Finalize();	         /* MPI termina */
	if (err != MPI_SUCCESS) {
		printf("Error en MPI_Finalize!\n");
		return -1;
	}
	
    return 0;
    
}
