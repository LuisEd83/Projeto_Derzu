#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;
using namespace chrono;

void transf(std::string& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
    }
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
            quantidade = quant; 
        }
        void setUnid(int uni){unidade_pacote = uni;}
        void setValor(float v){valor = v;}
        void setValid(vector<tdata> datas) {
            validade.resize(datas.size());
            for(size_t i = 0; i < datas.size(); i++) {
                validade[i] = datas[i];
            }
        }

        string getNome() const {return nome_produto;}
        vector<int>& getQuant(){return quantidade;}
        int getUnid(){return unidade_pacote;}
        float getValor(){return valor;}
        vector<tdata>& getValid(){return validade;}

            mercadoria(){
                nome_produto = "void";
                valor = 0.0;
                unidade_pacote = 0;
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

            bool ehUnidade() const {
                return nome_produto.find(" unidade") != string::npos;
            }
};

class pessoa{
    protected:
        string nome;
        string confianca;
        int tipo;
    public:
        pessoa(){nome = "Indefinido"; confianca = "Indefinido"; tipo = 0;}
        pessoa(int t){nome = "Indefinido"; confianca = "Indefinido"; tipo = t;}
        pessoa(string n, string con, int t){nome = n; confianca = con; tipo = t;}

        string getNome() const{return nome;}
        string getConf() const{return confianca;}
        virtual int getTipo(){return 0;}

        void setNome(string nome){this -> nome = nome;}
        void setConf(string conf){confianca = conf;}
        void setTipo(int t){tipo = t;}

        template <class T>
        bool F_ind(vector<T>& Nome_class, string Nome_p){
            for(size_t i = 0; i<Nome_class.size(); i++){
                string t = Nome_class[i].getNome();
                size_t p = t.find(Nome_p);
                if(p != string::npos){return true;}
            }
            return false;
        }

        virtual vector<string> dadosPtabela() const {
            return {
                getNome(),
                getConf(),
                };
        }
};

class proletariado : public pessoa{
    private:
        string cargo;
        float salario;
        tdata dia_pagamento;
    public:
        proletariado() : pessoa(1){setSalario(0.0); setPagamento({0,0,0});}
        proletariado(string name, string conf, string carg, float sal, tdata pag)
        : pessoa(name, conf, 1) {
            setCargo(carg);    
            setSalario(sal);
            setPagamento(pag);
        }

        void setCargo(string C){cargo = C;}
        void setSalario(float valor){salario = valor;}
        void setPagamento(tdata data){dia_pagamento = {data.day, data.mon, data.year};}
        void setTipo(){tipo = 1;}

        string getCargo() const {return cargo;}
        float getSalario() const {return salario;}
        tdata getpay_day() const {return dia_pagamento;}
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

        vector<string> dadosPtabela() const {
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
            cliente() : pessoa(2){
                itens.clear();
            }

            cliente(string nome, string conf, vector<mercadoria> merc)
            :  pessoa(nome, conf, 2), itens(merc) {} 

            void setTipo(){tipo = 2;}
            void setMerc(vector<mercadoria>& merc){
                for(mercadoria m : merc){
                    itens.push_back(m);
                }
            }

            vector<mercadoria>& getMerc(){return itens;}
            int getTipo(){return 2;}

            vector<string> dadosPtabela() const {
                string itensStr = " ";
                for(size_t i = 0; i<itens.size(); i++){
                    itensStr += itens[i].getNome() + ", ";
                }

                return{
                    getNome(),
                    getConf(),
                    itensStr,
                };
            }

            float divida(vector<mercadoria> prod){
                float soma = 0.0;
                for(size_t i = 0; i<prod.size(); i++){
                    for(size_t j = 0; j<prod[i].getQuant().size(); j++){
                        soma += (prod[i].getValor() * prod[i].getQuant()[j]);
                    }
                }
                return soma;
            }
};

class Key{
    private:
        string senha;
    public:

        Key(){senha = "void";}

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