////
//// Created by Naor_Shlomo on 04/04/2022.
////

#include <iostream>

using std::cout; using std::cin;
using std::endl; using std::string;

int main() {
//    char tmp[256];
    char *tmp;
    tmp = getenv("PATH");
    cout << "Current "<< endl << tmp << endl;

    return EXIT_SUCCESS;
}