#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace chrono;

class mercadoria{
    public:
        string nome_produto;
        int quantidade;
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
            int indice(vector<cliente> compradores, string nome_search){
                int indice = -1;

                for(int i=0; i<compradores.size(); i++){
                    if(compradores[i].nome_cliente == nome_search ){
                        indice = i;
                    }
                }

                return indice;
            }//retorna a posição do cliente no vector

};

float receita(vector<mercadoria> itens_Co, vector<mercadoria> itens_Ve){
    float venda = 0.0, compra = 0.0;

    for(int i=0; i<itens_Co.size()+itens_Ve.size(); i++){
        compra += itens_Co[i].quantidade * itens_Co[i].valor;
        venda += itens_Ve[i].quantidade * itens_Ve[i].valor;
    }
    
    return venda - compra;
}

int main(){
    vector<cliente> consumidor;

    return 0;
}