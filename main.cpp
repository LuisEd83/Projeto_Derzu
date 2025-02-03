#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <unistd.h>

using namespace std;
using namespace chrono;

typedef struct{
    int day = 0;
    int mon = 0;
    int year = 0;
}tdata;

class mercadoria{
    public:
        string nome_produto;
        vector<int> quantidade;
        int unidade_pacote;
        float valor;
        vector<tdata> validade;
            mercadoria(){
                nome_produto = "void";
                quantidade.push_back(0);
                valor = 0.0;
                unidade_pacote = 0;
                validade.push_back({0,0,0});
            }
            mercadoria(string nome_produto, vector<int> quantidade, int unidade_pacote, float valor, vector<tdata> validade) 
                : nome_produto(nome_produto), quantidade(quantidade), unidade_pacote(unidade_pacote), valor(valor), validade(validade) {}
};

class cliente{
    public:
        string nome_cliente;
        vector<mercadoria> itens;
            cliente(){
                nome_cliente = "void";
                itens.clear();
            }
};

int indice_prod(vector<mercadoria> nome_vector, string nome_search){
    int ind = -1;

    for(size_t i = 0; i<nome_vector.size(); i++){
        if(nome_vector[i].nome_produto == nome_search){
            return i;
        }
    }
    return ind; 
}

int indice_cli(vector<cliente> nome_vector, string nome_search){
    int ind = -1;

    for(size_t i = 0; i<nome_vector.size(); i++){
        if(nome_vector[i].nome_cliente == nome_search){
            return i;
        }
    }
    return ind; 
}

bool desagrupador(vector<mercadoria>& itens, size_t indice, int quant_x_separacao, tdata data_prod){
    mercadoria nova_mercadoria;
    int ind = -1;
    if (indice >= itens.size()) {
        cerr << "Índice inválido." << endl;
        return false;
    }
    
    for(size_t i = 0; i<itens[indice].validade.size(); i++){
        if((itens[indice].validade[i].day - data_prod.day) + (itens[indice].validade[i].mon - data_prod.mon) + (itens[indice].validade[i].year - data_prod.year) == 0){
            ind = i;
        }
    }
    if(ind == -1){
        cout << "Não foi possivel separar." << endl; return false;
    }

    if(itens[indice].quantidade[ind] - quant_x_separacao < 0){
        cout << "Não foi possivel separar." << endl; return false;
    }else{
        nova_mercadoria.nome_produto = itens[indice].nome_produto + " unidade";
        nova_mercadoria.quantidade.push_back(quant_x_separacao * itens[indice].unidade_pacote);
        nova_mercadoria.valor = (itens[indice].valor / itens[indice].unidade_pacote) * 1.2;
        nova_mercadoria.unidade_pacote = 1;
        nova_mercadoria.validade.push_back(data_prod);

        if(itens[indice].quantidade[ind] - quant_x_separacao == 0){
            cout << "Foi possível realizar a separação, porém, o produto " << itens[indice].nome_produto;
            cout << " com a validade " << itens[indice].validade[ind].day << "/" << itens[indice].validade[ind].mon << "/" << itens[indice].validade[ind].year;
            cout << " ficou com 0 unidades" << endl;
        }else{
            cout << "Foi possível realizar a separação, porém, o produto " << itens[indice].nome_produto;
            cout << " com a validade " << itens[indice].validade[ind].day << "/" << itens[indice].validade[ind].mon << "/" << itens[indice].validade[ind].year;
            cout << " ficou com " << itens[indice].quantidade[ind] << " unidades" << endl;
        }
    }

    itens[indice].quantidade[ind] -= quant_x_separacao;

    for(size_t i = 0; i<itens.size(); i++){
        if(itens[i].nome_produto == nova_mercadoria.nome_produto){
            for(size_t j = 0; j<itens[i].validade.size(); j++){
                if(itens[i].validade[j].day + itens[i].validade[j].mon + itens[i].validade[j].year == nova_mercadoria.validade[0].day + nova_mercadoria.validade[0].mon + nova_mercadoria.validade[0].year){
                    itens[i].quantidade[j] += nova_mercadoria.quantidade[0];
                }else{
                    itens[i].quantidade.push_back(nova_mercadoria.quantidade[0]);
                    itens[i].validade.push_back(nova_mercadoria.validade[0]);
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
    size_t posicao = itens[indice].nome_produto.find(busca);
    string nome_provisorio;
    int ind = -1, num_unidades = 0, n = 0, unid_fard, soma_ditens = 0, soma = 0;

    mercadoria new_mercadoria;

    if (indice >= itens.size() || (posicao == string::npos)) {
        cout << "Índice inválido ou esta mercadoria não é uma unidade." << endl;
        return false;
    }else{
        nome_provisorio = itens[indice].nome_produto.erase(posicao, sizeof(busca));
        itens[indice].nome_produto += busca;
        new_mercadoria.nome_produto = nome_provisorio;
    }

    for(size_t i = 0; i < itens.size(); i++){
        string busca = nome_provisorio;
        size_t posicao = itens[i].nome_produto.find(busca);
        if(!(posicao == string::npos)){
            ind = i;
        }
    }

     if(ind == -1){
        cout << "Não foi possível encontrar o item original (o fardo). \n";
        cout << "Portanto, digite quantas unidades possui no agrupamento desta unidade: ";
        cin >> unid_fard;
     }else{
        new_mercadoria.valor = itens[ind].valor;
     }

    do{
        itens[indice].quantidade[0 + n]--;
        num_unidades++;
        soma++;

        if((soma == unid_fard) || (soma == itens[ind].unidade_pacote)){
            new_mercadoria.quantidade.push_back(num_unidades);
            new_mercadoria.validade.push_back(itens[indice].validade[0+n]);
            break;
        }else{
            if(itens[indice].quantidade[0 + n] == 0){
                new_mercadoria.quantidade.push_back(num_unidades);
                new_mercadoria.validade.push_back(itens[indice].validade[0 + n]);
                n++;
                num_unidades=0;
            }
            for(size_t i = 0; i<itens[indice].quantidade.size(); i++){
                soma_ditens += itens[indice].quantidade[i];
            }
            if(soma == 0){
                cout << "Não é possível realizar o agrupamento destas unidades.";
                return false;
            }
        }    
    }while((soma != unid_fard )|| (soma != itens[ind].unidade_pacote)); //retira as unidades
    for(size_t i = 0; i < itens[indice].quantidade.size(); i++){
        if(itens[indice].quantidade[i] == 0){
            itens[indice].quantidade.erase(itens[indice].quantidade.begin()+i);
            itens[indice].validade.erase(itens[indice].validade.begin()+i);
        }
    }
    new_mercadoria.quantidade.push_back(1);
    new_mercadoria.valor = num_unidades * itens[indice].valor; // Nova mercadoria construída.

    for(size_t i = 0; i<itens.size(); i++){
        string busca = new_mercadoria.nome_produto;
        size_t posicao = itens[i].nome_produto.find(busca);
        if(!(posicao == string::npos)){
            for(size_t j = 0; j<new_mercadoria.quantidade.size(); j++){
                itens[i].quantidade.push_back(new_mercadoria.quantidade[j]);
                itens[i].validade.push_back(new_mercadoria.validade[j]);
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

    for(size_t i = 0; i<comprador.size(); i++){
        if(comprador[i].nome_cliente == nome_search){
            ind = i;
        }
    }
    if(ind == -1.0){
        cout << "Nome não encontrado.\n" << "Verifique se o nome foi digitado incorretamente.";
        return ind;
    }else{
        for(size_t i = 0; i<comprador[ind].itens.size(); i++){
            for(size_t j = 0; j<comprador[ind].itens[i].quantidade.size(); i++){
                valor_conta += comprador[ind].itens[i].quantidade[j] * comprador[ind].itens[i].valor;
            }
        }
    }

    return valor_conta;
}//Retorna o que o cliente deve.

float receita(vector<mercadoria> itens_Co, vector<mercadoria> itens_Ve, vector<cliente> compradores){
    float venda = 0.0, compra = 0.0, venda_cli_arm = 0.0;

    for(size_t i = 0; i<itens_Co.size(); i++){
        for(size_t j = 0; j < itens_Co[i].quantidade.size(); j++){
            compra += itens_Co[i].quantidade[j] * itens_Co[i].valor;
        }
    }
    for(size_t i = 0; i<itens_Ve.size(); i++){
        for(size_t j = 0; j < itens_Ve[i].quantidade.size(); j++){
            venda += itens_Ve[i].quantidade[j] * itens_Ve[i].valor;
        }
    }
    for(size_t i = 0; i<compradores.size(); i++){
        for(size_t j = 0; j<compradores[i].itens.size(); j++){
            for(size_t k = 0; k<compradores[i].itens[j].quantidade.size(); k++){
                venda_cli_arm += compradores[i].itens[j].quantidade[k] * compradores[i].itens[j].valor;
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
        comparador_data(itens_p_verific[i].validade, comp_data);
        for(size_t j = 0; j<comp_data.size(); j++){
            if(comp_data[j] <= 0){
                cout << "O produto " << itens_p_verific[i].nome_produto << " com a validade ";
                cout << itens_p_verific[i].validade[j].day << "/"  << itens_p_verific[i].validade[j].mon << "/" << itens_p_verific[i].validade[j].year;
                cout << "possui " <<  itens_p_verific[i].quantidade[j] << "unidades vencidas";
                return;
            }
        }
    }
}

void extracao_arq_prodCO(vector<mercadoria>& itens_p_arm, const string& nome_arq){
    string nome_p_prod;
    vector<int> num_quant;
    int quant_pacot;
    float preco;
    vector<tdata> dat_ven;
    int tam, num_quantidade;

    fstream arq_merc(nome_arq, fstream::in);
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
}

void extracao_arq_cli(vector<cliente>& client_p_arm, const string& nome_arq){
    string nome_cl;
    int tam_itens;
    int tam_data;

    fstream arq_cli(nome_arq, fstream::in);
    if (!arq_cli.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }
    while(arq_cli >> nome_cl >> tam_itens){
        cliente cli;
        cli.nome_cliente = nome_cl;

        for(int i=0; i<tam_itens; i++){
            mercadoria itens_C;
            int tam;
            arq_cli >> itens_C.nome_produto >> tam >> itens_C.valor >> itens_C.unidade_pacote >> tam_data;

            for(int j = 0; j < tam; j++){
                int quantidade_prod;
                arq_cli >> quantidade_prod;
                itens_C.quantidade.push_back(quantidade_prod);
            }

            for(int j = 0; j<tam_data ; j++){
                tdata data;
                arq_cli >> data.day >> data.mon >> data.year;
                itens_C.validade.emplace_back(data);
            }
            cli.itens.emplace_back(itens_C);
        }
        client_p_arm.emplace_back(cli);
    }
    arq_cli.close();
    cout << "A extração de dados foi realizada com sucesso" << endl;
}//O embace_beck foi usado para adicionar diretamente, no vector cliente, o vector objeto mercadoria.

bool compra(vector<mercadoria>& mercadoria_CO, vector<mercadoria>& mercadoria_VE, string& nome_dproduto, int unidades_compradas, tdata data_dprod){
    int ind = -1, ind_d = -1; 
    mercadoria item_VE = mercadoria();

    for(size_t i = 0; i<mercadoria_CO.size(); i++){
        string busca = nome_dproduto;
        size_t posicao = mercadoria_CO[i].nome_produto.find(busca);
        if(!(posicao == string::npos)){
            ind = i;
            item_VE.nome_produto = mercadoria_CO[i].nome_produto;
            item_VE.valor = mercadoria_CO[i].valor * 1.2;
            item_VE.unidade_pacote = mercadoria_CO[i].unidade_pacote;
        }
        for(size_t j = 0; j<mercadoria_CO[i].validade.size(); j++){
            if(mercadoria_CO[i].validade[j].day + mercadoria_CO[i].validade[j].mon + mercadoria_CO[i].validade[j].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                ind_d = j;
                item_VE.validade.push_back(mercadoria_CO[i].validade[j]);
            }
        }
    }
    if(ind == -1 || ind_d == -1){
        cout << "A mercadoria não foi registrada ou foi escrita incorretamente." << endl;
        cout << "Redirecionando ao Menu...";
        return false;
    }
    if(((mercadoria_CO[ind].validade[ind_d].day <=0) && (mercadoria_CO[ind].validade[ind_d].mon <=0) && (mercadoria_CO[ind].validade[ind_d].year <=0)) || (mercadoria_CO[ind].validade[ind_d].year < 0)){
        cout << "A mercadoria que está prestes a ser vendida está vencida!" << endl;
        cout << "Verifique o produto imediatamente!";
        return false;
    }

    int quant = mercadoria_CO[ind].quantidade[ind_d];
    if(quant - unidades_compradas < 0){
        cout << "Não há unidades suficiente para a compra.\n";
        cout << "Verifique o nome ou a data digitados.";
        return false;
    }else{
        for(size_t i = 0; i<mercadoria_VE.size(); i++){
            if(mercadoria_VE[i].nome_produto == mercadoria_CO[ind].nome_produto){
                for(size_t j = 0; j<mercadoria_VE[i].validade.size(); j++){
                    if(mercadoria_VE[i].validade[j].day + mercadoria_VE[i].validade[j].mon + mercadoria_VE[i].validade[j].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                        mercadoria_VE[i].quantidade[j] += unidades_compradas;
                    }
                }
                return true;
            }
        }
        if(quant - unidades_compradas == 0){
            mercadoria_CO[ind].quantidade[ind_d] -= unidades_compradas;
            item_VE.quantidade.push_back(unidades_compradas);
            cout << "O produto: " << mercadoria_CO[ind].nome_produto << " com a validade " << mercadoria_CO[ind].validade[ind_d].day << "/" << mercadoria_CO[ind].validade[ind_d].mon << "/" << mercadoria_CO[ind].validade[ind_d].year << "está em falta.";
            return true;
        }else{
            mercadoria_CO[ind].quantidade[ind_d] -= unidades_compradas;
            item_VE.quantidade.push_back(unidades_compradas);
            cout << " com a validade " << mercadoria_CO[ind].validade[ind_d].day << "/" << mercadoria_CO[ind].validade[ind_d].mon << "/" << mercadoria_CO[ind].validade[ind_d].year;
            cout << " ficou com " << mercadoria_CO[ind].quantidade[ind_d] << " unidades" << endl;
            return true;
        }
    }
}

void transf(string& nome){
    transform(nome.begin(), nome.end(), nome.begin(), [](unsigned char c){return tolower(c);});
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

void menu(){
    vector<cliente> consumidor, consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    fstream arq_Cliente, arq_Cliente_beckup, arq_mercadoria, arq_mercadoria_beckup;
    string resp;

    if(((verificador_arquivo("Clientes.txt") == false) && (verificador_arquivo("Clientes_beckup.txt") == false)) && ((verificador_arquivo("Mercadorias.txt") == false) && (verificador_arquivo("Mercadoria_beckup.txt") == false))){
        cout << "Seja bem-vindo!" << endl;
    }else{
        cout << "Seja bem-vindo novamente!" << endl;
        if(verificador_arquivo("Mercadorias.txt") == false){
            cout << "O arquivo Mercadorias.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            copiararq("Mercadoria_beckup.txt", "Mercadorias.txt");
            if(verificador_arquivo("Mercadorias.txt") == true){
                extracao_arq_prodCO(merc_CO, "Mercadorias.txt");
                verfic_venc(merc_CO);
            }
        }else{
            extracao_arq_prodCO(merc_CO, "Mercadorias.txt");
            verfic_venc(merc_CO);
        }
        if(verificador_arquivo("Clientes.txt") == false){
            cout << "O arquivo Clientes.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            copiararq("Clientes_beckup.txt", "Clientes.txt");
            extracao_arq_cli(consumidor_amigo, "Clientes.txt");
        }else{
            extracao_arq_cli(consumidor_amigo, "Clientes.txt");
        }
        resp_saida(resp);
    }

    int alternativa;

    do{
        do{
            cout << "Selecione a sua próxima ação (escreva o número da opção):" << endl;
            cout << "1 - Registrar cliente; \n" << "2 - Registrar produto; \n" << "3 - Excluir cliente; \n";
            cout << "4 - Excluir produto; \n" << "5 - Abrir lista de produto; \n" << "6 - Abrir lista de cliente; \n" << "7 - Verificar validade dos produtos; \n";
            cout << "8 - Realizar beckup da nova lista de produtos; \n" << "9 - Realizar beckup da nova lista de clientes; \n";
            cout << "10 - Utilizar beckup de produtos mais recente; \n" << "11 - Utilizar beckup de clientes mais recente; \n";
            cout << "12 - Saber conta de cliente; \n" << "13 - Realizar conta; \n" << "14 - Separar produto; \n" << "15 - Agrupar unidades; \n";
            cout << "16 - Realizar receita; \n" << "17 - Alterar cliente; \n" << "18 - Alterar produto; \n" << "19 - Tutorial; \n";
            cout << "20 - Sair. \n" << endl;
            cout << "Digite a sua resposta: ";
            cin >> alternativa;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida. Tente novamente." << endl;
            }else{
                if(alternativa <= 0 || alternativa > 20){
                    cout << "Opção inválida, tente novamente." << endl;
                }
            }
        }while(alternativa <= 0 || alternativa > 20 || cin.fail());

        //system("cls");
        cout << "você escolheu:" << alternativa << endl;

        string resposta = "sim", nome_dproduto = "void", nome_dcliente = "void";
        cliente comprador_atual = cliente();
        mercadoria merc_comprada = mercadoria();
        tdata data_dprod = {0,0,0};
        int ind = -1;

        switch(alternativa){
            cout << "tá aqui_1";
            case 1:{
                cout << "Aqui é a área de registro de cliente. \n" << endl;
                int tam, unidades_vend; ind = -1;
                nome_dproduto.clear(); resposta.clear();
                
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                comprador_atual = cliente();
                system("cls");
                if(merc_CO.empty()){
                    cerr << "Não há mercadoria registrada, por favor, registre o(os) produto(s) primeiro." << endl;
                    break;
                }
                system("cls");
                cout << "Aqui é a área de registro de cliente. \n" << endl;
                cout << "O cliente possui as seguintes caracterísicas: ";
                cout << " - Nome; \n - Número de produtos comprados; \n - Produtos comprados; \n - Data de vencimento do produto \n" << endl;
                cout << "Digite o nome: "; cin.ignore(); getline(cin, comprador_atual.nome_cliente); cout << endl; transf(comprador_atual.nome_cliente);
                for(size_t i = 0; i<consumidor_amigo.size(); i++){
                    if(consumidor_amigo[i].nome_cliente == comprador_atual.nome_cliente){
                        cout << "Este nome já é cadastrado no nosso sistema." << endl;
                        cout << "Sugerimos que vá para a área 'Realizar conta'." << endl;
                        cout << "Redirecionando para o menu...";
                        break;
                    }
                }
                cout << "Digite quantos produtos o cliente pegou: "; cin >> tam; cout << endl;
                cout << "Digite o nome, a quantidade de unidades vendidas e a data de vencimento (no formato dia/mes/ano) deste produto." << endl;
                for(int i = 0; i<tam; i++){
                    unidades_vend = 0; ind = 0;

                    cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                    cin >> unidades_vend;
                    for(size_t j = 0; j<merc_CO.size(); j++){
                        if(merc_CO[i].nome_produto == nome_dproduto){
                            ind = j;
                        }
                    }
                    if(ind == -1){
                        cout << "Mercadoria não encontrada. Por favor, verifique se a mercadoria existe ou se você escreveu incorretamente.\n";
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...";
                        }
                        break;
                    }

                    cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                    compra(merc_CO, merc_VE, nome_dproduto, unidades_vend, data_dprod);
                    ind = indice_prod(merc_VE, nome_dproduto);
                    comprador_atual.itens.push_back(merc_VE[ind]);
                }
                consumidor_amigo.push_back(comprador_atual);
                cout << "Cliente cadastrado com sucesso!";
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 2:{
                cout << "Aqui é a área de registro de produto." << endl;
                nome_dproduto.clear();
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                merc_comprada = mercadoria();
                int quant;
                resposta.clear();
                system("cls");
                cout << "Aqui é a área de registro de produto." << endl;
                cout << "A Mercadoria possui as seguintes características: " << endl;
                cout << " - Nome;\n" << " - Quantidade de itens;\n" << " - Quantidade por pacote;\n" << " - Valor (em reais);\n" << " - Validade.\n";
                cout << "Agora, registre a mercadoria:\n";
                cout << "Nome da mercadoria: "; cin >> merc_comprada.nome_produto; cout << endl; transf(merc_comprada.nome_produto);
                cout << "Quantidade de mercadoria: "; cin >> quant; merc_comprada.quantidade.push_back(quant); cout << endl;
                cout << "Quantidade de unidades por fardo: "; cin >> merc_comprada.unidade_pacote; cout << endl;
                cout << "Valor da mercadoria (em reais): "; cin >> merc_comprada.valor;
                cout << "Data de validade (no formato dia/mês/ano): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year; merc_comprada.validade.push_back(data_dprod); cout << endl;

                for(size_t i = 0; i<merc_CO.size(); i++){
                    string busca = merc_CO[i].nome_produto;
                    size_t posicao = merc_CO[i].nome_produto.find(busca);
                    if(!(posicao == string::npos)){
                        cout << "Uma mercadoria já está registrada no nosso sistema.\n Caso for para adicionar unidades, sugerimos que vá para a área 'Alterar produto'.\n";
                        cout << "Por encontrar um produto já cadastrado, iremos te redirecionar para o menu." <<endl;
                        cout << "Redirecionando ao menu...";
                        break;
                    }
                }
                merc_CO.push_back(merc_comprada);
                cout << "Registro completo!";

                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 3:{
                cout << "Aqui é a área para excluir um cliente. \n";
                nome_dcliente.clear();
                if(verific(resposta)){
                    system("cls");
                    break;
                }   
                system("cls");
                cout << "Aqui é a área para excluir um cliente. \n";
                cout << "Digite o nome do cliente corretamente: ";
                cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);

                for(size_t i = 0; i<consumidor_amigo.size(); i++){
                    if(consumidor_amigo[i].nome_cliente == nome_dcliente){
                        ind = i;
                        cout << "Cliente encontrado!" << endl;
                        if(!(consumidor_amigo[i].itens.empty())){
                            cout << "Você não pode deletar este cliente, pois apresenta itens comprados!\n";
                            cout << "Caso o cliente tenha pagado tudo, sugerimos que vá primeiro para a área 'Alterar cliente' e depois retorne.\n";
                            cout << "Redirecionando ao Menu...";
                            break;
                        }else{
                            resposta.clear();
                            cout << "Deseja deletar o cliente " << consumidor_amigo[i].nome_cliente << "?";
                            cin >> resposta; transform(resposta.begin(), resposta.end(), resposta.begin(), [](unsigned char c){return tolower(c);});
                            if(resposta == "sim"){
                                cout << "Cliente " << consumidor_amigo[i].nome_cliente << "deletado!";
                                consumidor_amigo.erase(consumidor_amigo.begin() + i);
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...";
                                    break;
                                }
                            }else{
                                cout << "Certo, redirecionando ao menu...";
                            }
                        }
                    }
                }

                if(ind == -1){
                    cout << "Cliente não encontrado. Por favor, verifique se o nome foi digitado corretamente.";
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 4:{
                cout << "Aqui é a área de exclusão de produto.\n";
                nome_dproduto.clear();
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                system("cls");
                cout << "Aqui é a área de exclusão de produto.\n";
                cout << "Digite o nome do produto: ";
                cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);

                int soma = 0;    
                for(size_t i = 0; i<merc_CO.size(); i++){
                    if(merc_CO[i].nome_produto == nome_dproduto){
                        ind = i;
                        cout << "Produto encontrado!" << endl;
                        for(int quant : merc_CO[i].quantidade){
                            soma += quant;
                        }
                        if(soma != 0){
                            cout << "O produto apresenta unidades. Caso ele, na realidade, não possua mais unidades, sugerimos que vá para a área 'Alterar produto'.";
                            cout << endl << "Redirecionando para o Menu...";
                            sleep(3);
                            break;
                        }else{
                            resposta.clear();
                            cout << "Deseja deletar o produto " << merc_CO[i].nome_produto << "?\n";
                            cin >> resposta; transform(resposta.begin(), resposta.end(), resposta.begin(), [](unsigned char c){return tolower(c);});
                            if(resposta == "sim"){
                                cout << "Produto " << merc_CO[i].nome_produto << "deletado!\n";
                                merc_CO.erase(merc_CO.begin()+i);
                                cout << "Retornando para o menu...";
                                sleep(3); break;
                            }else{
                                 cout << "Certo, redirecionando ao menu...";
                            }
                        }
                    }
                }
                if(ind == -1){
                    cout << "Produto não encontrado. Por favor, verifique se o nome foi digitado corretamente.";
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 5:{
                if(verific(resposta)){
                    system("cls");
                    menu();
                }
            break;
            }
            case 6:{
                if(verific(resposta)){
                    system("cls");
                    menu();
                }
            break;
            }
            case 7:{
                cout << "Aqui é a área de validades.\n";
                if(verific(resposta)){
                    system("cls");
                    menu();
                }
                if(merc_CO.empty()){
                    cout << "Não há produtos registrados!" <<endl;
                    if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    }
                    break;
                }
                system("cls"); nome_dproduto.clear(); resposta.clear();
                vector<double> diff_data;
                cout << "Aqui é a área de validades.\n";
                cout << "Deseja saber a validade de um único produto? (sim/não)\n";
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    cout << "Digite o nome do produto que deseja ver a validade: "; cin.ignore(); getline(cin, nome_dproduto); transf(resposta);
                    int ind_dprod = indice_prod(merc_CO,nome_dproduto);
                    if(ind_dprod < 0){
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...";
                        }
                        break;
                    }
                    comparador_data(merc_CO[ind_dprod].validade, diff_data);
                    for(size_t i = 0; i<diff_data.size(); i++){
                        if(diff_data[i] <= 0){
                            cout << "O produto " << merc_CO[ind_dprod].nome_produto << "apresenta " << merc_CO[ind_dprod].quantidade[i] << "unidades venciadas.\n";
                        }else{
                            cout << merc_CO[ind_dprod].nome_produto << "(" << merc_CO[ind_dprod].validade[i].day << "/" << merc_CO[ind_dprod].validade[i].mon << "/" << merc_CO[ind_dprod].validade[i].year << ")";
                            cout << "||";
                            cout << "Há " << merc_CO[ind_dprod].quantidade[i] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                        }
                    }
                }else{
                    cout << "Aqui estão as validades e o tempo (em dias) até o vencimento:\n";                
                    for(size_t i = 0; i<merc_CO.size(); i++){
                        comparador_data(merc_CO[i].validade, diff_data);
                        for(size_t j = 0; i<diff_data.size(); i++){
                            if(diff_data[j] <= 0){
                                cout << "O produto " << merc_CO[i].nome_produto << "apresenta " << merc_CO[i].quantidade[j] << "unidades venciadas.\n";
                            }else{
                                cout << merc_CO[i].nome_produto << " (" << merc_CO[i].validade[j].day << "/" << merc_CO[i].validade[j].mon << "/" << merc_CO[i].validade[j].year << ")";
                                cout << " || ";
                                cout << "Há " << merc_CO[i].quantidade[j] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                            }
                        }
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 8:{
                cout << "Aqui é a área de beckup da lista de produto.\n";
                if(verific(resposta)){
                    system("cls");
                    menu();
                }
                resposta.clear();
                system("cls");
                cout << "Aqui é a área de beckup da lista de produto.\n";
                cout << "Deseja salvar a lista de produtos no arquivo de beckup? (sim/não)\n";
                cin >> resposta; transf(resposta);

                if(resposta != "não"){
                    if(verificador_arquivo("Mercadorias.txt")){
                        cout << "O beckup será realizado, aguarde um momento.\n";
                        if(copiararq("Mercadorias.txt","Mercadoria_beckup.txt")){
                            cout << "Beckup realizado com sucesso.!\n";
                        }else{
                            cout << "Infelizmente houve um erro durante o processo.\n";
                            cout << "Redirecionando para o menu...";
                            if(resp_saida(resposta)){
                                cout << "Retornando ao menu...";
                            }
                            break;
                        }
                    }else{
                        cout << "A lista de produtos está vazia. Por favor, registre os itens primeiro.\n";
                        cout << "Redirecionando para o Menu...";
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...";
                        }
                        break;
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 9:{
                cout << "Aqui é a área de beckup da lista de clientes.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                resposta.clear();
                system("cls");
                cout << "Aqui é a área de beckup da lista de clientes.\n";
                cout << "Deseja salvar a lista de clientes no arquivo de beckup? (sim/não)\n";
                cin >> resposta; transf(resposta);

                if(resposta != "não"){
                    if(verificador_arquivo("")){
                        cout << "O beckup será realizado, aguarde um momento.\n";
                        if(copiararq("Clientes.txt","Clientes_beckup.txt")){
                            cout << "Beckup realizado com sucesso.!\n";
                        }else{
                            cout << "Infelizmente houve um erro durante o processo.\n";
                        }
                    }else{
                        cout << "A lista de clientes está vazia. Por favor, registre os clientes primeiro.\n";
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 10:{
                cout << "Aqui é a área para utilizar o beckup de produtos. \n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }   
                system("cls");
                resposta.clear();
                cout << "Aqui é a área para utilizar o beckup de produtos. \n";
                cout << "Caso o beckup seja utilizado, as informações alteradas na lista de produtos serão peridas.\n";
                cout << "Deseja utilizar o beckup? (sim/não)\n";
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    cout << "Aguarde um momento...\n";
                    if(copiararq("Mercadoria_beckup.txt","Mercadorias.txt")){
                        cout << "Beckup feito com sucesso!\n";
                        sleep(2); system("cls");
                        break;
                    }
                }else{
                    cout << "Redirecionando para o menu...";
                    sleep(2); system("cls");
                }

            break;
            }
            case 11:{
                cout << "Aqui é a área para utilizar o beckup de clientes. \n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }   
                system("cls");
                cout << "Aqui é a área para utilizar o beckup de clientes. \n";
                cout << "Caso o beckup seja utilizado, as informações alteradas na lista de clientes serão peridas.\n";
                cout << "Deseja utilizar o beckup? (sim/não)\n";
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    cout << "Aguarde um momento...\n";
                    if(copiararq("Clientes_beckup.txt","Clientes.txt")){
                        cout << "Beckup feito com sucesso!\n";
                        sleep(2); system("cls");
                        break;
                    }
                }else{
                    cout << "Redirecionando para o menu...";
                    sleep(2); system("cls");
                }
            break;
            }
            case 12:{
                cout << "Aqui é a área para realizar a conta do cliente. \n";
                if(verific(resposta)){
                    system("cls");
                    menu();
                }

                system("cls");
                cout << "Aqui é a área para saber a conta do cliente, ou seja, quanto o cliente cadastrado deve. \n";
                cout << "Digire o nome do cliente: ";
                cin >> nome_dcliente; transf(nome_dcliente);
                float soma = conta_cliente(consumidor_amigo, nome_dcliente);
                if(soma == -1.0){
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...";
                    }
                    break;
                }else{
                    cout << "O valor da conta do cliente " << nome_dcliente << "é de: R$" << soma << endl; 
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 13:{
                cout << "Aqui é a área para realizar a conta da compra.\n";
                if(verific(resposta)){
                    system("cls");
                    menu();
                }

                system("cls");
                int quant_itens, unid_comp; nome_dproduto.clear();
                float valor_conta = 0.0, pagamento_cl, troco;
                cout << "Aqui é a área para realizar a conta da compra.\n";
                cout << "OBS: caso os produtos do mesmo item possuirem datas de validade diferentes, digite a quatidade de produtos de cada validade.\n";
                cout << "Digite quantos tipos de itens o cliente comprou: ";
                cin >> quant_itens;
                system("cls");
                for(int i = 0; i < quant_itens; i++){
                    cout << "Aqui é a área para realizar a conta da compra.\n";
                    do{
                        cout << "Digite o nome do produto, a quantidade comprada deste produto: ";
                        cin.ignore(); getline(cin,nome_dproduto); transf(nome_dproduto);
                        cin >> unid_comp;
                        if((indice_prod(merc_CO, nome_dproduto)) < 0){
                            cout << "Produto não encontrado!" << endl;
                            cout << "Tente novamente.";
                            system("cls");
                        }
                    }while((indice_prod(merc_CO, nome_dproduto)) < 0);
                    cout << "Digite a data de validade deste produto (d/m/a): ";
                    cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                    compra(merc_CO, merc_VE, nome_dproduto, unid_comp, data_dprod);
                    valor_conta += merc_CO[indice_prod(merc_CO, nome_dproduto)].valor;
                }

                cout << "Valor da conta: R$" << valor_conta << endl;
                resposta.clear();
                do{
                    cout << "Digite quanto o cliente pagou (em reais): "; cin >> pagamento_cl; 
                    troco = pagamento_cl - valor_conta;
                    if(troco > 0.0){
                        cout << "Valor do troco (em reais):" << troco << endl;
                    }else{
                        if(troco == 0){
                            cout << "Não há troco." << endl;
                        }else{
                            cout << "O cliente não pagou tudo, cobre-o." << endl;
                            cout << "O cliente está devendo (em reais): " << -troco << endl;
                        }
                    }
                    cout << "Digite 'sim' para sair." << endl; cin >> resposta; transf(resposta);
                    if(resposta == "sim"){
                        cout << "Se o cliente não pagou, cadastre-o para controlar suas finâncias." << endl;
                        cout << "Após o cadastro, vá em 'alterar cliente' para diminuir o valor da compra." << endl;
                    }
                }while((troco < 0) || resposta != "sim");
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 14:{
                cout << "Aqui é a área de separação de produto.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                int quant_sep;
                system("cls"); nome_dproduto.clear();
                cout << "Aqui é a área de separação de produto.\n";
                cout << "Digite o nome do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                cout << "\nDigite quantas vezes essa separação ocorrerá: "; cin >> quant_sep;
                cout << "\nDigite a data de validade do produto a ser desagrupado (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                if(desagrupador(merc_CO, indice_prod(merc_CO, nome_dproduto), quant_sep, data_dprod)){
                    if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    }
                }  
            break;
            }
            case 15:{
                cout << "Aqui é a área para agrupar produtos de um único nome.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                system("cls"); nome_dproduto.clear();
                cout << "Aqui é a área para agrupar produtos de um único nome.\n";
                cout << "Digite o nome do produto corretamente: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);

                agrupador(merc_CO, indice_prod(merc_CO, nome_dproduto));
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 16:{
                cout << "Aqui é a área da receita.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                system("cls");
                cout << "Aqui é a área da receita.\n";
                cout << "Só será possível saber a receita total, ou seja, não há como escolher perídos para analisar!\n";
                cout << "A sua receita é de: R$" << receita(merc_CO, merc_VE, consumidor_amigo) << endl;

                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 17:{
                cout << "Aqui é a área para alterar o cliente.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                
                if(consumidor_amigo.empty()){
                    cout << "Não há cliente cadastrado." << endl;
                    break;
                }

                int opcao;
                system("cls"); nome_dcliente.clear(); ind = -1;
                cout << "Aqui é a área para alterar o cliente.\n";
                cout << "Digite o nome do cliente que será alterado: "; cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);
                for(size_t i = 0; i<consumidor_amigo.size(); i++){
                    if(consumidor_amigo[i].nome_cliente == nome_dcliente){
                        ind = i;
                        cout << "Cliente encontrado!" << endl;
                        sleep(2);system("cls");
                        cout << "Escolha uma das opções abaixo para modificar:" << endl;
                        do{
                            do{
                                cout << " 1 - Nome;\n" << " 2 - Lista de compras;\n" << " 3 - Pagamento;\n" << " 4 - Sair.\n" << endl;
                                cin >> opcao;
                                if((opcao < 1) || (opcao > 4)){
                                    cout << "Opção inválida, tente novamente.\n";
                                }
                            }while((opcao < 1) || (opcao > 4));

                            system("cls");
                            switch(opcao){
                                case 1:{
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }

                                    nome_dcliente.clear();
                                    cout << "Digite o novo nome do cliente: "; cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);
                                    for(size_t j = 0; j<consumidor_amigo.size(); j++){
                                        if(consumidor_amigo[j].nome_cliente == nome_dcliente){
                                            cout << "O nome digitado já está cadastrado no nosso sistema!\n";
                                            cout << "Tente novamente.\n";
                                            if(resp_saida(resposta)){
                                                cout << "Retornando à escolha das alterações...";
                                            }
                                            break;
                                        }
                                    }
                                    consumidor_amigo[i].nome_cliente = nome_dcliente;
                                    cout << "Alteração de nome concluída!\n";
                                    if(resp_saida(resposta)){
                                        cout << "Retornando à escolha das alterações...";
                                    }
                                break;
                                }
                                case 2:{
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }
                                    
                                    int quanti_itens, unid_compradas, ind_data = -1;
                                    cout << "Quantos tipos de itens por data de validade o cliente comprou?";
                                    cin >> quanti_itens; cout << endl;
                                    for(int j = 0; j < quanti_itens; j++){
                                        do{
                                            nome_dproduto.clear(); ind = -1; merc_comprada.nome_produto.clear(); data_dprod = {0,0,0};
                                            cout << "Digite o nome do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                                            for(size_t k = 0; k<merc_CO.size(); k++){
                                                if(merc_CO[k].nome_produto == nome_dproduto){
                                                    ind = k;
                                                    cout << "Produto encontrado!\n";
                                                    merc_comprada.nome_produto == nome_dproduto;
                                                    merc_comprada.valor = merc_CO[k].valor * 1.2;
                                                    merc_comprada.unidade_pacote = merc_CO[k].unidade_pacote;
                                                }
                                            }
                                            if(ind == -1){
                                                cout << "Produto não encontrado. Por favor, tente novamente.\n";
                                            }
                                        }while(ind == -1);
                                        cout << "Digite quantas unidades o cliente comprou: "; cin >> unid_compradas;
                                        cout << "Digite a data de validade (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                                        merc_comprada.validade.push_back(data_dprod); 
                                        
                                        if(!(compra(merc_CO, merc_VE, nome_dproduto, unid_compradas, data_dprod))){
                                            cout << "Por favor, verifique a lista de itens e a atualize se necessário." << endl;
                                            if(resp_saida(resposta)){
                                                cout << "Retornando à escolha das alterações...";
                                            }
                                            break;
                                        }

                                        ind = -1;
                                        for(size_t k = 0; k<consumidor_amigo[i].itens.size(); k++){
                                            if(consumidor_amigo[i].itens[k].nome_produto == nome_dproduto){
                                                ind = k;
                                                for(size_t L = 0; L < consumidor_amigo[i].itens[k].validade.size(); L++){
                                                    if(consumidor_amigo[i].itens[k].validade[L].day + consumidor_amigo[i].itens[k].validade[L].mon + consumidor_amigo[i].itens[k].validade[L].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                                                        ind_data = L;
                                                        consumidor_amigo[i].itens[k].quantidade[L] += unid_compradas;
                                                    }
                                                }
                                            }
                                        }
                                        if(ind == -1){
                                            consumidor_amigo[i].itens.push_back(merc_comprada);
                                        }
                                        if((ind != -1) && (ind_data == -1)){
                                            consumidor_amigo[i].itens[ind].validade.push_back(data_dprod);
                                            consumidor_amigo[i].itens[ind].quantidade.push_back(unid_compradas);
                                        }
                                    }
                                    cout << "Cliente atualizado!\n";
                                    if(resp_saida(resposta)){
                                        cout << "Retornando à escolha das alterações...";
                                    }
                                break;
                                }
                                case 3:{
                                    cout << "Aqui é a área de pagamento.\n";
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }

                                    system("cls");
                                    float pagamento = 0.0;
                                    mercadoria merc_troca;
                                    size_t tam_quant;
                                    cout << "Digite quanto o cliente pagou (em reais): "; cin >> pagamento;
                                    for(size_t j = 0; j<consumidor_amigo[i].itens.size(); j++){
                                        for(size_t k = 0; k < consumidor_amigo[i].itens.size()-1-j; k++){
                                            if(consumidor_amigo[i].itens[k].valor > consumidor_amigo[i].itens[k+1].valor){
                                                merc_troca.nome_produto = consumidor_amigo[i].itens[k].nome_produto;
                                                for(size_t L = 0; L < consumidor_amigo[i].itens[k].quantidade.size(); L++){
                                                    merc_troca.quantidade.push_back(consumidor_amigo[i].itens[k].quantidade[L]);
                                                    merc_troca.validade.push_back(consumidor_amigo[i].itens[k].validade[L]);
                                                }
                                                merc_troca.unidade_pacote = consumidor_amigo[i].itens[k].unidade_pacote;
                                                merc_troca.valor = consumidor_amigo[i].itens[k].valor;

                                                int x = consumidor_amigo[i].itens[k].quantidade.size();
                                                int y = consumidor_amigo[i].itens[k+1].quantidade.size();

                                                if(x - y < 0){
                                                    tam_quant = consumidor_amigo[i].itens[k+1].quantidade.size();
                                                }else{
                                                    tam_quant = consumidor_amigo[i].itens[k].quantidade.size();
                                                }
                                                consumidor_amigo[i].itens[k].nome_produto = consumidor_amigo[i].itens[k+1].nome_produto;

                                                for(size_t L = 0; L < tam_quant; L++){
                                                    consumidor_amigo[i].itens[k].quantidade.push_back(consumidor_amigo[i].itens[k+1].quantidade[L]);
                                                    consumidor_amigo[i].itens[k].validade.push_back(consumidor_amigo[i].itens[k+1].validade[L]);
                                                }
                                                consumidor_amigo[i].itens[k].valor = consumidor_amigo[i].itens[k+1].valor;
                                                consumidor_amigo[i].itens[k].unidade_pacote = consumidor_amigo[i].itens[k+1].unidade_pacote;

                                                consumidor_amigo[i].itens[k+1].nome_produto = merc_troca.nome_produto;
                                                for(size_t L = 0; L < merc_troca.quantidade.size(); L++){
                                                    consumidor_amigo[i].itens[k+1].quantidade.push_back(merc_troca.quantidade[L]);
                                                    consumidor_amigo[i].itens[k+1].validade.push_back(merc_troca.validade[L]);
                                                }
                                                consumidor_amigo[i].itens[k+1].valor = merc_troca.valor;
                                                consumidor_amigo[i].itens[k+1].unidade_pacote = merc_troca.unidade_pacote;
                                            }
                                        }
                                    }
                                    int n = -1;
                                    float soma_comp = 0.0;
                                    mercadoria merc_resto;
                                    for(size_t j = 0; j<consumidor_amigo[i].itens.size(); j++){
                                        for(size_t k = 0; k < consumidor_amigo[i].itens[j].quantidade.size(); k++){
                                            while((consumidor_amigo[i].itens[j].quantidade[k] != 0) && (pagamento > 0)){
                                                consumidor_amigo[i].itens[j].quantidade[k]--;
                                                pagamento -= consumidor_amigo[i].itens[j].valor;
                                                if(pagamento < 0){
                                                    for(size_t L=0; L<consumidor_amigo[i].itens.size(); L++){
                                                        size_t posicao = consumidor_amigo[i].itens[L].nome_produto.find("Resto");
                                                        if(!(posicao == string::npos)){
                                                            n = L;
                                                            consumidor_amigo[i].itens[L].valor += -pagamento;
                                                        }
                                                    }
                                                    if(n == -1){
                                                        merc_resto.nome_produto = "Resto";
                                                        merc_resto.quantidade.push_back(1);
                                                        merc_resto.unidade_pacote = 1;
                                                        merc_resto.valor = -pagamento;
                                                        merc_resto.validade.push_back({0,0,0});
                                                        consumidor_amigo[i].itens.push_back(merc_resto);
                                                    }
                                                }
                                            }
                                            if(pagamento <= 0){
                                                break;
                                            }
                                        }
                                        if(pagamento <= 0){
                                            break;
                                        }
                                    }
                                    for(size_t j = 0; j<consumidor_amigo[i].itens.size(); j++){
                                        for(size_t k = 0; k < consumidor_amigo[i].itens[j].quantidade.size(); k++){
                                            soma_comp += consumidor_amigo[i].itens[j].quantidade[k];
                                        }
                                        if(soma_comp == 0){
                                            consumidor_amigo[i].itens.erase(consumidor_amigo[i].itens.begin()+j);
                                            j=0;
                                        }else{
                                            for(size_t k = 0; k < consumidor_amigo[i].itens[j].quantidade.size(); k++){
                                                if(consumidor_amigo[i].itens[j].quantidade[k] == 0){
                                                    consumidor_amigo[i].itens[j].quantidade.erase(consumidor_amigo[i].itens[j].quantidade.begin()+k);
                                                    consumidor_amigo[i].itens[j].validade.erase(consumidor_amigo[i].itens[j].validade.begin()+k);
                                                    k=0;
                                                }
                                            }
                                        }
                                        soma_comp = 0.0;
                                    }
                                    cout << "pagamento realizado!" << endl;
                                    cout << "Agora o cliente está devendo (em reais): R$" << conta_cliente(consumidor_amigo, consumidor_amigo[i].nome_cliente);
                                    if(resp_saida(resposta)){
                                        cout << "Retornando à escolha das alterações...";
                                    }
                                break;
                                }
                                case 4:
                                    if(resp_saida(resposta)){
                                        cout << "Saindo da tela de alterações..." << endl;
                                    }
                                break;
                            }
                        }while(opcao != 4);
                    }
                }

                if(ind == -1){
                    cout << "Cliente não encontrado. Por favor, verifique se o nome está cadastrado ou se foi digitado corretamente.\n";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                    }
                }
            break;
            }
            case 18:{
                cout << "Aqui é a área para alterar o produto.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                if(merc_CO.empty()){
                    cout << "Não há mercadorias cadastradas. Por favor, cadastre primeiro." << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                    }
                    break;
                }
                system("cls"); nome_dproduto.clear(); ind = -1;
                int opcao, ind_comp = -1;

                string novo_nome;
                int nova_quant;
                int nova_unidppacote;
                float novo_valor;
                tdata nova_valid;
                mercadoria merc_nova;

                cout << "Aqui é a área para alterar o produto.\n";
                cout << "Digite o nome do produto que desejas modificar: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                for(size_t i = 0; i<merc_CO.size(); i++){
                    if(merc_CO[i].nome_produto == nome_dproduto){
                        ind = i;
                        cout << "Mercadoria comprada!" << endl;
                        sleep(2);system("cls");
                        cout << "Escolha uma das opções abaixo para modificar:" << endl;
                        
                        do{
                            do{
                                cout << " 1 - Nome;\n" << " 2 - Quantidade;\n" << " 3 - Unidade por pacote;\n";
                                cout << " 4 - Valor;\n" << " 5 - Validade;\n " << " 6 - Bônus: Realizar promoção;\n";
                                cout << " 7 - Sair.\n" << endl;
                                cout << "Digite a sua escolha: "; cin >> opcao;
                                if((opcao < 1) || (opcao > 7)){
                                    cout << "Opção inválida, tente novamente." << endl;
                                }
                            }while((opcao < 1) || (opcao > 7));

                            switch(opcao){
                                case 1:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }

                                    system("cls"); novo_nome.clear(); ind_comp = -1;
                                    cout << "Digite o novo nome do produto: "; cin.ignore(); getline(cin, novo_nome); transf(novo_nome);
                                    for(size_t j = 0; j<merc_CO.size(); j++){
                                        if(merc_CO[j].nome_produto == novo_nome){
                                            cout << "Este nome não pode ser utilizado, pois já há um produto cadastrado com este nome." << endl;
                                            ind_comp = j;
                                        }
                                    }
                                    if(ind_comp == -1){
                                        merc_CO[i].nome_produto.clear(); merc_CO[i].nome_produto = novo_nome;

                                        cout << "Alteração realizada com sucesso!" << endl;
                                    }
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 2:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }
                                    ind_comp = 0;
                                    do{
                                        if(ind_comp == -1){
                                            if(verific(resposta)){
                                                system("cls");
                                                break;
                                            }
                                        }
                                        system("cls"); ind_comp = -1;
                                        cout << "Digite a data de validade do produto que você deseja alterar (d/m/a): "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                        for(size_t j = 0; j<merc_CO[i].validade.size(); j++){
                                            if(merc_CO[i].validade[j].day + merc_CO[i].validade[j].mon + merc_CO[i].validade[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Digite a nova quantidade deste produto: "; cin >>nova_quant;
                                                merc_CO[i].quantidade[j] = nova_quant;
                                            }
                                        }
                                        if(ind_comp == -1){
                                            cout << "Data inválida, tente novamente." << endl;
                                        }
                                    }while(ind_comp == -1);
                                    if(ind_comp == -1){
                                        cout << "A data digitada é inválida." << endl;
                                        cout << "Retornando ao menu de alterações...";
                                        break;
                                    }
                                    cout << "Alteração realizada com sucesso!\n";
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 3:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }

                                    system("cls");
                                    cout << "Digite a nova unidade por pacote: "; cin >> nova_unidppacote;
                                    merc_CO[i].unidade_pacote = nova_unidppacote;

                                    cout << "Alteração realizada com sucesso!" << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 4:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }
                                    system("cls");
                                    cout << "Digite o novo valor da mercadoria: "; cin >> novo_valor;
                                    merc_CO[i].valor = novo_valor;

                                    cout << "Alteração realizada com sucesso!" << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 5:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }
                                    ind_comp = 0;
                                    do{
                                        if(ind_comp == -1){
                                            if(verific(resposta)){
                                                system("cls");
                                                break;
                                            }
                                        }
                                        system("cls"); ind_comp = -1;
                                        cout << "Digite a data de validade do produto que você deseja alterar (d/m/a): "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                        for(size_t j = 0; j<merc_CO[i].validade.size(); j++){
                                            if(merc_CO[i].validade[j].day + merc_CO[i].validade[j].mon + merc_CO[i].validade[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Digite a nova validade: "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                                merc_CO[i].validade.insert(merc_CO[i].validade.begin() + j, nova_valid);
                                                merc_CO[i].validade.erase(merc_CO[i].validade.begin() + j + 1);
                                            }
                                        }
                                        if(ind_comp == -1){
                                            cout << "Data inválida, tente novamente." << endl;
                                        }
                                    }while(ind_comp == -1);
                                    if(ind_comp == -1){
                                        cout << "A data digitada é inválida." << endl;
                                        cout << "Retornando ao menu de alterações...";
                                        break;
                                    }
                                    cout << "Alteração realizada com sucesso!" << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 6:
                                    if(verific(resposta)){
                                        system("cls");
                                        break;
                                    }

                                    ind_comp = 0;
                                    do{
                                        if(ind_comp == -1){
                                            if(verific(resposta)){
                                                system("cls");
                                                break;
                                            }
                                        }
                                        system("cls"); ind_comp = -1;
                                        cout << "Digite a data de validade do produto que você deseja realizar a promoção (d/m/a): "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                        for(size_t j = 0; j<merc_CO[i].validade.size(); j++){
                                            if(merc_CO[i].validade[j].day + merc_CO[i].validade[j].mon + merc_CO[i].validade[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Todas as unidades deste produto (com a mesma data de validade) ficarão em promoção!" << endl;
                                                merc_nova.nome_produto = merc_CO[i].nome_produto + "(promoção)";
                                                merc_nova.quantidade.push_back(merc_CO[i].quantidade[j]);
                                                merc_nova.unidade_pacote = merc_CO[i].unidade_pacote;
                                                merc_nova.validade.push_back(nova_valid);
                                                cout << "Digite o novo valor do produto: "; cin >> merc_CO[i].valor;
                                            }
                                        }
                                        if(ind_comp == -1){
                                            cout << "Data inválida, tente novamente." << endl;
                                        }
                                    }while(ind_comp == -1);
                                    if(ind_comp == -1){
                                        cout << "A data digitada é inválida." << endl;
                                        cout << "Retornando ao menu de alterações...";
                                        break;
                                    }
                                    merc_CO.push_back(merc_nova);
                                    cout << "Alteração realizada com sucesso!" << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu de alterações...\n";
                                    }
                                break;
                                case 7:
                                    if(resp_saida(resposta)){
                                        cout << "Saindo da tela de alterações..." << endl;
                                    }
                                break;
                            }
                        }while(opcao != 7);
                    }
                }
                if(ind == -1){
                    cout << "Produto não encontrado. Por favor, verifique se o produto está cadastrado ou se foi escrito corretamente.\n";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                    }
                }
            break;
            }
            case 19:{
                if(verific(resposta)){
                    system("cls");
                    menu();
                }
            break;
            }
            case 20:{
                cout << "Salvando alterações...\n" <<endl;

                sleep(2);
                cout << "Saindo..." <<endl;
            break;
            default:
                cout << "Opção inválida.\n" << endl;
            break;
            }
        }
    cout << "tá aqui_2";
    }while((alternativa != 20) || (alternativa < 20));
    cout << "Está no final.";
}

int main(){

    menu();
    sleep(2); system("cls");
    cout << "Alterações salvas!" << endl;
    return 0;
}