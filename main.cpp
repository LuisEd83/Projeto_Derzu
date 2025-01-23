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
                    if((validade[i].day - dia <= 0) && (validade[i].mon - mes <= 0) && (validade[i].year - ano <= 0) || ((validade[i].year - ano <= 0))){
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
    for(int i = 0; i<itens[indice].validade.size(); i++){
    nova_mercadoria.validade.push_back({itens[indice].validade[i].day, 
                                itens[indice].validade[i].mon, 
                                itens[indice].validade[i].year});
    }

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
                cout << "Este item não possui unidades o suficiente para criar um pacote!" << endl;
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

bool verificador_arquivo(const string nome_arq){
    string line;
    fstream arq(nome_arq,fstream::in);

    if(arq>>line){
        arq.close();
        return true;
    }else{
        arq.close();
        return false;
    }
}//Esta função analisa se o arquivo está vazio ou não

void copiararq(const string& arq_origin, const string& arq_dest){
    string line;

    ifstream arq_O(arq_origin, ios::in);
    if(!arq_O.is_open()){
        cerr << "O arquivo: "<< arq_origin << " não pode ser aberto!" << endl;
        arq_O.close();
        return;
    }

    ofstream arq_D(arq_dest, ios::out);
    if(!arq_D.is_open() || (verificador_arquivo(arq_dest) == false)){
        cerr << "O arquivo: "<< arq_dest << " não pode ser aberto ou se encontra vazio!" << endl;
        arq_D.close();
        return;
    }

    while(getline(arq_O,line)){
        arq_D << line << endl;
    }
    
    arq_O.close(); arq_D.close();

    cout << "Transferência de dados realizado com sucesso!" << endl;
    return;
}

void verfic_venc(vector<mercadoria>& itens_p_verific){
    string busca = " (vencido)";
    for(int i = 0; i<itens_p_verific.size(); i++){
        size_t posicao = itens_p_verific[i].nome_produto.find(busca);
            for(int j = 0; j<itens_p_verific[i].validade.size(); j++){
                if((itens_p_verific[i].comparador_data(itens_p_verific[i].validade) == true) || (posicao != string::npos)){
                    cout << "A mercadoria: " << itens_p_verific[i].nome_produto << " Apresenta produto com validade vencida." << endl;
                }
            }
            if(posicao == string::npos && (itens_p_verific[i].comparador_data(itens_p_verific[i].validade) == true)){
                itens_p_verific[i].nome_produto += busca;
            }
        }
}

void menu(){
    vector<cliente> consumidor, consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    fstream arq_Cliente, arq_Cliente_beckup, arq_mercadoria, arq_mercadoria_beckup;

    if((verificador_arquivo("Clientes.txt") == false) && (verificador_arquivo("Mercadorias.txt") == false)){
        cout << "Seja bem-vindo!" << endl;
    }else{
        cout << "Seja bem-vindo novamente!" << endl;
        if(verificador_arquivo("Mercadorias.txt") == false){
            cout << "O arquivo Mercadorias.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            copiararq("Mercadoria_beckup.txt", "Mercadorias.txt");
            arq_mercadoria.open("Mercadorias.txt", fstream::in);
            if(verificador_arquivo("Mercadorias.txt") == true){
                string nome_p_prod;
                int num_quant;
                int quant_pacot;
                float preco;
                vector<tdata> dat_ven;
                int tam = dat_ven.size();
                while(arq_mercadoria >> nome_p_prod >> num_quant >> quant_pacot >> preco >> tam){
                    dat_ven.clear();

                    for(int i = 0; i<tam; i++){
                        tdata data;
                        arq_mercadoria >> data.day >> data.mon >> data.year;
                        dat_ven.push_back(data);
                    }
                    merc_CO.emplace_back(nome_p_prod, num_quant, quant_pacot, preco, dat_ven);
                }
                verfic_venc(merc_CO);
            }
            arq_mercadoria.close();
        }else{
            string nome_p_prod;
            int num_quant;
            int quant_pacot;
            float preco;
            vector<tdata> dat_ven;
            int tam = dat_ven.size();
            while(arq_mercadoria >> nome_p_prod >> num_quant >> quant_pacot >> preco >> tam){
                dat_ven.clear();

                for(int i = 0; i<tam; i++){
                    tdata data;
                    arq_mercadoria >> data.day >> data.mon >> data.year;
                    dat_ven.push_back(data);
                }
                merc_CO.emplace_back(nome_p_prod, num_quant, quant_pacot, preco, dat_ven);
            }
            verfic_venc(merc_CO);
        }
    }
}

int main(){

    menu();

    return 0;
}