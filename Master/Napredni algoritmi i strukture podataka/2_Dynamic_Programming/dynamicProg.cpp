#include <vector>
#include <algorithm>
#include <utility> 
#include <bits/stdc++.h> 
using namespace std;

/* fja trazi optimalni put po polju problem
*  izlaz = kolicina pregazenog cvijeca
*/
unsigned int solve(const unsigned int N, const unsigned int M, uint16_t *const problem, 
    vector<pair<unsigned int, unsigned int>> &solution){
	//alociraj memoriju za array najmanje kolicine cvijeca do svakog polja livade
    unsigned int **rezultat;
    rezultat = new unsigned int*[N];
    for(int i=0;i<N;i++){
        rezultat[i]=new unsigned int[M];
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
    unsigned int cvijece = rezultat[N-1][M-1];

   //oslobodi memoriju arraya
    for(int i=0;i<N;i++){
        delete []rezultat[i];
    }
    delete []rezultat;

    return cvijece;
}


int main() { 

    //uint16_t polje[] = {1, 7, 9, 2, 8, 6, 6, 7, 1, 3, 2, 8};

    uint16_t polje[] = {100, 200, 1000, 0,
                        200, 100,  600, 0,
                        300, 1600, 100, 0};
    uint16_t *ptr;
    ptr = polje;
    vector<pair<unsigned int, unsigned int>> solution;

    uint16_t brCvijeca = solve(3, 4, ptr, solution);

    cout << "Kolicina cvijeca: " << brCvijeca;
    cout << "\n";

    for(int i=0; i<solution.size(); ++i){
        cout << "(" << solution[i].first << ", " << solution[i].second << ")" << endl;
    }
  
    return 0; 
} 