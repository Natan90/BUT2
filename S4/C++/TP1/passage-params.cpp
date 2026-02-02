#include <iostream>

using namespace std;

void swap1(int a, int b) {
    int c = a;
    a = b;
    b = c;
}

void swap2(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void swap3(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}

int main() {
    int x = 10, y = 20;

    cout << "Valeurs initiales : x = " << x << ", y = " << y << endl;

    swap1(x, y);
    cout << "Après swap1 (valeur) : x = " << x << ", y = " << y << endl;

    swap2(&x, &y);
    cout << "Après swap2 (pointeur) : x = " << x << ", y = " << y << endl;

    x = 10; y = 20;

    swap3(x, y);
    cout << "Après swap3 (référence) : x = " << x << ", y = " << y << endl;

    return 0;
}
