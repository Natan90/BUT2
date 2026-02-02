#include <iostream>

class C {
public:
    friend std::ostream &operator<<(std::ostream &os, const C &c);
};

void display_c(const C &c) {
    std::cout << c << std::endl;
}

void display_int(int v) {
    std::cout << v << std::endl;
}

void set_c(C *c) {
    *c = C{};
}

void set_int(int *i) {
    *i = 42;
}

int get_int(int i) {
    return i + 1;
}

C *make_c() {
    return new C{};
}

int main() {
    C *c1 = make_c();
    C c2{};

    display_c(*c1);
    display_c(c2);

    int i1 = 55;
    int *i2 = new int(25);

    display_int(i1);
    display_int(*i2);

    set_int(&i1);
    set_int(i2);

    std::cout << get_int(i1) << std::endl;
    std::cout << get_int(*i2) << std::endl;

    set_c(c1);
    set_c(&c2);

    delete c1;
    delete i2;

    return 0;
}

std::ostream &operator<<(std::ostream &os, const C &c) {
    os << "I'm a C";
    return os;
}
