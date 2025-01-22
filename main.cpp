#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <iomanip>

using namespace std;
using namespace chrono;

template <class T>
void reinicia_vector(vector<T>& nome_vector){
    while(!nome_vector.empty()){
        nome_vector.pop_back();
    }
}//Basicamente, esta função reinicia um vetor, fazendo ele retornar a ter tamanho 0

typedef struct{
    int day;
    int mon;
    int year;
}tdata;

class mercadoria{
    public:
        string nome_produto;
        int quantidade;
        int unidade_pacote;
        float valor;
        vector<tdata> validade;
            mercadoria(){
                nome_produto = nullptr;
                quantidade = 0;
                valor = 0.0;
                unidade_pacote = 0;
                validade.push_back({0,0,0});
            }
            bool comparador_data(vector<tdata> validade){
                auto now = system_clock::now();
                time_t time_now = system_clock::to_time_t(now);
                tm* local_time = localtime(&time_now);
                int dia = local_time->tm_mday;
                int mes = local_time->tm_mon + 1;    
                int ano = local_time->tm_year + 1900;

                for(int i = 0; i<validade.size(); i++){
                    if(((validade[i].day - dia == 0) && (validade[i].mon - mes == 0) && (validade[i].year - ano == 0)) || (validade[i].year - ano < 0)){
                        return true;
                    }else{
                        return false;
                    }
                }

            }
            int indice(vector<mercadoria> itens, string nome_search){
                int indice = -1;

                for(int i=0; i<itens.size(); i++){
                    if(itens[i].nome_produto == nome_search ){
                        indice = i;
                    }
                }

                return indice;
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

bool separador(vector<mercadoria>& itens, int separacao, int indice, int quant_x_separacao){
                
    if (indice < 0 || indice >= itens.size()) {
        cerr << "Índice inválido." << endl;
        return false;
    }

    mercadoria nova_mercadoria;
    nova_mercadoria.nome_produto = itens[indice].nome_produto + " unidade";
    nova_mercadoria.quantidade = quant_x_separacao * separacao;
    nova_mercadoria.unidade_pacote = 1;
    nova_mercadoria.valor = (itens[indice].valor / separacao) * 1.2;
    nova_mercadoria.validade.push_back({itens[indice].validade[0].day, 
                                itens[indice].validade[0].mon, 
                                itens[indice].validade[0].year});

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
}//Separa o item original em unidades

bool agrupador(vector<mercadoria>& itens, int indice){
    string busca = " unidade";
    size_t posicao = itens[indice].nome_produto.find(busca);
    string nome_provisorio;

    if (indice < 0 || indice >= itens.size() || (posicao == string::npos)) {
        cout << "Índice inválido ou esta mercadoria não é uma unidade." << endl;
        return false;
    }else{
        nome_provisorio = itens[indice].nome_produto.erase(posicao, sizeof(busca));
    }

    for(int i = 0; i<itens.size(); i++){
        if((itens[i].nome_produto == nome_provisorio) && itens[indice].unidade_pacote - itens[i].unidade_pacote > 0){
            itens[i].quantidade++; itens[indice].quantidade -= itens[i].unidade_pacote;
            for(int j = 0; j<itens[indice].validade.size(); j++){
                itens[i].validade.push_back(itens[indice].validade[j]);
            }
            cout << "Agrupamento realizado!" << endl;
            return true;
        }else{
            if((itens[i].nome_produto == nome_provisorio) && (itens[indice].unidade_pacote - itens[i].unidade_pacote == 0)){
                itens[i].quantidade++;
                for(int j = 0; j<itens[indice].validade.size(); j++){
                itens[i].validade.push_back(itens[indice].validade[j]);
                }
                itens.erase(itens.begin()+indice-1);
                cout << "Agrupamento realizado!" << endl;
                return true;
            }else{
                cout << "Este item não possui unidades o suficiente para criar um pacote" << endl;
                return false;
            }
        }
    }
}//Agrupa unidades em um unico item.

float conta (vector<cliente> comprador, string nome_search){
    float valor_conta = 0.0;

    for (int i = 0; i < comprador.size(); i++){
        if(comprador[i].nome_cliente == nome_search){
            for(int j = 0; j < (comprador[i].itens).size(); j++){
                valor_conta = valor_conta + comprador[i].itens[j].valor;
            }
        }
    }

    return valor_conta;
}//Retorna o que o cliente deve.

float receita(vector<mercadoria> itens_Co, vector<mercadoria> itens_Ve){
    float venda = 0.0, compra = 0.0;

    for(int i = 0; i<itens_Co.size(); i++){
        compra += itens_Co[i].quantidade * itens_Co[i].valor;
    }
    for(int i = 0; i<itens_Ve.size(); i++){
        venda += itens_Ve[i].quantidade * itens_Ve[i].valor;;
    }
    
    return venda - compra;
}//Retorna o lucro (positivo ou não).

void menu(){
    vector<cliente> consumidor, consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    
}

int main(){

    return 0;
}