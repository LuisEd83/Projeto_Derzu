#include <iostream>
#include <vector>
#include <unistd.h>
#include <windows.h>
#include "Menu.h"

using namespace std;

int main(){
    SetConsoleOutputCP(65001);

    Key Senha_geral;
    Senha_geral.setSenha("_INDEFINIDA_");

    menu(Senha_geral);
    sleep(2); system("cls");
    cout << "Alterações salvas!" << endl;
    return 0;
}