#include<iostream>
#include<cstdlib>
#include<random>
#include<time.h>
// nombre par defaut
#define NBR 100

// Point cardinaut
#define N "N"
#define NE "NE"
#define E "E"
#define SE "SE"
#define S "S"
#define SO "SO"
#define O "O"
#define NO "NO"

// il nous faut un plateau de m*n cases avec de l'herbe sur chaque cases et le nbr des mouton et des loups sont parametrable par le joueur

// il nous faut un loup qui mange un mouton sur le plateau le loup peut mourire de viellesse se déplace "pour le moment aléatoirement"

// fin de partis quand il n'y a plus de mouton et de loup

using namespace std;

class Vivant{
    protected:
    int row;
    int col;
    int pv;
    int pv_max;
    char type;

    public:
    void deplacement(char const *direction) {
        if (direction == N){
            col += 1;
        }
        else if (direction == NE){
            col += 1;
            row += 1;
        }
        else if (direction == NO){
            col += 1;
            row -= 1;
        }
        else if (direction == S){
            col -= 1;
        }
        else if (direction == SE){
            col -= 1;
            row += 1;
        }
        else if (direction == SO){
            col -= 1;
            row -= 1;
        }
        else if (direction == E){
            row += 1;
        }
        else if (direction == O){
            row -= 1;
        }
    }
};


class Mouton : public Vivant {
    public:
    Mouton(int row, int col){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::row = row;
        Vivant::col = col;
        Vivant::type = 'M';
    }

    char getType(){return type;}
};

class Loup : public Vivant {
    public:
    Loup(int row, int col){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::row = row;
        Vivant::col = col;
        Vivant::type = 'L';
    }
    char getType(){return type;}
};


class Nonvivant{
    protected:
    int jour;
    int row;
    int col;
    char type;
    public:
    Nonvivant(int jour, int row, int col):jour(jour), row(row), col(col) {
        if (jour == 0)
        {
            type = 'H';
        }
        else if (jour == 1){
            type = 'S';
        }
    }

    void transformation(){
        if (type == 'S' && jour <= 0){
            type = 'H';
        }
    }
};


class Case{
    protected:
    int row;
    int col;
    char type_vi;
    char type_nvi;
    Vivant* vi;
    Nonvivant* nvi;
    public:
    Case() : row(0), col(0), vi(nullptr), nvi(nullptr) {}
    int getRow(){return row;}
    int getCol(){return col;}
    char getTypeVi(){return type_vi;}
    char getTypeNvi(){return type_nvi;}
    Vivant* getVivant(){return vi;}
    Nonvivant* getNonvivant(){return nvi;}
    void setRow(int newRow){row = newRow;}
    void setCol(int newCol){col = newCol;}
    void setTypeVi(char newType){type_vi = newType;}
    void setTypeNvi(char newType){type_nvi = newType;}
    void setVi(Vivant* newVi){vi = newVi;}
    void setNvi(Nonvivant* newNvi){nvi = newNvi;}
};


















void print_plateau(int m, int n, int nbr_tours, int nbr_moutons, int nbr_loups, Case tab[][NBR]);
void initialisation(Case tab[][NBR], int col, int row, int nbr_loups, int nbr_moutons);
void tours(Case tab[][NBR], int row, int col, Mouton moutons[NBR], Loup loups[NBR]);

int main(){
    srand(time(NULL));

    int col = 10;
    int row = 10;
    int nbr_tours = 0;
    int nbr_moutons = 20;
    int nbr_loups = 10;
    Case tab[NBR][NBR];
    initialisation(tab, col, row, nbr_loups, nbr_moutons);
    // print_plateau(col, row, tab);

    print_plateau(col, row, nbr_tours, nbr_moutons, nbr_loups, tab);

    bool game = true;
    while(game == true){
        
    }
    return 0;
}




















Mouton* genMouton(int nbr_moutons, int row, int col, Case tab[][NBR]){
    Mouton* tabmouton[nbr_moutons];
    for(int m = 0; m < nbr_moutons; m++) {
        int i = rand() % row; 
        int j = rand() % col;
        while(tab[i][j].getTypeNvi() != 'H' || tab[i][j].getTypeVi() == 'M') {
            i = rand() % row; 
            j = rand() % col;
        }
        tabmouton[m] = new Mouton(i, j);
        tab[i][j].setTypeVi(tabmouton[m]->getType());
    }
    return *tabmouton;
}

Loup* genLoup(int nbr_Loups, int row, int col, Case tab[][NBR]){
    Loup* tabLoup[nbr_Loups];
    for(int l = 0; l < nbr_Loups; l++) {
        int i = rand() % row; 
        int j = rand() % col;
        while(tab[i][j].getTypeNvi() != 'H' || tab[i][j].getTypeVi() == 'L') {
            i = rand() % row; 
            j = rand() % col;
        }
        tabLoup[l] = new Loup(i, j);
        tab[i][j].setTypeVi(tabLoup[l]->getType());
    }
    return *tabLoup;
}


void print_plateau(int m, int n, int nbr_tours, int nbr_moutons, int nbr_loups, Case tab[][NBR]){
    int temp = 0;
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < m; i++)
        {
            cout << "+---";
        }
        cout << "+" << endl;
        for (int i = 0; i < (4*m)+1; i++)
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
        for (int i = 0; i < (4*m)+1; i++)
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
    for (int i = 0; i < m; i++)
    {
        cout << "+---";
    }
    cout << "+" << endl;

    cout << "\nTours : " << nbr_tours << " | " << "Loups : " << nbr_loups << " | " << "Moutons : " << nbr_moutons << endl; 
}

void initialisation(Case tab[][NBR], int col, int row, int nbr_loups, int nbr_moutons){
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {   
            tab[i][j].setRow(j);
            tab[i][j].setCol(i);
            tab[i][j].setTypeNvi('H');
            tab[i][j].setTypeVi(' ');
        }
    }
    genMouton(nbr_moutons, row, col, tab);
    genLoup(nbr_loups, row, col, tab);
}

void tours(Case tab[][NBR], int row, int col, Mouton moutons[NBR], Loup loups[NBR]){
    // for(int a = 0; a < NBR; a++) {
        // moutons[a].choisir_action();
        // loups[a].choisir_action();
        
        // faire une surcharge avec chaque objet la exemple avec les moutons
        // tab[moutons[a].getRow()][moutons[a].getCol()].update_tab(moutons); 
    // }
}