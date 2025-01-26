#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;
using namespace chrono;

template <class T>
int indice(vector<T> nome_vector, string nome_search){
    int ind = -1;

    for(int i = 0; i<nome_vector.size(); i++){
        if(nome_vector[i].nome == nome_search){
            return i;
        }
    }
    return ind;
}

typedef struct{
    int day;
    int mon;
    int year;
}tdata;

class mercadoria{
    public:
        string nome_produto;
        vector<int> quantidade;
        int unidade_pacote;
        float valor;
        vector<tdata> validade;
            mercadoria(){
                nome_produto = nullptr;
                quantidade.push_back(0);
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
                    if((validade[i].day - dia == 0) && (validade[i].mon - mes == 0) && (validade[i].year - ano == 0) || ((validade[i].year - ano < 0))){
                        return true;
                    }else{
                        if((validade[i].day - dia < 0) && (validade[i].mon - mes < 0) && (validade[i].year - ano <= 0)){
                            return true;
                        }else{
                        return false;
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
                itens.clear();
            }
};

bool separador(vector<mercadoria>& itens, int indice, int quant_x_separacao, tdata data_prod){
    mercadoria nova_mercadoria;
    int ind = -1;
    if (indice < 0 || indice >= itens.size()) {
        cerr << "Índice inválido." << endl;
        return false;
    }
    
    for(int i = 0; i<itens[indice].validade.size(); i++){
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

    for(int i = 0; i<itens.size(); i++){
        if(itens[i].nome_produto == nova_mercadoria.nome_produto){
            for(int j = 0; j<itens[i].validade.size(); j++){
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

bool agrupador(vector<mercadoria>& itens, int indice){
    string busca = " unidade";
    size_t posicao = itens[indice].nome_produto.find(busca);
    string nome_provisorio;
    int ind = -1, num_unidades = 0, n = 0, unid_fard, soma = 0;

    mercadoria new_mercadoria;

    if (indice < 0 || indice >= itens.size() || (posicao == string::npos)) {
        cout << "Índice inválido ou esta mercadoria não é uma unidade." << endl;
        return false;
    }else{
        nome_provisorio = itens[indice].nome_produto.erase(posicao, sizeof(busca));
        itens[indice].nome_produto += busca;
        new_mercadoria.nome_produto = nome_provisorio;
    }

    for(int i = 0; i < itens.size(); i++){
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
        if((num_unidades == unid_fard) || (num_unidades == itens[ind].unidade_pacote)){
            break;
        }else{
            if(itens[indice].quantidade[0 + n] == 0){
                new_mercadoria.validade.push_back(itens[indice].validade[0+n]);
                n++;
            }
            for(int i = 0; i<itens[indice].quantidade.size(); i++){
                soma += itens[indice].quantidade[i];
            }
            if(soma == 0){
                cout << "Não é possível realizar o agrupamento destas unidades.";
                return false;
            }
        }
    }while((num_unidades != unid_fard )|| (num_unidades != itens[ind].unidade_pacote)); //retira as unidades
    for(int i = 0; i < itens[indice].quantidade.size(); i++){
        if(itens[indice].quantidade[i] == 0){
            itens[indice].quantidade.erase(itens[indice].quantidade.begin()+i);
            itens[indice].validade.erase(itens[indice].validade.begin()+i);
        }
    }
    new_mercadoria.quantidade.push_back(1);
    new_mercadoria.valor = num_unidades * itens[indice].valor; // Nova mercadoria construída.

    for(int i = 0; i<itens.size(); i++){
        string busca = new_mercadoria.nome_produto;
        size_t posicao = itens[i].nome_produto.find(busca);
        if(!(posicao == string::npos)){
            for(int j = 0; j<new_mercadoria.quantidade.size(); j++){
                itens[i].quantidade.push_back(new_mercadoria.quantidade[j]);
                itens[i].validade.push_back(new_mercadoria.validade[j]);
            }
            return true;
        }
    }
    itens.insert(itens.begin() + indice, new_mercadoria);
    return true;
    
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
        for(int j = 0; j < itens_Co[i].quantidade.size(); j++){
            compra += itens_Co[i].quantidade[j] * itens_Co[i].valor;
        }
    }
    for(int i = 0; i<itens_Ve.size(); i++){
        for(int j = 0; j < itens_Ve[i].quantidade.size(); j++){
            venda += itens_Ve[i].quantidade[j] * itens_Ve[i].valor;
        }
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

    for(int i = 0; i<mercadoria_CO.size(); i++){
        string busca = nome_dproduto;
        size_t posicao = mercadoria_CO[i].nome_produto.find(busca);
        if(!(posicao == string::npos)){
            ind = i;
            item_VE.nome_produto = mercadoria_CO[i].nome_produto;
            item_VE.valor = mercadoria_CO[i].valor;
            item_VE.unidade_pacote = mercadoria_CO[i].unidade_pacote;
        }
        for(int j = 0; j<mercadoria_CO[i].validade.size(); j++){
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
    int quant = mercadoria_CO[ind].quantidade[ind_d];
    if(quant - unidades_compradas < 0){
        cout << "Não há unidades suficiente para a compra.\n";
        cout << "Verifique o nome ou a data digitados.";
        return false;
    }else{
        for(int i = 0; i<mercadoria_VE.size(); i++){
            if(mercadoria_VE[i].nome_produto == mercadoria_CO[ind].nome_produto){
                for(int j = 0; j<mercadoria_VE[i].validade.size(); j++){
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

void menu(){
    vector<cliente> consumidor, consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    fstream arq_Cliente, arq_Cliente_beckup, arq_mercadoria, arq_mercadoria_beckup;
    using namespace std;

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
    }

    int alternativa; string resp;


    do{
        do{
            {_sleep(1000); system("cls");}
            cout << "Selecione a sua próxima ação (escreva o número da opção):" << endl;
            cout << "1 - Registrar cliente; \n" << "2 - Registrar produto; \n" << "3 - Excluir cliente; \n";
            cout << "4 - Excluir produto; \n" << "5 - Abrir lista de produto; \n" << "6 - Abrir lista de cliente; \n" << "7 - Verificar validade dos produtos; \n";
            cout << "8 - Realizar beckup da nova lista de produtos; \n" << "19 - Realizar beckup da nova lista de clientes; \n";
            cout << "10 - Utilizar beckup de produtos mais recente; \n" << "11 - Utilizar beckup de clientes mais recente; \n";
            cout << "12 - Saber conta de cliente; \n" << "13 - Realizar conta; \n" << "14 - Separar produto; \n" << "15 - Agrupar unidades; \n";
            cout << "16 - Realizar receita; \n" << "17 - Alterar cliente; \n" << "18 - Alterar produto; \n" << "19 - Tutorial; \n";
            cout << "20 - Sair. \n" << endl;
            cout << "Digite a sua resposta: ";
            cin >> alternativa;
            if(alternativa <= 0 || alternativa > 18){
                cout << "Opção inválida, tente novamente." << endl;
            }else{
                cout << "Escolheu a opção correta? (sim/não)\n";
                cin >> resp;
                transform(resp.begin(), resp.end(), resp.begin(), [](unsigned char c){return tolower(c);});
            }
        }while(alternativa <= 0 || alternativa > 19 || resp != "sim");

        system("cls");
        arq_Cliente.open("Clientes.txt", fstream::out);

        string resposta, nome_dproduto;
        cliente comprador_atual;
        mercadoria merc_comprada;

        switch(alternativa){
            case 1:
                int tam, unidades_vend, ind;
                tdata data_dprod;
                resposta.clear(); nome_dproduto.clear();

                cout << "Deseja sair desta área? (sim/não)\n";
                cin >> resposta; transform(resposta.begin(), resposta.end(), resposta.begin(), [](unsigned char c){return tolower(c);});
                if(resposta != "não"){
                    menu();
                }
                comprador_atual = cliente();
                if(merc_CO.empty()){
                    cerr << "Não há mercadoria registrada, por favor, registre o(os) produto(s) primeiro." << endl;
                    menu();
                }
                system("cls");
                cout << "Aqui é a área de registro de cliente. \n" << endl;
                cout << "O cliente possui as seguintes caracterísicas: ";
                cout << "Nome; \n Número de produtos comprados; \n Produtos comprados; \n Data de vencimento do produto \n" << endl;
                cout << "Digite o nome: "; cin >> comprador_atual.nome_cliente; cout << endl;
                for(int i = 0; i<consumidor_amigo.size(); i++){
                    if(consumidor_amigo[i].nome_cliente == comprador_atual.nome_cliente){
                        cout << "Este nome já é cadastrado no nosso sistema." << endl;
                        cout << "Sugerimos que vá para a área 'Realizar conta'." << endl;
                        cout << "Redirecionando para o menu...";
                        menu();
                    }
                }
                cout << "Digite quantos produtos o cliente pegou: "; cin >> tam; cout << endl;
                cout << "Digite o nome, a quantidade de unidades vendidas e a data de vencimento (no formato dia/mes/ano) deste produto." << endl;
                for(int i = 0; i<tam; i++){
                    nome_dproduto.clear(); unidades_vend = 0; ind = 0;

                    cin >> nome_dproduto >> unidades_vend;
                    cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                    compra(merc_CO, merc_VE, nome_dproduto, unidades_vend, data_dprod);
                    ind = indice(merc_VE, nome_dproduto);
                    comprador_atual.itens.push_back(merc_VE[ind]);
                }
                consumidor_amigo.push_back(comprador_atual);
            break;
            case 2:
                resposta.clear(); nome_dproduto.clear();
                cout << "Deseja sair desta área? (sim/não)\n";
                cin >> resposta; transform(resposta.begin(), resposta.end(), resposta.begin(), [](unsigned char c){return tolower(c);});
                if(resposta != "não"){
                    menu();
                }
                merc_comprada = mercadoria();
                int quant;
                tdata data_dprod;
                resposta.clear();

                cout << "Aqui é a área de registro de produto." << endl;
                cout << "A Mercadoria possui as seguintes características: " << endl;
                cout << " - Nome;\n" << " - Quantidade de itens;\n" << " - Quantidade por pacote;\n" << " - Valor (em reais);\n" << " - Validade.\n";
                cout << "Agora, registre a mercadoria:\n";
                cout << "Nome da mercadoria: "; cin >> merc_comprada.nome_produto; cout << endl;
                cout << "Quantidade de mercadoria: "; cin >> quant; merc_comprada.quantidade.push_back(quant); cout << endl;
                cout << "Quantidade de unidades por fardo: "; cin >> merc_comprada.unidade_pacote; cout << endl;
                cout << "Valor da mercadoria (em reais): "; cin >> merc_comprada.valor;
                cout << "Data de validade (no formato dia/mês/ano): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year; merc_comprada.validade.push_back(data_dprod); cout << endl;

                for(int i = 0; i<merc_CO.size(); i++){
                    string busca = merc_CO[i].nome_produto;
                    size_t posicao = merc_CO[i].nome_produto.find(busca);
                    if(!(posicao == string::npos)){
                        cout << "Uma mercadoria já está registrada no nosso sistema.\n Caso for para adicionar unidades, sugerimos que vá para a área 'Alterar produto'.\n";
                        cout << "Por encontrar um produto já cadastrado, iremos te redirecionar para o menu." <<endl;
                        cout << "Redirecionando ao menu...";
                        menu();
                    }
                }
                merc_CO.push_back(merc_comprada);
                cout << "Registro completo!";
            break;
            case 3:

            break;
            case 4:

            break;
            case 5:

            break;
            case 6:

            break;
            case 7:

            break;
            case 8:

            break;
            case 9:

            break;
            case 10:

            break;
            case 11:

            break;
            case 12:

            break;
            case 13:

            break;
            case 14:

            break;
            case 15:

            break;
            case 16:

            break;
            case 17:

            break;
            case 18:

            break;
            case 19:

            break;
            case 20:
                cout << "Salvando alterações..." <<endl;

                _sleep(2500);
                cout << "Saindo..." <<endl;
            break;
        }
    }while(alternativa != 20);
}

int main(){

    menu();
    _sleep(2500); system("cls");
    cout << "Alterações salvas!" << endl;
    return 0;
}