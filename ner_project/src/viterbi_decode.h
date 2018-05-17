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

void viterbi_decode(vector<vector<double> > &logits,vector<vector<double> > &trans,vector<int> &output)
{
    double best=-10000.0;
    int L=logits.size();
    int T=logits[0].size();
    vector<vector<double> > state(L);
    for (int i = 0; i < state.size(); i++)
        state[i].resize(T);
    vector<vector<int> > back(L);
    for (int i = 0; i < back.size(); i++)
        back[i].resize(T);
    for(int t=0;t<T;t++)
        state[0][t]=logits[0][t];
    for(int i=1;i<L;i++){
        for(int t=0;t<T;t++){
            best=-10000.0;
            for (int pt=0;pt<T;pt++){
                double s=state[i-1][pt]+trans[pt][t];
                if(s>best){
                    best=s;
                    back[i][t]=pt;   
                }
            }  
            state[i][t]=best+logits[i][t];
        }    
    }

    for(int t=0;t<T;t++){
        double s=state[L-1][t];
        if(s>best){
            best=s;
            output[L-1]=t;
        }
    }

    for(int i=L-2;i>=0;i--)
        output[i]=back[i+1][output[i+1]];
}
#endif

