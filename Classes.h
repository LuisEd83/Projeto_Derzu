#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace chrono;

void transf(string& nome){
    transform(nome.begin(), nome.end(), nome.begin(), [](unsigned char c){return tolower(c);});
}

typedef struct{
    int day = 0;
    int mon = 0;
    int year = 0;
}tdata;

class mercadoria{
    private:
        string nome_produto;
        vector<int> quantidade;
        int unidade_pacote;
        float valor;
        vector<tdata> validade;
        
    public:
        void setN_prod(string name){nome_produto = name;}
        void setQuant(vector<int> quant){
            for(size_t i = 0; i <quant.size(); i++){
                quantidade[i] = quant[i];
            }
        }
        void setUnid(int uni){unidade_pacote = uni;}
        void setValor(float v){valor = v;}
        void setValid(vector<tdata> datas){
            for(size_t i = 0; i<datas.size(); i++){
                validade[i] = datas[i];
            }
        }

        string getNome(){return nome_produto;}
        vector<int> getQuant(){return quantidade;}
        int getUnid(){return unidade_pacote;}
        float getValor(){return valor;}
        vector<tdata> getValid(){return validade;}

            mercadoria(){
                nome_produto = "void";
                quantidade.push_back(0);
                valor = 0.0;
                unidade_pacote = 0;
                validade.push_back({0,0,0});
            }
            mercadoria(string nome_produto, vector<int> quantidade, int unidade_pacote, float valor, vector<tdata> validade) 
                : nome_produto(nome_produto), quantidade(quantidade), unidade_pacote(unidade_pacote), valor(valor), validade(validade) {}
            
            bool F_ind(mercadoria merc, string Nome_p){
                size_t p = merc.getNome().find(Nome_p);
                return (p != string::npos);
            }
            int F_show(vector<mercadoria> nome_vector, string nome_search){
                vector<int> ind_prod;
                int opcao, n = 0;
                string resp;

                cout << "Produtos com este nome: " << endl;
                for(size_t i = 0; i < nome_vector.size(); i++){
                    size_t p = nome_vector[i].getNome().find(nome_search);
                    if(p != string::npos){
                        n++;
                        ind_prod.push_back(i);
                        cout << n << " - " << nome_vector[i].getNome() << endl;
                    }
                }
                if(ind_prod.empty()){return -1;}

                n-=1;
                do{
                    cout << "Digite a opção desejada: "; cin >> opcao; opcao -= 1;
                    if(opcao < 0 || opcao > n){
                        cout << "Opção inválida." << endl;
                        cout << "Deseja sair desta área? (sim/não)"; cin.ignore(); cin >> resp; transf(resp);
                        if(resp == "sim"){return -1;}
                        cout << "Tente novamente." << endl;
                    }
                }while(opcao < 0 || opcao > n);

                return ind_prod[opcao];
            }
};

class pessoa{
    protected:
        string nome;
        string confianca;
        int tipo;
    public:
        pessoa(string n, string con, int t){nome = n; confianca = con; tipo = t;}

        virtual string getNome() const{return nome;}
        virtual string getConf() const{return confianca;}
        virtual int getTipo(){}

        void setNome(string nome){this -> nome = nome;}
        void setConf(string conf){confianca = conf;}

        virtual vector<string> dadosPtabela(){}

        template <class T>
        bool F_ind(vector<T>& Nome_class, string Nome_p){
            for(size_t i = 0; i<Nome_class.size(); i++){
                string t = Nome_class[i].getNome();
                size_t p = t.find(Nome_p);
                if(p != string::npos){return true;}
            }
            return false;
        }
};

class proletariado : public pessoa{
    private:
        string cargo;
        float salario;
        tdata dia_pagamento;
    public:
        proletariado() : pessoa("void", "void", 1){setSalario(0.0); setPagamento({0,0,0});}

        void setCargo(string C){cargo = C;}
        void setSalario(float valor){salario = valor;}
        void setPagamento(tdata data){dia_pagamento = {data.day, data.mon, data.year};}

        string getCargo(){return cargo;}
        float getSalario(){return salario;}
        tdata getpay_day(){return dia_pagamento;}
        int getTipo(){return 1;}

        bool Pagamento(tdata dia_pagamento){
            double diff_data;

            auto now = system_clock::now();
            time_t time_now = system_clock::to_time_t(now);
            tm* local_time = localtime(&time_now);
            
            struct tm data_pag;
            time_t tempo_f, tempo_i;

            data_pag.tm_year = dia_pagamento.year - 1900;
            data_pag.tm_mon = dia_pagamento.mon - 1;
            data_pag.tm_mday = dia_pagamento.day;

            tempo_f = mktime(&data_pag);
            tempo_i = mktime(local_time);

            if((tempo_i != -1) && (tempo_f != -1)){
                diff_data = difftime(tempo_f,tempo_i)/(60*60*24);
            }

            if(diff_data <= 0){return true;}
            return false;
        }

        vector<string> dadosPtabela(){
            return {
                getNome(),
                getConf(),
                getCargo(),
                to_string(salario),
                to_string(dia_pagamento.day) + "/" + to_string(dia_pagamento.mon) + "/" + to_string(dia_pagamento.year) 
            };
        }

};

class cliente : public pessoa{
    protected:
        vector<mercadoria> itens;
    public:
            cliente() : pessoa("void", "void", 2){
                itens.clear();
            }
            vector<mercadoria> getMerc(){return itens;}
            int getTipo(){return 2;}

            vector<string> dadosPtabela(){
                string itensStr;
                for(size_t i = 0; i<itens.size(); i++){
                    itensStr += itens[i].getNome() + ", ";
                }

                return{
                    getNome(),
                    getConf(),
                    itensStr,
                };
            }
};

class Key{
    private:
        string senha;
    public:
        void setSenha(string k){senha = k;}
        string getSenha(){return senha;}


        bool verfic_tam(string k){
            size_t n = 0;

            n = k.size();
            if(n != senha.size()){return false;}
                return true;
        }
        bool verific_S(string k, proletariado p){
            if((p.getCargo() == "chefe") && (k == senha) && verfic_tam(k)){
                return true;
            }
            return false;
        }
};