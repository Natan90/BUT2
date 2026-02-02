#include <iostream>
#include <vector>
using namespace std;
int main() {
    vector<int> v{1, 2, 3, 4};
    try
    {
            cout << v.at(0) << " " << v.at(4) << endl;
            return 0;
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << e.what() << '\n';
    }
}
    

