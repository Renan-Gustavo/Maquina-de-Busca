#include "MDB.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;




int main() {

	MDB::MapsModel Modelagem;
	vector<string> PalavrasDaBusca;
	set<int> NumeroDosDocumentosDaPesquisa;
	MDB::MapsQuery PalavrasPesquisadas;

	string ArqResultadoDaBusca = MDB::configuracoes_iniciais(Modelagem);
	ArqResultadoDaBusca = ArqResultadoDaBusca + "_RESULTADO_DA_BUSCA.txt";

	while (true) {
		MDB::cls();
		cout << "Resultados da busca sao salvos no documento:" << endl << ArqResultadoDaBusca << endl;
		NumeroDosDocumentosDaPesquisa.clear();
		PalavrasPesquisadas.CoordPesquisa.clear();
		PalavrasPesquisadas.Pesquisa.clear();
		PalavrasDaBusca = MDB::Buscar();
		MDB::configuracaoDePesquisa(PalavrasDaBusca, NumeroDosDocumentosDaPesquisa, PalavrasPesquisadas, Modelagem);
		map<double, int> ranking = MDB::CosineRanking(NumeroDosDocumentosDaPesquisa, PalavrasPesquisadas, Modelagem);
		MDB::imprimirInformacaoEmArquivo(Modelagem, ranking, ArqResultadoDaBusca, PalavrasDaBusca);
	}

	return 0;
}



