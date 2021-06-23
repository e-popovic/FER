#include <bits/stdc++.h> 
#include <vector>
#include <algorithm>
#include <utility> 
using namespace std;

/* rjesi simplex */
int solve(vector<vector<double>> &tableau, vector<double> &solution, double &obj){
    //napravi tablicu od tableau 
    int columns = tableau[0].size();
    int rows = tableau.size();
    int i, j, pivotCol, pivotRow = -1;

    for (i=0; i<columns; i++){  // maksimizacija!
        tableau[0][i] = tableau[0][i] * -1; 
    }

    cout << "START:\n";
    for (i = 0; i<rows;i++){
        for (j=0;j<columns;j++){
            cout<< tableau[i][j] << "  ";
        }     
        cout<<endl;
    }
    cout<< "-----------------------\n";

    //iteriraj do se ne dobije optimum
    int loop = 0;
    double mostNegative;
    bool nasaoPrviNeg = false;
    while(1){
        loop++;
        pivotCol = -1;
        pivotRow = -1;
        nasaoPrviNeg = false;
        // pronadi stupac za pivot (iz nultog reda -> funkcije cilja)
        for (i=0; i<columns-1; i++){
            if ( pivotCol < 0 && (tableau[0][i] < 0) ){
                pivotCol = i;
               nasaoPrviNeg = true;
            }
            else if (tableau[0][i] < tableau[0][pivotCol] && nasaoPrviNeg) {
                pivotCol = i;
            }                
        }
        if ( tableau[0][pivotCol] >= 0){    //ako nema negativnog broja doslo je do optimuma, izadi iz whilea
            break;
        }

        // pronadi element(red) za pivot u stupcu pivotCol
        double najmanjiOmjer = DBL_MAX, omjer;
        int indexOmjer=0;
        for (i=1; i<rows; i++) {    // izmjeri sve epsilone
            if (tableau[i][pivotCol] > 0){ // ako je nazivnik pozitivan prihvatljiv je
                omjer = tableau[i][columns-1] / tableau[i][pivotCol];
                if (omjer >= 0){  //ako ulazi u obzir                 
                    if(pivotRow < 0 || omjer < najmanjiOmjer){    //ako je prvi ili manji od postojeceg najmanjeg = pivotRow
                        pivotRow = i;
                        najmanjiOmjer = omjer;
                    }
                }
            }
        }   
        if (pivotRow < 0){   // ako nijedan omjer ne dolazi u obzir vrati 1 - neograniceni problem
            return 1;
        }

        // pivotaj i napravi potrebne promjene u ostalim stupcima
        double koef = tableau[pivotRow][pivotCol];
        for (i=0; i<columns; i++) { // sredi pivot red
            tableau[pivotRow][i] /= koef;
        }
        for(i=0; i<rows; i++) { // sredi ostale redove
            if (i == pivotRow) {
                continue;
            }
            koef = -1 * tableau[i][pivotCol];
            for(j=0; j<columns; j++) {
                tableau[i][j] += koef * tableau[pivotRow][j];
            }
        }

        for (i = 0; i<rows;i++){
            for (j=0;j<columns;j++){
                cout<< tableau[i][j] << "  ";
            }     
            cout<<endl;
        }
        cout<< "-----------------------\n";
    }

    // upisi dobiveni rezultat
    int zerosNum, onesNum, index = 0;
    for (i=0; i<columns-1; i++){   
        zerosNum = 0;  
        onesNum = 0;
        for (j=0; j<rows; j++) {
            if (tableau[j][i] == 0) {
                zerosNum++;
            }  
            else if (tableau[j][i] == 1){
                onesNum++;
                index = j;
            }
        }
        if (onesNum == 1 && zerosNum == (rows-1)) {
            solution.push_back(tableau[index][columns-1]);
        }
        else {
            solution.push_back(0);
        }
    }
    obj = tableau[0].back();    //optimizirana funkcija cilja   
    return 0;
}


int main() { 
    double obj = 0.0;
    vector<double> solution;
    vector<vector<double>> tableau{
        {1.0, -2.0, 3.0, 0.0, 0.0, 0.0, 0.0, },
        {0.0, 1.0, 0.2, 1.0, 0.2, 0.0, 3.4, },
        {1.0, 0.0, -0.2, 0.0, -0.2, 0.0, 1.6, },
        {0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 5.0, }, 
    };
  
    int i = solve(tableau,solution,obj);
    if (i == 0) {
        cout << "Optimalna tocka: ";
        for (double num: solution)
            cout << num << "  ";
        cout << endl;
        cout << "Fja cilja u opt: " << obj;
    }
    return 0; 
}
