#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <windows.h>
#include "Classes.h"
#include "Gerenciador.h"
#include "Funções.h"

using namespace std;
using namespace chrono;

void menu(Key chave_geral){
    vector<cliente> consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    vector<proletariado> funcionario;
    fstream arq_Cliente, arq_Cliente_beckup, arq_mercadoria, arq_mercadoria_beckup, arq_chaveg, arq_fun, arq_fun_beckup;
    string resposta = "sim", nome_dproduto = "void", nome_dcliente = "void", nome_dworker = "void", nome_temp = "void", confia = "void";
    cliente comprador_atual = cliente();
    mercadoria merc_comprada = mercadoria();
    proletariado worker;
    tdata data_dprod = {0,0,0};
    int ind = -1, alternativa;
    Gerenciador G;

    vector<shared_ptr<pessoa>> people;
    vector<shared_ptr<proletariado>> work;
    vector<shared_ptr<cliente>> clientes;

    int cli_cadast = 0, prod_cadast = 0, fun_cadast = 0, num_acoes = 0;
    
    if(((verificador_arquivo("Clientes.txt") == false) && (verificador_arquivo("Clientes_beckup.txt") == false)) && ((verificador_arquivo("Mercadorias.txt") == false) && (verificador_arquivo("Mercadoria_beckup.txt") == false)) && ((verificador_arquivo("Pessoas.txt") == false) && (verificador_arquivo("Pessoa_beckup.txt") == false))){
        cout << "Seja bem-vindo!" << endl;
        nome_dworker.clear();
        float v; tdata data_p;
        cout << "Vejo que é a primeira vez que você abre este programa." << endl;
        cout << "Portanto, não é ingênuo pensar que você é o chefe!" << endl;
        cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
        cout << "Digite o seu salário (em reais):"; cin >> v;
        cout << "Digite o dia que você recebe o seu lucro (d/m/a): "; cin >> data_p.day >> data_p.mon >> data_p.year;  
        if(dataValida(data_p) == false){
            cerr << "Data inválida!" << endl;
        }
        funcionario.emplace_back(nome_dworker, "alta", "chefe", v, data_p);
    }else{
        cout << "Seja bem-vindo novamente!" << endl;
        if(verificador_arquivo("Mercadorias.txt") == false){
            cout << "O arquivo Mercadorias.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Mercadoria_beckup.txt", "Mercadorias.txt") == true){
                extracao_arq_prodCO(merc_CO, "Mercadorias.txt");
                verfic_venc(merc_CO);
            }else{
                cout << "Foi impossível realizar o beckup dos arquivos!" << endl;
            }
        }else{
            extracao_arq_prodCO(merc_CO, "Mercadorias.txt");
            verfic_venc(merc_CO);
        }
        if(verificador_arquivo("Pessoas.txt") == false){
            cout << "O arquivo Pessoas.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Pessoa_beckup.txt", "Pessoas.txt")){
                people = extracao_arq_pes("Pessoas.txt");
            }else{
                cout << "Foi impossível realizar o beckup dos arquivos!" << endl;
            }
        }else{
            people = extracao_arq_pes("Pessoas.txt");
        }
    }
    resp_saida(resposta);

    extracao_vector(people, work, clientes, consumidor_amigo, funcionario);
    do{
        do{
            cout << "Selecione a sua próxima ação (escreva o número da opção):" << endl;
            cout << "1 - Registrar cliente; \n" << "2 - Registrar produto; \n" << "3 - Registrar funcionário; \n" << "4 - Excluir cliente; \n";
            cout << "5 - Excluir produto; \n"  << "6 - Demitir funcionário; \n" << "7 - Abrir lista de produto; \n" << "8 - Abrir lista de pessoas; \n";
            cout << "9 - Verificar validade dos produtos; \n" << "10 - Verificar dia de pagamento de funcionários; \n";
            cout << "11 - Realizar beckup da nova lista de produtos; \n" << "12 - Realizar beckup da nova lista de clientes; \n" << "13 - Realizar beckup de lista de funcionários; \n";
            cout << "14 - Utilizar beckup de produtos mais recente; \n" << "15 - Utilizar beckup de clientes mais recente; \n" << "16 - Utilizar beckup de funcionários mais recente; \n";
            cout << "17 - Saber conta de cliente; \n" << "18 - Realizar conta; \n" << "19 - Separar produto; \n" << "20 - Agrupar unidades; \n";
            cout << "21 - Realizar receita; \n" << "22 - Buscar por (cliente, produto, funcionário);\n " << "23 - Alterar cliente; \n" << "24 - Alterar produto; \n" << "25 - Alterar funcioário; \n";
            cout << "26 - Alterar senha geral; \n" << "27 - Sair. \n" << endl;
            cout << "Digite a sua resposta: ";
            cin >> alternativa;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida. Tente novamente." << endl;
            }else{
                if(alternativa <= 0 || alternativa > 27){
                    cout << "Opção inválida, tente novamente." << endl;
                }
            }
        }while(alternativa <= 0 || alternativa > 27 || cin.fail());

        switch(alternativa){
            case 1:{
                if(G.registra_cli(merc_CO, merc_VE, consumidor_amigo, cli_cadast) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 2:{
                if(G.registra_prod(merc_CO, prod_cadast) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 3:{
                if(G.registra_fun(funcionario, fun_cadast) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 4:{
                if(G.ex_cli(consumidor_amigo) == false){
                    cerr << "Ops! Houve um problema na hora da exclusão." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 5:{
                if(G.ex_prod(merc_CO) == false){
                    cerr << "Ops! Houve um problema na hora da exclusão." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 6:{
                if(G.demissao(funcionario, chave_geral) == false){
                    cerr << "Ops! Houve algum problema no processo de demissão." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu..." << endl;
                }
            break;
            }
            case 7:{
                G.lista_prod(merc_CO, merc_VE);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu..." << endl;
                }
            break;
            }
            case 8:{
                cout << "Aqui é a área da lista de pessoas." << endl;
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                system("cls");
                resposta.clear();

                vector<cliente> clientes(consumidor_amigo); 
                vector<proletariado> Work(funcionario);

                vector<std::unique_ptr<pessoa>> person;
                for(const cliente& c : clientes){
                    person.push_back(make_unique<cliente>(c));
                }
                for(const proletariado& p : Work){
                    person.push_back(make_unique<proletariado>(p));
                }

                cout << "Aqui é a área da lista de pessoas." << endl;
                cout << "Digite qual lista você gostaria de ver (cliente/funcionário): "; getline(cin, resposta); transf(resposta);
                
                if(resposta == "cliente"){
                    if(clientes.empty()){
                        cout << "Não foi registrado nenhum cliente!" << endl;
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu..." << endl;
                            break;
                        }
                    }
                    const int larguraNome = 15;
                    const int larguraConf = 15;
                    const int larguraItens = 15;

                    cout << left << setw(larguraNome) << "Nome"
                        << setw(larguraConf) << "Confiança"
                        << setw(larguraItens * 3) << "Itens" << endl;
                        cout << string(75, '-') << endl;
                    for(const auto& P : person){
                        if(P->getTipo() == 2){
                            vector<string> d = P->dadosPtabela();
                            cout << left << setw(larguraNome) << d[0]
                            << setw(larguraConf) << d[1]
                            << setw(larguraItens * 3) << d[2] << endl;
                        }
                    }
                }else{
                    if(resposta == "funcionário"){
                        if(Work.empty()){
                            cout << "Não foi registrado nenhum funcionário!" << endl;
                            if(resp_saida(resposta)){
                                cout << "Retornando ao menu..." << endl;
                                break;
                            }
                        }
                        const int larguraNome = 15;
                        const int larguraConf = 15;
                        const int larguraCarg = 20;
                        const int larguraSal = 10;
                        const int larguraDat = 15;

                        cout << left << setw(larguraNome) << "Nome"
                            << setw(larguraConf) << "Confiança"
                            << setw(larguraCarg) << "Cargo"
                            << setw(larguraSal) << "Salário (reais)"
                            << setw(larguraDat) << "Pagamento" << endl;
                            cout << string(75, '-') << endl;
                            for(const auto& P : person){
                            if(P->getTipo() == 1){
                                vector<string> d = P->dadosPtabela();
                                cout << left << setw(larguraNome) << d[0]
                                    << setw(larguraConf) << d[1]
                                    << setw(larguraCarg) << d[2]
                                    << setw(larguraSal) << d[3]
                                    << setw(larguraDat) << d[4] << endl;
                            }
                        }
                    }else{
                        cout << "Ops! Ocorreu um erro de digitação." << endl;
                        cout << "Não foi possível mostrar os dados." << endl;
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu..." << endl;
                    break;
                }
            break;
            }
            case 9:{
                cout << "Aqui é a área de validades.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
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
                int ind_dprod = -1;
                cout << "Aqui é a área de validades.\n";
                cout << "Deseja saber a validade de um único produto? (sim/não)\n";
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    cout << "Digite o nome do produto que deseja ver a validade: "; cin.ignore(); getline(cin, nome_dproduto); transf(resposta);
                    for(size_t i = 0; i < merc_CO.size(); i++){
                        if(merc_CO[i].getNome() == nome_dproduto){
                            ind_dprod = merc_CO[i].F_show(merc_CO,nome_dproduto);
                        }
                    }
                    if(ind_dprod < 0){
                        cout << "Produto não encontrado.\n";
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...";
                        }
                        break;
                    }
                    comparador_data(merc_CO[ind_dprod].getValid(), diff_data);
                    for(size_t i = 0; i<diff_data.size(); i++){
                        if(diff_data[i] <= 0){
                            cout << "O produto " << merc_CO[ind_dprod].getNome() << "apresenta " << merc_CO[ind_dprod].getQuant()[i] << "unidades venciadas.\n";
                        }else{
                            cout << merc_CO[ind_dprod].getNome() << "(" << merc_CO[ind_dprod].getValid()[i].day << "/" << merc_CO[ind_dprod].getValid()[i].mon << "/" << merc_CO[ind_dprod].getValid()[i].year << ")";
                            cout << "||";
                            cout << "Há " << merc_CO[ind_dprod].getQuant()[i] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                        }
                    }
                }else{
                    cout << "Aqui estão as validades e o tempo (em dias) até o vencimento:\n";                
                    for(size_t i = 0; i<merc_CO.size(); i++){
                        comparador_data(merc_CO[i].getValid(), diff_data);
                        for(size_t j = 0; i<diff_data.size(); i++){
                            if(diff_data[j] <= 0){
                                cout << "O produto " << merc_CO[i].getNome() << "apresenta " << merc_CO[i].getQuant()[j] << "unidades venciadas.\n";
                            }else{
                                cout << merc_CO[i].getNome() << " (" << merc_CO[i].getValid()[j].day << "/" << merc_CO[i].getValid()[j].mon << "/" << merc_CO[i].getValid()[j].year << ")";
                                cout << " || ";
                                cout << "Há " << merc_CO[i].getQuant()[j] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                            }
                        }
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 10:{
                cout << "Aqui é a área que mostra o tempo até o dia de pagamento do funcionário." << endl;
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                double diff_t;
                cout << "Aqui é a área que mostra o tempo até o dia de pagamento do funcionário." << endl;
                cout << left << setw(15) << "Nome"
                    << setw(15) << "Data de pagamento"
                    << setw(15) << "Quantidade de dias" << endl;
                for(size_t i = 0; i<funcionario.size(); i++){
                    auto now = system_clock::now();
                    time_t time_now = system_clock::to_time_t(now);
                    tm* local_time = localtime(&time_now);

                    struct tm data_p;
                    time_t tempo_f, tempo_i;

                    data_p.tm_year = funcionario[i].getpay_day().year - 1900;
                    data_p.tm_mon = funcionario[i].getpay_day().mon - 1;
                    data_p.tm_mday = funcionario[i].getpay_day().day;

                    tempo_f = mktime(&data_p);
                    tempo_i = mktime(local_time);
                    
                    diff_t = difftime(tempo_f,tempo_i)/(60*60*24);
                    
                    cout << left << setw(15) << funcionario[i].getNome()
                        << setw(15) << to_string(funcionario[i].getpay_day().day) + "/" + to_string(funcionario[i].getpay_day().mon) + "/" + to_string(funcionario[i].getpay_day().year)
                        << setw(15) << diff_t << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }//data de pagamento do funcionário
            case 11:{
                cout << "Aqui é a área de beckup da lista de produto.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
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
            case 12:{
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
            case 13:{
                cout << "Aqui é a área de beckup da lista de funcionários.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                resposta.clear(); nome_dworker.clear();
                system("cls");
                string senha_t;

                cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
                ind = escolha(funcionario, nome_dworker);
                cout << "Digite a senha: "; getline(cin, senha_t);
                if(chave_geral.verific_S(senha_t, funcionario[ind])){
                    cout << "Acesso liberado!" << endl;
                    cout << "Aqui é a área de beckup da lista de funcionários.\n";
                    cout << "Deseja salvar a lista de funcionários no arquivo de beckup? (sim/não)\n";
                    getline(cin, resposta); transf(resposta);

                    if(resposta != "não"){
                        if(verificador_arquivo("")){
                            cout << "O beckup será realizado, aguarde um momento.\n";
                            if(copiararq("Funcionarios.txt","Funcionarios_beckup.txt")){
                                cout << "Beckup realizado com sucesso.!\n";
                            }else{
                                cout << "Infelizmente houve um erro durante o processo.\n";
                            }
                        }else{
                            cout << "A lista de funcionários está vazia. Por favor, registre os funcionários primeiro.\n";
                        }
                    }
                }else{
                    cout << "Senha e/ou nome inválidos" << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }//realizar beckup de funcionario
            case 14:{
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
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }

            break;
            }
            case 15:{
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
                    }
                }else{
                    cout << "Beckup não realizado!" << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 16:{
                cout << "Aqui é a área para utilizar o beckup de funcionários. \n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }   
                system("cls");
                string senha_t;
                nome_dworker.clear();
                ind = -1;
                
                cout << "Aqui é a área para utilizar o beckup de clientes. \n";
                cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
                ind = escolha(funcionario, nome_dworker);
                if(ind == -1){
                    cout << "Nome não encontrado.";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...";
                        break;
                    }
                }
                cout << "Digite a senha: "; getline(cin, senha_t);
                if(chave_geral.verific_S(senha_t, funcionario[ind])){
                    cout << "Acesso liberado!" << endl;
                    cout << "Caso o beckup seja utilizado, as informações alteradas na lista de clientes serão peridas.\n";
                    cout << "Deseja utilizar o beckup? (sim/não)\n";
                    cin >> resposta; transf(resposta);
                    if(resposta == "sim"){
                        cout << "Aguarde um momento...\n";
                        if(copiararq("Clientes_beckup.txt","Clientes.txt")){
                            cout << "Beckup feito com sucesso!\n";
                        }
                    }
                }else{
                    cout << "Ops! senha e/ou nome inválidos!" << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    break;
                }
            break;
            }
            case 17:{
                cout << "Aqui é a área para realizar a conta do cliente. \n";
                if(verific(resposta)){
                    system("cls");
                    break;
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
            case 18:{
                cout << "Aqui é a área para realizar a conta da compra.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                system("cls");
                int quant_itens, unid_comp, ind_p = -1; nome_dproduto.clear();
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
                        for(size_t j = 0; j < merc_CO.size(); j++){
                            if(merc_CO[j].getNome() == nome_dproduto){
                                ind_p = merc_CO[j].F_show(merc_CO, nome_dproduto);
                            }
                        }
                        if(ind_p < 0){
                            cout << "Produto não encontrado!" << endl;
                            cout << "Tente novamente.";
                            system("cls");
                        }
                    }while(ind_p < 0);
                    cout << "Digite a data de validade deste produto (d/m/a): ";
                    cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                    compra(merc_CO, merc_VE, nome_dproduto, unid_comp, data_dprod);
                    valor_conta += merc_CO[ind_p].getValor();
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
            case 19:{
                cout << "Aqui é a área de separação de produto.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                int quant_sep, ind_p = -1;
                system("cls"); nome_dproduto.clear();
                cout << "Aqui é a área de separação de produto.\n";
                cout << "Digite o nome do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);

                ind_p = escolha(merc_CO, nome_dproduto);
                
                if(ind_p < 0){
                    cout << "Produto não encontrado." << endl;
                    if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    }
                    break;
                }
                cout << "\nDigite quantas vezes essa separação ocorrerá: "; cin >> quant_sep;
                cout << "\nDigite a data de validade do produto a ser desagrupado (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                if(desagrupador(merc_CO, ind_p, quant_sep, data_dprod)){
                    if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    }
                }  
            break;
            }
            case 20:{
                cout << "Aqui é a área para agrupar produtos de um único nome.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                system("cls"); nome_dproduto.clear();
                int ind_p = -1;
                cout << "Aqui é a área para agrupar produtos de um único nome.\n";
                cout << "Digite o nome do produto corretamente: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                for(size_t i = 0; i < merc_CO.size(); i++){
                    if(merc_CO[i].getNome() == nome_dproduto){
                        ind_p = merc_CO[i].F_show(merc_CO, nome_dproduto);
                    }
                }
                if(ind_p < 0){
                    cout << "Produto não encontrado." << endl;
                    if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    }
                    break;
                }
                agrupador(merc_CO, ind_p);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 21:{
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

            case 22:{
                cout << "Aqui é a área de busca.\n";
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                system("cls");
                nome_dcliente.clear(); nome_dproduto.clear(); nome_dworker.clear();
                resposta.clear();

                cout << "Aqui é a área de busca.\n";
                cout << "O que você está produrando no sistema? (cliente, produto, funcionário)" << endl; getline(cin,resposta); transf(resposta);
                if(resposta != "cliente" || resposta != "produto" || resposta != "funcionário"){
                    cerr << "Resposta inválida" << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                        break;
                    }
                }
                if(resposta == "cliente"){
                    ind = -1;
                    cout << "Digite o nome do cliente: "; getline(cin, nome_dcliente); transf(nome_dcliente);
                    ind = escolha(consumidor_amigo, nome_dcliente);
                    if(ind == -1){
                        cerr << "Cliente não encontrado!" << endl;
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...\n";
                            break;
                        }
                    }
                    cout << "Aqui estão as informações deste cliente: " << endl;
                    cout << " - Nome: " << consumidor_amigo[ind].getNome() << endl;
                    cout << " - Confiança: " << consumidor_amigo[ind].getConf() << endl;
                    cout << " - Itens pegos : " << endl;
                    for(size_t i = 0; i<consumidor_amigo[ind].getMerc().size(); i++){
                        int qtdT = 0;
                        for(size_t j = 0; j<consumidor_amigo[ind].getMerc()[i].getQuant().size(); j++){
                            qtdT += consumidor_amigo[ind].getMerc()[i].getQuant()[j];
                        }
                        cout << "  - " << consumidor_amigo[ind].getMerc()[i].getNome() << " | Quantidade comprada: " << qtdT << endl;
                        cout << " - Dívida (em reais): " << consumidor_amigo[ind].divida(consumidor_amigo[ind].getMerc()) << endl;
                    }
                }
                if(resposta == "produto"){
                    ind = -1;
                    cout << "Digite o nome do produto: "; getline(cin, nome_dproduto); transf(nome_dproduto);
                    ind = escolha(merc_CO, nome_dproduto);
                    if(ind == -1){
                        cerr << "Produto não encontrado!" << endl;
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...\n";
                            break;
                        }
                    }

                    cout << "Aqui estão as informações deste produto: " << endl;
                    cout << " - Nome: " << merc_CO[ind].getNome() << endl;
                    cout << " - Quantidades do produto (por validade): " << endl;
                    for(size_t i = 0; i<merc_CO[ind].getQuant().size(); i++){
                        cout << "  - " << merc_CO[ind].getQuant()[i] << " || " << merc_CO[ind].getValid()[i].day << "/" << merc_CO[ind].getValid()[i].mon << "/" << merc_CO[ind].getValid()[i].year << endl;
                    }
                    cout << " - Unidade por pacote: " << merc_CO[ind].getUnid() << endl;
                    cout << " - Valor (em reais): " << merc_CO[ind].getValor() << endl; 
                }
                if(resposta == "funcionário"){
                    ind = -1;
                    cout << "Digite o nome do funcionário: "; getline(cin, nome_dworker); transf(nome_dworker);
                    ind = escolha(funcionario, nome_dworker);
                    if(ind == -1){
                        cerr << "Nome não encontrado!" << endl;
                        if(resp_saida(resposta)){
                            cout << "Retornando ao menu...\n";
                            break;
                        }
                    }

                    cout << "Aqui estão as informações do funcionário: " << endl;
                    cout << " - Nome: " << funcionario[ind].getNome() << endl;
                    cout << " - Confiança: " << funcionario[ind].getConf() << endl;
                    cout << " - Cargo: " << funcionario[ind].getCargo() << endl;
                    cout << " - Salário (em reais): " << funcionario[ind].getSalario() << endl;
                    cout << " - Dia de pagamento: " << funcionario[ind].getpay_day().day << "/" << funcionario[ind].getpay_day().mon << "/" << funcionario[ind].getpay_day().year << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 23:{
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
                ind = escolha(consumidor_amigo,nome_dcliente);
                if(ind == -1){
                    cout << "Cliente não encontrado. Por favor, verifique se o nome está cadastrado ou se foi digitado corretamente.\n";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                    }
                }else{
                    cout << "Cliente encontrado!" << endl;
                    sleep(2);system("cls");
                    cout << "Escolha uma das opções abaixo para modificar:" << endl;
                    do{
                        do{
                            cout << " 1 - Nome;\n" << " 2 - Lista de compras;\n" << " 3 - Confiabilidades;\n" <<  " 4 - Pagamento;\n" << " 5 - Sair.\n" << endl;
                            cin >> opcao;
                            if((opcao < 1) || (opcao > 5)){
                                cout << "Opção inválida, tente novamente.\n";
                            }
                        }while((opcao < 1) || (opcao > 5));

                        system("cls");
                        switch(opcao){
                            case 1:{
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                nome_dcliente.clear();
                                cliente client_t;
                                resposta.clear();
                                cout << "Digite o novo nome do cliente: "; cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);
                                for(size_t i = 0; i<consumidor_amigo.size(); i++){
                                    if(client_t.F_ind(consumidor_amigo, nome_dcliente)){
                                        cout << "O nome digitado já está cadastrado no nosso sistema!\n";
                                        cout << "Deseja realmente cadastrar este nome? (sim/não)\n"; cin.ignore(); getline(cin, resposta); transf(resposta);
                                        if(resposta == "sim"){
                                            consumidor_amigo[ind].setNome(nome_dcliente);
                                        }else{
                                            resp_saida(resposta);
                                            cout << "Retornando à escolha das alterações...";
                                            break;
                                        }
                                    }
                                }
                                consumidor_amigo[ind].setNome(nome_dcliente);
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
                                
                                int quanti_itens, ind_c, unid_compradas, ind_data = -1;
                                cout << "Quantos tipos de itens por data de validade o cliente comprou?";
                                cin >> quanti_itens; cout << endl;
                                for(int j = 0; j < quanti_itens; j++){
                                    do{
                                        nome_dproduto.clear(); ind = -1; merc_comprada.getNome().clear(); data_dprod = {0,0,0};
                                        cout << "Digite o nome correto do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                                        for(size_t k = 0; k<merc_CO.size(); k++){
                                            if(merc_CO[k].getNome() == nome_dproduto){
                                                ind_c = k;
                                                cout << "Produto encontrado!\n";
                                                merc_comprada.setN_prod(nome_dproduto);
                                                merc_comprada.setValor(merc_CO[k].getValor() * 1.2);
                                                merc_comprada.setUnid(merc_CO[k].getUnid());
                                            }
                                        }
                                        if(ind_c == -1){
                                            cout << "Produto não encontrado. Por favor, tente novamente.\n";
                                        }
                                    }while(ind_c == -1);
                                    cout << "Digite quantas unidades o cliente comprou: "; cin >> unid_compradas;
                                    cout << "Digite a data de validade (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                                    merc_comprada.getValid().push_back(data_dprod); 
                                    
                                    if(!(compra(merc_CO, merc_VE, nome_dproduto, unid_compradas, data_dprod))){
                                        cout << "Por favor, verifique a lista de itens e a atualize se necessário." << endl;
                                        if(resp_saida(resposta)){
                                            cout << "Retornando à escolha das alterações...";
                                        }
                                        break;
                                    }

                                    ind_c = -1;
                                    for(size_t k = 0; k<consumidor_amigo[ind].getMerc().size(); k++){
                                        if(consumidor_amigo[ind].getMerc()[k].getNome() == nome_dproduto){
                                            ind_c = k;
                                            for(size_t L = 0; L < consumidor_amigo[ind].getMerc()[k].getValid().size(); L++){
                                                if(consumidor_amigo[ind].getMerc()[k].getValid()[L].day + consumidor_amigo[ind].getMerc()[k].getValid()[L].mon + consumidor_amigo[ind].getMerc()[k].getValid()[L].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                                                    ind_data = L;
                                                    consumidor_amigo[ind].getMerc()[k].getQuant()[L] += unid_compradas;
                                                }
                                            }
                                        }
                                    }
                                    if(ind_c == -1){
                                        consumidor_amigo[ind].getMerc().push_back(merc_comprada);
                                    }
                                    if((ind_c != -1) && (ind_data == -1)){
                                        consumidor_amigo[ind].getMerc()[ind_c].getValid().push_back(data_dprod);
                                        consumidor_amigo[ind].getMerc()[ind_c].getQuant().push_back(unid_compradas);
                                    }
                                }
                                cout << "Cliente atualizado!\n";
                                if(resp_saida(resposta)){
                                    cout << "Retornando à escolha das alterações...";
                                }
                            break;
                            }
                            case 3:{
                                cout << "Aqui é a área de confiabilidade.\n";
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }
                                system("cls");
                                string conf = "void";
                                
                                cout << "Aqui é a área de confiabilidade.\n";
                                cout << "Digite a confiabilidade do cliente: "; cin.ignore(); getline(cin, conf); transf(conf);
                                consumidor_amigo[ind].setConf(conf);
                                cout << "Alteração concluída!" << endl;
                                if(resp_saida(resposta)){
                                    cout << "Retornando à escolha das alterações...";
                                }
                            break;
                            }
                            case 4:{
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
                                for(size_t j = 0; j<consumidor_amigo[ind].getMerc().size(); j++){
                                    for(size_t k = 0; k < consumidor_amigo[ind].getMerc().size()-1-j; k++){
                                        if(consumidor_amigo[ind].getMerc()[k].getValor() > consumidor_amigo[ind].getMerc()[k+1].getValor()){
                                            merc_troca.setN_prod(consumidor_amigo[ind].getMerc()[k].getNome());
                                            for(size_t L = 0; L < consumidor_amigo[ind].getMerc()[k].getQuant().size(); L++){
                                                merc_troca.getQuant().push_back(consumidor_amigo[ind].getMerc()[k].getQuant()[L]);
                                                merc_troca.getValid().push_back(consumidor_amigo[ind].getMerc()[k].getValid()[L]);
                                            }
                                            merc_troca.setUnid(consumidor_amigo[ind].getMerc()[k].getUnid());
                                            merc_troca.setValor(consumidor_amigo[ind].getMerc()[k].getValor());

                                            int x = consumidor_amigo[ind].getMerc()[k].getQuant().size();
                                            int y = consumidor_amigo[ind].getMerc()[k+1].getQuant().size();

                                            if(x - y < 0){
                                                tam_quant = consumidor_amigo[ind].getMerc()[k+1].getQuant().size();
                                            }else{
                                                tam_quant = consumidor_amigo[ind].getMerc()[k].getQuant().size();
                                            }
                                            consumidor_amigo[ind].getMerc()[k].setN_prod(consumidor_amigo[ind].getMerc()[k+1].getNome());

                                            for(size_t L = 0; L < tam_quant; L++){
                                                consumidor_amigo[ind].getMerc()[k].getQuant().push_back(consumidor_amigo[ind].getMerc()[k+1].getQuant()[L]);
                                                consumidor_amigo[ind].getMerc()[k].getValid().push_back(consumidor_amigo[ind].getMerc()[k+1].getValid()[L]);
                                            }
                                            consumidor_amigo[ind].getMerc()[k].setValor(consumidor_amigo[ind].getMerc()[k+1].getValor());
                                            consumidor_amigo[ind].getMerc()[k].setUnid(consumidor_amigo[ind].getMerc()[k+1].getUnid());

                                            consumidor_amigo[ind].getMerc()[k+1].setN_prod(merc_troca.getNome());
                                            for(size_t L = 0; L < merc_troca.getQuant().size(); L++){
                                                consumidor_amigo[ind].getMerc()[k+1].getQuant().push_back(merc_troca.getQuant()[L]);
                                                consumidor_amigo[ind].getMerc()[k+1].getValid().push_back(merc_troca.getValid()[L]);
                                            }
                                            consumidor_amigo[ind].getMerc()[k+1].setValor(merc_troca.getValor());
                                            consumidor_amigo[ind].getMerc()[k+1].setUnid(merc_troca.getUnid());
                                        }
                                    }
                                }
                                int n = -1;
                                float soma_comp = 0.0;
                                mercadoria merc_resto;
                                for(size_t j = 0; j<consumidor_amigo[ind].getMerc().size(); j++){
                                    for(size_t k = 0; k < consumidor_amigo[ind].getMerc()[j].getQuant().size(); k++){
                                        while((consumidor_amigo[ind].getMerc()[j].getQuant()[k] != 0) && (pagamento > 0)){
                                            consumidor_amigo[ind].getMerc()[j].getQuant()[k]--;
                                            pagamento -= consumidor_amigo[ind].getMerc()[j].getValor();
                                            if(pagamento < 0){
                                                for(size_t L=0; L<consumidor_amigo[ind].getMerc().size(); L++){
                                                    size_t posicao = consumidor_amigo[ind].getMerc()[L].getNome().find("Resto");
                                                    if(!(posicao == string::npos)){
                                                        n = L;
                                                        consumidor_amigo[ind].getMerc()[L].setValor(consumidor_amigo[ind].getMerc()[L].getValor() - pagamento);
                                                    }
                                                }
                                                if(n == -1){
                                                    merc_resto.setN_prod("Resto");
                                                    merc_resto.getQuant().push_back(1);
                                                    merc_resto.setUnid(1);
                                                    merc_resto.setValor(-pagamento);
                                                    merc_resto.getValid().push_back({0,0,0});
                                                    consumidor_amigo[ind].getMerc().push_back(merc_resto);
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
                                for(size_t j = 0; j<consumidor_amigo[ind].getMerc().size(); j++){
                                    for(size_t k = 0; k < consumidor_amigo[ind].getMerc()[j].getQuant().size(); k++){
                                        soma_comp += consumidor_amigo[ind].getMerc()[j].getQuant()[k];
                                    }
                                    if(soma_comp == 0){
                                        consumidor_amigo[ind].getMerc().erase(consumidor_amigo[ind].getMerc().begin()+j);
                                        j=0;
                                    }else{
                                        for(size_t k = 0; k < consumidor_amigo[ind].getMerc()[j].getQuant().size(); k++){
                                            if(consumidor_amigo[ind].getMerc()[j].getQuant()[k] == 0){
                                                consumidor_amigo[ind].getMerc()[j].getQuant().erase(consumidor_amigo[ind].getMerc()[j].getQuant().begin()+k);
                                                consumidor_amigo[ind].getMerc()[j].getValid().erase(consumidor_amigo[ind].getMerc()[j].getValid().begin()+k);
                                                k=0;
                                            }
                                        }
                                    }
                                    soma_comp = 0.0;
                                }
                                cout << "pagamento realizado!" << endl;
                                cout << "Agora o cliente está devendo (em reais): R$" << conta_cliente(consumidor_amigo, consumidor_amigo[ind].getNome());
                                if(resp_saida(resposta)){
                                    cout << "Retornando à escolha das alterações...";
                                }
                            break;
                            }
                            case 5:
                                if(resp_saida(resposta)){
                                    cout << "Saindo da tela de alterações..." << endl;
                                }
                            break;
                        }
                    }while(opcao != 5);
                }
            break;
            }
            case 24:{
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
                    if(merc_CO[i].getNome() == nome_dproduto){
                        ind = i;
                        cout << "Mercadoria encontrada!" << endl;
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
                                        if(merc_CO[j].getNome() == novo_nome){
                                            cout << "Este nome não pode ser utilizado, pois já há um produto cadastrado com este nome." << endl;
                                            ind_comp = j;
                                        }
                                    }
                                    if(ind_comp == -1){
                                        merc_CO[i].getNome().clear(); merc_CO[i].setN_prod(novo_nome);

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
                                        for(size_t j = 0; j<merc_CO[i].getValid().size(); j++){
                                            if(merc_CO[i].getValid()[j].day + merc_CO[i].getValid()[j].mon + merc_CO[i].getValid()[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Digite a nova quantidade deste produto: "; cin >>nova_quant;
                                                merc_CO[i].getQuant()[j] = nova_quant;
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
                                    merc_CO[i].setUnid(nova_unidppacote);

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
                                    cout << "Digite o novo valor da mercadoria: "; cin >> novo_valor; merc_CO[i].setValor(novo_valor);

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
                                        for(size_t j = 0; j<merc_CO[i].getValid().size(); j++){
                                            if(merc_CO[i].getValid()[j].day + merc_CO[i].getValid()[j].mon + merc_CO[i].getValid()[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Digite a nova validade: "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                                if(dataValida(nova_valid)){
                                                    merc_CO[i].getValid().insert(merc_CO[i].getValid().begin() + j, nova_valid);
                                                    merc_CO[i].getValid().erase(merc_CO[i].getValid().begin() + j + 1);
                                                }else{
                                                    ind_comp = -1;
                                                }
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
                                    float v;
                                    do{
                                        if(ind_comp == -1){
                                            if(verific(resposta)){
                                                system("cls");
                                                break;
                                            }
                                        }
                                        system("cls"); ind_comp = -1;
                                        cout << "Digite a data de validade do produto que você deseja realizar a promoção (d/m/a): "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                        for(size_t j = 0; j<merc_CO[i].getValid().size(); j++){
                                            if(merc_CO[i].getValid()[j].day + merc_CO[i].getValid()[j].mon + merc_CO[i].getValid()[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                                ind_comp = j;
                                                cout << "Todas as unidades deste produto (com a mesma data de validade) ficarão em promoção!" << endl;
                                                merc_nova.setN_prod(merc_CO[i].getNome() + "(promoção)");
                                                merc_nova.getQuant().push_back(merc_CO[i].getQuant()[j]);
                                                merc_nova.setUnid( merc_CO[i].getUnid());
                                                merc_nova.getValid().push_back(nova_valid);
                                                cout << "Digite o novo valor do produto: "; cin >> v; merc_CO[i].setValor(v);
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
            case 25:{
                cout << "Aqui é a área para alterar funcionário." << endl;
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                if(funcionario.empty()){
                    cerr << "Não há funcionários registrados." << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                    }
                    break;
                }

                system("cls");
                string senha_t;
                nome_dworker.clear();
                ind = -1;
                int opcao;

                cout << "Digite seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
                cout << "Digite a senha: "; getline(cin, senha_t);
                ind = escolha(funcionario, nome_dworker);
                if(ind == -1){
                    cout << "Funcionário não encontrado." << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                        break;
                    }
                }
                if(chave_geral.verific_S(senha_t, funcionario[ind]) == false){
                    cerr << "Ops! Senha e/ou nome incorretos." << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                        break;
                    }
                }else{
                    cout << "Acesso liberado!" << endl;
                    do{
                        do{
                            cout << "Escolha o que desejas alterar: " << endl;
                            cout << " 1 - Nome; \n" << " 2 - Confiabilidade; \n" << " 3 - Cargo; \n" << " 4 - Salário; \n";
                            cout << " 5 - Data de pagamento; \n" << " 6 - Realizar pagamento do salário; \n" << " 7 - Sair." << endl;
                            cout << "Digite a opção: " ; cin >> opcao; cin.ignore();
                            if(opcao < 1 || opcao > 7){
                                cerr << "Opção inválida." << endl;
                                cout << "Tente novamente." << endl;
                            }
                        }while(opcao < 1 || opcao > 7);

                        switch(opcao){
                            case 1: {
                                cout << "Aqui é a área do nome." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                string new_name;
                                resposta.clear();
                                cout << "Digite o novo nome do funcionário: "; getline(cin, new_name);
                                if(worker.F_ind(funcionario, new_name)){
                                    cout << "Há alguém com um nome semelhante. Deseja alterar o nome escolhido para este? (sim/não)"; getline(cin, resposta); transf(resposta);
                                    if(resposta != "sim"){
                                        cout << "O nome não foi alterado!";
                                    }else{
                                        cout << "Certo, retornando à alteração..." << endl;
                                    }
                                }else{
                                    funcionario[ind].setNome(new_name);
                                    cout << "Alteração concluída!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                            case 2: {
                                cout << "Aqui é a área da confiabilidade." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                system("cls");
                                string conf;
                                resposta.clear();
                                cout << "Aqui é a área da confiabilidade." << endl;
                                cout << "Digite a nova confiabilidade do funcionário (pouco confiável/ confiável/ muito confiável): "; getline(cin, conf); transf(conf);
                                if(conf != "pouco confiável" || conf != "confiável" || conf != "muito confiável"){
                                    cerr << "Ops! A confiabilidade não foi bem definida." << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu...\n";
                                        break;
                                    }
                                }
                                cout << "Deseja alterar para esta confiança? (sim/não)"; getline(cin, resposta); transf(resposta);
                                if(resposta != "sim"){
                                    cout << "A confiança não foi alterado!";
                                }else{
                                    cout << "Certo, retornando à alteração..." << endl;
                                    funcionario[ind].setConf(conf);
                                    cout << "Alteração concluída!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                            case 3: {
                                cout << "Aqui é a área de alteração de cargo." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                system("cls");
                                string carg;
                                resposta.clear();
                                cout << "Aqui é a área da confiabilidade." << endl;
                                cout << "Digite o novo cargo do funcionário: "; getline(cin, carg); transf(carg);
                                if(proc_carg(funcionario, "chefe")){
                                    cout << "Já há alguém com o cargo 'chefe'." << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando ao menu...\n";
                                        break;
                                    }
                                }
                                cout << "Deseja alterar para este cargo? (sim/não)"; getline(cin, resposta); transf(resposta);
                                if(resposta != "sim"){
                                    cout << "O cargo não foi alterado!";
                                }else{
                                    cout << "Certo, retornando à alteração..." << endl;
                                    funcionario[ind].setCargo(carg);
                                    cout << "Alteração concluída!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                            case 4: {
                                cout << "Aqui é a área de alteração de salário." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                system("cls");
                                float sal;
                                resposta.clear();
                                cout << "Aqui é a área da confiabilidade." << endl;
                                cout << "Digite o novo salário do funcionário (em reais): "; cin >> sal;
                                cout << "Deseja alterar para este salário? (sim/não)"; getline(cin, resposta); transf(resposta);
                                if(resposta != "sim"){
                                    cout << "O salário não foi alterado!";
                                }else{
                                    cout << "Certo, retornando à alteração..." << endl;
                                    funcionario[ind].setSalario(sal);
                                    cout << "Alteração concluída!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                            case 5: {
                                cout << "Aqui é a área para alterar a data de pagamento." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }

                                system("cls");
                                tdata data_t; resposta.clear();
                                cout << "Aqui é a área para alterar a data de pagamento." << endl;
                                cout << "Digite a nova data da pagamento do funcionário (d/m/a): ";
                                cin >> data_t.day >> data_t.mon >> data_t.year;
                                if(dataValida(data_t)){
                                    cout << "Deseja alterar para este salário? (sim/não)"; getline(cin, resposta); transf(resposta);
                                    if(resposta != "sim"){
                                        cout << "O dia de pagamento não foi alterado!";
                                    }else{
                                        cout << "Certo, retornando à alteração..." << endl;
                                        funcionario[ind].setPagamento(data_t);
                                        cout << "Alteração concluída!" << endl;
                                    }
                                }else{
                                    cerr << "Data inválida!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                            case 6:{
                                cout << "Aqui é a área para registrar o pagamento do funcionário." << endl;
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }
                                nome_dworker.clear();
                                ind = -1;
                                tdata data_t;
                                cout << "Aqui é a área para registrar o pagamento do funcionário." << endl;
                                cout << "Digite o nome do funcionário que será pago: "; getline(cin, nome_dworker); transf(nome_dworker);
                                ind = escolha(funcionario, nome_dworker);
                                if(ind == -1){
                                    cerr << "Funcionário não encontrado." << endl;
                                }else{
                                    data_t.day = funcionario[ind].getpay_day().day;
                                    data_t.mon = funcionario[ind].getpay_day().mon;
                                    data_t.year = funcionario[ind].getpay_day().year;
                                    next_d(data_t, 30);
                                    cout << "Pagamento realizado!" << endl;
                                }
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                    break;
                                }
                            break;
                            }
                            case 7: {
                                if(resp_saida(resposta)){
                                    cout << "Retornando ao menu...\n";
                                }
                            break;
                            }
                        }
                    }while(opcao != 7);
                }
            break;
            }
            case 26:{
                cout << "Aqui é a área para alterar a senha geral." << endl;
                if(verific(resposta)){
                    system("cls");
                    break;
                }

                Key chave;
                chave.setSenha("void");
                string senha_t, nome_t;
                ind = -1;

                system("cls");
                cout << "Aqui é a área para alterar a senha geral." << endl;
                cout << "Digite o seu nome: "; getline(cin, nome_t); transf(nome_t);
                ind = escolha(funcionario, nome_t);
                if(ind == -1){
                    cout << "O seu nome não foi encontrado." << endl;
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                        break;
                    }
                }
                cout << "Digite a senha: "; getline(cin, senha_t);
                if(chave.verific_S(senha_t, funcionario[ind]) == false){
                    cout << "Ops! Senha e/ou nome incorretos." << endl;
                }else{
                    senha_t.clear();
                    resposta.clear();
                    cout << "Acesso liberado!" << endl;
                    cout << "Digite a nova senha: "; getline(cin, senha_t);
                    cout << "Deseja, realmente, alterar a senha? (sim/não)"; getline(cin, resposta); transf(resposta);
                    if(resposta == "sim"){
                        chave_geral.setSenha(senha_t);
                    }else{
                        cout << "A senha não foi alterada." << endl;
                    }
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 27:{
                cout << "Salvando alterações...\n" <<endl;
                arq_Cliente.open("Clientes.txt", fstream::out); arq_mercadoria.open("Mercadorias.txt", fstream::out); arq_fun.open("Funcionarios.txt", fstream::out); arq_chaveg.open("Chave_g.txt", fstream::out);
                if(!arq_Cliente.is_open() || !arq_mercadoria.is_open() || !arq_chaveg.is_open() || !arq_fun.is_open()){
                    cerr << "Ops! não foi possível abrir os arquivos!";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...\n";
                        break;
                    }
                }
                
                for(size_t i = 0; i<consumidor_amigo.size(); i++){
                    comprador_atual = consumidor_amigo[i];
                    arq_Cliente << comprador_atual.getNome() << endl;
                    arq_Cliente << comprador_atual.getConf() << endl;
                    for(size_t j = 0; j<comprador_atual.getMerc().size(); j++){
                        arq_Cliente << comprador_atual.getMerc()[j].getNome() << " ";
                        for(size_t k = 0; k<comprador_atual.getMerc()[j].getQuant().size(); k++){
                            arq_Cliente << comprador_atual.getMerc()[j].getQuant()[k] << " ";
                        }
                        arq_Cliente << endl;
                        arq_Cliente << comprador_atual.getMerc()[j].getUnid() << endl;
                        arq_Cliente << comprador_atual.getMerc()[j].getValor() << endl;
                        for(size_t k = 0; k<comprador_atual.getMerc()[j].getQuant().size(); k++){
                            arq_Cliente << comprador_atual.getMerc()[j].getValid()[k].day << "/" << comprador_atual.getMerc()[j].getValid()[k].mon << "/" << comprador_atual.getMerc()[j].getValid()[k].year << " ";
                        }
                        arq_Cliente << endl;
                    }
                }

                for(size_t i = 0; i<merc_CO.size(); i++){
                    merc_comprada = merc_CO[i];
                    arq_mercadoria << merc_comprada.getNome() << endl;
                    for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                        arq_mercadoria << merc_comprada.getQuant()[j] << " ";
                    }
                    arq_mercadoria << endl;
                    arq_mercadoria << merc_comprada.getUnid() << endl;
                    arq_mercadoria << merc_comprada.getValor() << endl;
                    for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                        arq_mercadoria << merc_comprada.getValid()[j].day << "/" << merc_comprada.getValid()[j].mon << "/" << merc_comprada.getValid()[j].year << " ";
                    }
                    arq_mercadoria << endl;
                }

                for(size_t i = 0; i<funcionario.size(); i++){
                    worker = funcionario[i];
                    arq_fun << worker.getNome() << endl;
                    arq_fun << worker.getConf() << endl;
                    arq_fun << worker.getCargo() << endl;
                    arq_fun << worker.getSalario() << endl;
                    arq_fun << worker.getpay_day().day << "/" << worker.getpay_day().mon << "/" << worker.getpay_day().year; 
                }

                arq_chaveg << chave_geral.getSenha() << endl;

                arq_Cliente.close(); arq_mercadoria.close(); arq_chaveg.close(); arq_fun.close();
                cout << "Arquivos salvos!" <<endl;
                sleep(1);
                cout << "Saindo...";
                sleep(2);
            break;
            }
            default:{
                cout << "Opção inválida.\n" << endl;
                break;
            }
        }
    }while((alternativa != 27));
}
int main(){
    SetConsoleOutputCP(65001);

    Key Senha_geral;
    Senha_geral.setSenha("_INDEFINIDA_");

    menu(Senha_geral);
    sleep(2); system("cls");
    cout << "Alterações salvas!" << endl;
    return 0;
}