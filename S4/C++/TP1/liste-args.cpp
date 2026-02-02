#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Nombre d'arguments : " << argc - 1 << endl;
    
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
    }
    
    return 0;
}
