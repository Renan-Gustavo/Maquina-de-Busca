
#include "MDB.h"

string MDB::split_e_manda_2_ultimas_strings(string str, const char* op) {
	vector<string> result;
	string resto = str, bloco;
	size_t posicao = resto.find_first_of(op);
	while (posicao != string::npos) {
		bloco = resto.substr(0, posicao);
		resto = resto.substr(posicao + 1);
		posicao = resto.find_first_of(op);
		result.push_back(bloco);
	}

	if (resto.length() > 0)
		result.push_back(resto);

	string s;
	s.clear();

	if (result.size() > 1) {
		s = result[result.size() - 2] + "\\" + result[result.size() - 1];
	}
	else {
		s = str;
	}

	return s;
}

vector<string> MDB::Gerar_nome_dos_arquivos(string PastasIntervaladas, MDB::MapsModel& M) {
	vector<string> v;
	fstream arquivo(PastasIntervaladas);
	if (arquivo.is_open()) {
		string lixo;
		arquivo >> lixo;
		fstream saidas;
		int min, max;
		string pasta;
		string c0, c1; // tem a string do caminho onde irá salvar (c0) e onde buscar (c1)
		arquivo >> c0;
		arquivo >> c1;

		while (!arquivo.eof()) {
			arquivo >> pasta;
			arquivo >> min;
			arquivo >> max;
			v.push_back(c0 + pasta + ".txt");
			saidas.open(c0 + pasta + ".txt", fstream::out);
			saidas << c1 + pasta + "\\" << endl;
			for (int i = min; i <= max; i++) {
				if (i != max) {
					saidas << i << endl;
				}
				else {
					saidas << i;
				}
			}
			saidas.close();
		}
	}
	else {
		MDB::erro(PastasIntervaladas, M);
	}
	return v;
}

vector<string> MDB::Preencher_os_maps
(map <string, double>& PI, map <string, map <int, int>>& PNT, map <int, string>& ND, string path) {

	// se houverem erros ao abrir o arquivo, a variavel abaixo diz quais não abriram
	vector<string> Erros;

	string nome;
	string frase;
	string palavra;

	fstream arquivoX(path);

	if (arquivoX.is_open()) {
		cout << endl << " -- " << path;
		int tam = signed(ND.size());
		ND[tam] = path;
		frase.clear();

		while (!arquivoX.eof()) {
			arquivoX >> frase;

			for (int k = 0, n = 0; k < signed(frase.size()); k++) {
				if (isalpha(frase[k])) {
					frase[k] = tolower(frase[k]);
				}
				if (!isalnum(frase[k]) && frase[k] != '-') {
					frase[k] = ' ';
				}
			}

			istringstream paraSeparar(frase);

			// se a frase vier do tipo "alface.13214", após o for acima, ela vira "alface 13214"
			// o getline abaixo separa a frase em palavras menores
			while (getline(paraSeparar, palavra, ' ')) {

				if (palavra != "") {

					// arruma o caso de palavras com travessao, e.g. guarda-chuva
					istringstream separarDNV(palavra);
					string sAUX;
					vector<string> s;
					while (getline(separarDNV, sAUX, '-')) {
						if (sAUX != "-") {
							s.push_back(sAUX);
						}
					}
					palavra.clear();
					for (int l = 0; l < signed(s.size()); l++) {
						palavra = palavra + s[l];
					}
					
					if (palavra != "") {

						// se não existe uma palavra X então a quantidade de vezes que a palavra X
						// apareceu até agora é inicialmente 0
						if (PNT.find(palavra) == PNT.end()) {
							PNT[palavra][tam] = 0;
						}

						// preenche a matriz Palavra_NumDoc_TF com a palavra, o numero do documento e 
						// soma 1 no numero de vezes que a palavra aparece
						PNT[palavra][tam]++;
					}
				}
			}
		}
	}
	else {
		string s = split_e_manda_2_ultimas_strings(path, "\\");
		Erros.push_back(s);
	}
	arquivoX.close();

	auto iterador = PNT.begin();

	while (iterador != PNT.end()) {
		double N = ND.size();
		double nt = iterador->second.size();

		PI[iterador->first] = log10(N / nt);
		iterador++;
	}
	return Erros;
}

string MDB::configuracoes_iniciais(MDB::MapsModel& M) {

	string entradaAux;
	vector<string> caminhos_dos_arquivos_ofc, ErrosParcial, ErrosTotal;
	vector<string> NomePastas;
	string caminho_arquivo_Pastas_Intervaladas;
	cout << "\nCaminho do arquivo de Configuracao Inicial\n\n--->>> ";
	cin >> caminho_arquivo_Pastas_Intervaladas;


	string e_path;
	string caminhoDeRetorno;
	fstream aux(caminho_arquivo_Pastas_Intervaladas, fstream::in);

	if (aux.is_open()) {
		cout << endl << endl << " -- AGUARDE, CARREGANDO BASE DO BANCO DE DADOS...";

		aux >> entradaAux;

		aux >> e_path;
		aux.close();
		caminhoDeRetorno = e_path;

		e_path = e_path + "_ERROS.txt";
		fstream erro(e_path, fstream::out);

		if (entradaAux == "PI.config.ok") {
			cout << endl << " -- GERANDO ARQUIVOS BASE PARA CONFIGURACAO INICIAL..." << endl;
			NomePastas = Gerar_nome_dos_arquivos(caminho_arquivo_Pastas_Intervaladas, M);
		}
		else {

			fstream file(caminho_arquivo_Pastas_Intervaladas);
			string a, ci, co;
			file >> a;
			file >> ci;
			file >> co;
			while (!file.eof()) {
				string pasta;
				file >> pasta;
				NomePastas.push_back(ci + pasta + ".txt");
			}

			file.close();
		}

		cout << " -- ABRINDO ARQUIVOS E PREENCHENDO O BANCO DE DADOS..." << endl;

		for (int i = 0; i < signed(NomePastas.size()); i++) {
			string caminho;
			string documentoDetexto;

			fstream arq(NomePastas[i]);
			if (arq.is_open()) {
				arq >> caminho;
				while (!arq.eof()) {
					arq >> documentoDetexto;
					string AUX = caminho + documentoDetexto;
					ErrosParcial = Preencher_os_maps(M.Palavra_IDF, M.Palavra_NumDoc_TF, M.NumDoc_Documento, AUX);
					for (int k = 0; k < signed(ErrosParcial.size()); k++) {
						ErrosTotal.push_back(ErrosParcial[k]);
					}

				}
			}
		}
		if (ErrosTotal.size() != 0) {
			erro << "ERRO: NÃO FOI POSSÍVEL ABRIR OS SEGUINTES ARQUIVOS";

			for (int i = 0; i < signed(ErrosTotal.size()); i++) {
				erro << endl << ErrosTotal[i];
			}

		}
		else {
			erro << "\tTUDO OK POR AQUI ;)\n\n NENHUM ERRO ENCONTRADO AO ABRIR OS ARQUIVOS";
		}

		erro.close();

		cout << endl << endl << " -- REALIZANDO OS ULTIMOS AJUSTES...";

		//Cria uma map feito com o numero do documento relacionado as suas coordenadas
		for (auto i = M.NumDoc_Documento.begin(); i != M.NumDoc_Documento.end(); i++) {
			int NumDoc = i->first;
			M.W.Documento_coordenadas[NumDoc] = MDB::PalavraParaCoordenadas(M, NumDoc);
		}
	}
	else {
		MDB::erro(caminho_arquivo_Pastas_Intervaladas, M);
	}

	return caminhoDeRetorno;
}

void MDB::erro(string s, MDB::MapsModel M) {
	istringstream paraSeparar(s);
	string palavra;
	while (getline(paraSeparar, palavra, '\\'));
	cout << "\nErro ao abrir o arquivo " << palavra;
	cout << "\n\nVerifique se:\n-> O arquivo digitado existe.\n-> Se a extensao do arquivo foi digitada corretamente.\n-> Se o arquivo ja esta sendo usado por outro programa.";
	cout << "\n\nTentar Novamente? \t(1) Sim\t(0) Nao\n--->>> ";
	bool resposta;
	cin >> resposta;
	if (resposta) {
		MDB::cls();
		MDB::configuracoes_iniciais(M);
	}
	else {
		exit(1);
	}
}

void MDB::MapsModel::operator=(MDB::MapsModel a) {
	this->NumDoc_Documento = a.NumDoc_Documento;
	this->Palavra_IDF = a.Palavra_IDF;
	this->Palavra_NumDoc_TF = a.Palavra_NumDoc_TF;
}

MDB::MapsModel::MapsModel() {}

MDB::MapsModel::MapsModel(map <string, double> a, map <string, map <int, int>> b, map <int, string> c) {
	this->Palavra_IDF = a;
	this->Palavra_NumDoc_TF = b;
	this->NumDoc_Documento = c;
}

string MDB::Stringtolower(string s) {
	for (int i = 0; i < signed(s.size()); i++) {
		s[i] = tolower(s[i]);
	}
	return s;
}

string MDB::justAlNum(string s) {
	string retorno;
	retorno.clear();
	for (int i = 0; i< signed(s.size()); i++) {
		if (isalnum(s[i])) {
			retorno = retorno + s[i];
		}
	}
	return retorno;
}

void MDB::imprimir(MDB::MapsModel& m, string s) {
	string s2 = Stringtolower(s);
	s2 = justAlNum(s2);
	auto it1 = m.Palavra_NumDoc_TF.find(s2);

	if (it1 != m.Palavra_NumDoc_TF.end()) {
		cout << endl << endl << "-> " << s << " (IDF = " << m.Palavra_IDF[s2] << "): " << endl;
		cout << "-> Explicacao: Caminho do Documento ---- TF" << endl;
		for (auto i = m.Palavra_NumDoc_TF[it1->first].begin(); i != m.Palavra_NumDoc_TF[it1->first].end(); i++) {
			cout << endl << m.NumDoc_Documento[i->first];
			cout << " ---- " << i->second;
		}
		cout << endl;
	}
	else {
		cout << endl << endl << "-> " << s << "(IDF = 0)" << endl;
	}
}

double MDB::normaVetorial(vector<double> v) {
	double nV = 0;
	for (int i = 0; i < signed(v.size()); i++) {
		nV += v[i] * v[i];
	}
	return nV;
}

vector<double> MDB::PalavraParaCoordenadas(MDB::MapsModel& M, int NumDoc) {
	vector<double> COORD;
	COORD.clear();

	for (auto itPalavra = M.Palavra_IDF.begin(); itPalavra != M.Palavra_IDF.end(); itPalavra++) {
		string s = itPalavra->first;
		auto itAux = M.Palavra_NumDoc_TF[s].find(NumDoc);
		if (itAux == M.Palavra_NumDoc_TF[s].end()) {
			COORD.push_back(0);
		}
		else {
			while (itAux->first != NumDoc) { itAux++; }
			double idf(itPalavra->second), tf(itAux->second);
			COORD.push_back(idf*tf);
		}
	}

	return COORD;
}

vector<double> MDB::PalavraParaCoordenadasDaPesquisa(MDB::MapsModel& M, map<string, double> Pesquisa) {
	vector<double> COORD;
	COORD.clear();

	for (auto itPalavra = M.Palavra_IDF.begin(); itPalavra != M.Palavra_IDF.end(); itPalavra++) {
		string s = itPalavra->first;
		auto itAux = Pesquisa.find(s);
		if (itAux == Pesquisa.end()) {
			COORD.push_back(0);
		}
		else {
			double idf(itPalavra->second), tf(itAux->second);
			COORD.push_back(idf * tf);
		}
	}

	return COORD;
}

map<double, int> MDB::CosineRanking(set<int> NumeroDosDocumentosDaPesquisa, MDB::MapsQuery PalavrasPesquisadas, MDB::MapsModel& M) {
	map<double, int> sim;
	sim.clear();

	for (auto itSet = NumeroDosDocumentosDaPesquisa.begin(); itSet != NumeroDosDocumentosDaPesquisa.end(); itSet++) {
		double sim_parcial = 0;

		//unicos documentos que aparecem no resultado da pesquisa sao os que tem pelo menos 1 palavra em comum com a pesquisa
		for (int i = 0; i < signed(M.Palavra_IDF.size()); i++) {

			double d = M.W.Documento_coordenadas[*itSet][i];
			double q = PalavrasPesquisadas.CoordPesquisa[i];
			sim_parcial += d * q;
		}

		double normaDnormaQ = MDB::normaVetorial(M.W.Documento_coordenadas[*itSet])
			* MDB::normaVetorial(PalavrasPesquisadas.CoordPesquisa);
		//sim[*itSet] significa o a similaridade do documento *itSet com a pesquisa
		sim[(sim_parcial / sqrt(normaDnormaQ))] = *itSet;
	}

	return sim;
}

void MDB::configuracaoDePesquisa(const vector<string>& P, set<int>& NumeroDosDocumentosDaPesquisa, MDB::MapsQuery& PalavrasPesquisadas, MDB::MapsModel& M) {

	for (int i = 0; i < signed(P.size()); i++) {
		auto it1 = M.Palavra_NumDoc_TF.find(P[i]);
		if (it1 != M.Palavra_NumDoc_TF.end()) {
			for (auto k = M.Palavra_NumDoc_TF[it1->first].begin(); k != M.Palavra_NumDoc_TF[it1->first].end(); k++) {
				auto FindDoc = NumeroDosDocumentosDaPesquisa.find(k->first);
				auto END = NumeroDosDocumentosDaPesquisa.end();
				if (FindDoc == END) {
					NumeroDosDocumentosDaPesquisa.insert(k->first);
				}
			}
		}

		if (PalavrasPesquisadas.Pesquisa.find(P[i]) == PalavrasPesquisadas.Pesquisa.end()) {
			PalavrasPesquisadas.Pesquisa[P[i]] = 0;
		}
		PalavrasPesquisadas.Pesquisa[P[i]]++;
	}

	PalavrasPesquisadas.CoordPesquisa = MDB::PalavraParaCoordenadasDaPesquisa(M, PalavrasPesquisadas.Pesquisa);
}

vector<string> MDB::Buscar() {
	vector<string> Frase_SEPARADA;
	cout << endl << endl << "BUSCAR: ";
	string frase, palavra;

	cin.ignore();
	getline(cin, frase);

	istringstream paraSeparar(frase);

	while (getline(paraSeparar, palavra, ' ')) {
		if (palavra != "") {
			palavra = Stringtolower(palavra);
			palavra = justAlNum(palavra);
			Frase_SEPARADA.push_back(palavra);
		}
	}

	return Frase_SEPARADA;
}

void MDB::imprimirInformacaoEmArquivo(MDB::MapsModel& m, map<double, int> CosRank_NumDoc, string path, vector<string> Frase) {

	string search;
	search.clear();
	fstream arquivo(path, fstream::out);
	
	if (arquivo.is_open()) {

		arquivo << "RESULTADOS DA BUSCA POR: ";

		for (auto i = Frase.begin(); i != Frase.end(); i++) {
			search += *i + " ";
		}

		arquivo << search << endl ;

		if (CosRank_NumDoc.size() == 0) {
			arquivo << endl << endl <<
				"Sua pesquisa - " << search << "- não encontrou nenhum documento correspondente." << endl << endl <<
				"\tSugestões:" << endl << endl <<
				"Certifique-se de que todas as palavras estejam escritas corretamente." << endl <<
				"\t->Tente palavras-chave diferentes." << endl <<
				"\t->Tente palavras-chave mais genéricas." << endl;
			arquivo.close();
		}
		else {
			arquivo << "( " << CosRank_NumDoc.size() <<  " RESULTADOS )" << endl << endl;

			int k = 1;
			// inverso pois o indice de cosseno no programa vai do menor pro maior
			for (auto i = CosRank_NumDoc.rbegin(); i != CosRank_NumDoc.rend(); i++, k++) {
				arquivo << k << "-> " << m.NumDoc_Documento[i->second] << endl;
			}
			arquivo.close();
		}
	}
}

void MDB::CLEAR(HANDLE hConsole) {

	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer. 

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer 
		(TCHAR) ' ',     // Character to write to the buffer
		dwConSize,       // Number of cells to write 
		coordScreen,     // Coordinates of first cell 
		&cCharsWritten))// Receive number of characters written
	{
		return;
	}

	// Get the current text attribute.

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Set the buffer's attributes accordingly.

	if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer 
		csbi.wAttributes, // Character attributes to use
		dwConSize,        // Number of cells to set attribute 
		coordScreen,      // Coordinates of first cell 
		&cCharsWritten)) // Receive number of characters written
	{
		return;
	}

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(hConsole, coordScreen);
}

void MDB::cls() {
	HANDLE hStdout;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CLEAR(hStdout);
}