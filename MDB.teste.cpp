#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "MDB.h"

#include <limits>
#include <string>
#include <cmath>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <set>
#include <windows.h>

using namespace std;

// Não é necessário criar uma Classe Teste já que todos os parâmetros
// das classes utilizadas são públicos

//=========================================================================
TEST_SUITE("MapsModel"){

    TEST_CASE("MDB::MapsModel::MapsModel()"){
        // Verifica se o construtor MapsModel() está correto: 
        MapsModel x;
        CHECK(x->Palavra_IDF.size() == 0);
        CHECK(x->Palavra_NumDoc_TF.size() == 0);
	    CHECK(x->NumDoc_Documento.size() == 0);
    }

    TEST_CASE("MDB::MapsModel::MapsModel(map <string, double> a, map <string, map <int, int>> b, map <int, string> c)"){
        // Verifica se o construtor MapsModel(a,b,c) está correto:
        map <string,double> a = {{"aaa",1.5},{"bbb",1.6},{"ccc",1.7}};
        map <string, map <int, int>> b = {{"a",0,0},{"b",1,1},{"c",2,2}};
        map <int, string> c = {{1,"abc"},{2,"def"},{3,"ghi"}};

        MapsModel x( a , b , c );
        CHECK(x->Palavra_IDF["aaa"] == 1.5);
        CHECK(x->Palavra_IDF["bbb"] == 1.6);
        CHECK(x->Palavra_IDF["ccc"] == 1.7);

        CHECK(x->Palavra_NumDoc_Tf["a"][0] == 0);
        CHECK(x->Palavra_NumDoc_Tf["b"][1] == 1);
        CHECK(x->Palavra_NumDoc_Tf["c"][2] == 2);

        CHECK(x->NumDoc_Documento[1] == "abc");
        CHECK(x->NumDoc_Documento[2] == "def");
        CHECK(x->NumDoc_Documento[3] == "ghi");
    }

    TEST_CASE("void MDB::MapsModel::operator=(MDB::MapsModel a)"){
        // Verifica se a atribuição foi realizada corretamente:
        CHECK(this->Palavra_IDF == a.Palavra_IDF);
        CHECK(this->Palavra_NumDoc_TF == a.Palavra_NumDoc_TF);
	    CHECK(this->NumDoc_Documento == a.NumDoc_Documento);
    }
}
//=========================================================================
TEST_SUITE("MapsQuery"){}
//=========================================================================
TEST_SUITE("MDB"){
    TEST_CASE("string MDB::Stringtolower(string s)"){
        // Verifica se a função 'tolower()' está sendo usada corretamente:
        // Análise baseada na tabela ASCII:
        //  Maiúscula = Minúscula - 32;
        string x = "ABC";
        string y = x.Stringtolower();
        string z = "cdE";
        string w = z.Stringtolower();
        string k = "012"
        string l = k.Stringtolower();
        REQUIRE(x.length()=y.length());
        CHECK(x[0] == y[0]-32);
        CHECK(x[1] == y[1]-32);
        CHECK(x[2] == y[2]-32);
        // Função tolower() não altera caracteres minúsculos:
        REQUIRE(z.length()=w.length());
        CHECK(z[0] == w[0]);
        CHECK(z[1] == w[1]);
        CHECK(z[2] == w[2]-32);
        // Função tolower() retorna a mesma string se ela não
        // possuir equivalente em minúsculo:
        REQUIRE(k.length()=l.length());
        CHECK(k[0] == l[0]);
        CHECK(k[1] == l[1]);
        CHECK(k[2] == l[2]);
    }

    TEST_CASE("string MDB::justAlNum(string s)"){
        // Função isalnum(): decide se o argumento é alfanumérico
        // Função isalpha(): decide se o argumento é uma letra
        string x = "0";
        string y = "A"; 
        CHECK(x.isalnum());
        CHECK_FALSE(x.isalpha());
        CHECK(y.isalnum());
        CHECK_FALSE(y.isalnum());
        // OBS: o retorno de ambas as funções é 'bool'
    }

    TEST_CASE("double MDB::normaVetorial(vector<double> v)"){
        // Verifica se a norma de vetores foi feita corretamente:
        vector<double> v1 = { 3 , 4 };
        vector <double> v2 = { 1 };
        vector<double> v3 = { 4 , 3 };
        vector<double> v4 = { -4 , -3 };
        vector<double> v5 = { 0 };

        double test_v1 = v1.normaVetorial();
        double test_v2 = v2.normaVetorial();
        double test_v3 = v3.normaVetorial();
        // Propriedades da norma:
        //  A ordem dos termos não altera o resultado
        CHECK(test_v1 == 5 );
        CHECK(test_v3 == 5 );
        CHECK(test_v3 == test_v1);
        // A norma de um vetor de um único termo é o próprio termo
        CHECK(test_v2 == 1 );
        CHECK(test_v2 == 0 );
        //  No cálculo da norma toma-se valores absolutos
        CHECK(test_v4 == 5 );
        CHECK(test_v3 == test_v4);
    }


//=========================================================================
    TEST_CASE("string MDB::split_e_manda_2_ultimas_strings(string str, const char* op)"){
        
        string x = "dhdh/ehejdbb/dhjddn/djjdhd/hrhdjd/oi/ola";
        char y = '/';
        string z = "";
        z = split_e_manda_2_ultimas_strings(x, y);
        CHECK(z == "oi/ola");

        x = "arroz";
        y = "/";
        z = split_e_manda_2_ultimas_strings(x, y);
        CHECK(z == "arroz");

        x = "arroz.arroz";
        y = "/";
        z = split_e_manda_2_ultimas_strings(x, y);
        CHECK(z == "arroz.arroz");

        x = "arroz/arroz";
        y = "/";
        z = split_e_manda_2_ultimas_strings(x, y);
        CHECK(z == "arroz/arroz");
    }

    TEST_CASE("vector<double> MDB::PalavraParaCoordenadas(MDB::MapsModel& M, int NumDoc)"){
        MapsModel x;
        
        x.Palavra_IDF["a"] = 1.4;
        x.Palavra_IDF["b"] = 8.1;
        x.Palavra_IDF["c"] = 1.3;

        x.Palavra_NumDoc_TF["a"][1] = 1;
        x.Palavra_NumDoc_TF["a"][2] = 8;
        x.Palavra_NumDoc_TF["b"][1] = 14;
        x.Palavra_NumDoc_TF["b"][3] = 3;
        x.Palavra_NumDoc_TF["c"][1] = 9;

        vector<double> y;
        y.push_back(1.4*1.0);
        y.push_back(8.1*14.0);
        y.push_back(1.3*9.0);

        int NumDoc = 1;

        CHECK(PalavraParaCoordenadas(x, NumDoc) == y);

    }

    TEST_CASE("vector<double> PalavraParaCoordenadasDaPesquisa(MDB::MapsModel& M, map<string, double> Pesquisa)"){
         MapsModel x;
        
        x.Palavra_IDF["a"] = 1.4;
        x.Palavra_IDF["b"] = 8.1;
        x.Palavra_IDF["c"] = 1.3;

        
        map<string, double> p;
        p["a"] = 1;
        p["b"] = 2;
        p["c"] = 3;
        
        vector<double> y;
        y.push_back(1.4*1.0);
        y.push_back(8.1*2.0);
        y.push_back(1.3*3.0);

        CHECK(y == PalavraParaCoordenadasDaPesquisa(x, p));
    }

    TEST_CASE("map<double, int> CosineRanking(set<int> NumeroDosDocumentosDaPesquisa, MDB::MapsQuery PalavrasPesquisadas, MDB::MapsModel& M)"){
        set<int>n;
         MapsModel x;
         MapsQuery y;

        x.W.Documento_coordenadas[1][0] = 0.5;
        x.W.Documento_coordenadas[1][1] = 0.7;
        x.W.Documento_coordenadas[1][2] = 0.8;
        x.Palavra_IDF["a"] = 1.4;
        x.Palavra_IDF["b"] = 2.7;
        x.Palavra_IDF["c"] = 3.8;

        y.CoordPesquisa[0] = 3.1;
        y.CoordPesquisa[1] = 2.4;
        y.CoordPesquisa[2] = 4.3;
        
        n.push_back(1);
        
        double aux = sqrt(0.5*0.5+0.7*0.7+0.8*0.8) * sqrt(3.1*3.1+2.4*2.4+4.3*4.3);
        
        map<double, int> p; 
        p[(0.5*3.1+0.7*2.7+0.8*3.8)/aux] = 1;
        

        CHECK(p == CosineRanking(n, y, x));
    }
}