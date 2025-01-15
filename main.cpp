#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace chrono;

template <class T>
void reinicia_vector(vector<T> nome_vector){
    for(int i = nome_vector.size(); i>0; i--){
        nome_vector.erase(nome_vector.end());
    }
}//Basicamente, esta função reinicia um vetor, fazendo ele retornar a ter tamanho 1

class mercadoria{
    public:
        string nome_produto;
        int quantidade;
        float valor;
            mercadoria(){
                nome_produto = nullptr;
                quantidade = 0;
                valor = 0.0;
            }
};

class cliente{
    public:
        string nome_cliente;
        vector<mercadoria> itens;
            cliente(){
                nome_cliente = nullptr;
                reinicia_vector(itens);
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