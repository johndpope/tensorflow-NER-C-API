#ifndef __VITERBI_DECODE_H__
#define __VITERBI_DECODE_H__
#include<vector>
#include<iostream>
#include "viterbi_decode.h"
//logits L*T
//trans T*T
//state L*T
//back L*T
//best
//output L
using namespace std;

void viterbi_decode(vector<vector<double> > *logits,vector<vector<double> > *trans,vector<int> *output)
{
     double best=-10000.0;
     int L=logits->size();
     int T=(*logits)[0].size();
     vector<vector<double> > state(L);
     for (int i = 0; i < state.size(); i++)
         state[i].resize(T);
     vector<vector<int> > back(L);
     for (int i = 0; i < back.size(); i++)
         back[i].resize(T);
         
      
     for(int t=0;t<T;t++){
        state[0][t]=(*logits)[0][t];
     }
     for(int i=1;i<L;i++){
        for(int t=0;t<T;t++){
            best=-10000.0;
             for (int pt=0;pt<T;pt++){
             double s=state[i-1][pt]+(*trans)[pt][t];
             if(s>best){
               best=s;
                back[i][t]=pt;   
              }
             }
            state[i][t]=best+(*logits)[i][t];
        }    
      }
     
     for(int t=0;t<T;t++){
       double s=state[L-1][t];
       if(s>best){
          best=s;
          (*output)[L-1]=t;
        }
     }
     
     for(int i=L-2;i>=0;i--){
     (*output)[i]=back[i+1][(*output)[i+1]];
    }
        
}
#endif
/*
int main()
{
     vector<vector<double> > logits(3);
    for (int i = 0; i < logits.size(); i++)
         logits[i].resize(5);
    for(int i = 0; i < logits.size(); i++)
    {
    for (int j = 0; j < logits[0].size();j++)
    {
        logits[i][j] =(double) (i+1)*(j+1);
    }
    } 
     
     vector<vector<double> > trans(5);
     for (int i = 0; i < trans.size(); i++)
         trans[i].resize(5);
        for(int i = 0; i < trans.size(); i++)
    {
    for (int j = 0; j < trans[0].size();j++)
    {
        trans[i][j] = (double)(i+2)*(j+2);
    }
    }
    
    vector<int> output(3);
    viterbi_decode(&logits,&trans,&output);
    for (int i=0;i<3;i++){
     cout<<output[i]<<endl;
}
}*/
