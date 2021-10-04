#include <iostream>
#include <vector>
// DONE
// TWO POINTER, Kruth's Optimization,
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