#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <set>
#include <windows.h>
#include <cmath>

#ifndef MDB_H
#define MDB_H

using namespace std;

namespace MDB { // maquina de busca
	struct Wcoord {
		map <int, vector<double>> Documento_coordenadas;
	};

	class MapsModel { // conjunto de maps para os dados vindos dos documentos
	public:
		map <string, double> Palavra_IDF;
		map <string, map <int, int>> Palavra_NumDoc_TF;
		map <int, string> NumDoc_Documento;
		Wcoord W;

		void operator=(MapsModel a);

		// construtor padrao, nao faz nada
		MapsModel();
		// constroi o objeto MapsModel a partir de alguns map
		MapsModel(map <string, double> a, map <string, map <int, int>> b, map <int, string> c);

	};

	class MapsQuery { // conjunto de dados necessarios para a Pesquisa
	public:
		map<string, double> Pesquisa;
		vector<double> CoordPesquisa;
	};

	//separa a string em pedacos e retorna os 2 ultimos
	string split_e_manda_2_ultimas_strings(string str, const char* op);

	//pega a primeira linha como caminho onde irá salvar, a segunda como
	//caminho onde irá buscar os dados e o resto são nomes de pastas e limites 
	//numerais para o nome dos arquivos
	//retorna os caminhos referentes ao nome das pastas
	vector<string> Gerar_nome_dos_arquivos(string caminho, MDB::MapsModel& M);

	//preenche os maps
	//retorna os caminhos que deram erro
	vector<string> Preencher_os_maps
	(map <string, double>& PI, map <string, map <int, int>>& PNT, map <int, string>& ND, string path);

	//faz todas as configuracoes iniciais necessarias para preencher os maps e preenche os mesmos
	//retorna o caminho onde o resultado da busca será salvo
	string configuracoes_iniciais(MDB::MapsModel& M);

	//funcao para imprimir uma mensagem de erro de abertura de arquivo e pede para reabrir ou n o arquivo
	void erro(string s, MDB::MapsModel M);

	//funcao que imprime as informacoes principais de uma dada palavra
	//uso: somente teste 
	void imprimir(MDB::MapsModel& m, string s);

	//funcao que passa todos os caracteres de uma palavra para minusculo
	string Stringtolower(string s);

	//funcao que retorna apenas os caracteres alfanumericos de uma string
	string justAlNum(string s);

	//funcao que retorna a norma de um vetor
	double normaVetorial(vector<double> v);

	//funcao que converte um documento em um vector de double
	vector<double> PalavraParaCoordenadas(MDB::MapsModel& M, int NumDoc);

	//funcao que converte a pesquisa em um vector de double
	vector<double> PalavraParaCoordenadasDaPesquisa(MDB::MapsModel& M, map<string, double> Pesquisa);

	//faz o rankeamento dos documentos e retorna um map contendo o numero do documento e sua posicao no ranking
	map<double, int> CosineRanking(set<int> NumeroDosDocumentosDaPesquisa, MDB::MapsQuery PalavrasPesquisadas, MDB::MapsModel& M);

	//preenche o set com os documentos da pesquisa e o mapsQuery de palavrasPesquisadas que é conjunto de dados necessarios para a Pesquisa
	void configuracaoDePesquisa(const vector<string>& P, set<int>& NumeroDosDocumentosDaPesquisa, MDB::MapsQuery& PalavrasPesquisadas, MDB::MapsModel& M);

	//pede a frase a ser pesquisada e retorna essa frase separada em strings menores
	vector<string> Buscar();

	//imprime o resultado da busca em um arquivo
	void imprimirInformacaoEmArquivo(MDB::MapsModel& m, map<double, int> CosRank_NumDoc, string path, vector<string> Frase);

	//limpa a tela do console do windows
	void CLEAR(HANDLE hConsole);

	//passa os dados necessarios para executar CLEAR()
	void cls();
}
#endif //MDB_H
