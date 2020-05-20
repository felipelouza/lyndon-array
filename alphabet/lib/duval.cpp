#include "duval.hpp"

/*******************************************************************/
//adapted from https://cp-algorithms.com/string/lyndon_factorization.html
vector<size_t> duval(unsigned char* str, size_t n){

    size_t i = 0;
    vector<size_t> P;
    while (i < n) {
        size_t j = i + 1, k = i;
        while (j < n && str[k] <= str[j]) {
            if (str[k] < str[j]){
                k = i;
            }
            else{
                k++;
            }
            j++;
        }
        while (i <= k) {
            i += j - k;
            P.push_back(i);
        }
    }
    return P;
}

/*******************************************************************/
