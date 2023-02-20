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

Graph getMesh(int size,int k){
    Graph Gmesh = Graph(size*size);
    for(int i=0 ; i < size ; i++){
        for(int j=0 ; j < size-1 ; j++){ 
            Gmesh.adj[getindex(i,j,size)].push_back(getindex(i,j+1,size)); // i,j -> i,j+1 
            Gmesh.adj[getindex(i,j+1,size)].push_back(getindex(i,j,size));
        }
    }
    for(int i=0 ; i < size-1 ; i++){
        for(int j=0 ; j < size ; j++){ 
            Gmesh.adj[getindex(i,j,size)].push_back(getindex(i+1,j,size)); // i,j -> i+1,j 
            Gmesh.adj[getindex(i+1,j,size)].push_back(getindex(i,j,size));
        }
    }

    /*Randomization*/
    for(int i=0 ; i< Gmesh.V ; i++){
        for(int j=0; j < k ; j++){
            Gmesh.adj[i].push_back(rand()%(Gmesh.V));
        }
    }

    return Gmesh;
}

Graph getTree(int size,int k){
    
    Graph Gtree = Graph(pow(2,size));
    vector<int> list;
    list.push_back(0);
    int lsize;
    for(int index =1 ; index < pow(2,size) ; index = index*2){
        lsize = list.size();
        for(int j =0 ; j <lsize ;j++ ){
            Gtree.adj[list[j]].push_back(list[j]+index);
            Gtree.adj[list[j]+index].push_back(list[j]);
            list.push_back(list[j]+index);
        }
    }

    /*Randomization*/
    for(int i=0 ; i< Gtree.V ; i++){
        for(int j=0; j < k ; j++){
            Gtree.adj[i].push_back(rand()%(Gtree.V));
        }
    }

    return Gtree;
}

/*-------------------------*/

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

int getSimulatedDiameter(int noftrails, Graph graph){
    int diameter = -1;
    for(int i =0 ; i < noftrails ; i++){
        diameter = max(DistBFS(graph,rand()%(graph.V),rand()%(graph.V)),diameter);
    }
    return diameter;
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

int Dilation(Graph G1,Graph G2){
    int dilation = 1;
    for(int i=0 ; i < G1.V ; i++){
        for(auto node : G1.adj[i]){
            dilation = max(dilation,DistBFS(G2,i,node));
        }
    }
    return dilation
}



int main(int argc, char *argv[]) {

    int network = atoi(argv[1]);
    int size = atoi(argv[2]);
    int k = atoi(argv[3]);

    int diameter;
    Graph graph(0);

    if(network ==0){ // Ring
        graph = getRing(size);
        diameter = getSimulatedDiameter(10000,graph);
    }else if(network ==1){
        graph = getMesh(size,k);
        diameter = getSimulatedDiameter(100000,graph);
    }else if(network ==2){ 
        graph = getTree(size,k);
        diameter = getSimulatedDiameter(5,graph);
    }
    
    // printf("No of nodes in G %4d and it's diameter is %d \n",graph.V,diameter);
    printf(" %4d , %d \n",graph.V,diameter);

    return 0;
}