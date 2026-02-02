#include <iostream>

using namespace std;

int* make_odd() {
    int* impair = new int(15);
    return impair;
}

int* make_even() {
    int* pair = new int(12);
    return pair;
}

int main() {
    int* (*f_ptr)() = nullptr;

    int entree;
    cout << "Entrez 0 pour pair, 1 pour impair : ";
    cin >> entree;

    if (entree == 0) {
        f_ptr = make_even;
    } else {
        f_ptr = make_odd;
    }

    int* resultat = f_ptr();

    cout << "Résultat : " << *resultat << endl;

    delete resultat;

    return 0;
}