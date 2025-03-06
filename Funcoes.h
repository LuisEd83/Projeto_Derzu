#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include "Classes.h"

using namespace std;
using namespace chrono;

template <class T>
int escolha(vector<T> Nome_vector, string Nome_p){
    vector<int> indices;
    int n = 0, opcao = -1;

    for(size_t i = 0; i<Nome_vector.size(); i++){
        size_t p = Nome_vector[i].getNome().find(Nome_p);
        if(p != string::npos){
            n++;
            indices.push_back(i);
            cout << n << Nome_vector[i].getNome() << endl;
        }
    }

    if(indices.empty()){return -1;}

    n-=1;
    do{
        cout << "Escolha o nome desejado: "; cin >> opcao;
        opcao -= 1;
        if(opcao < 0 || opcao > n){
            cout << "Opção inválida, tente novamente." << endl;
        }
    }while(opcao < 0 || opcao > n);
    return indices[opcao];
}

void verif_dataPG(vector<proletariado> workers){
    for(size_t i = 0; i<workers.size(); i++){
        if(workers[i].Pagamento(workers[i].getpay_day())){
            cout << "O dia do pagamento do salário do funcioário " << workers[i].getNome() << "chegou ou está atrasado!" << endl;
        }
    }
}

bool desagrupador(vector<mercadoria>& itens, size_t indice, int quant_x_separacao, tdata data_prod){
    mercadoria nova_mercadoria;
    int ind = -1;
    if (indice >= itens.size()) {
        cerr << "Índice inválido." << endl;
        return false;
    }
    
    for(size_t i = 0; i<itens[indice].getValid().size(); i++){
        if((itens[indice].getValid()[i].day - data_prod.day) + (itens[indice].getValid()[i].mon - data_prod.mon) + (itens[indice].getValid()[i].year - data_prod.year) == 0){
            ind = i;
        }
    }
    if(ind == -1){
        cout << "Não foi possivel separar." << endl; return false;
    }

    if(itens[indice].getQuant()[ind] - quant_x_separacao < 0){
        cout << "Não foi possivel separar." << endl; return false;
    }else{
        nova_mercadoria.setN_prod(itens[indice].getNome() + " unidade");
        nova_mercadoria.getQuant().push_back(quant_x_separacao * itens[indice].getUnid());
        nova_mercadoria.setValor((itens[indice].getValor() / itens[indice].getUnid()) * 1.2);
        nova_mercadoria.setUnid(1);
        nova_mercadoria.getValid().push_back(data_prod);

        if(itens[indice].getQuant()[ind] - quant_x_separacao == 0){
            cout << "Foi possível realizar a separação, porém, o produto " << itens[indice].getNome();
            cout << " com a validade " << itens[indice].getValid()[ind].day << "/" << itens[indice].getValid()[ind].mon << "/" << itens[indice].getValid()[ind].year;
            cout << " ficou com 0 unidades" << endl;
        }else{
            cout << "Foi possível realizar a separação, porém, o produto " << itens[indice].getNome();
            cout << " com a validade " << itens[indice].getValid()[ind].day << "/" << itens[indice].getValid()[ind].mon << "/" << itens[indice].getValid()[ind].year;
            cout << " ficou com " << itens[indice].getQuant()[ind] << " unidades" << endl;
        }
    }

    itens[indice].getQuant()[ind] -= quant_x_separacao;

    for(size_t i = 0; i<itens.size(); i++){
        if(itens[i].getNome() == nova_mercadoria.getNome()){
            for(size_t j = 0; j<itens[i].getValid().size(); j++){
                if(itens[i].getValid()[j].day + itens[i].getValid()[j].mon + itens[i].getValid()[j].year == nova_mercadoria.getValid()[0].day + nova_mercadoria.getValid()[0].mon + nova_mercadoria.getValid()[0].year){
                    itens[i].getQuant()[j] += nova_mercadoria.getQuant()[0];
                }else{
                    itens[i].getQuant().push_back(nova_mercadoria.getQuant()[0]);
                    itens[i].getValid().push_back(nova_mercadoria.getValid()[0]);
                }
            }
            return true;
        }
    }
    itens.insert(itens.begin() + indice, nova_mercadoria);
    return true;

}//Separa o item original em unidades

bool agrupador(vector<mercadoria>& itens, size_t indice){
    string busca = " unidade";
    size_t posicao = itens[indice].getNome().find(busca);
    string nome_provisorio;
    int ind = -1, num_unidades = 0, n = 0, unid_fard, soma_ditens = 0, soma = 0;

    mercadoria new_mercadoria;

    if (indice >= itens.size() || (posicao == string::npos)) {
        cout << "Índice inválido ou esta mercadoria não é uma unidade." << endl;
        return false;
    }else{
        nome_provisorio = itens[indice].getNome().erase(posicao, sizeof(busca));
        itens[indice].getNome() += busca;
        new_mercadoria.getNome() = nome_provisorio;
    }

    for(size_t i = 0; i < itens.size(); i++){
        string busca = nome_provisorio;
        size_t posicao = itens[i].getNome().find(busca);
        if(!(posicao == string::npos)){
            ind = i;
        }
    }

     if(ind == -1){
        cout << "Não foi possível encontrar o item original (o fardo). \n";
        cout << "Portanto, digite quantas unidades possui no agrupamento desta unidade: ";
        cin >> unid_fard;
     }else{
        new_mercadoria.setValor(itens[ind].getValor());
     }

    do{
        itens[indice].getQuant()[0 + n]--;
        num_unidades++;
        soma++;

        if((soma == unid_fard) || (soma == itens[ind].getUnid())){
            new_mercadoria.getQuant().push_back(num_unidades);
            new_mercadoria.getValid().push_back(itens[indice].getValid()[0+n]);
            break;
        }else{
            if(itens[indice].getQuant()[0 + n] == 0){
                new_mercadoria.getQuant().push_back(num_unidades);
                new_mercadoria.getQuant().push_back(itens[indice].getQuant()[0 + n]);
                n++;
                num_unidades=0;
            }
            for(size_t i = 0; i<itens[indice].getQuant().size(); i++){
                soma_ditens += itens[indice].getQuant()[i];
            }
            if(soma == 0){
                cout << "Não é possível realizar o agrupamento destas unidades.";
                return false;
            }
        }    
    }while((soma != unid_fard )|| (soma != itens[ind].getUnid())); //retira as unidades
    for(size_t i = 0; i < itens[indice].getQuant().size(); i++){
        if(itens[indice].getQuant()[i] == 0){
            itens[indice].getQuant().erase(itens[indice].getQuant().begin()+i);
            itens[indice].getValid().erase(itens[indice].getValid().begin()+i);
        }
    }
    new_mercadoria.getQuant().push_back(1);
    new_mercadoria.setValor(num_unidades * itens[indice].getValor()); // Nova mercadoria construída.

    for(size_t i = 0; i<itens.size(); i++){
        string busca = new_mercadoria.getNome();
        size_t posicao = itens[i].getNome().find(busca);
        if(!(posicao == string::npos)){
            for(size_t j = 0; j<new_mercadoria.getQuant().size(); j++){
                itens[i].getQuant().push_back(new_mercadoria.getQuant()[j]);
                itens[i].getValid().push_back(new_mercadoria.getValid()[j]);
            }
            return true;
        }
    }
    itens.insert(itens.begin() + indice, new_mercadoria);
    return true;
    
}//Agrupa unidades em um unico item.

float conta_cliente (vector<cliente> comprador, string nome_search){
    float valor_conta = 0.0;
    float ind = -1.0;

    ind = escolha(comprador, nome_search);
    if(ind == -1.0){
        cout << "Nome não encontrado.\n" << "Verifique se o nome foi digitado incorretamente.";
        return -1.0;
    }else{
        for(size_t i = 0; i<comprador[ind].getMerc().size(); i++){
            for(size_t j = 0; j<comprador[ind].getMerc()[i].getQuant().size(); i++){
                valor_conta += comprador[ind].getMerc()[i].getQuant()[j] * comprador[ind].getMerc()[i].getValor();
            }
        }
    }

    return valor_conta;
}//Retorna o que o cliente deve.

float receita(vector<mercadoria> itens_Co, vector<mercadoria> itens_Ve, vector<cliente> compradores){
    float venda = 0.0, compra = 0.0, venda_cli_arm = 0.0;

    for(size_t i = 0; i<itens_Co.size(); i++){
        for(size_t j = 0; j < itens_Co[i].getQuant().size(); j++){
            compra += itens_Co[i].getQuant()[j] * itens_Co[i].getValor();
        }
    }
    for(size_t i = 0; i<itens_Ve.size(); i++){
        for(size_t j = 0; j < itens_Ve[i].getQuant().size(); j++){
            venda += itens_Ve[i].getQuant()[j] * itens_Ve[i].getValor();
        }
    }
    for(size_t i = 0; i<compradores.size(); i++){
        for(size_t j = 0; j<compradores[i].getMerc().size(); j++){
            for(size_t k = 0; k<compradores[i].getMerc()[j].getQuant().size(); k++){
                venda_cli_arm += compradores[i].getMerc()[j].getQuant()[k] * compradores[i].getMerc()[j].getValor();
            }
        }
    }
    
    return venda - compra - venda_cli_arm;
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

bool copiararq(const string& arq_origin, const string& arq_dest){
    string line;

    ifstream arq_O(arq_origin, ios::in);
    if(!arq_O.is_open()){
        cerr << "O arquivo: "<< arq_origin << " não pode ser aberto!" << endl;
        arq_O.close();
        return false;
    }

    ofstream arq_D(arq_dest, ios::out);
    if(!arq_D.is_open() || (verificador_arquivo(arq_dest) == false)){
        cerr << "O arquivo: "<< arq_dest << " não pode ser aberto ou se encontra vazio!" << endl;
        arq_D.close();
        return false;
    }

    while(getline(arq_O,line)){
        arq_D << line << endl;
    }
    
    arq_O.close(); arq_D.close();

    cout << "Transferência de dados realizado com sucesso!" << endl;
    return true;
}

void comparador_data(vector<tdata> validade, vector<double>& diff_data){
    auto now = system_clock::now();
    time_t time_now = system_clock::to_time_t(now);
    tm* local_time = localtime(&time_now);
    diff_data.clear();

    struct tm data_valid;
    time_t tempo_f, tempo_i;

    for(size_t i = 0; i<validade.size(); i++){
        data_valid.tm_year = validade[i].year - 1900;
        data_valid.tm_mon = validade[i].mon - 1;
        data_valid.tm_mday = validade[i].day;

        tempo_f = mktime(&data_valid);
        tempo_i = mktime(local_time);
        if((tempo_i != -1) && (tempo_f != -1)){
            diff_data.push_back(difftime(tempo_f,tempo_i)/(60*60*24));
        }
    }
}

void verfic_venc(vector<mercadoria>& itens_p_verific){
    string busca = " (vencido)";
    vector<double> comp_data;

    for(size_t i = 0; i<itens_p_verific.size(); i++){
        comparador_data(itens_p_verific[i].getValid(), comp_data);
        for(size_t j = 0; j<comp_data.size(); j++){
            if(comp_data[j] <= 0){
                cout << "O produto " << itens_p_verific[i].getNome() << " com a validade ";
                cout << itens_p_verific[i].getValid()[j].day << "/"  << itens_p_verific[i].getValid()[j].mon << "/" << itens_p_verific[i].getValid()[j].year;
                cout << "possui " <<  itens_p_verific[i].getQuant()[j] << "unidades vencidas";
            }
        }
    }
}

bool extracao_arq_prod(vector<mercadoria>& itens_p_arm, const string& nome_arq){
    string nome_p_prod;
    vector<int> num_quant;
    int quant_pacot;
    float preco;
    vector<tdata> dat_ven;
    int tam, num_quantidade;

    fstream arq_merc(nome_arq, fstream::in);
    if(!arq_merc.is_open()){
        cerr << "Erro ao abrir o arquivo." << endl;
        return false;
    }
    while(arq_merc >> nome_p_prod >> num_quantidade >> quant_pacot >> preco >> tam){
        dat_ven.clear();

        for(int i = 0; i < num_quantidade; i++){
            int quantidade_itens;
            arq_merc >> quantidade_itens;
            num_quant.push_back(quantidade_itens);
        }

        for(int i = 0; i<tam; i++){
            tdata data;
            arq_merc >> data.day >> data.mon >> data.year;
            dat_ven.push_back(data);
        }
        itens_p_arm.emplace_back(nome_p_prod, num_quant, quant_pacot, preco, dat_ven);
    }
    arq_merc.close();
    cout << "A extração de dados foi realizada com sucesso" << endl;
    return true;
}

vector<shared_ptr<pessoa>> extracao_arq_pes(const string nome_arq){
    fstream arq_pes(nome_arq, fstream::in);
    vector<shared_ptr<pessoa>> people;

    if (!arq_pes.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return people;
    }

    string name, conf;
    int tipo;

    while(arq_pes >> name >> conf >> tipo){
        if(tipo == 1){
            string cargo;
            float v;
            tdata data_pg;
            arq_pes >> cargo >> v >> data_pg.day >> data_pg.mon >> data_pg.year;
            people.emplace_back(make_shared<proletariado>(name, conf, cargo, v, data_pg));

        }else{
            if(tipo == 2){
                string nome_prod;
                int tam_qtd, unid, tam_data, tam;
                float v;
                vector<int> num_quant;
                vector<tdata> data_t;
                vector<mercadoria> merc;
                
                arq_pes >> tam;
                for(int i = 0; i<tam; i++){
                    arq_pes >> nome_prod >> tam_qtd >> unid >> v >> tam_data;
                    for(int j = 0; j<tam_qtd; j++){
                        int num_q;
                        arq_pes >> num_q;
                        num_quant.push_back(num_q);
                    }
                    for(int j = 0; j<tam_data; j++){
                        tdata data;
                        arq_pes >> data.day >> data.mon >> data.year;
                        data_t.push_back(data);
                    }
                    merc.emplace_back(nome_prod, num_quant, unid, v, data_t);
                }
                
                people.emplace_back(make_shared<cliente>(name, conf, merc));
            }else{
                cerr << "Houve um erro durante a extração do arquivo." << endl;
                return people;
            }
        }
    }
    cout << "Extração concluída com sucesso." << endl;
    arq_pes.close();
    return people;
}

bool extracao_arq_key(Key& chave, const string& nome_arq){
    string senha;

    fstream arq_key(nome_arq, fstream::in);
    if(!arq_key.is_open()){
        cerr << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    arq_key >> senha;
    chave.setSenha(senha);
    arq_key.close();
    cout << "A extração de dados foi realizada com sucesso!" << endl;
    return true;
}

void extracao_vector(vector<shared_ptr<pessoa>>& people, vector<shared_ptr<proletariado>>& workers, vector<shared_ptr<cliente>>& clientes, vector<cliente>& cli, vector<proletariado>& w){
    for(const auto& pessoa : people){
        if(auto c = dynamic_pointer_cast<cliente>(pessoa)){
            clientes.push_back(c);
        }else{
            if(auto w = dynamic_pointer_cast<proletariado>(pessoa)){
                workers.push_back(w);
            }
        }
    }

    for(size_t i = 0; i<clientes.size(); i++){
        string nome = clientes[i]->getNome();
        string conf = clientes[i]->getConf();
        vector<mercadoria> merc_t = clientes[i]->getMerc();
        cli.emplace_back(nome, conf, merc_t);
    }

    for(size_t i = 0; i<workers.size(); i++){
        string nome = workers[i]->getNome();
        string conf = workers[i]->getConf();
        string carg = workers[i]->getCargo();
        float sal = workers[i]->getSalario();
        tdata data = workers[i]->getpay_day();
        w.emplace_back(nome, conf, carg, sal, data);
    }
}

void inversa_extracao_vector(vector<shared_ptr<pessoa>>& people, vector<cliente> cli, vector<proletariado> w){
    vector<cliente> clientes(cli);
    vector<proletariado> workers(w);

    people.clear();

    for(auto& C : clientes){people.push_back(make_shared<cliente>(move(C)));}
    for(auto& W : workers){people.push_back(make_shared<proletariado>(move(W)));}
}

bool quadro_prod(vector<mercadoria> produtos){
    const int larguraNome_p = 10;
    const int larguraUnid = 10;
    const int larguraValor = 10;
    const int larguraQuant = 10;
    const int larguraValid = 10;

    if(produtos.empty()){
        return false;
    }

    cout << left << setw(larguraNome_p) << "Nome do produto"
        << setw(larguraQuant) << "Quantidade(s)"
        << setw(larguraUnid) << "Unidade por pacote"
        << setw(larguraValor) << "Valor da mercadoria"
        << setw(larguraValid) << "Validades dos produtos" << endl;
    cout <<  string(50, '-') << endl;

    for(size_t i = 0; i<produtos.size(); i++){
        cout << left << setw(larguraNome_p) << produtos[i].getNome();
        for(size_t j = 0; j<produtos[i].getQuant().size(); j++){
            cout << left << setw(larguraQuant) << produtos[i].getQuant()[j] << endl;
        }
        cout << left << setw(larguraUnid) << produtos[i].getUnid();
        cout << left << setw(larguraValor) << produtos[i].getValor();
        for(size_t j = 0; j<produtos[i].getValid().size(); j++){
            cout << left << setw(larguraValid) << produtos[i].getValid()[j].day << "/" << produtos[i].getValid()[j].mon << "/" << produtos[i].getValid()[j].year << endl;
        }
    }
    return true;
}

bool proc_carg(vector<proletariado> workers, string carg){
    vector<int> indices;

    for(size_t i = 0; i < workers.size(); i++){
        size_t p = workers[i].getNome().find(carg);
        if(p != string::npos){
            return true;
        }
    }
    return false;
}

bool dataValida(tdata data){
    int diaspmes[]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    auto now = system_clock::now();
    time_t time_now = system_clock::to_time_t(now);
    tm* local_time = localtime(&time_now);
    int ano = local_time->tm_year + 1900;
    int mes = local_time->tm_mon + 1;
    int dia = local_time->tm_mday;

    if((data.year < ano)||(data.year == ano && (data.mon < mes || (data.mon == mes && data.day < dia)))){return false;}

    if(data.mon < 1 || data.mon > 12){return false;}
    if(data.mon == 2 && data.year%4==0){diaspmes[2] = 29;}

    return (data.day >= 1 && data.day<=diaspmes[data.mon]);
}
bool next_d(tdata data, int x){
    int diaspmes[]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(data.mon == 2 && data.year%4==0){diaspmes[2] = 29;}

    if(dataValida(data)){
        for(int i = 0; i<x; i++){    
            data.day++;
            if(data.day > diaspmes[data.mon]){data.day = 1; data.mon++;}
            if(data.mon > 12){data.mon = 1; data.year++;}
        }
        return true;
    }else{
        return false;
    }
}

bool compra(vector<mercadoria>& mercadoria_CO, vector<mercadoria>& mercadoria_VE, string& nome_dproduto, int unidades_compradas, tdata data_dprod){
    int ind = -1, ind_d = -1; 
    mercadoria item_VE = mercadoria();

    for(size_t i = 0; i<mercadoria_CO.size(); i++){
        string busca = nome_dproduto;
        size_t posicao = mercadoria_CO[i].getNome().find(busca);
        if(!(posicao == string::npos)){
            ind = i;
            item_VE.setN_prod(mercadoria_CO[i].getNome());
            item_VE.setValor(mercadoria_CO[i].getValor() * 1.2);
            item_VE.setUnid(mercadoria_CO[i].getUnid());
        }
        for(size_t j = 0; j<mercadoria_CO[i].getValid().size(); j++){
            if(mercadoria_CO[i].getValid()[j].day + mercadoria_CO[i].getValid()[j].mon + mercadoria_CO[i].getValid()[j].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                ind_d = j;
                item_VE.getValid().push_back(mercadoria_CO[i].getValid()[j]);
            }
        }
    }
    if(ind == -1 || ind_d == -1){
        cout << "A mercadoria não foi registrada ou foi escrita incorretamente." << endl;
        cout << "Redirecionando ao Menu...";
        return false;
    }
    if(((mercadoria_CO[ind].getValid()[ind_d].day <=0) && (mercadoria_CO[ind].getValid()[ind_d].mon <=0) && (mercadoria_CO[ind].getValid()[ind_d].year <=0)) || (mercadoria_CO[ind].getValid()[ind_d].year < 0)){
        cout << "A mercadoria que está prestes a ser vendida está vencida!" << endl;
        cout << "Verifique o produto imediatamente!";
        return false;
    }

    int quant = mercadoria_CO[ind].getQuant()[ind_d];
    if(quant - unidades_compradas < 0){
        cout << "Não há unidades suficiente para a compra.\n";
        cout << "Verifique o nome ou a data digitados.";
        return false;
    }else{
        for(size_t i = 0; i<mercadoria_VE.size(); i++){
            if(mercadoria_VE[i].getNome() == mercadoria_CO[ind].getNome()){
                for(size_t j = 0; j<mercadoria_VE[i].getValid().size(); j++){
                    if(mercadoria_VE[i].getValid()[j].day + mercadoria_VE[i].getValid()[j].mon + mercadoria_VE[i].getValid()[j].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                        mercadoria_VE[i].getQuant()[j] += unidades_compradas;
                    }
                }
                return true;
            }
        }
        if(quant - unidades_compradas == 0){
            mercadoria_CO[ind].getQuant()[ind_d] -= unidades_compradas;
            item_VE.getQuant().push_back(unidades_compradas);
            cout << "O produto: " << mercadoria_CO[ind].getNome() << " com a validade " << mercadoria_CO[ind].getValid()[ind_d].day << "/" << mercadoria_CO[ind].getValid()[ind_d].mon << "/" << mercadoria_CO[ind].getValid()[ind_d].year << "está em falta.";
            return true;
        }else{
            mercadoria_CO[ind].getQuant()[ind_d] -= unidades_compradas;
            item_VE.getQuant().push_back(unidades_compradas);
            cout << " com a validade " << mercadoria_CO[ind].getValid()[ind_d].day << "/" << mercadoria_CO[ind].getValid()[ind_d].mon << "/" << mercadoria_CO[ind].getValid()[ind_d].year;
            cout << " ficou com " << mercadoria_CO[ind].getQuant()[ind_d] << " unidades" << endl;
            return true;
        }
    }
}

bool verific(string resposta){
    resposta.clear();
    cout << "Deseja sair desta área? (sim/não)\n";
    cin >> resposta; transf(resposta);
    if(resposta == "sim"){
        return true;
    }else{
        return false;
    }
}

bool resp_saida(string resposta){
    resposta.clear();
    cout << "Digite qualquer coisa quando for para sair: ";
    cin >> resposta;
    if(resposta != "não"){
        return true;
    }else{
        return true;
    }
}