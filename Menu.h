#include <vector>
#include <unistd.h>
#include <windows.h>
#include "Classes.h"
#include "Gerenciador.h"
#include "Funcoes.h"

using namespace std;

void menu(Key chave_geral){
    Gerenciador G;
    vector<shared_ptr<pessoa>> people;
    vector<shared_ptr<proletariado>> work;
    vector<shared_ptr<cliente>> clientes;

    vector<cliente> consumidor_amigo;
    vector<mercadoria> merc_CO, merc_VE;
    vector<proletariado> funcionario;

    int cli_cadast = 0, prod_cadast = 0, fun_cadast = 0;
    int cli_ex = 0, prod_ex = 0, fun_ex = 0, num_prod_v = 0;
    string resposta, nome_dworker, senha;
    int alternativa, ind = -1;
    
    if(((verificador_arquivo("Mercadorias.txt") == false) && (verificador_arquivo("Mercadoria_beckup.txt") == false)) && ((verificador_arquivo("Pessoas.txt") == false) && (verificador_arquivo("Pessoa_beckup.txt") == false))){
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
        cout << "Digite a nova senha do sistema: "; getline(cin, senha);
        chave_geral.setSenha(senha);
    }else{
        cout << "Seja bem-vindo novamente!" << endl;
        if(verificador_arquivo("Mercadorias.txt") == false){
            cout << "O arquivo Mercadorias.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Mercadoria_beckup.txt", "Mercadorias.txt") == true){
                extracao_arq_prod(merc_CO, "Mercadorias.txt");
                verfic_venc(merc_CO);
            }else{
                cout << "Foi impossível realizar o beckup dos arquivos!" << endl;
            }
        }else{
            extracao_arq_prod(merc_CO, "Mercadorias.txt");
            verfic_venc(merc_CO);
        }
        if(verificador_arquivo("Pessoas.txt") == false){
            cout << "O arquivo Pessoas.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Pessoa_beckup.txt", "Pessoas.txt")){
                people = extracao_arq_pes("Pessoas.txt");
                extracao_vector(people, work, clientes, consumidor_amigo, funcionario);
            }else{
                cout << "Foi impossível realizar o beckup dos arquivos!" << endl;
            }
        }else{
            people = extracao_arq_pes("Pessoas.txt");
            extracao_vector(people, work, clientes, consumidor_amigo, funcionario);
        }
        if(verificador_arquivo("Mercadorias.txt") == false){
            cout << "O arquivo Mercadorias_V.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Mercadoria_V_beckup.txt", "Mercadorias.txt") == true){
                extracao_arq_prod(merc_VE, "Mercadorias_V.txt");
            }else{
                cout << "Foi impossível realizar o beckup dos arquivos!" << endl;
            }
        }else{
            extracao_arq_prod(merc_VE, "Mercadorias_V.txt");
        }
        if(verificador_arquivo("Chave_g.txt") == false){
            cout << "O arquivo Chave_g.txt está vazio, será necessário utilizar o último beckup feito." << endl;
            if(copiararq("Chave_g_beckup.txt", "Chave_g.txt") == true){
                extracao_arq_key(chave_geral, "Chave_g.txt");
            }else{
                do{
                    cerr << "A senha geral foi perdida! Por favor, digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
                    ind = escolha(funcionario, nome_dworker);
                    
                    if(funcionario[ind].getCargo() == "chefe"){
                        cout << "Acesso liberado!" << endl;
                        cout << "Digite a nova senha: "; getline(cin, senha);
                        chave_geral.setSenha(senha);
                        cout << "Nova senha criada!" << endl;
                    }else{
                        cerr << "O nome digitado não é o chefe!" << endl;
                    }
                }while(ind == -1 || funcionario[ind].getCargo() != "chefe");
            }
        }  
    }
    resp_saida(resposta);

    do{
        do{
            cout << "Selecione a sua próxima ação (escreva o número da opção):" << endl;
            cout << "1 - Registrar cliente; \n" << "2 - Registrar produto; \n" << "3 - Registrar funcionário; \n" << "4 - Excluir cliente; \n";
            cout << "5 - Excluir produto; \n"  << "6 - Demitir funcionário; \n" << "7 - Abrir lista de produto; \n" << "8 - Abrir lista de pessoas; \n";
            cout << "9 - Verificar validade dos produtos; \n" << "10 - Verificar dia de pagamento de funcionários; \n";
            cout << "11 - Realizar beckup da nova lista de produtos; \n" << "12 - Realizar beckup da nova lista de clientes; \n" << "13 - Realizar beckup de lista de produtos vendidos; \n";
            cout << "14 - Utilizar beckup de produtos mais recente; \n" << "15 - Utilizar beckup de clientes mais recente; \n" << "16 - Utilizar beckup de produtos vendidos mais recente; \n";
            cout << "17 - Saber conta de cliente; \n" << "18 - Realizar conta; \n" << "19 - Separar produto; \n" << "20 - Agrupar unidades; \n";
            cout << "21 - Realizar receita; \n" << "22 - Buscar por (cliente, produto, funcionário);\n " << "23 - Alterar cliente; \n" << "24 - Alterar produto; \n" << "25 - Alterar funcioário; \n";
            cout << "26 - Alterar senha geral; \n" << "27 - Relatório; \n " << "28 - Sair. \n" << endl;
            cout << "Digite a sua resposta: ";
            cin >> alternativa;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida. Tente novamente." << endl;
            }else{
                if(alternativa <= 0 || alternativa > 28){
                    cout << "Opção inválida, tente novamente." << endl;
                }
            }
        }while(alternativa <= 0 || alternativa > 28 || cin.fail());

        switch(alternativa){
            case 1:{
                if(G.registra_cli(merc_CO, merc_VE, consumidor_amigo, &cli_cadast, &num_prod_v) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 2:{
                if(G.registra_prod(merc_CO, &prod_cadast) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 3:{
                if(G.registra_fun(funcionario, &fun_cadast) == false){
                    cerr << "Ops! Houve um problema na hora do cadastro." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 4:{
                if(G.ex_cli(consumidor_amigo, &cli_ex) == false){
                    cerr << "Ops! Houve um problema na hora da exclusão." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 5:{
                if(G.ex_prod(merc_CO, &prod_ex) == false){
                    cerr << "Ops! Houve um problema na hora da exclusão." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 6:{
                if(G.demissao(funcionario, chave_geral, &fun_ex) == false){
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
                if(G.lista_pessoas(consumidor_amigo, funcionario) == false){
                    cerr << "Ops! Houve algum problema durante a construção da tabela." << endl;
                }
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu..." << endl;
                    break;
                }
            break;
            }
            case 9:{
                G.lista_venc(merc_CO);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 10:{
                G.temp_pg_fun(funcionario);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }//data de pagamento do funcionário
            case 11:{
                G.realiza_beckup_prod();
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 12:{
                G.realiza_beckup_pessoas(chave_geral, funcionario);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 13:{
                G.realiza_beckup_prod_ve();
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 14:{
                G.utiliza_beckup_prod();
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }

            break;
            }
            case 15:{
                G.utiliza_beckup_pessoas(funcionario, chave_geral);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 16:{
                G.utiliza_beckup_prod_V();
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                    break;
                }
            break;
            }
            case 17:{
                G.conta_cli(consumidor_amigo);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 18:{
                G.realiza_compra(merc_CO, merc_VE, &num_prod_v);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...";
                }
            break;
            }
            case 19:{
                G.separador_prod(merc_CO);
                
                if(resp_saida(resposta)){
                cout << "Retornando ao menu...";
                }  
            break;
            }
            case 20:{
                G.agrupador_prod(merc_CO);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 21:{
                G.receita_total(merc_CO, merc_VE, consumidor_amigo);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }

            case 22:{
                G.busca(merc_CO, funcionario, consumidor_amigo);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 23:{
                G.alterar_cli(consumidor_amigo, merc_CO, merc_VE, &num_prod_v);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 24:{
                G.alterar_prod(merc_CO);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 25:{
                G.alterar_fun(funcionario, chave_geral);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 26:{
                G.alterar_chave(chave_geral, funcionario);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 27:{
                G.relatorio(cli_cadast, prod_cadast, fun_cadast, cli_ex, prod_ex, fun_ex, num_prod_v, merc_VE, merc_CO);
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            case 28:{
                inversa_extracao_vector(people, consumidor_amigo, funcionario);
                if(G.salvacao_dados(chave_geral, merc_CO, merc_VE, people) == false){
                    cerr << "Houve um problema durante as salvações dos arquivos." << endl;
                }
                sleep(2);
                cout << "Salvo!" << endl;
                if(resp_saida(resposta)){
                    cout << "Retornando ao menu...\n";
                }
            break;
            }
            default:{
                cout << "Opção inválida.\n" << endl;
                break;
            }
        }
    }while((alternativa != 28));
}