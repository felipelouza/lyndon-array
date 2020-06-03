#include "random.hpp"

/*******************************************************************/
int random(unsigned char* str, size_t n, int verbose){


  map<char,char> A;

  for(int i=0; i<n-1;i++){
    A[str[i]] = str[i];
  }

  vector<pair<char,bool>> V;
  for(auto it=A.begin(); it!=A.end(); it++){
    V.push_back({it->first, false});
  }

  int sigma = V.size();
  
  std::random_device rd;
  for(auto it=A.begin(); it!=A.end(); it++){

    int p=rd()%sigma;
    while(V[p].second){
      p = rd()%sigma;
    }

    V[p].second=true;
    A[it->first] = V[p].first;

  }

  if(verbose){
    for(auto it=A.begin(); it!=A.end(); it++){
      std::cout<<it->first<<": "<<it->second<<endl;
    }
  }

  for(int i=0; i<n-1;i++){
    str[i] = A[str[i]];
  }

return 0;
}

/*******************************************************************/
