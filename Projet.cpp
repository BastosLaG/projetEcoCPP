#include<iostream>
#include<cstdlib>
#include<random>
#include<time.h>
// nombre par defaut
#define NBR 100

// Point cardinaut
#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SO 5
#define O 6
#define NO 7

static int _row;
static int _col;

// il nous faut un plateau de m*n cases avec de l'herbe sur chaque cases et le nbr des mouton et des loups sont parametrable par le joueur

// il nous faut un loup qui mange un mouton sur le plateau le loup peut mourire de viellesse se déplace "pour le moment aléatoirement"

// fin de partis quand il n'y a plus de mouton et de loup

using namespace std;

class Case;
class Vivant;
class Mouton;
class Loup;
class NonVivant;


class Case{
    protected:
    int row;
    int col;
    char type_vi;
    char type_nvi;
    Vivant* vi;
    NonVivant* nvi;
    public:
    Case() : row(0), col(0), vi(nullptr), nvi(nullptr) {}
    int getRow(){return row;}
    int getCol(){return col;}
    char getTypeVi(){return type_vi;}
    char getTypeNvi(){return type_nvi;}
    Vivant* getVivant(){return vi;}
    NonVivant* getNonVivant(){return nvi;}
    void setRow(int newRow){row = newRow;}
    void setCol(int newCol){col = newCol;}
    void setTypeVi(char newType){type_vi = newType;}
    void setTypeNvi(char newType){type_nvi = newType;}
    void setVi(Vivant* newVi){vi = newVi;}
    void setNvi(NonVivant* newNvi){nvi = newNvi;}
};
static Case tab[NBR][NBR];

class Vivant : public Case{
    protected:
    int row;
    int col;
    int pv;
    int pv_max;
    char type;

    public:
    void deplacement(int direction) {
        int new_row = row;
        int new_col = col;
        if (direction == N){
            new_row--;
        }
        else if (direction == NE){
            new_row--;
            new_col++;
        }
        else if (direction == E){
            new_col++;
        }
        else if (direction == SE){
            new_row++;
            new_col++;
        }
        else if (direction == S){
            new_row++;
        }
        else if (direction == SO){
            new_row++;
            new_col--;
        }
        else if (direction == O){
            new_col--;
        }
        else if (direction == NO){
            new_row--;
            new_col--;
        }
        // Vérification des limites de la grille
        if (new_row >= 0 && new_row < NBR && new_col >= 0 && new_col < NBR) {
            // Déplacement si la case est libre
            if (tab[new_row][new_col].getVivant() == nullptr) {
                tab[row][col].setVi(nullptr);
                tab[new_row][new_col].setVi(this);
                row = new_row;
                col = new_col;
            }
        }
    }
};


class Mouton : public Vivant {
    protected:
    char type;
    public:
    Mouton(){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        type = 'M';
    }
    Mouton(int row, int col){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::row = row;
        Vivant::col = col;
        type = 'M';
    }
    void setCol(int col) {col = col;}
    void setRow(int row) {row = row;}
    int getCol() {return Vivant::col;}
    int getRow() {return Vivant::row;}
    char getType(){return type;}
    void deplacement(){
        int temp = rand() % 8;
        if (temp == 0)
        {
            Vivant::deplacement(N);
        }
        else if (temp == 1){
            Vivant::deplacement(NE);
        }
        else if (temp == 2){
            Vivant::deplacement(NO);
        }
        else if (temp == 3){
            Vivant::deplacement(O);
        }
        else if (temp == 4){
            Vivant::deplacement(E);
        }
        else if (temp == 5){
            Vivant::deplacement(SE);
        }
        else if (temp == 6){
            Vivant::deplacement(SO);
        }
        else if (temp == 7){
            Vivant::deplacement(S);
        }
    }
};



class Loup : public Vivant {
    protected:
    char type;
    public:
    Loup(){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        type = 'L';
    }
    Loup(int row, int col){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::row = row;
        Vivant::col = col;
        type = 'L';
    }
    void setCol(int col) {col = col;}
    void setRow(int row) {row = row;}
    int getCol() {return col;}
    int getRow() {return row;}
    char getType(){return type;}
};


class NonVivant : public Case{
    protected:
    int jour;
    int row;
    int col;
    char type;
    public:
    NonVivant(int jour, int row, int col):jour(jour), row(row), col(col) {
        if (jour <= 0)
        {
            type = 'H';
        }
        else if (jour == 1){
            type = 'S';
        }
    }

    void transformation(){
        jour--;
        if (type == 'S' && jour <= 0){
            type = 'H';
        }
    }
};




















void initialisation();
void print_plateau(int nbr_tours, int nbr_moutons, int nbr_loups);
void genMouton(Mouton* tabmouton[NBR], int nbr_moutons);
void genLoup(Loup* tabloup[NBR], int nbr_Loups);
void tourMouton(Mouton* moutons[NBR], int nbr_moutons);

int main(){
    srand(time(NULL));

    _col = 10;
    _row = 10;
    int nbr_tours = 0;
    int nbr_moutons = 20;
    int nbr_loups = 10;
    
    Mouton* tabMouton[NBR];
    Loup* tabloup[NBR];

    initialisation();
    genMouton(tabMouton, nbr_moutons);
    genLoup(tabloup, nbr_loups);

    print_plateau(nbr_tours, nbr_moutons, nbr_loups);

    tourMouton(tabMouton, nbr_moutons);

    print_plateau(nbr_tours, nbr_moutons, nbr_loups);
    /*
    bool game = true;
    while(game == true){
        char user;
        cout << "\ny/n\n";
        cin >> user;
        if(user == 'n'){
            game = false;
        }

        if (nbr_loups == 0 && nbr_moutons == 0){
            game = false;
        }
        
    }
    */
    return 0;
}















void genMouton(Mouton* tabmouton[NBR], int nbr_moutons){
    for(int m = 0; m < nbr_moutons; m++) {
        // spawn aleatoire 
        int i = rand() % _row; 
        int j = rand() % _col;
        // si est sur un animal choisir un autre spawn
        while(tab[i][j].getTypeVi() == 'M' || tab[i][j].getTypeVi() == 'L') {
            i = rand() % _row; 
            j = rand() % _col;
        }
        // setter de mouton
        tabmouton[m] = new Mouton(i, j);
        // change se qu'il y a dans la case
        tab[i][j].setTypeVi(tabmouton[m]->getType());
        tab[i][j].setVi(tabmouton[m]);
    }
}

void genLoup(Loup* tabloup[NBR], int nbr_Loups){
    for(int l = 0; l < nbr_Loups; l++) {
        // spawn aleatoire 
        int i = rand() % _row; 
        int j = rand() % _col;
        // si est sur un animal choisir un autre spawn
        while(tab[i][j].getTypeVi() == 'L' || tab[i][j].getTypeVi() == 'M') {
            i = rand() % _row; 
            j = rand() % _col;
        }
        // setter de mouton
        tabloup[l] = new Loup(i, j);
        // change se qu'il y a dans la case
        tab[i][j].setTypeVi(tabloup[l]->getType());
        tab[i][j].setVi(tabloup[l]);
    }
}

void tourMouton(Mouton* moutons[NBR], int nbr_moutons){
    for(int a = 0; a < nbr_moutons; a++) {
        moutons[a]->deplacement();
    }
}

void print_plateau(int nbr_tours, int nbr_moutons, int nbr_loups){
    int temp = 0;
    for (int j = 0; j < _row; j++)
    {
        for (int i = 0; i < _col; i++)
        {
            cout << "+---";
        }
        cout << "+" << endl;
        for (int i = 0; i < (4*_row)+1; i++)
        {    
            if (i%4 == 0)
            {
                cout << "|";
            }
            else if(i%2 == 0){
                if ( tab[j][temp].getTypeVi() == 'M'){
                    cout << "\033[0m" << tab[j][temp].getTypeVi() << "\033[0m";
                }
                else if ( tab[j][temp].getTypeVi() == 'L'){
                    cout << "\033[31m" << tab[j][temp].getTypeVi() << "\033[0m";
                }
                else{
                    cout << tab[j][temp].getTypeVi();
                }
                temp++;
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
        temp = 0;
        for (int i = 0; i < (4*_row)+1; i++)
        {    
            if (i%4 == 0)
            {
                cout << "|";
            }
            else if(i%2 == 0){
                if ( tab[j][temp].getTypeNvi() == 'H'){
                    cout << "\033[32m" << tab[j][temp].getTypeNvi() << "\033[0m";
                }
                temp++;
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
        temp = 0;
    }
    for (int i = 0; i < _row; i++)
    {
        cout << "+---";
    }
    cout << "+" << endl;

    cout << "\nTours : " << nbr_tours << " | " << "Loups : " << nbr_loups << " | " << "Moutons : " << nbr_moutons << endl; 
}

void initialisation(){
    for (int i = 0; i < _col; i++)
    {
        for (int j = 0; j < _row; j++)
        {   
            tab[i][j].setRow(j);
            tab[i][j].setCol(i);
            tab[i][j].setTypeNvi('H');
            tab[i][j].setTypeVi(' ');
        }
    }
}

