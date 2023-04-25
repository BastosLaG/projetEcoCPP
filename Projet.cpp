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
static int _nbr_moutons = 20;
static int _nbr_loups = 20;

// il nous faut un plateau de m*n cases avec de l'herbe sur chaque cases et le nbr des mouton et des loups sont parametrable par le joueur

// il nous faut un loup qui mange un mouton sur le plateau le loup peut mourire de viellesse se déplace "pour le moment aléatoirement"

// fin de partis quand il n'y a plus de mouton et de loup

using namespace std;

class Case;
class Vivant;
class Mouton;
class Loup;
class NonVivant;


static Mouton* tabMouton[NBR];
static Loup* tabloup[NBR];


////////////////////////////////////////////////////////////////////////////////



class Case{
    protected:
    int row;
    int col;
    char type_vi;
    char type_nvi;
    int nb_moutons; 

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

////////////////////////////////////////////////////////////////////////////////

class NonVivant : public Case{
    protected:
    int jour;
    int row;
    int col;
    char type;
    public:
    NonVivant(int row, int col):row(row), col(col) {
        jour = 1;
        type = 'S';
    }
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

////////////////////////////////////////////////////////////////////////////////

class Vivant : public Case{
    protected:
    int row;
    int col;
    int rang;
    int pv_max; // a donner a un agneau
    int pv; // longévité
    int faim_max; // faim
    int faim; // faim
    int sexe;
    char type;



    public:
    int getRow() {return row;}
    int getCol() {return col;}
    int getRang(){return rang;}
    int getPvMax(){return pv_max;}
    int getPv(){return pv;}
    int getFaim(){return faim;}
    int getFaimMax(){return faim_max;}
    int getSexe(){return sexe;}
    char getType(){return type;}

    void setRow(int new_row) {row = new_row;}
    void setCol(int new_col) {col = new_col;}
    void setRang(int new_rang) {rang = new_rang;}
    void setPv(int new_pv) {pv = new_pv;}
    void setFaim(int faim_restor){faim = faim_restor;}
    void setSexe(int new_sexe){sexe = new_sexe;}
    void setType(char new_type) {type = new_type;}


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
        if (new_row >= 0 && new_row < _row && new_col >= 0 && new_col < _col) {
            // Déplacement si la case est libre
            if (tab[new_row][new_col].getVivant() == nullptr) {
                if (getType() == 'L'){
                    cout << "\033[31mdéplacement de [" << row << "][" << col << "] vers " << "[" << new_row << "][" << new_col << "]\033[0m" << endl;
                }
                else{
                    cout << "déplacement de [" << row << "][" << col << "] vers " << "[" << new_row << "][" << new_col << "]" << endl;
                }
                tab[row][col].setVi(nullptr);
                tab[row][col].setTypeVi(' ');
                tab[new_row][new_col].setVi(this);
                tab[new_row][new_col].setTypeVi(getType());
                row = new_row;
                col = new_col;
            }
        }
    }
    virtual void ken() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class Mouton : public Vivant {
    public:
    Mouton(int row, int col, int rang, int sexe){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::faim_max = 5;
        Vivant::faim = 5;
        Vivant::row = row;
        Vivant::col = col;
        Vivant::rang = rang;
        Vivant::sexe = sexe;
        Vivant::type = 'M';
    }
    void deplacement(){
        int temp = rand() % 8;
        Vivant::deplacement(temp);
    }
    void mangerHerbe() {
        if (tab[row][col].getTypeNvi() == 'H') {
            tab[row][col].setNvi(nullptr);
            tab[row][col].setTypeNvi(' ');
            cout << "Un mouton [" << row << "][" << col << "] a mangé" << endl;
            Vivant::setFaim(Vivant::getFaimMax());
        }
    }
    int lavidaloca(Mouton* tabu[NBR]){
        pv--;
        faim--;
        if (faim == 0)
        {
            cout << "Un mouton est mort de faim [" << row << "][" << col << "]" << endl;
            tab[row][col].setVi(nullptr);
            tab[row][col].setTypeVi(' ');
            tab[row][col].setTypeNvi('S');
            for (int i = rang; i < _nbr_moutons-1; i++)
            {
                tabu[i] = tabu[i+1];
                tabu[i]->setRang(i);
            }
            _nbr_moutons--;
            return 0;
        }
        else if (pv == 0){
            cout << "Un mouton est mort de vieilliesse [" << row << "][" << col << "]" << endl;
            tab[row][col].setVi(nullptr);
            tab[row][col].setTypeVi(' ');
            tab[row][col].setTypeNvi('S');
            for (int i = rang; i < _nbr_moutons-1; i++)
            {
                tabu[i] = tabu[i+1];
                tabu[i]->setRang(i);
            }
            _nbr_moutons--;
            return 0;
        }
        return 1;
    }
    void ken(){
        if (sexe == 0){
            // on fait des bb
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < _row && j >= 0 && j < _col && tab[i][j].getTypeVi() == getType() && tab[i][j].getVivant()->getSexe() != getSexe()){
                        // Le loup a trouver son ame soeur
                        for(int k=row-1 ; k<row+1; k++){
                            for(int a= col-1; a<col+1; a++){
                                for(int b= tab[i][j].getVivant()->getRow()-1; b<tab[i][j].getVivant()->getRow()+1; b++){
                                    for (int o = tab[i][j].getVivant()->getCol()-1; o < tab[i][j].getVivant()->getCol()+1; o++){
                                        // On est sur la meme case
                                        if (k == b && a == o){
                                            // Est vide ?
                                            if (tab[k][a].getTypeVi() == ' '){
                                                // creer BB
                                                tabMouton[_nbr_moutons] = new Mouton(k,a,_nbr_moutons, rand()%2);
                                                tab[k][a].setVi(tabMouton[_nbr_moutons]);
                                                tab[k][a].setTypeVi('M');
                                                _nbr_moutons++;
                                                cout << "\033[36mUn Mouton vient de naître [" << k << "][" << a << "]" << "\033[0m" << endl;
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////

class Loup : public Vivant {
    public:
    Loup(int row, int col, int rang, int sexe){
        Vivant::pv_max = 10;
        Vivant::pv = pv_max;
        Vivant::faim_max = 5;
        Vivant::faim = 5;
        Vivant::row = row;
        Vivant::col = col;
        Vivant::rang = rang;
        Vivant::sexe = sexe;
        Vivant::type = 'L';
    }
    void deplacement(){
        int temp = rand() % 8;
        Vivant::deplacement(temp);
    }
    void mangerMouton() {
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i >= 0 && i < _row && j >= 0 && j < _col && tab[i][j].getTypeVi() == 'M') {
                    // Le loup mange le mouton
                    Vivant::setFaim(Vivant::getFaimMax());
                    for (int k = tab[i][j].getVivant()->getRang(); k < _nbr_moutons-1; k++)
                    {
                        tabMouton[k] = tabMouton[k+1];
                        tabMouton[k]->setRang(k);
                    }
                    tab[this->getRow()][this->getCol()].setVi(nullptr);
                    tab[this->getRow()][this->getCol()].setTypeVi(' ');
                    tab[i][j].setVi(this);
                    tab[i][j].setTypeVi(this->getType());
                    this->setRow(i);
                    this->setCol(j);
                    _nbr_moutons--;
                    cout << "\033[31m" << "Un Loup [" << row << "][" << col << "] a mangé\033[0m" << endl;
                    return;
                }
            }
        }
    }
    int lavidaloca(Loup* tabu[NBR]){
        pv--;
        faim--;
        if (faim == 0)
        {
            cout << "\033[31mUn Loup est mort de faim [" << row << "][" << col << "]\033[0m" << endl;
            tab[row][col].setVi(nullptr);
            tab[row][col].setTypeVi(' ');
            tab[row][col].setTypeNvi('S');
            for (int i = rang; i < _nbr_loups-1; i++)
            {
                tabu[i] = tabu[i+1];
                tabu[i]->setRang(i);
            }
            _nbr_loups--;
            return 0;
        }
        else if (pv == 0){
            cout << "\033[31mUn loup est mort de vieilliesse [" << row << "][" << col << "]\033[0m" << endl;
            tab[row][col].setVi(nullptr);
            tab[row][col].setTypeVi(' ');
            tab[row][col].setTypeNvi('S');
            for (int i = rang; i < _nbr_loups-1; i++)
            {
                tabu[i] = tabu[i+1];
                tabu[i]->setRang(i);
            }
            _nbr_loups--;
            return 0;
        }
        return 1;
    }
    void ken(){
        if (sexe == 0){
            // on fait des bb
            cout << "Je veux ken\n";
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < _row && j >= 0 && j < _col && tab[i][j].getTypeVi() == getType() && tab[i][j].getVivant()->getSexe() != getSexe()){
                        // on fait des bb
                        cout << "On veux ken\n";
                        for (int i = row - 1; i <= row + 1; i++) {
                            for (int j = col - 1; j <= col + 1; j++) {
                                if (i >= 0 && i < _row && j >= 0 && j < _col && tab[i][j].getTypeVi() == getType() && tab[i][j].getVivant()->getSexe() != getSexe()){
                                    cout << "On cherche ou accoucher\n";
                                    // trouver une case vide pour le spawn du BB
                                    for(int k=row-1 ; k<row+1; k++){
                                        for(int a= col-1; a<col+1; a++){
                                            for(int b= tab[i][j].getVivant()->getRow()-1; b<tab[i][j].getVivant()->getRow()+1; b++){
                                                for (int o = tab[i][j].getVivant()->getCol()-1; o < tab[i][j].getVivant()->getCol()+1; o++){
                                                    // On est sur la meme case
                                                    if (k == b && a == o){
                                                        cout << "Ca serait bien ici \n";
                                                        // Est vide ?
                                                        if (tab[k][a].getTypeVi() == ' '){
                                                            // creer BB
                                                            tabloup[_nbr_loups] = new Loup(k,a,_nbr_loups, rand()%2);
                                                            tab[k][a].setVi(tabloup[_nbr_loups]);
                                                            tab[k][a].setTypeVi('L');
                                                            _nbr_loups++;

                                                            cout << "\033[33mLoup 1["<< row << "][" << col << "]" << "Loup 2["<< tab[i][j].getVivant()->getRow() << "][" << tab[i][j].getVivant()->getCol() << "]" <<"Un loup vient de naître [" << k << "][" << a << "]" << "\033[0m" << endl;
                                                            return;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////

void initialisation();
void print_plateau(int nbr_tours, int _nbr_moutons, int _nbr_loups);
void genMouton(Mouton* tabmouton[NBR], int _nbr_moutons);
void genLoup(Loup* tabloup[NBR], int _nbr_loups);
void tourMouton(Mouton* moutons[NBR], int _nbr_moutons);
void tourLoup(Loup* loups[NBR], int _nbr_loups);

////////////////////////////////////////////////////////////////////////////////

int main(){
    srand(time(NULL));

    _col = 10;
    _row = 10;
    int nbr_tours = 0;

    initialisation();
    genMouton(tabMouton, _nbr_moutons);
    genLoup(tabloup, _nbr_loups);

    print_plateau(nbr_tours, _nbr_moutons, _nbr_loups);

    bool game = true;
    while(game == true){
        char user;
        cout << "\ny/n\n";
        cin >> user;
        if(user == 'n'){
            game = false;
            break;
        }

        //remet de l'herbe sale chien
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _col; j++){
                if (tab[i][j].getTypeNvi() == 'S'){
                    tab[i][j].setTypeNvi('H');
                }
            }
        }
        
        tourMouton(tabMouton, _nbr_moutons);
        tourLoup(tabloup, _nbr_loups);
        nbr_tours++; 
        print_plateau(nbr_tours, _nbr_moutons, _nbr_loups);
        if (_nbr_loups == 0 && _nbr_moutons == 0){
            game = false;
        }
        
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void genMouton(Mouton* tabmouton[NBR], int __nbr_moutons){
    int leS;
    for(int m = 0; m < __nbr_moutons; m++) {
        // spawn aleatoire 
        int i = rand() % _row; 
        int j = rand() % _col;
        // si est sur un animal choisir un autre spawn
        while(tab[i][j].getTypeVi() == 'M' || tab[i][j].getTypeVi() == 'L') {
            i = rand() % _row; 
            j = rand() % _col;
        }
        // setter de mouton
        if (leS == 0){
            tabmouton[m] = new Mouton(i, j, m, leS);
            leS = 1;
        }
        else{
            tabmouton[m] = new Mouton(i, j, m, leS);
            leS = 0;
        }
        // change se qu'il y a dans la case
        tab[i][j].setTypeVi(tabmouton[m]->getType());
        tab[i][j].setVi(tabmouton[m]);
    
    }
}

void genLoup(Loup* tabloup[NBR], int _nbr_loups){
    int leS;
    for(int l = 0; l < _nbr_loups; l++) {
        // spawn aleatoire 
        int i = rand() % _row; 
        int j = rand() % _col;
        // si est sur un animal choisir un autre spawn
        while(tab[i][j].getTypeVi() == 'L' || tab[i][j].getTypeVi() == 'M') {
            i = rand() % _row; 
            j = rand() % _col;
        }
        // setter de Loup
        if (leS == 0){
            tabloup[l] = new Loup(i, j, l, leS);
            leS = 1;
        }
        else{
            tabloup[l] = new Loup(i, j, l, leS);
            leS = 0;
        }
        // change se qu'il y a dans la case
        tab[i][j].setTypeVi(tabloup[l]->getType());
        tab[i][j].setVi(tabloup[l]);
    }
}

////////////////////////////////////////////////////////////////////////////////

void tourMouton(Mouton* moutons[NBR], int _nbr_moutons){
    int temp = 0;
    for(int a = 0; a < _nbr_moutons; a++) {
        temp = rand() % 3;
        if (moutons[a]->lavidaloca(tabMouton) == 0){
            continue;
        }
        else{
            if(temp == 0) {
                moutons[a]->deplacement();
            }
            else if (temp == 1){
                moutons[a]->mangerHerbe();
            }
            else if (temp == 2){
                moutons[a]->ken();
            }  
        }
    }
}
void tourLoup(Loup* loups[NBR], int _nbr_loups){
    int temp = 0;
    for(int a = 0; a < _nbr_loups; a++) {
        temp = rand() % 3;
        if (loups[a]->lavidaloca(loups) == 0){
            continue;
        }
        else{
            if(temp == 0) {
                loups[a]->deplacement();
            }
            else if (temp == 1){
                loups[a]->mangerMouton();
            }
            else if (temp == 2){
                loups[a]->ken();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

void print_plateau(int nbr_tours, int __nbr_moutons, int _nbr_loups){
    int temp = 0;
    for (int j = 0; j < _row; j++)
    {
        // print ligne de separation de row
        for (int i = 0; i < _col; i++)
        {
            cout << "+---";
        }
        cout << "+" << endl;
        // print des cases de type Vivant
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
                else if ( tab[j][temp].getTypeVi() == ' '){
                    cout << ' ';
                }
                temp++;
            }
            else{
                cout << " ";
            }
        }
        cout << endl;
        temp = 0;
        // print des cases de type NonVivant
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
                else if (tab[j][temp].getTypeNvi() == 'S'){
                    cout << "\033[35m" << tab[j][temp].getTypeNvi() << "\033[0m";
                }
                else if(tab[j][temp].getTypeNvi() == ' '){
                    cout << tab[j][temp].getTypeNvi();
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

    // print derniere ligne
    for (int i = 0; i < _row; i++)
    {
        cout << "+---";
    }
    cout << "+" << endl;

    // print scorboards 
    cout << "\nTours : " << nbr_tours << " | " << "Loups : " << _nbr_loups << " | " << "Moutons : " << _nbr_moutons << endl; 
}