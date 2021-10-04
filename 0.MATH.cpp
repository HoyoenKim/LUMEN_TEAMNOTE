#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
// DONE
// GCD, 분할 정복을 이용한 거듭제곱 (수, 행렬), 고속 푸리에 변환(FFT)
// TODO
// 뤼카의 정리, 폴라드 로 알고리즘, 확장 유클리드 알고리즘

int gcd(int a, int b) {
    if(a % b == 0) {
        return b;
    }
    else {
        return gcd(b, a % b);
    }
}
int A, B, C;
int multiplication(int x, int n) {
    if(n == 0) {
        return x;
    }
    else{
        int square = ((unsigned long long) x * (unsigned long long) x) % C;
        return multiplication(square, n - 1);
    }
}
int NumberPow() {    
    std::cin >> A >> B >> C;
    int m = std::log(B) / std::log(2);
    unsigned long long ret = 1;
    for(int i = m ; i >= 0 ; i--) {
        if( (B & ((unsigned long long)1 << i)) == ((unsigned long long)1 << i)) {
            //std::cout << i << "\n";
            ret *= (unsigned long long) multiplication(A, i);
            ret %= C;
        }
    }
    std::cout << ret << "\n";
}
int n;
std::vector<std::vector<int>> matrixMultiplication(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B) {
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < n ; j++) {
            int sum = 0;
            for(int k = 0 ; k < n ; k++) {
                sum += (A[i][k] * B[k][j]);
            }
            C[i][j] = sum;
        }
    }
    return C;
}
int MatrixPow() {
    unsigned long long B;
    std::cin >> n >> B;
    std::vector<std::vector<int>> A(n, std::vector<int>(n, 0));
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < n ; j++) {
            std::cin >> A[i][j];
        }
    }
    int m = std::log(B) / std::log(2);
    std::vector<std::vector<std::vector<int>>> MT (m + 1, std::vector<std::vector<int>>(n, std::vector<int>(n, 0)));
    for(int i = 0 ; i <= m ; i++) {
        if(i == 0) {
            MT[i] = A;
        }
        else {
            MT[i] = matrixMultiplication(MT[i - 1], MT[i - 1]);
        }
    }
    std::vector<std::vector<int>> ret(n, std::vector<int>(n, 0));
    for(int i = 0 ; i < n ; i++) {
        for(int j = 0 ; j < n ; j++) {
            if(i == j) {
                ret[i][j] = 1;
            }
        }
    }
    for(int i = 0 ; i <= m ; i++) {
        if( (B & ((unsigned long long)1 << i)) == ((unsigned long long)1 << i) ) {
            ret = matrixMultiplication(ret, MT[i]);
        }
    }
}
const long double PI = acos(-1.L);
unsigned long long bitconvert(unsigned long long  n, unsigned long long  k) {
    unsigned long long  ret = 0;
    for(unsigned long long  i = 0 ; i < k ; ++i) {
        ret |= ( (n >> i) & (unsigned long long) 1 ) << (k - i - (unsigned long long) 1); 
    }
    return ret;
}
void fft(std::vector<std::complex<long double>>&a, bool is_reverse = false) {
    // 0 1 2 3 4 5 6 7
    // 0 2 4 6 1 3 5 7
    // 0 4 2 6 1 5 3 7
    unsigned long long n = a.size();
    unsigned long long k = __builtin_ctz(n);
    for(unsigned long long i = 0 ; i < n ; i++) {
        unsigned long long j = bitconvert(i, k);
        if(i < j) {
            std::complex<long double> temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    // 0 1 2 3 4 5 6 7
    // 0 2 4 6 / 1 3 5 7
    // 0 4 / 2 6 / 1 5 / 3 7
    // a[n] = aeven[n] + w[n] * aodd[n]
    // a[n + N/2] = aeven[n] - w[n] * odd[n]
    // w[n] = exp(2 pi/ s )
    for(unsigned long long s = 2 ; s <= n ; s *= 2) {
        std::vector<std::complex<long double>> w(s/2);
        for(int i = 0 ; i < s/2 ; i++) {
            long double t = (long double)2 * PI * i / (long double)s;
            if(is_reverse) {
                t *= (long double) -1;
            }
            w[i] = std::complex<long double> (cos(t), sin(t));
        }
        for(unsigned long long i = 0 ; i < n ; i += s) {
            for(unsigned long long j = 0 ; j < s/2 ; j++) {
                std::complex<long double> odd = a[i + j + s/2];
                std::complex<long double> even = a[i + j];
                a[i + j] = even + w[j] * odd;
                a[i + j + s/2] = even - w[j] * odd;
            }
        }
    }
    if(is_reverse) {
        for(unsigned long long  i = 0 ; i < n ; i++) {
            a[i] /= (long double) n;
        }
    }
}
void convolution (std::vector<std::complex<long double>> &a, std::vector<std::complex<long double>> &b) {
    fft(a);
    fft(b);
    for(unsigned long long i = 0 ; i < a.size() ; i++) {
        a[i] *= b[i];
    }
    fft(a, true);
}