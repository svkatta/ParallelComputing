#include <bits/stdc++.h>
using namespace std;

class Graph {
    public:
        int V;
        std::vector<std::vector<int>> adj;
        Graph(int v){
            V = v;
            adj.resize(v);
        }
};


Graph getRing(int size){
    Graph Gring = Graph(size);
    for(int i=0 ; i < size ; i++){  // Ring
        for(int j=1 ; j < 5 ; j++){ // 4 counter clockwise connections
            Gring.adj[i].push_back( (i+j)%size );
            Gring.adj[(i+j)%size].push_back(i);
        }
    }
    return Gring;
}

int getindex(int i , int  j ,int size){
    return (i%size)*size + j %size;
}

Graph getMesh(int size){
    Graph Gmesh = Graph(size*size);
    for(int i=0 ; i < size ; i++){
        for(int j=0 ; j < size ; j++){ 
            Gmesh.adj[getindex(i,j,size)].push_back(getindex(i,j+1,size)); // i,j -> i,j+1 
            Gmesh.adj[getindex(i,j+1,size)].push_back(getindex(i,j,size));
            Gmesh.adj[getindex(i,j,size)].push_back(getindex(i+1,j,size)); // i,j -> i+1,j 
            Gmesh.adj[getindex(i+1,j,size)].push_back(getindex(i,j,size));
        }
    }
    return Gmesh;
}


int GraphWidth(Graph G, vector<int> equipartionlist){
    int count =0;
    for(int i=0 ; i < G.V ; i++){
        for(auto & node : G.adj[i]){
            if(equipartionlist[i]==0 && equipartionlist[node]==1){
                count++;
            }
        }
    }
    return count;
}

int BisectionWidth(Graph G){
    vector<int> positions(G.V,0);
    for (int i = 0; i < G.V/2; i++) {
        positions[i]=1;
    }

    int bisecwidth =INT_MAX;
    for(int i =0 ; i < 20*(G.V)*(G.V)*(G.V) ; i++){
        random_shuffle(positions.begin(), positions.end());
        bisecwidth = min(bisecwidth,GraphWidth(G,positions));
    }
    return bisecwidth;
}

int main(int argc, char *argv[]) {

    int network = atoi(argv[1]);
    int size = atoi(argv[2]);

    int bisectionWidth;
    Graph graph(0);

    if(network ==0){ // Ring bisection width is 20 after
        graph = getRing(size);
        bisectionWidth = BisectionWidth(graph);
    }else if(network ==1){
        graph = getMesh(size);
        bisectionWidth = BisectionWidth(graph);
    }else if(network ==2){

    }
    
    printf("No of nodes in G %3d and it's bisection width is %d \n",graph.V,bisectionWidth);

    return 0;
}