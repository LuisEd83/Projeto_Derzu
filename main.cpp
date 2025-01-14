#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

class mercadoria{
    public:
        string nome_produto;
        float valor;
            mercadoria(){
                nome_produto = nullptr;
                valor = 0.0;
            }
};

class cliente{
    public:
        string nome_cliente;
        vector<mercadoria> itens;
            cliente(){
                nome_cliente = nullptr;
                for(int i=0; i< itens.size(); i++){
                    itens.push_back(mercadoria());
                }
            }
            int indice(vector<cliente> compradores, const string nome_search){
                int indice = -1;

                for(int i=0; i<compradores.size(); i++){
                    if(compradores[i].nome_cliente == nome_search ){
                        indice = i;
                    }
                }

                return indice;
            }

};

int main(){
    vector<cliente> consumidor;


    return 0;
}