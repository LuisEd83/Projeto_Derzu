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
int escolha(const vector<T>& Nome_vector, string Nome_p) {
    vector<int> indices;
    int quantidade_opcoes = 0;
    string Nome_p_lower = Nome_p; transf(Nome_p_lower);

    for (size_t i = 0; i < Nome_vector.size(); i++) {
        string nome_item = Nome_vector[i].getNome();
        transf(nome_item);
        
        if (nome_item.find(Nome_p_lower) != string::npos) {
            quantidade_opcoes++;
            indices.push_back(i);
            cout << quantidade_opcoes << " - " << Nome_vector[i].getNome() << endl;
        }
    }

    if (indices.empty()) {
        return -1;
    }

    int opcao = -1;
    int indice_maximo = quantidade_opcoes - 1;

    do {
        cout << "Escolha o nome desejado: ";
        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Digite um número.\n";
            opcao = -1;
        } else {
            opcao -= 1;
            if (opcao < 0 || opcao > indice_maximo) {
                cout << "Opção inválida. Escolha entre 1 e " << quantidade_opcoes << ".\n";
            }
        }
    } while (opcao < 0 || opcao > indice_maximo);

    return indices[opcao];
}


void verif_dataPG(vector<proletariado> workers){
    for(size_t i = 0; i<workers.size(); i++){
        if(workers[i].Pagamento(workers[i].getpay_day())){
            cout << "O dia do pagamento do salário do funcioário " << workers[i].getNome() << "chegou ou está atrasado!" << endl;
        }
    }
}

bool desagrupador(vector<mercadoria>& itens, size_t indice, int quant_x_separacao, tdata data_prod) {
    if (indice >= itens.size() || quant_x_separacao <= 0) {
        cerr << "Índice inválido ou quantidade inválida." << endl;
        return false;
    }

    mercadoria& original = itens[indice];
    int ind = -1;

    for (size_t i = 0; i < original.getValid().size(); i++) {
        const tdata& validade = original.getValid()[i];
        if (validade.day == data_prod.day && 
            validade.mon == data_prod.mon && 
            validade.year == data_prod.year) {
            ind = i;
            break;
        }
    }

    if (ind == -1 || original.getQuant()[ind] < quant_x_separacao) {
        cout << "Não foi possível separar: quantidade insuficiente ou data inválida." << endl;
        return false;
    }

    mercadoria nova_mercadoria;
    nova_mercadoria.setN_prod(original.getNome() + " unidade");
    nova_mercadoria.setUnid(1);
    nova_mercadoria.setValor((original.getValor() / original.getUnid()) * 1.2f);
    nova_mercadoria.getQuant().push_back(quant_x_separacao * original.getUnid());
    nova_mercadoria.getValid().push_back(data_prod);

    original.getQuant()[ind] -= quant_x_separacao;
    
    if (original.getQuant()[ind] == 0) {
        original.getQuant().erase(original.getQuant().begin() + ind);
        original.getValid().erase(original.getValid().begin() + ind);
    }

    bool encontrou = false;
    for (auto& item : itens) {
        if (item.getNome() == nova_mercadoria.getNome()) {
            for (size_t j = 0; j < item.getValid().size(); j++) {
                const tdata& val = item.getValid()[j];
                if (val.day == data_prod.day && 
                    val.mon == data_prod.mon && 
                    val.year == data_prod.year) {
                    item.getQuant()[j] += nova_mercadoria.getQuant()[0];
                    encontrou = true;
                    break;
                }
            }
            if (!encontrou) {
                item.getQuant().push_back(nova_mercadoria.getQuant()[0]);
                item.getValid().push_back(nova_mercadoria.getValid()[0]);
            }
            return true;
        }
    }
    itens.push_back(nova_mercadoria);
    return true;
}//Separa o item original em unidades

bool agrupador(vector<mercadoria>& itens, size_t indice) {
    if (indice >= itens.size()) {
        cerr << "Erro: Índice inválido.\n";
        return false;
    }

    mercadoria& produto = itens[indice];
    if (!produto.ehUnidade()) {
        cerr << "Erro: O produto não é uma unidade.\n";
        return false;
    }

    // Extrai o nome base (ex: "Arroz unidade" → "Arroz")
    size_t pos = produto.getNome().find(" unidade");
    if (pos == string::npos) {
        cerr << "Erro: Formato do nome inválido.\n";
        return false;
    }
    string nome_base = produto.getNome().substr(0, pos);

    // Procura pelo fardo correspondente
    int indice_fardo = -1;
    for (size_t i = 0; i < itens.size(); i++) {
        if (itens[i].getNome() == nome_base) {
            indice_fardo = i;
            break;
        }
    }

    // Define unidades por fardo
    int unidades_por_fardo;
    if (indice_fardo == -1) {
        cout << "Digite o número de unidades por fardo: ";
        while (!(cin >> unidades_por_fardo) || unidades_por_fardo <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << "Valor inválido! Digite um número maior que 0: ";
        }
    } else {
        unidades_por_fardo = itens[indice_fardo].getUnid();
    }

    // Calcula o total de unidades disponíveis
    int total_unidades = 0;
    for (int q : produto.getQuant()) total_unidades += q;

    if (total_unidades < unidades_por_fardo) {
        cerr << "Erro: Unidades insuficientes (" << total_unidades << ") para formar 1 fardo.\n";
        return false;
    }

    // Calcula fardos e unidades restantes
    int num_fardos = total_unidades / unidades_por_fardo;
    int unidades_restantes = total_unidades % unidades_por_fardo;

    // Atualiza ou cria o fardo
    if (indice_fardo == -1) {
        mercadoria fardo;
        fardo.setN_prod(nome_base);
        fardo.setUnid(unidades_por_fardo);
        fardo.setValor(produto.getValor() * unidades_por_fardo); 
        fardo.getQuant().push_back(num_fardos);
        fardo.getValid().push_back(produto.getValid()[0]);
        itens.push_back(fardo); // Adiciona ao final do vetor
    } else {
        // Adiciona aos fardos existentes (mesma validade)
        itens[indice_fardo].getQuant()[0] += num_fardos;
    }

    // Atualiza a unidade original
    if (unidades_restantes > 0) {
        produto.getQuant().clear();
        produto.getQuant().push_back(unidades_restantes);
        produto.getValid().resize(1); // Mantém apenas a primeira validade
    } else {
        // Remove o item original do vetor
        itens.erase(itens.begin() + indice);
    }

    cout << "Agrupamento concluído. Vetor modificado com sucesso!\n";
    return true;
}

//Agrupa unidades em um unico item.

float conta_cliente (vector<cliente> comprador, string nome_search){
    float valor_conta = 0.0;
    float ind = -1;

    ind = escolha(comprador, nome_search);
    if(ind == -1){
        cout << "Nome não encontrado.\n" << "Verifique se o nome foi digitado incorretamente.";
        return -1.0;
    }else{
        for(size_t i = 0; i<comprador[ind].getMerc().size(); i++){
            for(size_t j = 0; j<comprador[ind].getMerc()[i].getQuant().size(); j++){
                valor_conta += comprador[ind].getMerc()[i].getQuant()[j] * comprador[ind].getMerc()[i].getValor();
            }
        }
    }

    return valor_conta;
}//Retorna o que o cliente deve.

float receita(vector<mercadoria> itens_Co, vector<mercadoria> itens_Ve, vector<cliente> compradores) {
    float compra = 0.0, venda = 0.0, venda_cli_arm = 0.0;

    for ( auto& merc : itens_Co) {
        for (int quant : merc.getQuant()) {
        compra += merc.getValor() * quant;
        }
    }

    for ( auto& merc : itens_Ve) {
        for (int quant : merc.getQuant()) {
        venda += merc.getValor() * quant;
        }
    }

    for ( auto& cliente : compradores) {
        venda_cli_arm += cliente.divida(cliente.getMerc()); 
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

bool lerData(string data_str, tdata& data) {
    std::stringstream ss(data_str);
    char slash;

    if (ss >> data.day >> slash >> data.mon >> slash >> data.year) {
        if (slash == '/') {
            return true;
        }
    }
    std::cerr << "Formato de data inválido! Use o formato dia/mes/ano.\n";
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
bool next_d(tdata& data, int x){
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

bool extracao_arq_prod(vector<mercadoria>& itens_p_arm, const string& nome_arq) {
    fstream arq_merc(nome_arq, fstream::in);
    if (!arq_merc.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return false;
    }

    string line;
    while (getline(arq_merc, line)) {
        string nome_p_prod = line;
        vector<int> num_quant;
        int quant_pacot = 0;
        float preco = 0.0;
        vector<tdata> dat_ven;

        if (!getline(arq_merc, line)) break;
        istringstream quant_stream(line);
        int q;
        while (quant_stream >> q) {
            num_quant.push_back(q);
        }

        if (!getline(arq_merc, line)) break;
        try {
            quant_pacot = stoi(line);
        } catch (const invalid_argument& e) {
            cerr << "Erro ao converter unidade por pacote: " << line << endl;
            continue;
        }

        if (!getline(arq_merc, line)) break;
        try {
            preco = stof(line);
        } catch (const invalid_argument& e) {
            cerr << "Erro ao converter preço: " << line << endl;
            continue;
        }

        if (!getline(arq_merc, line)) break;
        istringstream data_stream(line);
        string data_str;
        while (data_stream >> data_str) {
            tdata data = {0, 0, 0};
            size_t pos1 = data_str.find('/');
            size_t pos2 = data_str.rfind('/');
            if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) {
                cerr << "Erro no formato da data: " << data_str << endl;
                continue;
            }
            try {
                data.day = stoi(data_str.substr(0, pos1));
                data.mon = stoi(data_str.substr(pos1 + 1, pos2 - pos1 - 1));
                data.year = stoi(data_str.substr(pos2 + 1));
                dat_ven.push_back(data);
            } catch (const invalid_argument& e) {
                cerr << "Erro ao converter data: " << data_str << endl;
                continue;
            }
        }

        itens_p_arm.emplace_back(nome_p_prod, num_quant, quant_pacot, preco, dat_ven);
    }

    arq_merc.close();
    cout << "Extração realizada com sucesso!" << endl;
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
            string cargo, data_str;
            float v;
            tdata data_pg;
            arq_pes >> cargo >> v >> data_str; lerData(data_str, data_pg);
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
            if(auto worker  = dynamic_pointer_cast<proletariado>(pessoa)){
                workers.push_back(worker);
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

bool compra(vector<mercadoria>& mercadoria_CO, vector<mercadoria>& mercadoria_VE, string& nome_dproduto, int unidades_compradas, tdata data_dprod){
    int ind = -1, ind_d = -1; 
    mercadoria item_VE = mercadoria();

    ind = escolha(mercadoria_CO, nome_dproduto);
    if(ind == -1){
        cout << "A mercadoria não foi registrada ou foi escrita incorretamente." << endl;
        cout << "Redirecionando ao Menu...";
        return false;
    }else{
        item_VE.setN_prod(mercadoria_CO[ind].getNome());
        item_VE.setValor(mercadoria_CO[ind].getValor() * 1.2);
        item_VE.setUnid(mercadoria_CO[ind].getUnid());

        for(size_t j = 0; j<mercadoria_CO[ind].getValid().size(); j++){
            if(mercadoria_CO[ind].getValid()[j].day == data_dprod.day && mercadoria_CO[ind].getValid()[j].mon == data_dprod.mon && mercadoria_CO[ind].getValid()[j].year == data_dprod.year){
                ind_d = j;
                item_VE.getValid().push_back(mercadoria_CO[ind].getValid()[j]);
            }
        }
    }
    if(ind_d == -1){
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

            item_VE.setN_prod(mercadoria_CO[ind].getNome());
            item_VE.getQuant().push_back(unidades_compradas);
            item_VE.setUnid(mercadoria_CO[ind].getUnid());
            item_VE.setValor(mercadoria_CO[ind].getValor());
            item_VE.getValid().push_back(data_dprod);
            mercadoria_VE.push_back(item_VE);

            cout << "O produto: " << mercadoria_CO[ind].getNome() << " com a validade " << mercadoria_CO[ind].getValid()[ind_d].day << "/" << mercadoria_CO[ind].getValid()[ind_d].mon << "/" << mercadoria_CO[ind].getValid()[ind_d].year << "está em falta.";
        }else{
            mercadoria_CO[ind].getQuant()[ind_d] -= unidades_compradas;

            item_VE.setN_prod(mercadoria_CO[ind].getNome());
            item_VE.getQuant().push_back(unidades_compradas);
            item_VE.setUnid(mercadoria_CO[ind].getUnid());
            item_VE.setValor(mercadoria_CO[ind].getValor());
            item_VE.getValid().push_back(data_dprod);
            mercadoria_VE.push_back(item_VE);

            cout << " com a validade " << mercadoria_CO[ind].getValid()[ind_d].day << "/" << mercadoria_CO[ind].getValid()[ind_d].mon << "/" << mercadoria_CO[ind].getValid()[ind_d].year;
            cout << " ficou com " << mercadoria_CO[ind].getQuant()[ind_d] << " unidades" << endl;
        }
    }
    return true;
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
    if(resposta != "não"){cout << " ";}
    return true;
}