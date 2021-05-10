#include <vector>
#include <algorithm>
#include <utility> 
#include <bits/stdc++.h> 
using namespace std;
typedef unsigned int uint;

/* fja trazi optimalni put po polju problem
*  izlaz = kolicina pregazenog cvijeca
*/
uint solve(const uint N, const uint M, uint16_t *const problem, 
    vector<pair<uint, uint>> &solution){
	//alociraj memoriju za array najmanje kolicine cvijeca do svakog polja livade
    uint **rezultat;
    rezultat = new uint*[N];
    for(int i=0;i<N;i++){
        rezultat[i]=new uint[M];
    }

    //zig-zag put kroz livadu
    for (int i = 0; i < N + M - 1; ++i){
        //spusta se dolje lijevo
        if (i%2) {
            int x = (i < M ? 0 : i - M + 1);
            int y = (i < M ? i : M - 1);
            while (x < N && y >= 0) {

                //usporedi dal je bolje doc od gornjeg il lijevog
                if (x > 0 && y > 0){
                    rezultat[x][y] = *(problem + x*M + y) + (rezultat[x-1][y] > rezultat[x][y-1]? rezultat[x][y-1] : rezultat[x-1][y]);
                }
                else if (x==0){ //prvi red
                    rezultat[x][y] = *(problem + x*M + y) + rezultat[x][y-1];
                }
                else{   //prvi stupac
                    rezultat[x][y] = *(problem + x*M + y) + rezultat[x-1][y];
                }

                ++x;
                --y;

            }
        }

        //penje se gore desno
        else {
            int x = (i < N? i : N - 1);
            int y = (i < N? 0 : i - N + 1);
            while (x >= 0 && y < M){

                //usporedi dal je bolje doc od gornjeg il lijevog
                if (x > 0 && y > 0){
                    rezultat[x][y] = *(problem + x*M + y) + (rezultat[x-1][y] > rezultat[x][y-1]? rezultat[x][y-1] : rezultat[x-1][y]);
                }
                else if (x==0){ //prvi red
                    rezultat[x][y] = *(problem + x*M + y) + (y ? rezultat[x][y-1] : 0);
                }
                else{   //prvi stupac
                    rezultat[x][y] = *(problem + x*M + y) + rezultat[x-1][y];
                }
                --x;
                ++y;
            }
        }
    }

    //definiraj najkraci put, kreni od kraja i trazi put do pocetka 
    int x = N - 1;
    int y = M - 1;
    while(x>0 || y>0){
        solution.push_back( make_pair(x,y) ); 
        if (y){
            if ( rezultat[x][y] - *(problem + x*M + y) == rezultat[x][y-1]){
                --y;
            }
            else{
                --x;
            }
        }
        else{
            --x;
        }        
    }
    solution.push_back( make_pair(0,0) );
    reverse(solution.begin(),solution.end());

    //vrati minimalno ukupno cvijece - vrijednost polja skroz dolje desno
    uint cvijece = rezultat[N-1][M-1];

   //oslobodi memoriju arraya
    for(int i=0;i<N;i++){
        delete []rezultat[i];
    }
    delete []rezultat;

    return cvijece;
}