#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include "Classes.h"
#include "Funções.h"

using namespace std;
using namespace chrono;

class Gerenciador{
    public:
        bool registra_cli(vector<mercadoria>& merc_c, vector<mercadoria>& merc_v, vector<cliente>& amigo, int& cli_cadast){
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
                for(size_t j = 0; j<merc_c.size(); j++){
                    if(merc_c[i].getNome() == nome_dproduto){
                        ind = j;
                    }
                }
                if(ind == -1){
                    cout << "Mercadoria não encontrada. Por favor, verifique se a mercadoria existe ou se você escreveu incorretamente.\n";
                    if(resp_saida(resposta)){
                        cout << "Retornando ao menu...";
                    }
                    return false;
                }

                cin >> data_dprod.day >> data_dprod.mon >> data_dprod.year;
                compra(merc_c, merc_v, nome_dproduto, unidades_vend, data_dprod);
                ind = merc_v[ind].F_show(merc_v, nome_dproduto);
                comprador_atual.getMerc().push_back(merc_v[ind]);
            }
            amigo.push_back(comprador_atual);
            cout << "Cliente cadastrado com sucesso!";
            cli_cadast ++;
            return true;
        }

        bool registra_prod(vector<mercadoria>& merc_c, int& prod_cadast){
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
        }

        bool registra_fun(vector<proletariado>& trabalhador, int& fun_cadast){
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

        bool ex_cli(vector<cliente>& amigo){
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
                    return true;
                }
            }
            return false;
        }

        bool ex_prod(vector<mercadoria>& merc_c){
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
                    return true;
                }
            }
            return false;
        }

        bool demissao(vector<proletariado>& worker, Key chave_geral){
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
                }
            }
        }


};
//classe especial.