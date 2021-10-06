#include <iostream>
#include <vector>
// DONE
// TWO POINTER, Knapsack, Kruth's Optimization
// TODO
// .....

int twoPointer() {
    int n;
    unsigned long long m;
    std::cin >> n >> m;

    std::vector<int> array(n);
    for(int i = 0 ; i < n ; i++) {
        std::cin >> array[i];
    }

    int count = 0, sum = 0, left = 0, right = 0;
    while(1) {
        if(right > n) {
            break;
        }

        if(sum == m) {
            count++;
        }
        
        if(m <= sum) {
            sum -= array[left];
            left++;
        }
        else {
            sum += array[right];
            right++;
        }
    }
    
    std::cout << count << "\n";
}

bool compare(std::vector<int> a, std::vector<int> b) {
    return a[0] < b[0];
}

int Knapsack() {
    int n, k;
    std::cin >> n >> k;

    std::vector<int> w(n, 0);
    std::vector<int> v(n, 0);
    for(int i = 0 ; i < n ; i++) {
        std::cin >> w[i] >> v[i];
    }

    std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n, 0));
    // 가방의 크기가 i 일 때, j 번째 물건까지 담을 수 있는 경우 최대 가치

    for(int i = 1 ; i <= k ; i ++) {
        for(int j = 0 ; j < n ; j++) {
            if(i >= w[j]) {
                dp[i][j] = std::max(dp[i][j - 1], dp[i - w[j]][j - 1] + v[j]);
            }
            else{
                dp[i][j] = dp[i][j - 1];
            }
            
        }
    }
    std::cout << dp[k][n - 1] << "\n";
}

int Kruth() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(NULL);
    std::cin.tie(NULL);

    int N;
    std::cin >> N;
    for(int i = 0 ; i < N ; i++) {
        int n;
        std::cin >> n;
        std::vector<int> file(n + 1, 0);
        std::vector<int> sumFile(n + 1, 0);

        for(int i = 1 ; i <= n ; i++) {
            std::cin >> file[i];
            sumFile[i] = sumFile[i - 1] + file[i];
        }        
        /*
            Kruth's Optimization

            1. 최적화 아이디어
            dp[i][j] = min(dp[i][k] + dp[k + 1][j]) + C[i][j] 을 구하기 위해서 모든 k를 비교하면서 min을 찾을 필요가 있을까? (i < k < j)
            (C[i][j] = sumFile[j] - sumFile[i - 1], 즉 i ~ j 까지 file 사이즈의 합)

            2. 정리
            dp[i][j] = min(dp[i][k] + dp[k + 1][j]) + C[i][j], min(dp[i][k] + dp[k + 1][j])을 만족하는 k = A[i][j] 라고 할때

            만약 C[i][j]가 아래와 같은 조건을 만족한다면
            1) 사각 부등식: C[a][c] + C[b][d] <= C[a][b] + C[b][c] (a <= b <= c <= d)
            2) 단조 증가:   C[a][c] <= C[b][d]                     (a <= b <= c <= d)

            A[i][j - 1] <= A[i][j] <= A[i + 1][j] 이다.

            3. 따라서 k를 찾기 위해서 모든 범위를 찾을 필요는 없고 A[i][j - 1] ~ A[i + 1][j]의 범위만 찾아주면 된다.

            dp[i][j] = (i + 1 부터 j 까지의 합 비용의 최소)
                     = min(dp[i][k] + dp[k + 1][j]) + C[i][j] (i < k < j)
        */
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));
        std::vector<std::vector<int>> A(n + 1, std::vector<int>(n + 1, 0));
        for(int i = 0 ; i < n ; i++) {
            A[i][i + 1] = i + 1; 
        }

        for(int s = 2 ; s <= n ; s++) {
            for(int i = 0 ; i + s <= n ; i++) {
                int j = i + s;
                int sumMin = INT32_MAX - 1;
                for(int k = A[i][j - 1] ; k <= A[i + 1][j] ; k++) {
                    int candid =  dp[i][k] + dp[k][j];
                    if(sumMin > candid) {
                        sumMin = candid;
                        A[i][j] = k;
                    }
                }
                dp[i][j] = sumMin + sumFile[j] - sumFile[i];
            }
        }
        std::cout << dp[0][n] << "\n";
    }
}
int n;
std::vector<std::vector<int>> p2w(20, std::vector<int>(20, 0));
std::vector<std::vector<int>> dp(20, std::vector<int>(1 << 20, -1)); 
// i - 1 번째 사람까지 일이 할당 되었고, 그 일의 할당 배열은 j와 같다.
// j의 k 번째 비트가 1이면 k 번째 일이 할당 된 것이다.

int dfs(int x, int allocated) {
    if(allocated == ((1 << n) - 1)) {
        // 모든 일이 할당 됨.
        return 0;
    }
    if(dp[x][allocated] == -1) {
        dp[x][allocated] = INT32_MAX;
        for(int i = 0 ; i < n ; i++) {
            if((allocated & (1 << i)) == 0) {
                dp[x][allocated] = std::min(dp[x][allocated], dfs(x + 1, (allocated | (1 << i))) + p2w[x][i]);
            }
        }
    }
    return dp[x][allocated];
}

int bitSet() {
    std::cin >> n;    
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < n ; j++) {
            std::cin >> p2w[i][j];
        }
    }
    std::cout << dfs(0, 0) << "\n";
}

int panlinedrom() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n;
    std::cin >> n;
    std::vector<int> a(n, 0);
    for(int i = 0 ; i < n ; i++) {
        std::cin >> a[i];
    }

    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    // dp[i][j] = i 부터 j 까지 펠린드롬인지 아닌지
    // dp[i][j] = dp[i + 1][j - 1] && a[i] == a[j]

    for(int i = 0 ; i < n ; i++) {
        dp[i][i] = 1;
        if(i != n - 1) {
            if(a[i] == a[i + 1]) {
                dp[i][i + 1] = 1;
            }
        }
    }

    for(int i = n - 1 ; i >= 0 ; i--) {
        for(int j = i + 2 ; j < n ; j++) {
            if(a[i] == a[j] && dp[i + 1][j - 1] == 1) {
                dp[i][j] = 1;
            }
        }
    }
    int m;
    std::cin >> m;
    for(int i = 0 ; i < m ; i++) {
        int a, b;
        std::cin >> a >> b;
        std::cout << dp[a - 1][b - 1] << "\n";
    }

}

/*
1. Convex Hull Op
dp[i] = Min(dp[j] + A[i] * B[j]) (j < i)

2. Divide and Conquer Op
dp[i][j] = Min(dp[i - 1][k] + C[k][j]) (k < j)

3. Monotone Queue Op
dp[i] = Min(dp[j] + C[j][i]) (j < i)

4. Knuth's Op
dp[i][j] = Min(dp[i][k] + dp[k + 1][j] + C[i][j]) (i <= k < j)

5. Aliens Trick
dp[i][j] = Min(dp[i - 1][k] + C[k + 1][j]) (k < j)

6. Slope Trick

7. Hirschburg's Algorithm
dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + C[i][j] (LCS)

8. Circular LCS
BitSet 활용

9. Dynamic Tree DP
세그먼트 트리
dp[i][j] = min(dp[i - 1][k] + C[i][k][j]) for all k
*/
