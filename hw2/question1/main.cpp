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



int DistBFS(Graph G,int node1,int node2){
    vector<int> visited = vector<int> (G.V, 0);
    vector<int> parent = vector<int> (G.V, -1);
    vector<int> distance = vector<int> (G.V, -1);
    queue <int> que;
    int currnode;

    visited[node1] = 1;
    distance[node1] = 0;
    que.push(node1);
    while(!que.empty()){
        currnode = que.front();
        que.pop();
        for(auto & node : G.adj[currnode]){
            if (visited[node]==0){
                visited[node] = 1;
                parent[node] = currnode;
                distance[node] = distance[parent[node]]+1;
                que.push(node);
            }
        }
    }
    return distance[node2];
}

/*-------------------------------------*/

int getSimulatedDiameter(int noftrails, Graph graph){
    int diameter = -1;
    for(int i =0 ; i < noftrails ; i++){
        diameter = max(DistBFS(graph,rand()%(graph.V),rand()%(graph.V)),diameter);
    }
    return diameter;
}


int main(int argc, char *argv[]) {

    int network = atoi(argv[1]);
    int size = atoi(argv[2]);

    int diameter;
    Graph graph(0);

    if(network ==0){ // Ring
        graph = getRing(size);
        diameter = getSimulatedDiameter(1000,graph);
    }else if(network ==1){
        graph = getMesh(size);
        diameter = getSimulatedDiameter(1000,graph);
    }else if(network ==2){

    }
    
    printf("No of nodes in G %3d and it's diameter is %d \n",graph.V,diameter);

    return 0;
}