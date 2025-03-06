#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include "Classes.h"
#include "Funcoes.h"

using namespace std;
using namespace chrono;

class Gerenciador{
    public:
        bool registra_cli(vector<mercadoria>& merc_c, vector<mercadoria>& merc_v, vector<cliente>& amigo, int* cli_cadast, int* num_prod_v){
            cout << "Aqui é a área de registro de cliente. \n" << endl;
            int tam, unidades_vend, ind = -1;
            string nome_dproduto, resposta, nome_temp;
            string confia;
            tdata data_dprod;

            if(verific(resposta)){
                system("cls");
                return false;
            }
            cliente comprador_atual = cliente();
            system("cls");
            if(merc_c.empty()){
                cerr << "Não há mercadoria registrada, por favor, registre o(os) produto(s) primeiro." << endl;
                return false;
            }
            system("cls");
            cout << "Aqui é a área de registro de cliente. \n" << endl;
            cout << "O cliente possui as seguintes caracterísicas: ";
            cout << " - Nome; \n - Número de produtos comprados; \n - Produtos comprados; \n - Data de vencimento do produto \n" << endl;
            cout << "Digite o nome: "; cin.ignore(); getline(cin, nome_temp); cout << endl; transf(nome_temp); comprador_atual.setNome(nome_temp);
            ind = escolha(amigo, comprador_atual.getNome());
            if(ind != -1){
                cout << "Este nome já é cadastrado no nosso sistema." << endl;
                cout << "Deseja, realmente, cadastrar este nome? (sim/não)" << endl; resposta.clear(); getline(cin, resposta); transf(resposta);
                if(resposta != "não"){
                    comprador_atual.setNome(nome_temp);
                }else{
                    cout << "Sugerimos que vá para a área 'Alterar cliente'." << endl;
                    return false;
                }
            }
            
            cout << "Digite a confiabilidade deste cliente: "; getline(cin, confia); comprador_atual.setConf(confia); cout << endl;
            cout << "Digite quantos produtos o cliente pegou: "; cin >> tam; cout << endl;
            cout << "Digite o nome, a quantidade de unidades vendidas e a data de vencimento (no formato dia/mes/ano) deste produto." << endl;
            ind = -1;
            for(int i = 0; i<tam; i++){
                unidades_vend = 0; ind = 0;

                cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                cin >> unidades_vend;
                ind = escolha(merc_c, nome_dproduto);
                if(ind == -1){
                    cout << "Mercadoria não encontrada. Por favor, verifique se a mercadoria existe ou se você escreveu incorretamente.\n";
                    return false;
                }

                cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                compra(merc_c, merc_v, nome_dproduto, unidades_vend, data_dprod);
                num_prod_v += 1;
                ind = merc_v[ind].F_show(merc_v, nome_dproduto);
                comprador_atual.getMerc().push_back(merc_v[ind]);
            }
            amigo.push_back(comprador_atual);
            cout << "Cliente cadastrado com sucesso!";
            cli_cadast ++;
            return true;
        }

        bool registra_prod(vector<mercadoria>& merc_c, int* prod_cadast){
            tdata data_dprod;
            mercadoria merc_comprada = mercadoria();
            int quant, unid;
            string nome_t, nome_dproduto, resposta;
            float v;

            cout << "Aqui é a área de registro de produto." << endl;
                if(verific(resposta)){
                    system("cls");
                    return false;
                }

                resposta.clear();
                system("cls");
                cout << "Aqui é a área de registro de produto." << endl;
                cout << "A Mercadoria possui as seguintes características: " << endl;
                cout << " - Nome;\n" << " - Quantidade de itens;\n" << " - Quantidade por pacote;\n" << " - Valor (em reais);\n" << " - Validade.\n";
                cout << "Agora, registre a mercadoria:\n";
                cout << "Nome da mercadoria: "; cin.ignore(); cin >> nome_t; cout << endl; transf(nome_t); merc_comprada.setN_prod(nome_t);
                cout << "Quantidade de mercadoria: "; cin >> quant; merc_comprada.getQuant().push_back(quant); cout << endl;
                cout << "Quantidade de unidades por fardo: "; cin >> unid; merc_comprada.setUnid(unid); cout << endl;
                cout << "Valor da mercadoria (em reais): "; cin >> v; merc_comprada.setValor(v);
                cout << "Data de validade (no formato dia/mês/ano): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                if(dataValida(data_dprod)){
                    merc_comprada.getValid().push_back(data_dprod); cout << endl;
                }else{
                    cerr << "Data inválida!" << endl;
                    return false;
                }

                resposta.clear();
                for(size_t i = 0; i<merc_c.size(); i++){
                    if(merc_c[i].getNome() == nome_t){
                        cout << "Uma mercadoria já está registrada no nosso sistema.\n Caso for para adicionar unidades, sugerimos que vá para a área 'Alterar produto'.\n";
                        cout << "Deseja, mesmo assim, adicionar este produto (sim / não)?"; getline(cin, resposta); transf(resposta);
                        if(resposta != "sim"){
                            cout << "Por encontrar um produto já cadastrado, iremos te redirecionar para o menu." <<endl;
                            cout << "Redirecionando ao menu...";
                            return false;
                        }
                    }
                }
                merc_c.push_back(merc_comprada);
                cout << "Registro completo!";
                prod_cadast ++;
                return true;
        }

        bool registra_fun(vector<proletariado>& trabalhador, int* fun_cadast){
            string resposta,nome_dworker;
            string conf, carg;
            tdata data_p;
            float v;

            cout << "Aqui é a área para registrar um funcionário. \n";
            if(verific(resposta)){
                system("cls");
                return false;
            }

            cout << "Aqui é a área para registrar um funcionário. \n";
            cout << "Digite o nome do funcionário: "; cin.ignore(); getline(cin, nome_dworker); transf(nome_dworker);
            cout << "Digite a confiança deste funcionário (pouco confiável/ confiável/ muito confiável): "; getline(cin, conf); transf(conf);
            if(conf != "pouco confiável" || conf != "confiável" || conf != "muito confiável"){
                cerr << "Ops! A confiabilidade não foi bem definida." << endl;
                return false;
            }
            cout << "Digite o cargo deste funcionário: "; cin.ignore(); getline(cin, carg); transf(carg);
            if(proc_carg(trabalhador, "chefe")){
                cerr << "Já existe alguém com este cargo." << endl;
                return false;
            }
            cout << "Digite o salário do novo funcionário: "; cin >> v;
            cout << "Digite a data de pagamento do novo funcionário (d/m/a): "; cin >> data_p.day >> data_p.mon >> data_p.year;
            if(dataValida(data_p)){
                trabalhador.emplace_back(nome_dworker, conf, carg, v, data_p);
                cout << "Funcionário registrado!" << endl;
                fun_cadast++;
                return true;
            }else{
                cout << "Data inválida!" << endl;
                return false;
            }
        }

        bool ex_cli(vector<cliente>& amigo, int* cli_ex){
            string resposta, nome_dcliente;
            int ind = -1;

            cout << "Aqui é a área para excluir um cliente. \n";
            nome_dcliente.clear();
            if(verific(resposta)){
                system("cls");
                return false;
            }   
            system("cls");
            cout << "Aqui é a área para excluir um cliente. \n";
            cout << "Digite o nome do cliente corretamente: ";
            cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);

            ind = escolha(amigo, nome_dcliente);
            if(ind == -1){
                cerr << "Cliente não encontrado!" << endl;
                return false;
            }

            cout << "Cliente encontrado!" << endl;
            if(!(amigo[ind].getMerc().empty())){
                cout << "Você não pode deletar este cliente, pois apresenta itens comprados!\n";
                cout << "Caso o cliente tenha pagado tudo, sugerimos que vá primeiro para a área 'Alterar cliente' e depois retorne.\n";
                cout << "Redirecionando ao Menu...";
                return false;
            }else{
                resposta.clear();
                cout << "Deseja deletar o cliente " << amigo[ind].getNome() << "? (sim/não)" << endl;
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    amigo.erase(amigo.begin() + ind);
                    cout << "Cliente " << amigo[ind].getNome() << " deletado!" << endl;
                    cli_ex += 1;
                    return true;
                }
            }
            return false;
        }

        bool ex_prod(vector<mercadoria>& merc_c, int* prod_ex){
            string nome_dproduto, resposta;
            int ind = -1, soma;

            cout << "Aqui é a área de exclusão de produto.\n";
            if(verific(resposta)){
                system("cls");
                return false;
            }
            system("cls");
            cout << "Aqui é a área de exclusão de produto.\n";
            cout << "Digite o nome do produto: ";
            cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);

            ind = escolha(merc_c, nome_dproduto);
            if(ind == -1){
                cerr << "Produto não encontrado. Por favor, verifique se o nome foi digitado corretamente." << endl;
                return false;
            }

            cout << "Produto encontrado!" << endl;
            for(int quant : merc_c[ind].getQuant()){
                soma += quant;
            }
            if(soma != 0){
                cout << "O produto apresenta unidades. Caso ele, na realidade, não possua mais unidades, sugerimos que vá para a área 'Alterar produto'." << endl;
                return false;
            }else{
                resposta.clear();
                cout << "Deseja deletar o produto " << merc_c[ind].getNome() << "?\n";
                cin >> resposta; transf(resposta);
                if(resposta == "sim"){
                    merc_c.erase(merc_c.begin()+ind);
                    cout << "Produto " << merc_c[ind].getNome() << "deletado!\n";
                    prod_ex += 1;
                    return true;
                }
            }
            return false;
        }

        bool demissao(vector<proletariado>& worker, Key chave_geral, int* fun_ex){
            string chave_comp, nome_dworker, resposta;
            int ind = -1;

            cout << "Aqui é a área de demissão de funcionário.\n";
            do{
                if(verific(resposta)){
                    system("cls");
                    break;
                }
                system("cls");

                if(chave_geral.getSenha() == "_INDEFINIDA_"){
                    cout << "A senha ainda não foi definida, por favor, altere-a primeiro." << endl;
                    break;
                }
                cout << "Aqui é a área de demissão de funcionário.\n";
                cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
                ind = escolha(worker, nome_dworker);

                if(ind == -1){
                    cout << "Não foi possível encontrar o seu nome." << endl;
                }
            }while(ind == -1);
            if(ind == -1){
                return false;
            }
            cout << "Digite a senha geral: "; cin.ignore(); getline(cin, chave_comp);
            if(chave_geral.verific_S(chave_comp, worker[ind])){
                nome_dworker.clear();
                cout << "Acesso liberado!" << endl;
                
                cout << "Digite o nome do funcionário que será demitido: "; getline(cin, nome_dworker); transf(nome_dworker);
                ind = escolha(worker, nome_dworker);
                if(ind == -1){
                    cout << "O nome digitado não foi encontrado no nosso sistema." << endl;
                    cout << "Veja os nomes de funcionários escolhendo a opção 'lista de funcionários'." << endl;
                }else{
                    cout << "Deseja mesmo demitir este funcionário? (sim/não)" << endl; getline(cin, resposta); transf(resposta);
                    if(resposta == "sim"){
                        worker.erase(worker.begin() + ind);
                        cout << "Funcionário demitido!" << endl;
                        fun_ex += 1;
                        return true;
                    }
                }           
            }
            return false;
        }

        void lista_prod(vector<mercadoria> merc_c, vector<mercadoria> merc_v){
            string resposta;

            cout << "Aqui é a área da lista de produtos." << endl;
            if(verific(resposta)){
                system("cls");
                return;
            }

            system("cls");
            resposta.clear();

            cout << "Aqui é a área da lista de produtos." << endl;
            cout << "Deseja saber a lista de produtos comprados ou vendidos?"; cin.ignore(); getline(cin, resposta); transf(resposta);
            
            if(resposta == "comprados"){
                if(quadro_prod(merc_c) == false){
                    cout << "Os produtos não foram registrados, por favor, registre-os." << endl;
                }
            }else{
                if(quadro_prod(merc_v) == false){
                    cout << "Você ainda não vendeu os produtos." << endl;
                }else{
                    cerr << "Escolha incorreta." << endl;
                }
            }
        }

        bool lista_pessoas(vector<cliente> clientes, vector<proletariado> Work){
            string resposta;

            cout << "Aqui é a área da lista de pessoas." << endl;
            if(verific(resposta)){
                system("cls");
                return false;
            }

            system("cls");
            resposta.clear();

            cout << "Aqui é a área da lista de pessoas." << endl;
            cout << "Digite qual lista você gostaria de ver (cliente/funcionário): "; getline(cin, resposta); transf(resposta);
            
            if(resposta == "cliente"){
                if(clientes.empty()){
                    cerr << "Não foi registrado nenhum cliente!" << endl;
                    return false;
                }
                const int larguraNome = 15;
                const int larguraConf = 15;
                const int larguraItens = 15;

                cout << left << setw(larguraNome) << "Nome"
                    << setw(larguraConf) << "Confiança"
                    << setw(larguraItens * 3) << "Itens" << endl;
                    cout << string(75, '-') << endl;
                
                for(size_t i = 0; i<clientes.size(); i++){
                    vector<string> d = clientes[i].dadosPtabela();
                    cout << left << setw(larguraNome) << d[0]
                    << setw(larguraConf) << d[1]
                    << setw(larguraItens * 3) << d[2] << endl;
                }
                return true;
            }else{
                if(resposta == "funcionário"){
                    if(Work.empty()){
                        cerr << "Não foi registrado nenhum funcionário!" << endl;
                        return false;
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
                        for(size_t i = 0; i<Work.size(); i++){
                            vector<string> d = Work[i].dadosPtabela();
                            cout << left << setw(larguraNome) << d[0]
                                << setw(larguraConf) << d[1]
                                << setw(larguraCarg) << d[2]
                                << setw(larguraSal) << d[3]
                                << setw(larguraDat) << d[4] << endl;
                    }
                    return true;
                }else{
                    cout << "Ops! Ocorreu um erro de digitação." << endl;
                    cout << "Não foi possível mostrar os dados." << endl;
                    return false;
                }
            }
        }

        void lista_venc(vector<mercadoria> merc_c){
            string resposta, nome_dproduto;
            vector<double> diff_data;
            int ind = -1;

            cout << "Aqui é a área de validades.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }
            if(merc_c.empty()){
                cout << "Não há produtos registrados!" <<endl;
                if(resp_saida(resposta)){
                cout << "Retornando ao menu...";
                }
                return;
            }

            system("cls"); resposta.clear();
            cout << "Aqui é a área de validades.\n";
            cout << "Deseja saber a validade de um único produto? (sim/não)\n";
            cin >> resposta; transf(resposta);
            if(resposta == "sim"){
                cout << "Digite o nome do produto que deseja ver a validade: "; getline(cin, nome_dproduto); transf(resposta);
                ind = escolha(merc_c, nome_dproduto);
                if(ind < 0){
                    cout << "Produto não encontrado.\n";
                    return;
                }
                comparador_data(merc_c[ind].getValid(), diff_data);
                for(size_t i = 0; i<diff_data.size(); i++){
                    if(diff_data[i] <= 0){
                        cout << "O produto " << merc_c[ind].getNome() << "apresenta " << merc_c[ind].getQuant()[i] << "unidades venciadas.\n";
                    }else{
                        cout << merc_c[ind].getNome() << "(" << merc_c[ind].getValid()[i].day << "/" << merc_c[ind].getValid()[i].mon << "/" << merc_c[ind].getValid()[i].year << ")";
                        cout << "||";
                        cout << "Há " << merc_c[ind].getQuant()[i] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                    }
                }
            }else{
                cout << "Aqui estão as validades e o tempo (em dias) até o vencimento:\n";                
                for(size_t i = 0; i<merc_c.size(); i++){
                    comparador_data(merc_c[i].getValid(), diff_data);
                    for(size_t j = 0; i<diff_data.size(); i++){
                        if(diff_data[j] <= 0){
                            cout << "O produto " << merc_c[i].getNome() << "apresenta " << merc_c[i].getQuant()[j] << "unidades venciadas.\n";
                        }else{
                            cout << merc_c[i].getNome() << " (" << merc_c[i].getValid()[j].day << "/" << merc_c[i].getValid()[j].mon << "/" << merc_c[i].getValid()[j].year << ")";
                            cout << " || ";
                            cout << "Há " << merc_c[i].getQuant()[j] << "unidades que irá(ão) em" << diff_data[i] << "dias\n";
                        }
                    }
                }
            }
        }

        void temp_pg_fun(vector<proletariado> worker){
            double diff_t;
            string resposta;

            cout << "Aqui é a área que mostra o tempo até o dia de pagamento do funcionário." << endl;
            if(verific(resposta)){
                system("cls");
                return;
            }

            
            cout << "Aqui é a área que mostra o tempo até o dia de pagamento do funcionário." << endl;
            cout << left << setw(15) << "Nome"
                << setw(15) << "Data de pagamento"
                << setw(15) << "Quantidade de dias" << endl;
            for(size_t i = 0; i<worker.size(); i++){
                auto now = system_clock::now();
                time_t time_now = system_clock::to_time_t(now);
                tm* local_time = localtime(&time_now);

                struct tm data_p;
                time_t tempo_f, tempo_i;

                data_p.tm_year = worker[i].getpay_day().year - 1900;
                data_p.tm_mon = worker[i].getpay_day().mon - 1;
                data_p.tm_mday = worker[i].getpay_day().day;

                tempo_f = mktime(&data_p);
                tempo_i = mktime(local_time);
                
                diff_t = difftime(tempo_f,tempo_i)/(60*60*24);
                
                cout << left << setw(15) << worker[i].getNome()
                    << setw(15) << to_string(worker[i].getpay_day().day) + "/" + to_string(worker[i].getpay_day().mon) + "/" + to_string(worker[i].getpay_day().year)
                    << setw(15) << diff_t << endl;
            }
        }

        void realiza_beckup_prod(){
            string resposta;

            cout << "Aqui é a área de beckup da lista de produto.\n";
            if(verific(resposta)){
                system("cls");
                return;
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
                        return;
                    }
                }else{
                    cout << "A lista de produtos está vazia. Por favor, registre os itens primeiro.\n";
                    cout << "Redirecionando para o Menu...";
                    return;
                }
            }
        }
        void realiza_beckup_pessoas(Key chave_geral, vector<proletariado> W){
            string resposta, nome_dworker, senha_t;
            int ind = -1;

            cout << "Aqui é a área de beckup da lista de clientes.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            resposta.clear();
            system("cls");

            cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
            ind = escolha(W, nome_dworker);
            cout << "Digite a senha: "; getline(cin, senha_t);
            if(chave_geral.verific_S(senha_t, W[ind]) == false){
                cout << "Senha e/ou nome inválidos" << endl;
            }

            cout << "Acesso liberado!" << endl;
            cout << "Aqui é a área de beckup da lista de pessoas, ou seja, de funcionários e clientes.\n";
            cout << "Deseja salvar a lista de pessoas no arquivo de beckup? (sim/não)\n";
            cin >> resposta; transf(resposta);

            if(resposta != "não"){
                if(verificador_arquivo("")){
                    cout << "O beckup será realizado, aguarde um momento.\n";
                    if(copiararq("Pessoas.txt","Pessoa_beckup.txt")){
                        cout << "Beckup realizado com sucesso.!\n";
                    }else{
                        cout << "Infelizmente houve um erro durante o processo.\n";
                    }
                }else{
                    cout << "A lista de clientes está vazia. Por favor, registre os clientes primeiro.\n";
                }
            }
        }

        void realiza_beckup_prod_ve(){
            string resposta;

            cout << "Aqui é a área de beckup da lista de produtos vendidos.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }
            resposta.clear();
            system("cls");
            cout << "Aqui é a área de beckup da lista de produtos vendidos.\n";
            cout << "Deseja salvar a lista de produtos vendidos no arquivo de beckup? (sim/não)\n";
            cin >> resposta; transf(resposta);

            if(resposta != "não"){
                if(verificador_arquivo("Mercadorias_V.txt")){
                    cout << "O beckup será realizado, aguarde um momento.\n";
                    if(copiararq("Mercadorias_V.txt","Mercadoria_V_beckup.txt")){
                        cout << "Beckup realizado com sucesso.!\n";
                    }else{
                        cout << "Infelizmente houve um erro durante o processo.\n";
                        cout << "Redirecionando para o menu...";
                        return;
                    }
                }else{
                    cout << "A lista de produtos está vazia. Por favor, registre os itens primeiro.\n";
                    cout << "Redirecionando para o Menu...";
                    return;
                }
            }
        }

        void utiliza_beckup_prod(){
            string resposta;

            cout << "Aqui é a área para utilizar o beckup de produtos. \n";
            if(verific(resposta)){
                system("cls");
                return;
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
                    return;
                }
            }
        }

        void utiliza_beckup_pessoas(vector<proletariado> workers, Key chave_geral){
            string resposta, senha_t, nome_dworker;
            int ind = -1;

            cout << "Aqui é a área para utilizar o beckup de clientes. \n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            cout << "Digite o seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
            ind = escolha(workers, nome_dworker);
            if(ind == -1){
                cout << "Nome não encontrado.";
                return;
            }
            cout << "Digite a senha: "; getline(cin, senha_t);
            if(chave_geral.verific_S(senha_t, workers[ind]) == false){
                cerr << "Senha ou nome inválido." << endl;
                return;
            }
            system("cls");
            cout << "Acesso liberado!" << endl;
            cout << "Aqui é a área para utilizar o beckup de clientes. \n";
            cout << "Caso o beckup seja utilizado, as informações alteradas na lista de clientes serão peridas.\n";
            cout << "Deseja utilizar o beckup? (sim/não)\n";
            cin >> resposta; transf(resposta);
            if(resposta == "sim"){
                cout << "Aguarde um momento...\n";
                if(copiararq("Pessoa_beckup.txt","Pessoas.txt")){
                    cout << "Beckup feito com sucesso!\n";
                }
            }else{
                cout << "Beckup não realizado!" << endl;
            }
        }

        void utiliza_beckup_prod_V(){
            string resposta;

            cout << "Aqui é a área para utilizar o beckup de produtos vendidos. \n";
            if(verific(resposta)){
                system("cls");
                return;
            }   
            system("cls");
            resposta.clear();
            cout << "Aqui é a área para utilizar o beckup de produtos vendidos. \n";
            cout << "Caso o beckup seja utilizado, as informações alteradas na lista de produtos serão peridas.\n";
            cout << "Deseja utilizar o beckup? (sim/não)\n";
            cin >> resposta; transf(resposta);
            if(resposta == "sim"){
                cout << "Aguarde um momento...\n";
                if(copiararq("Mercadoria_V_beckup.txt","Mercadorias_V.txt")){
                    cout << "Beckup feito com sucesso!\n";
                    sleep(2); system("cls");
                    return;
                }
            }
        }

        void conta_cli(vector<cliente> cli){
            string resposta, nome_dcliente;

            cout << "Aqui é a área para realizar a conta do cliente. \n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            system("cls");
            cout << "Aqui é a área para saber a conta do cliente, ou seja, quanto o cliente cadastrado deve. \n";
            cout << "Digire o nome do cliente: ";
            cin >> nome_dcliente; transf(nome_dcliente);
            float soma = conta_cliente(cli, nome_dcliente);
            if(soma == -1.0){
                return;
            }else{
                cout << "O valor da conta do cliente " << nome_dcliente << "é de: R$" << soma << endl; 
            }
        }

        void realiza_compra(vector<mercadoria> merc_c, vector<mercadoria> merc_v, int* num_prod_v){
            string resposta, nome_dproduto;
            int quant_itens, unid_comp, ind = -1; nome_dproduto.clear();
            float valor_conta = 0.0, pagamento_cl, troco;
            tdata data_dprod;

            cout << "Aqui é a área para realizar a conta da compra.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            system("cls");
            cout << "Aqui é a área para realizar a conta da compra.\n";
            cout << "OBS: caso os produtos do mesmo item possuirem datas de validade diferentes, digite a quatidade de produtos de cada validade.\n";
            cout << "Digite quantos tipos de itens o cliente comprou: ";
            cin >> quant_itens;
            system("cls");
            for(int i = 0; i < quant_itens; i++){
                cout << "Aqui é a área para realizar a conta da compra.\n";
                do{
                    cout << "Digite o nome do produto e a quantidade comprada deste produto: ";
                    cin.ignore(); getline(cin,nome_dproduto); transf(nome_dproduto);
                    ind = escolha(merc_c, nome_dproduto);
                    cin >> unid_comp;
                    if(ind < 0){
                        cout << "Produto não encontrado!" << endl;
                        cout << "Tente novamente.";
                        system("cls");
                    }
                }while(ind < 0);
                cout << "Digite a data de validade deste produto (d/m/a): ";
                cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                compra(merc_c, merc_v, nome_dproduto, unid_comp, data_dprod);
                valor_conta += merc_c[ind].getValor() * unid_comp;
                num_prod_v += 1;
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
        }

        void separador_prod(vector<mercadoria> merc_c){
            string resposta, nome_dproduto;
            tdata data_dprod;
            int quant_sep, ind = -1;

            cout << "Aqui é a área de separação de produto.\n";
                if(verific(resposta)){
                    system("cls");
                    return;
                }

                system("cls");
                cout << "Aqui é a área de separação de produto.\n";
                cout << "Digite o nome do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);

                ind = escolha(merc_c, nome_dproduto);
                
                if(ind < 0){
                    cout << "Produto não encontrado." << endl;
                    return;
                }
                cout << "\nDigite quantas vezes essa separação ocorrerá: "; cin >> quant_sep;
                cout << "\nDigite a data de validade do produto a ser desagrupado (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                if(desagrupador(merc_c, ind, quant_sep, data_dprod) == false){
                    cerr << "Não foi possível realizar a separação do produto." << endl;
                    return;
                }
        }

        void agrupador_prod(vector<mercadoria> merc_c){
            string resposta, nome_dproduto;
            int ind = -1;

            cout << "Aqui é a área para agrupar produtos de um único nome.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            if(merc_c.empty()){
                cerr << "Não registrado nenhum produto!" << endl;
                return;
            }

            system("cls");
            cout << "Aqui é a área para agrupar produtos de um único nome.\n";
            cout << "Digite o nome do produto corretamente: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
            ind = escolha(merc_c, nome_dproduto);
            if(ind == -1){
                cout << "Produto não encontrado." << endl;
                return;
            }
            if(agrupador(merc_c, ind) == false){
                cerr << "Não foi possível realizar o agrupamento do produto escolhido." << endl;
            }
        }

        void receita_total(vector<mercadoria> merc_c, vector<mercadoria> merc_v, vector<cliente> amigos){
            string resposta;

            cout << "Aqui é a área da receita.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            system("cls");
            cout << "Aqui é a área da receita.\n";
            cout << "Só será possível saber a receita total, ou seja, não há como escolher perídos para analisar!\n";
            cout << "A sua receita é de: R$" << receita(merc_c, merc_v, amigos) << endl;
        }

        void busca(vector<mercadoria> merc_c, vector<proletariado> workers, vector<cliente> amigos){
            string resposta, nome_dcliente, nome_dproduto, nome_dworker;
            int ind = -1;

            cout << "Aqui é a área de busca.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }
            system("cls"); resposta.clear();

            cout << "Aqui é a área de busca.\n";
            cout << "O que você está produrando no sistema? (cliente, produto, funcionário)" << endl; getline(cin,resposta); transf(resposta);
            if(resposta != "cliente" || resposta != "produto" || resposta != "funcionário"){
                cerr << "Resposta inválida" << endl;
                return;
            }
            if(resposta == "cliente"){
                if(amigos.empty()){
                    cerr << "Não há clientes cadastrados." << endl;
                }
                cout << "Digite o nome do cliente: "; getline(cin, nome_dcliente); transf(nome_dcliente);
                ind = escolha(amigos, nome_dcliente);
                if(ind == -1){
                    cerr << "Cliente não encontrado!" << endl;
                    return;
                }
                cout << "Aqui estão as informações deste cliente: " << endl;
                cout << " - Nome: " << amigos[ind].getNome() << endl;
                cout << " - Confiança: " << amigos[ind].getConf() << endl;
                cout << " - Itens pegos : " << endl;
                for(size_t i = 0; i<amigos[ind].getMerc().size(); i++){
                    int qtdT = 0;
                    for(size_t j = 0; j<amigos[ind].getMerc()[i].getQuant().size(); j++){
                        qtdT += amigos[ind].getMerc()[i].getQuant()[j];
                    }
                    cout << "  - " << amigos[ind].getMerc()[i].getNome() << " | Quantidade comprada: " << qtdT << endl;
                    cout << " - Dívida (em reais): " << amigos[ind].divida(amigos[ind].getMerc()) << endl;
                }
            }
            if(resposta == "produto"){
                if(merc_c.empty()){
                    cerr << "Não há produtos cadastrados." << endl;
                    return;
                }
                cout << "Digite o nome do produto: "; getline(cin, nome_dproduto); transf(nome_dproduto);
                ind = escolha(merc_c, nome_dproduto);
                if(ind == -1){
                    cerr << "Produto não encontrado!" << endl;
                    return;
                }

                cout << "Aqui estão as informações deste produto: " << endl;
                cout << " - Nome: " << merc_c[ind].getNome() << endl;
                cout << " - Quantidades do produto (por validade): " << endl;
                for(size_t i = 0; i<merc_c[ind].getQuant().size(); i++){
                    cout << "  - " << merc_c[ind].getQuant()[i] << " || " << merc_c[ind].getValid()[i].day << "/" << merc_c[ind].getValid()[i].mon << "/" << merc_c[ind].getValid()[i].year << endl;
                }
                cout << " - Unidade por pacote: " << merc_c[ind].getUnid() << endl;
                cout << " - Valor (em reais): " << merc_c[ind].getValor() << endl; 
            }
            if(resposta == "funcionário"){
                if(workers.empty()){
                    cerr << "Não há funcionários cadastrados." << endl;
                    return;
                }

                cout << "Digite o nome do funcionário: "; getline(cin, nome_dworker); transf(nome_dworker);
                ind = escolha(workers, nome_dworker);
                if(ind == -1){
                    cerr << "Nome não encontrado!" << endl;
                    return;
                }

                cout << "Aqui estão as informações do funcionário: " << endl;
                cout << " - Nome: " << workers[ind].getNome() << endl;
                cout << " - Confiança: " << workers[ind].getConf() << endl;
                cout << " - Cargo: " << workers[ind].getCargo() << endl;
                cout << " - Salário (em reais): " << workers[ind].getSalario() << endl;
                cout << " - Dia de pagamento: " << workers[ind].getpay_day().day << "/" << workers[ind].getpay_day().mon << "/" << workers[ind].getpay_day().year << endl;
            }
        }

        void alterar_cli(vector<cliente>& amigo, vector<mercadoria> merc_c, vector<mercadoria> merc_v, int* num_prod_v){
            string resposta, nome_dcliente, nome_dproduto;
            int ind = -1;

            cout << "Aqui é a área para alterar o cliente.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }
            
            if(amigo.empty()){
                cout << "Não há cliente cadastrado." << endl;
                return;
            }

            int opcao;
            system("cls"); nome_dcliente.clear();
            cout << "Aqui é a área para alterar o cliente.\n";
            cout << "Digite o nome do cliente que será alterado: "; cin.ignore(); getline(cin, nome_dcliente); transf(nome_dcliente);
            ind = escolha(amigo,nome_dcliente);
            if(ind == -1){
                cout << "Cliente não encontrado. Por favor, verifique se o nome está cadastrado ou se foi digitado corretamente.\n";
                return;
            }else{
                cout << "Cliente encontrado!" << endl;
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

                            cout << "Digite o novo nome do cliente: "; getline(cin, nome_dcliente); transf(nome_dcliente);
                            for(size_t i = 0; i<amigo.size(); i++){
                                if(client_t.F_ind(amigo, nome_dcliente)){
                                    cout << "O nome digitado já está cadastrado no nosso sistema!\n";
                                    cout << "Deseja realmente cadastrar este nome? (sim/não)\n"; getline(cin, resposta); transf(resposta);
                                    if(resposta == "sim"){
                                        amigo[ind].setNome(nome_dcliente);
                                    }else{
                                        resp_saida(resposta);
                                        cout << "Retornando à escolha das alterações...";
                                        break;
                                    }
                                }
                            }
                            amigo[ind].setNome(nome_dcliente);
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
                            mercadoria merc_comprada;
                            tdata data_dprod;

                            cout << "Quantos tipos de itens por data de validade o cliente comprou?";
                            cin >> quanti_itens; cout << endl;
                            for(int j = 0; j < quanti_itens; j++){
                                do{
                                    nome_dproduto.clear(); ind = -1; merc_comprada.getNome().clear(); data_dprod = {0,0,0};
                                    cout << "Digite o nome correto do produto: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
                                    for(size_t k = 0; k<merc_c.size(); k++){
                                        if(merc_c[k].getNome() == nome_dproduto){
                                            ind_c = k;
                                            cout << "Produto encontrado!\n";
                                            merc_comprada.setN_prod(nome_dproduto);
                                            merc_comprada.setValor(merc_c[k].getValor() * 1.2);
                                            merc_comprada.setUnid(merc_c[k].getUnid());
                                        }
                                    }
                                    if(ind_c == -1){
                                        cout << "Produto não encontrado. Por favor, tente novamente.\n";
                                    }
                                }while(ind_c == -1);
                                cout << "Digite quantas unidades o cliente comprou: "; cin >> unid_compradas;
                                cout << "Digite a data de validade (d/m/a): "; cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                                merc_comprada.getValid().push_back(data_dprod); 
                                
                                if(!(compra(merc_c, merc_v, nome_dproduto, unid_compradas, data_dprod))){
                                    cout << "Por favor, verifique a lista de itens e a atualize se necessário." << endl;
                                    if(resp_saida(resposta)){
                                        cout << "Retornando à escolha das alterações...";
                                    }
                                    break;
                                }else{
                                    num_prod_v += 1;
                                }

                                int ind_c = -1;
                                for(size_t k = 0; k<amigo[ind].getMerc().size(); k++){
                                    if(amigo[ind].getMerc()[k].getNome() == nome_dproduto){
                                        ind_c = k;
                                        for(size_t L = 0; L < amigo[ind].getMerc()[k].getValid().size(); L++){
                                            if(amigo[ind].getMerc()[k].getValid()[L].day + amigo[ind].getMerc()[k].getValid()[L].mon + amigo[ind].getMerc()[k].getValid()[L].year == data_dprod.day + data_dprod.mon + data_dprod.year){
                                                ind_data = L;
                                                amigo[ind].getMerc()[k].getQuant()[L] += unid_compradas;
                                            }
                                        }
                                    }
                                }
                                if(ind_c == -1){
                                    amigo[ind].getMerc().push_back(merc_comprada);
                                }
                                if((ind_c != -1) && (ind_data == -1)){
                                    amigo[ind].getMerc()[ind_c].getValid().push_back(data_dprod);
                                    amigo[ind].getMerc()[ind_c].getQuant().push_back(unid_compradas);
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
                            amigo[ind].setConf(conf);
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
                            for(size_t j = 0; j<amigo[ind].getMerc().size(); j++){
                                for(size_t k = 0; k < amigo[ind].getMerc().size()-1-j; k++){
                                    if(amigo[ind].getMerc()[k].getValor() > amigo[ind].getMerc()[k+1].getValor()){
                                        merc_troca.setN_prod(amigo[ind].getMerc()[k].getNome());
                                        for(size_t L = 0; L < amigo[ind].getMerc()[k].getQuant().size(); L++){
                                            merc_troca.getQuant().push_back(amigo[ind].getMerc()[k].getQuant()[L]);
                                            merc_troca.getValid().push_back(amigo[ind].getMerc()[k].getValid()[L]);
                                        }
                                        merc_troca.setUnid(amigo[ind].getMerc()[k].getUnid());
                                        merc_troca.setValor(amigo[ind].getMerc()[k].getValor());

                                        int x = amigo[ind].getMerc()[k].getQuant().size();
                                        int y = amigo[ind].getMerc()[k+1].getQuant().size();

                                        if(x - y < 0){
                                            tam_quant = amigo[ind].getMerc()[k+1].getQuant().size();
                                        }else{
                                            tam_quant = amigo[ind].getMerc()[k].getQuant().size();
                                        }
                                        amigo[ind].getMerc()[k].setN_prod(amigo[ind].getMerc()[k+1].getNome());

                                        for(size_t L = 0; L < tam_quant; L++){
                                            amigo[ind].getMerc()[k].getQuant().push_back(amigo[ind].getMerc()[k+1].getQuant()[L]);
                                            amigo[ind].getMerc()[k].getValid().push_back(amigo[ind].getMerc()[k+1].getValid()[L]);
                                        }
                                        amigo[ind].getMerc()[k].setValor(amigo[ind].getMerc()[k+1].getValor());
                                        amigo[ind].getMerc()[k].setUnid(amigo[ind].getMerc()[k+1].getUnid());

                                        amigo[ind].getMerc()[k+1].setN_prod(merc_troca.getNome());
                                        for(size_t L = 0; L < merc_troca.getQuant().size(); L++){
                                            amigo[ind].getMerc()[k+1].getQuant().push_back(merc_troca.getQuant()[L]);
                                            amigo[ind].getMerc()[k+1].getValid().push_back(merc_troca.getValid()[L]);
                                        }
                                        amigo[ind].getMerc()[k+1].setValor(merc_troca.getValor());
                                        amigo[ind].getMerc()[k+1].setUnid(merc_troca.getUnid());
                                    }
                                }
                            }
                            int n = -1;
                            float soma_comp = 0.0;
                            mercadoria merc_resto;
                            for(size_t j = 0; j<amigo[ind].getMerc().size(); j++){
                                for(size_t k = 0; k < amigo[ind].getMerc()[j].getQuant().size(); k++){
                                    while((amigo[ind].getMerc()[j].getQuant()[k] != 0) && (pagamento > 0)){
                                        amigo[ind].getMerc()[j].getQuant()[k]--;
                                        pagamento -= amigo[ind].getMerc()[j].getValor();
                                        if(pagamento < 0){
                                            for(size_t L=0; L<amigo[ind].getMerc().size(); L++){
                                                size_t posicao = amigo[ind].getMerc()[L].getNome().find("Resto");
                                                if(!(posicao == string::npos)){
                                                    n = L;
                                                    amigo[ind].getMerc()[L].setValor(amigo[ind].getMerc()[L].getValor() - pagamento);
                                                }
                                            }
                                            if(n == -1){
                                                merc_resto.setN_prod("Resto");
                                                merc_resto.getQuant().push_back(1);
                                                merc_resto.setUnid(1);
                                                merc_resto.setValor(-pagamento);
                                                merc_resto.getValid().push_back({0,0,0});
                                                amigo[ind].getMerc().push_back(merc_resto);
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
                            for(size_t j = 0; j<amigo[ind].getMerc().size(); j++){
                                for(size_t k = 0; k < amigo[ind].getMerc()[j].getQuant().size(); k++){
                                    soma_comp += amigo[ind].getMerc()[j].getQuant()[k];
                                }
                                if(soma_comp == 0){
                                    amigo[ind].getMerc().erase(amigo[ind].getMerc().begin()+j);
                                    j=0;
                                }else{
                                    for(size_t k = 0; k < amigo[ind].getMerc()[j].getQuant().size(); k++){
                                        if(amigo[ind].getMerc()[j].getQuant()[k] == 0){
                                            amigo[ind].getMerc()[j].getQuant().erase(amigo[ind].getMerc()[j].getQuant().begin()+k);
                                            amigo[ind].getMerc()[j].getValid().erase(amigo[ind].getMerc()[j].getValid().begin()+k);
                                            k=0;
                                        }
                                    }
                                }
                                soma_comp = 0.0;
                            }
                            cout << "pagamento realizado!" << endl;
                            cout << "Agora o cliente está devendo (em reais): R$" << conta_cliente(amigo, amigo[ind].getNome());
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
        }

        void alterar_prod(vector<mercadoria>& merc_c){
            string resposta, nome_dproduto;
            int ind = -1;

            cout << "Aqui é a área para alterar o produto.\n";
            if(verific(resposta)){
                system("cls");
                return;
            }

            if(merc_c.empty()){
                cout << "Não há mercadorias cadastradas. Por favor, cadastre primeiro." << endl;
                return;
            }
            system("cls");
            int opcao, ind_comp = -1;

            string novo_nome;
            int nova_quant;
            int nova_unidppacote;
            float novo_valor;
            tdata nova_valid;
            mercadoria merc_nova;

            cout << "Aqui é a área para alterar o produto.\n";
            cout << "Digite o nome do produto que desejas modificar: "; cin.ignore(); getline(cin, nome_dproduto); transf(nome_dproduto);
            ind = escolha(merc_c, nome_dproduto);
            if(ind == -1){
                cerr << "Produto não encontrado." << endl;
                return;
            }
            
            
            cout << "Mercadoria encontrada!" << endl;
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
                        cout << "Digite o novo nome do produto: "; getline(cin, novo_nome); transf(novo_nome);
                        ind_comp = escolha(merc_c, novo_nome);
                        if(ind_comp == -1){
                            merc_c[ind].getNome().clear(); merc_c[ind].setN_prod(novo_nome);
                            cout << "Alteração realizada com sucesso!" << endl;
                        }else{
                            resposta.clear();
                            cout << "Há um produto com este nome, quer mesmo alterar o nome deste produto? (sim/não)" << endl;
                            getline(cin, resposta); transf(resposta);
                            if(resposta == "sim"){
                                merc_c[ind].setN_prod(novo_nome);
                                cout << "Alteração feita com sucesso!";
                            }else{
                                cout << "Certo. ";
                            }
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
                        do{
                            if(ind_comp == -1){
                                if(verific(resposta)){
                                    system("cls");
                                    break;
                                }
                            }
                            system("cls");
                            cout << "Digite a data de validade do produto que você deseja alterar (d/m/a): "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                        }while(dataValida(nova_valid) == false);
                        if(dataValida(nova_valid) == false){
                            cout << "A data digitada é inválida." << endl;
                            cout << "Retornando ao menu de alterações...";
                            break;
                        }else{
                            for(size_t i = 0; i<merc_c[ind].getValid().size(); i++){
                                if(merc_c[ind].getValid()[i].day + merc_c[ind].getValid()[i].mon + merc_c[ind].getValid()[i].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                    cout << "Digite a nova quantidade: "; cin >> nova_quant;
                                    merc_c[ind].getQuant()[i] = nova_quant;
                                }
                            }
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
                        merc_c[ind].setUnid(nova_unidppacote);

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
                        cout << "Digite o novo valor da mercadoria: "; cin >> novo_valor; merc_c[ind].setValor(novo_valor);

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
                            for(size_t j = 0; j<merc_c[ind].getValid().size(); j++){
                                if(merc_c[ind].getValid()[j].day + merc_c[ind].getValid()[j].mon + merc_c[ind].getValid()[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                    ind_comp = j;
                                    cout << "Digite a nova validade: "; cin >> nova_valid.day >> nova_valid.mon >> nova_valid.year;
                                    if(dataValida(nova_valid)){
                                        merc_c[ind].getValid().insert(merc_c[ind].getValid().begin() + j, nova_valid);
                                        merc_c[ind].getValid().erase(merc_c[ind].getValid().begin() + j + 1);
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
                            for(size_t j = 0; j<merc_c[ind].getValid().size(); j++){
                                if(merc_c[ind].getValid()[j].day + merc_c[ind].getValid()[j].mon + merc_c[ind].getValid()[j].year == nova_valid.day + nova_valid.mon + nova_valid.year){
                                    ind_comp = j;
                                    cout << "Todas as unidades deste produto (com a mesma data de validade) ficarão em promoção!" << endl;
                                    merc_nova.setN_prod(merc_c[ind].getNome() + "(promoção)");
                                    merc_nova.getQuant().push_back(merc_c[ind].getQuant()[j]);
                                    merc_nova.setUnid( merc_c[ind].getUnid());
                                    merc_nova.getValid().push_back(nova_valid);
                                    cout << "Digite o novo valor do produto: "; cin >> v; merc_c[ind].setValor(v);
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
                        merc_c.push_back(merc_nova);
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

        void alterar_fun(vector<proletariado>& workers, Key chave_geral){
            string resposta, senha_t, nome_dworker;
            int ind = -1, opcao;
            proletariado worker;

            cout << "Aqui é a área para alterar funcionário." << endl;
            if(verific(resposta)){
                system("cls");
                return;
            }

            if(workers.empty()){
                cerr << "Não há funcionários registrados." << endl;
                return;
            }

            system("cls");
            

            cout << "Digite seu nome: "; getline(cin, nome_dworker); transf(nome_dworker);
            cout << "Digite a senha: "; getline(cin, senha_t);
            ind = escolha(workers, nome_dworker);
            if(ind == -1){
                cout << "Funcionário não encontrado." << endl;
                return;
            }
            if(chave_geral.verific_S(senha_t, workers[ind]) == false){
                cerr << "Ops! Senha e/ou nome incorretos." << endl;
                return;
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
                            if(worker.F_ind(workers, new_name)){
                                cout << "Há alguém com um nome semelhante. Deseja alterar o nome escolhido para este? (sim/não)"; getline(cin, resposta); transf(resposta);
                                if(resposta != "sim"){
                                    cout << "O nome não foi alterado!";
                                }else{
                                    cout << "Certo, retornando à alteração..." << endl;
                                }
                            }else{
                                workers[ind].setNome(new_name);
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
                                workers[ind].setConf(conf);
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
                            if(proc_carg(workers, "chefe")){
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
                                workers[ind].setCargo(carg);
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
                                workers[ind].setSalario(sal);
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
                                    workers[ind].setPagamento(data_t);
                                    cout << "Alteração concluída!" << endl;
                                }
                            }else{
                                cerr << "Data inválida!" << endl;
                            }
                            if(resp_saida(resposta)){
                                cout << "Retornando ao menu de alterações...\n";
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
                            ind = escolha(workers, nome_dworker);
                            if(ind == -1){
                                cerr << "Funcionário não encontrado." << endl;
                            }else{
                                data_t.day = workers[ind].getpay_day().day;
                                data_t.mon = workers[ind].getpay_day().mon;
                                data_t.year = workers[ind].getpay_day().year;
                                next_d(data_t, 30);
                                cout << "Pagamento realizado!" << endl;
                            }
                            if(resp_saida(resposta)){
                                cout << "Retornando ao menu de alterações...\n";
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
        }

        void alterar_chave(Key& chave_geral, vector<proletariado> workers){
            string senha_t, nome_t, resposta;
            int ind = -1;

            cout << "Aqui é a área para alterar a senha geral." << endl;
            if(verific(resposta)){
                system("cls");
                return;
            }

            system("cls");
            cout << "Aqui é a área para alterar a senha geral." << endl;
            cout << "Digite o seu nome: "; getline(cin, nome_t); transf(nome_t);
            ind = escolha(workers, nome_t);
            if(ind == -1){
                cout << "O seu nome não foi encontrado." << endl;
                return;
            }
            cout << "Digite a senha atual: "; getline(cin, senha_t);
            if(chave_geral.verific_S(senha_t, workers[ind]) == false){
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
        }

        void relatorio(int cli_cadast, int prod_cadast, int fun_cadast, int cli_ex, int prod_ex, int fun_ex, int num_prod_v, vector<mercadoria> merc_v, vector<mercadoria> merc_c){
            float soma = 0.0;
            int num;
            for(size_t i = 0; i<merc_v.size(); i++){
                for(size_t j = 0; j<merc_v[i].getQuant().size(); j++){
                    soma += merc_v[i].getQuant()[j] * merc_v[i].getValor();
                }
            }
            for(size_t i = 0; i<merc_c.size(); i++){
                for(size_t j = 0; j<merc_c[i].getQuant().size(); j++){
                    num += merc_c[i].getQuant()[j];
                }
            }

            cout << "Relatório de ações:" << endl;
            cout << " - Clientes cadastrados: " << cli_cadast << endl;
            cout << " - Produtos cadastrados: " << prod_cadast << endl;
            cout << " - Funcionários cadastrados: " << fun_cadast << endl;
            cout << " - Clientes excluídos: " << cli_ex << endl;
            cout << " - Produtos excluídos: " << prod_ex << endl;
            cout << " - Funcionários demitidos: " << fun_ex << endl << endl;
            cout << " - Número de produtos no estoque: " << num << endl;
            cout << " - Número de produtos vendidos: " << num_prod_v << endl;
            cout << " - Valor total de vendas: R$" << soma << endl;
        }

        bool salvacao_dados(Key chave_geral, vector<mercadoria> merc_c, vector<mercadoria> merc_v, vector<shared_ptr<pessoa>> people){
            string resposta;

            cout << "Salvando alterações...\n" <<endl;
            fstream arq_mercadoriaCO("Mercadorias.txt", fstream::out), arq_chaveg("Chave_g.txt", fstream::out), arq_beckup_chaveg("Chave_g_beckup", fstream::out), arq_pessoa("Pessoas.txt", fstream::out);
            fstream arq_mercadoriaVE("Mercadorias_V.txt", fstream::out);
            if(!arq_pessoa.is_open() || !arq_mercadoriaCO.is_open() || !arq_chaveg.is_open()){
                cerr << "Ops! não foi possível abrir os arquivos!";
                return false;
            }
            
            for(size_t i = 0; i<merc_c.size(); i++){
                mercadoria merc_comprada = merc_c[i];
                arq_mercadoriaCO << merc_comprada.getNome() << endl;
                for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                    arq_mercadoriaCO << merc_comprada.getQuant()[j] << " ";
                }
                arq_mercadoriaCO << endl;
                arq_mercadoriaCO << merc_comprada.getUnid() << endl;
                arq_mercadoriaCO << merc_comprada.getValor() << endl;
                for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                    arq_mercadoriaCO << merc_comprada.getValid()[j].day << "/" << merc_comprada.getValid()[j].mon << "/" << merc_comprada.getValid()[j].year << " ";
                }
                arq_mercadoriaCO << endl;
            }

            for(size_t i = 0; i<merc_v.size(); i++){
                mercadoria merc_comprada = merc_v[i];
                arq_mercadoriaVE << merc_comprada.getNome() << endl;
                for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                    arq_mercadoriaVE << merc_comprada.getQuant()[j] << " ";
                }
                arq_mercadoriaVE << endl;
                arq_mercadoriaVE << merc_comprada.getUnid() << endl;
                arq_mercadoriaVE << merc_comprada.getValor() << endl;
                for(size_t j=0; j<merc_comprada.getQuant().size(); j++){
                    arq_mercadoriaVE << merc_comprada.getValid()[j].day << "/" << merc_comprada.getValid()[j].mon << "/" << merc_comprada.getValid()[j].year << " ";
                }
                arq_mercadoriaVE << endl;
            }

            arq_chaveg << chave_geral.getSenha() << endl;
            arq_beckup_chaveg << chave_geral.getSenha() << endl;

            for(auto& pessoa : people){
                if(auto c = dynamic_pointer_cast<cliente>(pessoa)){
                    arq_pessoa << c->getNome() << endl << c->getConf() << endl;
                    for(size_t i = 0; i<c->getMerc().size(); i++){
                        arq_pessoa << c->getMerc()[i].getNome() << endl;
                        for(size_t j = 0; j<c->getMerc()[i].getQuant().size(); j++){
                            arq_pessoa << c->getMerc()[i].getQuant()[j] << " ";
                        }
                        arq_pessoa << endl;
                        arq_pessoa << c->getMerc()[i].getUnid() << endl;
                        arq_pessoa << c->getMerc()[i].getValor() << endl;
                        for(size_t j = 0; j<c->getMerc()[i].getValid().size(); j++){
                            arq_pessoa << c->getMerc()[i].getValid()[j].day << "/" <<  c->getMerc()[i].getValid()[j].mon << "/" << c->getMerc()[i].getValid()[j].year << " ";
                        }
                        arq_pessoa << endl;
                    }
                }else{
                    if(auto w = dynamic_pointer_cast<proletariado>(pessoa)){
                        arq_pessoa << w->getNome() << endl << w->getConf() << endl;
                        arq_pessoa << w->getCargo() << endl << w->getSalario() << endl;
                        arq_pessoa << w->getpay_day().day << "/" << w->getpay_day().mon << "/" << w->getpay_day().year << endl; 
                    }
                }
            }

            arq_pessoa.close(); arq_mercadoriaCO.close(); arq_chaveg.close(); arq_mercadoriaVE.close(); arq_beckup_chaveg.close();
            return true;
        }
};
//classe especial.