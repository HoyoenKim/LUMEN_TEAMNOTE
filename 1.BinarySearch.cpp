#include <iostream>
#include <vector>
// DONE
// LIS
int LeastIncreaseSubstring() {
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::vector<int> array(n);
    for(int i = 0 ; i < n ; i++) {
        std::cin >> array[i];
    }
    std::vector<int> answer(n, 1);
    std::vector<int> xArray;
    std::vector<int> xAnswer;
    int num = -1;
    for(int i = 0 ; i < n ; i++) {
        if(i == 0) {
            xArray.push_back(array[i]);
            xAnswer.push_back(answer[i]);
            num ++;
        }
        else {
            if(xArray[num] < array[i]) {
                xArray.push_back(array[i]);
                answer[i] = xAnswer[num] + 1;
                xAnswer.push_back(answer[i]);
                num++;
            }
            else {
                int changeIndex = std::lower_bound(xArray.begin(), xArray.end(), array[i]) - xArray.begin();
                answer[i] = xAnswer[changeIndex];
                xArray[changeIndex] = array[i];
            }
        }
    }
    std::cout << xAnswer[num] << "\n";
}