#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <pbl.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>


using namespace std;

class Set{
public:
	vector<int> s;
	//int size;
public:
	Set(){}
	void addset(int e){
		bool aux=false;
		for(int i=0;i<s.size();++i)
		{
			if(s[i]==e) return;
		}
		s.push_back(e);
	}
	void clearset(){
		s.clear();
	}

	bool setcontains(int e){
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



int POS(int node1, int node2, int n_nodes)
{
	for(int i=0;i<n_nodes;++i)
		for(int j=0;j<n_nodes;++j)
		{
			if(node1==i && node2==j)
				return (i*n_nodes) + j;
		}

}

void read_graph_to_adjacency_matrix(FILE * f, int ** pmatrix, int ** other_matrix,int * pnodes) {
	//printf("entro a red graph \n");
	int edges, i;
	fscanf(f, " %d", pnodes);
	fscanf(f, " %d", &edges);
	////////////////////////////
	//int * other_matrix;
	*other_matrix = (int *) calloc(*pnodes * *pnodes, sizeof(int)); 
	////////////////////////
	*pmatrix = (int *) calloc(*pnodes * *pnodes, sizeof(int));


	//printf("node size %d \n",*pnodes);
	if (*pmatrix == NULL) {
		//printf("entro a if NULL\n");
		error("fuera de memoria\n");
		
		exit(-1);
	}
	
	/* read edges one by one */
	for (i = 0; i < edges; i++) {
		int node1, node2;
		int n = fscanf(f, " %d %d", &node1, &node2);
		if (n != 2 || node1 < 0 || node1 >= *pnodes || node2 < 0 || node2 >= *pnodes) {
			fprintf(stderr, "error en arista\n", i);
			exit(1);
		}
		(*pmatrix)[POS(node1, node2, *pnodes)] = 1;
		(*pmatrix)[POS(node2, node1, *pnodes)] = 1;
		(*other_matrix)[POS(node2, node1, *pnodes)] = 1;
		(*other_matrix)[POS(node1, node2, *pnodes)] = 1;
		
	}
	//printf("termino read graph\n");
}//

void print_adjacency_matrix(int pid, int * madjacency, int my_nodes, int nodes) {
	int i, j;
	//vector<vector<int > > rpta;
	printf("[rank %d] adjacency %d:\n", pid, my_nodes*nodes);
	for ( i = 0; i < my_nodes; i++ ) {
		//vector<int> aux;
		for ( j = 0; j < nodes; j++ ){	
			printf( "%d ", madjacency[i*nodes + j]);
			//aux.push_back(madjacency[i*nodes + j]);
		}
		printf( "[rank %d]\n",pid);
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

  //  return "purple";
    }

void color_greedy(int * madjacency, int nodes, int ** pcolors, int * numcolors) {
	int node, highestcolor = 1;
	
	if (madjacency == NULL || nodes == 0) {
		*pcolors = NULL;
		*numcolors = 0;
		return;
	}
		
	*pcolors = (int *) calloc(nodes, sizeof(int));
	if (*pcolors == NULL)
		error("fuera de memoria");
	
	for (node = 0; node < nodes; node++) {
		int color, othernode;
		
		Set psetcolors;

		for (othernode = 0; othernode < nodes; othernode++) {
			if (othernode != node) {
				if (madjacency[POS(othernode, node, nodes)] == 1) {
					int othercolor = (*pcolors)[othernode];
					if (othercolor != 0)
					/* the neighbor already has a color */
						//pblSetAdd(psetcolors, (void *) othercolor);
						psetcolors.addset(othercolor);
				}
				
				/* look for the first available color */
				for (color = 1; psetcolors.setcontains(color) && color < nodes; color++);
				(*pcolors)[node] = color;
				if (color > highestcolor)
					highestcolor = color;
			}
		}
		
		psetcolors.clearset();
	}
	*numcolors = highestcolor;
}

int main (int argc, const char * argv[]) {
	int * matrix, * colors, *other_matrix;
	int nbrnodes, nbrcolors;
	int i;
	clock_t ticks1, ticks2;

	

	//string name_file;
	//cout<<"nombre del archivo: ";
	//cin>>name_file;
		//printf("dentro de rank 0\n");
		//char * file = name_file;
	ticks1 = clock();

	FILE * f ;
	f = fopen(argv[1],"r");
		
	printf("archivo leyendo\n");

	read_graph_to_adjacency_matrix(f, &matrix, &other_matrix, &nbrnodes);

	color_greedy(matrix, nbrnodes, &colors, &nbrcolors);
	print_adjacency_matrix(0, matrix, nbrnodes, nbrnodes);
	//print_adjacency_matrix(0, other_matrix, nbrnodes, nbrnodes);



	printf("%d\n%d\n", nbrnodes, nbrcolors);
	cout<<"-----------------"<<endl;

	ofstream Esc("grafo_serial.dot");

    Esc<<"strict graph A {"<<endl;
    
    for (i = 0; i < nbrnodes; i++) {
	 	printf("color: %d\n", colors[i]);
		
		////////////////////////////
		Esc<<i<<" [shape=circle,sides=4,distortion=\"0.0\",orientation=\"0.0\",skew =\"0.0\",fontcolor=white,style=filled,fillcolor="<<string_color(colors[i])<<"] "<<";"<<endl; 
		
	}


	for ( i = 0; i < nbrnodes; i++ ) {
			
			for ( int j = 0; j < nbrnodes; j++ ){	
				if(i!=j){
					if(matrix[i*nbrnodes + j]==1){
					
					Esc<<i<<"--"<<j<<";"<<endl;
					
					}
				}
				else break;
		}
		
	}
	ticks2	= clock();
	float t_estimado = 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC;
	printf("Tiempo esperado = %lf\n", 1.0 * (ticks2 - ticks1) / CLOCKS_PER_SEC);
	
	stringstream ss (stringstream::in | stringstream::out);
	ss<<t_estimado;	
	string label = ss.str();
	Esc<<label<<endl;
	Esc<<"}"<<endl;
  		Esc.close();
   		system("dot grafo_serial.dot -Tpdf -o grafo_serial.pdf");
   		//system("evince grafo_serial.pdf");
		system("dot grafo_serial.dot -Tpng -o grafo_serial.png");

   		cout<<endl<<"VER EL PDF: grafo_serial.pdf y grafo_serial.png"<<endl;

	
    
    return 0;
}
