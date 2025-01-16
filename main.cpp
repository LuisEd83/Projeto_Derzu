#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace chrono;

template <class T>
void reinicia_vector(vector<T>& nome_vector){
    while(!nome_vector.empty()){
        nome_vector.pop_back();
    }
}//Basicamente, esta função reinicia um vetor, fazendo ele retornar a ter tamanho 1

typedef struct{
    int day;
    int mon;
    int year;
}tdata;

class mercadoria{
    public:
        string nome_produto;
        int quantidade;
        float valor;
        tdata validade;
            mercadoria(){
                nome_produto = nullptr;
                quantidade = 0;
                valor = 0.0;
                validade = {0,0,0};
            }
            bool comparador_data(tdata validade){
                auto now = system_clock::now();
                time_t time_now = system_clock::to_time_t(now);
                tm* local_time = localtime(&time_now);
                int dia = local_time->tm_mday;
                int mes = local_time->tm_mon + 1;    
                int ano = local_time->tm_year + 1900;

                if((validade.day - dia == 0) && (validade.mon - mes == 0) && (validade.year - ano == 0)){
                    return true;
                }else{
                    return false;
                }
            }
            bool separador(vector<mercadoria>& itens, int separacao, int indice, int quant_x_separacao){
                
                if (indice < 0 || indice >= itens.size()) {
                    cerr << "Índice inválido." << endl;
                    return false;
                }

                mercadoria nova_mercadoria;
                nova_mercadoria.nome_produto = itens[indice].nome_produto + " unidade";
                nova_mercadoria.quantidade = quant_x_separacao * separacao;
                nova_mercadoria.valor = (itens[indice].valor / separacao) * 1.2;
                nova_mercadoria.validade = {itens[indice].validade.day, 
                                            itens[indice].validade.mon, 
                                            itens[indice].validade.year};

                for(int i = 0; i<itens.size(); i++){
                    if(i==indice){
                        if(itens[i].quantidade - 1 < 0){
                            return false;
                        }else{
                            itens.insert(itens.begin() + indice-1, nova_mercadoria);
                            return true;
                        }
                    }
                }

            }
};

class cliente{
    public:
        string nome_cliente;
        vector<mercadoria> itens;
            cliente(){
                nome_cliente = nullptr;
                reinicia_vector(itens);
                itens[0] = mercadoria();
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

    for(int i = 0; i<itens_Co.size(); i++){
        compra += itens_Co[i].quantidade * itens_Co[i].valor;
    }
    for(int i = 0; i<itens_Ve.size(); i++){
        venda += itens_Ve[i].quantidade * itens_Ve[i].valor;;
    }
    
    return venda - compra;
}

int main(){
    vector<cliente> consumidor;

    return 0;
}