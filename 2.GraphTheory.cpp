#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
// DONE
// dfs, bfs, dijkstra, BallmanFord, treeTraversal, unionFind
// MST(최소 신장 트리, 크루스칼 알고리즘), topologySort, LCA(최소 공통 조상)
// Strongly Connected Component, Segment Tree
// BipatalMatching(이분 매칭), Network Flow(최대 유량)
// TODO
// ....

std::vector<int> dfsVisit;
std::vector<int> bfsVisit;
std::vector<int> checkDfs1(1001, 0);
void dfs1(std::vector<std::vector<int>> graph, int v) {
    // 루트 노드에서 시작해서 다음 분기로 넘어가기 전에 해당 분기를 완벽하게 탐색
    // 루트 노드는 depth 0, 루트 노드에서 n번에 갈 수 있는 노드는 depth n이다.

    // depth 1a -> 2a -> 3a -> 4a ... / 1a -> 2b -> ... / 1b -> 2c -> 3 -> 4 ...
    // 모든 노드를 방문하고자 하는 경우 사용한다.
    // 재귀 이용
    if(checkDfs1[v] == 0){
        dfsVisit.push_back(v);
        checkDfs1[v] = 1;
        for(int j = 0 ; j < graph[v].size() ; j++) {
            int next = graph[v][j];
            if(checkDfs1[next] == 0) {
                dfs1(graph, next);
            }
        }
    }
}

std::vector<int> checkDfs2(1001, 0);
std::stack<int> dfsStack;
void dfs2(std::vector<std::vector<int>> graph, int v) {
    // 스택 이용
    dfsStack.push(v);
    while(!dfsStack.empty()) {
        int current = dfsStack.top();
        dfsStack.pop();
        if(checkDfs2[current] == 0) {
            dfsVisit.push_back(current);
            checkDfs2[current] = 1;
            for(int j = graph[current].size() - 1 ; j >= 0 ; j--) {
                int next = graph[current][j];
                //std::cout << "current : " << current << " next : " << next << "\n";
                if(checkDfs2[next] == 0) {
                    dfsStack.push(next);
                }
            }
        }
    }
}

std::vector<int> checkBfs(1001, 0);
std::queue<int> bfsQueue;
void bfs(std::vector<std::vector<int>> graph, int v) {
    // 루트 노드에서 시작해서 인접한 노드를 탐색
    // 루트 노드는 depth 0, 루트 노드에서 n 번에 갈 수 있는 노드는 depth n이다.

    // dept 1 all -> 2 all -> 3 all -> 4 all -> ...
    // 두 노드 사이의 최단 경로 or 임의의 경로를 찾을 때 사용한다. Prim, Dijkstra 와 유사
    // 큐 이용
    bfsQueue.push(v);
    checkBfs[v] = 1;
    while(!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();
        bfsVisit.push_back(current);
        for(int j = 0 ; j < graph[current].size() ; j++) {
            int next = graph[current][j];
            if(checkBfs[next] == 0) {
                bfsQueue.push(next);
                checkBfs[next] = 1;
            }
        }
    }

}
struct compare{
    bool operator() (std::vector<int> a, std::vector<int> b) {
        return a[1] > b[1];
    }
};
int dijkstraNlogN() {
    // 출발 노드로부터 다른 노드로까지의 최단 거리를 구할 때 이용.
    // 시작점부터 갈 수 있는 곳의 거리를 구해 d를 갱신힌다.
    // 다음 이동은 방문하지 않았고 d가 가장 짧은 곳.
    int n, m;
    std::cin >> n >> m;

    int start;
    std::cin >> start;

    std::vector<std::vector<std::vector<int>>> graph(n + 1);

    for(int i = 0 ; i < m ; i++) {
        int x, y, value;
        std::cin >> x >> y >> value;
        graph[x].push_back({y, value});
    }

    int INF = 3000001;
    std::vector<int> check(n + 1, 0);
    std::vector<int> d(n + 1, INF);
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, compare> q;
    d[start] = 0;
    q.push({start, d[start]});

    while(!q.empty()) {
        int current = q.top()[0];
        q.pop();
        check[current] = 1;

        for(int i = 0 ; i < graph[current].size() ; i++) {
            int next = graph[current][i][0];
            int value = graph[current][i][1];

            int candid = d[current] + value;
            if(check[next] == 0 && candid < d[next]) {
                d[next] = candid;
                q.push({next, d[next]});
            }
        }
    }
    
    for(int i = 1 ; i <= n ; i++) {
        if(d[i] == INF) {
            std::cout << "INF\n";
        }
        else {
            std::cout << d[i] << "\n";
        }
    }
}

int BallmanFord() {
    std::ios_base :: sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<std::vector<long long>>> graph(n + 1);
    for(int i = 0 ; i < m ; i++) {
        long long x, y, value;
        std::cin >> x >> y >> value;
        graph[x].push_back({y, value});
    }

    int start = 1;
    long long INF = INT32_MAX;
    std::vector<long long> dist(n + 1, INF);
    // 음수 간선이 포함되어 있기 때문에 벨만 포드 알고리즘 사용.
    // 이는 한 점을 여러 번 방문 할 수도 있음.
    // 하지만, 음수 사이클이 존재하기 때문에 path의 길이는 n - 1이 최대임.
    // 거리를 무한대로 설정한 뒤 방문한 곳에 한해서만 거리를 갱신함.
    
    dist[start] = 0;
    int isCycle = 0;
    for(int i = 1 ; i <= n ; i++) {
        // path의 길이
        for(int j = 1 ; j <= n ; j++) {
            int current = j;
            // j -> graph[j][k][0] 으로 path를 고려해 갱신.
            for(int k = 0 ; k < graph[current].size() ; k++) {
                int next = graph[current][k][0];
                long long value = graph[current][k][1];
                long long candid = value + dist[current];
                if(dist[current] != INF && dist[next] > candid) {
                    dist[next] = candid;
                    if(i == n) {
                        isCycle = 1;
                    }
                }
            }
        }
    }

    if(isCycle) {
        std::cout << -1 << "\n";
    }
    else {
        for(int i = 2 ; i <= n ; i++) {
            if(dist[i] == INF) {
                std::cout << -1 << "\n";
            }
            else {  
                std::cout << dist[i] << "\n";
            }
        }
    }
}

int floydWarshall() {
    // 모든 정점에서 모든 정점으로의 최단 경로를 구하고자 할 때 사용
    // x -> y vs x -> z -> y (for z = 1, .. n)
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<std::vector<long long>>> graph(n + 1);
    for(int i = 0 ; i < m ; i++) {
        long long x, y, value;
        std::cin >> x >> y >> value;
        graph[x].push_back({y, value});
    }
    long long INF = INT32_MAX - 1;
    std::vector<std::vector<long long>> dist(n + 1, std::vector<long long>(n + 1, INF));
    for(int i = 1 ; i <= n ; i++) {
        dist[i][i] = 0;
        for(int j = 0 ; j < graph[i].size() ; j++) {
            dist[i][graph[i][j][0]] = std::min(dist[i][graph[i][j][0]], graph[i][j][1]);
        }
    }

    for(int k = 1 ; k <= n ; k++) {
        for(int i = 1 ; i <= n ; i++) {
            for(int j = 1 ; j <= n ; j++) {
                // i -> j vs i -> k -> j;
                long long dist1 = dist[i][j];
                long long dist2 = dist[i][k] + dist[k][j];
                if(dist2 < dist1) {
                    dist[i][j] = dist2;
                }
            }
        }
    }

    for(int i = 1 ; i <= n ; i++) {
        for(int j = 1 ; j <= n ; j++) {
            if(dist[i][j] == INF) {
                std::cout << 0 << " ";
            }
            else {
                std::cout << dist[i][j] << " ";
            }
        }
        std::cout << "\n";
    }

}

void preorder(std::vector<std::vector<std::vector<int>>> &graph, int parent, std::string &s) {
    s += (char) (parent + 'A');
    if(graph[parent].size() == 1) {
        preorder(graph, graph[parent][0][0], s);
    }
    else if(graph[parent].size() == 2) {
        preorder(graph, graph[parent][0][0], s);
        preorder(graph, graph[parent][1][0], s);
    }
}

void inorder(std::vector<std::vector<std::vector<int>>> &graph, int parent, std::string &s) {
    
    if(graph[parent].size() == 1) {
        if(graph[parent][0][1] == 0) {
            inorder(graph, graph[parent][0][0], s);
            s += (char) (parent + 'A');
        }
        else {
            s += (char) (parent + 'A');
            inorder(graph, graph[parent][0][0], s);
        }
        
    }
    else if(graph[parent].size() == 2) {
        inorder(graph, graph[parent][0][0], s);
        s += (char) (parent + 'A');
        inorder(graph, graph[parent][1][0], s);
    }
    else {
        s += (char) (parent + 'A');
    }
}

void postorder(std::vector<std::vector<std::vector<int>>> &graph, int parent, std::string &s) {
    if(graph[parent].size() == 1) {
        postorder(graph, graph[parent][0][0], s);
    }
    else if(graph[parent].size() == 2) {
        postorder(graph, graph[parent][0][0], s);
        postorder(graph, graph[parent][1][0], s);
    }
    s += (char) (parent + 'A');
}

int treeTraversal() {

    int n;
    std::cin >> n;
    std::vector<std::vector<std::vector<int>>> graph(n + 1);

    for(int i = 0 ; i < n ; i++) {
        char p, cl, cr;
        std::cin >> p >> cl >> cr;

        if(cl != '.') {
            graph[p - 'A'].push_back({cl - 'A', 0});        
        }

        if(cr != '.') {
            graph[p - 'A'].push_back({cr - 'A', 1});        
        }

        //std::cout << p << " : ";
        for(int j = 0 ; j < graph[p].size() ; j++) {
            //std::cout << graph[p][j][0] << " "; 
        }
        //std::cout << "\n";
    }

    std::string pre = "";
    preorder(graph, 0, pre);
    std::cout << pre << "\n";

    std::string in = "";
    inorder(graph, 0, in);
    std::cout << in << "\n";

    std::string post = "";
    postorder(graph, 0, post);
    std::cout << post << "\n";
}

int n, m;
int findParent(std::vector<int> &set, int a) {
    if(set[a] == a) {
        return a;
    }
    else {
        return set[a] = findParent(set, set[a]);
    }
}

int unionFind() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(NULL);
    std::cin.tie(NULL);

    std::cin >> n >> m;
    
    std::vector<int> set(n + 1);
    for(int i = 0 ; i <= n ; i++) {
        set[i] = i;
    }

    for(int i = 0 ; i < m ; i++){
        int order, a, b;
        std::cin >> order >> a >> b;

        int pA = findParent(set, a);
        int pB = findParent(set, b);
        if(order == 0) {    
            if(pA > pB) {
                set[pA] = pB;
            }
            else{
                set[pB] = pA;
            }
        }
        else if(order == 1) {
            if(pA == pB) {
                std::cout << "YES\n";
            }
            else {
                std::cout << "NO\n";
            }
        }
    }
}
int getParent(std::vector<int> &set, int a) {
    if(set[a] == a) {
        return a;
    }
    else {
        return set[a] = getParent(set, set[a]);
    }
}
void unionParent(std::vector<int> &set, int a, int b) {
    a = getParent(set, a);
    b = getParent(set, b);

    if(a > b) {
        set[a] = b;
    }
    else{
        set[b] = a;
    }
}
int findParent(std::vector<int> &set, int a, int b) {
    a = getParent(set, a);
    b = getParent(set, b);
    if(a == b) {
        return 1;
    }
    else{
        return 0;
    }
}

bool compare2(std::vector<int> a, std::vector<int> b) {
    return a[2] < b[2];
}
int MST() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> set(n + 1, 0);
    for(int i = 1 ; i <= n ; i++) {
        set[i] = i;
    }

    std::vector<std::vector<int>> graph;

    for(int i = 0 ; i < m ; i++) {
        int x, y, value;
        std::cin >> x >> y >> value;
        graph.push_back({x, y, value});
    }
    // 크루스칼 알고리즘 사용
    // value 를 기준으로 sort
    // x, y가 같은 집합에 있지 않으면 (연결되있지 않으면) 연결 시키고 가중치 더해줌
    std::sort(graph.begin(), graph.end(), &compare2);

    long long answer = 0;
    for(int i = 0 ; i < m ; i++) {
        int x = graph[i][0];
        int y = graph[i][1];
        if(!findParent(set, x, y)) {
            unionParent(set, x, y);
            answer += (long long) graph[i][2];
        }
    }

    std::cout << answer << "\n";
}

int topologySort() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n + 1);
    std::vector<int> inDegree(n + 1, 0);
    std::vector<int> answer(n + 1, 0);

    for(int i = 0 ; i < m ; i++) {
        int x, y;
        std::cin >> x >> y;
        graph[x].push_back(y);
        inDegree[y] += 1;
    }

    // 진입차수 0 인 노드를 넣고 빼면서 진입차수 갱신
    // 빼는 순서대로가 topology sort 된 배열

    std::queue<int> q;
    for(int i = 1 ; i <= n ; i++) {
        if(inDegree[i] == 0) {
            q.push(i);
        }
    }

    for(int i = 1 ; i <= n ; i++) {
        if(q.empty()) {
            // exist cycle
            break;
        }

        int current = q.front();
        answer[i] = current;
        q.pop();

        for(int j = 0 ; j < graph[current].size() ; j++) {
            int next = graph[current][j];
            inDegree[next] -= 1;
            if(inDegree[next] == 0) {
                q.push(next);
            }
        }
    }

    for(int i = 1 ; i <= n ; i++) {
        std::cout << answer[i] << " ";
    }
    std::cout << "\n";
}

int n;
int LOG = 17;
void dfs(int current, int d, std::vector<std::vector<int>> &graph, std::vector<std::vector<int>> &parent, std::vector<int> &depth, std::vector<int> &check) {
    depth[current] = d;
    check[current] = 1;
    for(int i = 0 ; i < graph[current].size() ; i++) {
        int next = graph[current][i];
        if(check[next] == 0) {
            parent[next][0] = current;
            dfs(next, d + 1, graph, parent, depth, check);
        }
    }

}

void findParent(int root, std::vector<std::vector<int>> &graph, std::vector<std::vector<int>> &parent, std::vector<int> &depth) {
    std::vector<int> check(n + 1, 0);
    dfs(root, 0, graph, parent, depth, check);
    for(int j = 1 ; j <= LOG; j++) {
        for(int i = 1 ; i <= n ; i++) {
            parent[i][j] = parent[parent[i][j - 1]][j - 1];
        }
    }
}

int findLCA(int A, int B, std::vector<std::vector<int>> &graph, std::vector<std::vector<int>> &parent, std::vector<int> &depth) {
    if(depth[A] > depth[B]) {
        std::swap(A, B);
    }

    for(int i = LOG ; i >= 0 ; i--) {
        if(depth[B] >= depth[A] + (1 << i)) {
            B = parent[B][i];
        }
    }

    if(A == B) {
        return A;
    }
    else {
        for(int i = LOG ; i >= 0 ; i--) {
            if(parent[A][i] != parent[B][i]) {
                A = parent[A][i];
                B = parent[B][i];
            }
        }
    }
    return parent[A][0];
}

int LeastCommonAncenstor() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    std::cin >> n;
    std::vector<std::vector<int>> graph(n + 1);
    for(int j = 0 ; j < n - 1 ; j++) {
        int A, B;
        std::cin >> A >> B;
        graph[A].push_back(B);
        graph[B].push_back(A);
    }

    int root = 1;
        
    std::vector<std::vector<int>> parent(n + 1, std::vector<int>(LOG + 1, root));
    std::vector<int> depth(n + 1);
        
    findParent(root, graph, parent, depth);

    int m;
    std::cin >> m;
    for(int i = 0 ; i < m ; i++) {
        int A, B;
        std::cin >> A >> B;
        std::cout << findLCA(A, B, graph, parent, depth) << "\n";
    }    
}

// 1 -> 2 -> 3 -> 1
// 3 -> 4 -> 5
// 5 <-> 6
// 5 <-> 7

// stack parent
//  7     5
//  6     5
//  5     5
//  4     4
//  3     1
//  2     1
//  1     1

std::vector<std::vector<int>> scc;
int id = 0;

std::stack<int> s;
int dfs2(int current, std::vector<std::vector<int>> &graph, std::vector<int> &uniqueNum, std::vector<int> &check) {
    id += 1;
    uniqueNum[current] = id;
    s.push(current);

    int parent = uniqueNum[current];
    for(int i = 0 ; i < graph[current].size() ; i++) {
        int next = graph[current][i];
        if(uniqueNum[next] == 0) {
            parent = std::min(parent, dfs2(next, graph, uniqueNum, check));
        }
        else if(check[next] == 0) {
            parent = std::min(parent, uniqueNum[next]);
        }
    }

    if(parent == uniqueNum[current]) {
        std::vector<int> sccComponent;
        while(1) {
            int t = s.top();
            s.pop();
            sccComponent.push_back(t);
            check[t] = 1;
            if(t == current) {
                break;
            }
        }
        scc.push_back(sccComponent);
    }

    return parent;
}
bool compare3(std::vector<int> a, std::vector<int> b) {
    return a[0] < b[0];
}

int StronglyConnectedComponent() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n + 1);

    for(int i = 0 ; i < m ; i++) {
        int x, y;
        std::cin >> x >> y;
        graph[x].push_back(y);
    }

    std::vector<int> uniqueNum(n + 1, 0);
    std::vector<int> check(n + 1, 0);
    
    for(int i = 1 ; i <= n ; i++) {
        if(uniqueNum[i] == 0) {
            dfs2(i, graph, uniqueNum, check);
        }
    }

    for(int i = 0 ; i < scc.size() ; i++){
        std::sort(scc[i].begin(), scc[i].end());
    }

    std::sort(scc.begin(), scc.end(), compare3);

    std::cout << scc.size() << "\n";
    for(int i = 0 ; i < scc.size() ; i++){
        std::sort(scc[i].begin(), scc[i].end());
        for(int j = 0 ; j < scc[i].size() ; j++){
            std::cout << scc[i][j] << " ";
        }
        std::cout << "-1\n"; 
    }
}

// 세그먼트 트리는 루트가 1로 시작한다.
// parent * 2 를 하면 왼쪽 child, parent * 2 + 1 을 하면 오른쪽 child가 나온다.
// 데이터의 합을 반씩 분할하여 저장한다.
// 0 ~ 11
// 0 ~ 5 / 6 ~ 11
// 0 ~ 2 / 3 ~ 5 / 6 ~ 8 / 9 ~ 11
// 0 ~ 1 / 2 / 3 ~ 4 / 5 / 6 ~ 7 / 8 / 9 ~ 10 / 11
// 0 / 1 / 3 / 4 / 6 / 7 / 9 / 10

long long init(int start, int end, int node, std::vector<long long> &num, std::vector<long long> &tree) {
    if(start == end) {
        return tree[node] = num[start];
    }

    int mid = (start + end) / 2;
    return tree[node] = init(start, mid, node * 2, num, tree) + init(mid + 1, end, node * 2 + 1, num, tree);
}

// left ~ right 까지 sum
// 4 ~ 8 범위 합 = 4 + 5 + 6 ~ 8
// 범위 안에 있는 경우만 더해주면 된다.

long long sum(int start, int end, int node, int left, int right, std::vector<long long> &num, std::vector<long long> &tree) {
    //std::cout << start << " " << end << " " << node << " " << left << " " << right << "\n";
    if(left > end || right < start) {
        return (long long) 0;
    }

    if(left <= start && end <= right) {
        return tree[node];
    }

    int mid = (start + end) / 2;
    return sum(start, mid, node * 2, left, right, num, tree) + sum(mid + 1, end, node * 2 + 1, left, right, num, tree);
}

// 수정하려는 인덱스가 범위 안에 있는 경우 수정.
void update(int start, int end, int node, int index, long long diff, std::vector<long long> &num, std::vector<long long> &tree) {
    if(index < start || end < index) {
        return;
    }

    tree[node] += diff;
    if (start == end) {
        return;
    }
    int mid = (start + end) / 2;
    update(start, mid, 2 * node, index, diff, num, tree);
    update(mid + 1, end, 2 * node + 1, index, diff, num, tree);
}

int SegmentTree() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(NULL);
    std::cin.tie(NULL);

    int n, m, k;
    std::cin >> n >> m >> k;

    std::vector<long long> num(n, 0);
    for(int i = 0 ; i < n ; i++) {
        std::cin >> num[i];
    }

    std::vector<long long> tree(4 * n + 1, 0);
    init(0, n - 1, 1, num, tree);

    for(int i = 0 ; i < m + k ; i++) {
        long long a, b, c;
        std::cin >> a >> b >> c;

        if(a == 1) {
            update(0, n - 1, 1, b - 1, c - num[b - 1], num, tree);
            num[b - 1] = c;

        }
        else if(a == 2) {
            std::cout << sum(0, n - 1, 1, b - 1, c - 1, num, tree) << "\n";
        }
    }
}

// 네트워크 플로우 문제에서 Capicity = 1 인 경우
// 이때는 에드몬드 카프 알고리즘 보다 더 효율적으로 DFS를 사용해서 해결 할 수 있다.
// 1 -> A, B, C / 2 -> A / 3 -> B;
// 1 -> A
// 2 -> A, 1 -> B
// 2 -> A, 3 -> B, 1 -> C

std::vector<std::vector<int>> graph(1001);
std::vector<int> allocation(1001, 0);
std::vector<int> check(1001, 0);

int dfs3(int person) {
   for(int i = 0 ; i < graph[person].size() ; i++) {
       int work = graph[person][i];

       if(check[work] == 0) {
           check[work] = 1;
           if(allocation[work] == 0 || dfs3(allocation[work])) {
               allocation[work] = person;
               return 1;
           }
       }
   }
   return 0;
}

int bipathalMatching() {
    int n, m;
    std::cin >> n >> m;
    //std::vector<std::vector<int>> graph(n + 1);
    //std::vector<int> before(n + 1);
    //std::vector<int> check(m + 1);

    for(int i = 1 ; i <= n ; i++) {
        int num;
        std::cin >> num;
        for(int j = 0 ; j < num ; j++) {
            int k;
            std::cin >> k;
            graph[i].push_back(k);
        }
    }

    int count = 0;
    for(int i = 1 ; i <= n ; i++) {
        std::fill(check.begin(), check.end(), 0);
        if(dfs3(i)) {
            count++;
        }
    }
    std::cout << count << "\n";
}

// 네트워크 플로우는 특정 지점에서 다른 지점으로 데이터가 얼마나 많이 흐르고 있는가를 측정하는 알고리즘이다.
// A - 8 -> B - 6 -> C - 7 -> D
// A -> D로 보낼 수 있는 최대 유량은 6이다.

// 포드 풀커슨(DFS, 실수에서는 사용 불가), 에드몬드 카프(BFS), 디닉 알고리즘 사용.

// s - 2 -> a - 2 -> t
//        - 1 ->
// s - 2 -> b - 2 -> t
// a -> b 로 흐르는 경로는 잘못된 경로이고 이를 보완하기 위해서 음의 유량을 계산한다.
// BFS를 사용해서 모든 가능한 경로를 다 찾은 뒤 남은 용량만큼 유량을 흘려 보내 준다. 이때 역방향으로는 음의 유량을 흘려 보내주면 된다.

int result = 0;
int INF = INT32_MAX - 1;
int num = 400;
std::vector<int> before(num);
std::vector<std::vector<int>> capacity(num, std::vector<int>(num));
std::vector<std::vector<int>> flow(num, std::vector<int>(num));
std::vector<std::vector<int>> graph(num);

void maxFlow(int start, int end) {
    while(1) {
        std::fill(before.begin(), before.end(), -1);
        std::queue<int> q;
        q.push(start);
        while(!q.empty()) {
            int current = q.front();
            q.pop();

            for(int i = 0 ; i < graph[current].size() ; i++) {
                int next = graph[current][i];
                if(before[next] == -1 && capacity[current][next] - flow[current][next] > 0) {
                    before[next] = current;
                    q.push(next);
                    if(next == end) {
                        break;
                    }
                }
            }
        }
        if(before[end] == -1) {
            break;
        }

        int f = INF;
        for(int i = end ; i != start ; i = before[i]) {
            f = std::min(f, capacity[before[i]][i] - flow[before[i]][i]);
        }

        for(int i = end ; i != start ; i = before[i]) {
            flow[before[i]][i] += f;
            flow[i][before[i]] -= f;
        }
        result += f;
    }
}

int networkFlow() {
    int n, m;
    std::cin >> n >> m;

    for(int i = 0 ; i < m ; i++) {
        int x, y;
        std::cin >> x >> y;
        graph[x - 1].push_back(y - 1);
        graph[y - 1].push_back(x - 1);
        capacity[x - 1][y - 1] = 1;
    }

    maxFlow(0, 1);
    std::cout << result << "\n";
}