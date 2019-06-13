# Instruções de Uso da Máquina da Busca
A máquina implementada utiliza os arquivos dentro dos arquivos DocumentosBASE.rar, initConfigFILES.rar e programFILES.rar.

 Descrição do conteudo dos arquivos de extensão ".rar":

  - DocumentosBASE.rar: Possui 20 pastas contendo uma série de documentos dentro de cada pasta;  
  
  - initConfigFILES.rar: Possui 3 arquivos de teste para configuração inicial para a execução do programa, sendo eles:
  
    - ConfigOK.txt: Configuração exclusiva para a coletânia de arquivos que contenham o nome do arquivo
                    composto exclusivamente de numeros e sem a extensão ".txt";
                    Possui erros na abertura dos arquivos colocados propositalmente para o teste de funções da máquina de busca;
                    
    - ConfigNOK.txt: Configuração que depende dos arquivos existentes dentro de programFILES.rar;
                     Assim como em ConfigOK.txt, possui erros na abertura dos arquivos colocados propositalmente para o teste
                     de funções da máquina de busca;
                     
    - ConfigSE.txt: Configuração sem erros na abertura de nenhum arquivo;
                    Assim como em ConfigOK.txt, é uma configuração exclusiva para a coletânia de arquivos que contenham o nome
                    do arquivo composto exclusivamente de numeros e sem a extensão .txt;
    
  - programFILES.rar: Contém arquivos gerados e/ou utilizados pelo programa. Também é o destino do arquivo que mostra os erros em
                      abertura de arquivos e do resultado da busca.

(Dica: abra os arquivos citados abaixo durante a explicação para melhor entendimento)

Dentro de todos os arquivos de configuração inicial, há uma estrutura em comum, a primeira linha é uma flag ("PI.config.ok") 
para indicar se o programa deve olhar alguns valores como nomes de outros arquivos ou como números. A segunda linha é onde 
irá ser realizado o restante da configuração e onde também será armazenado o resultado da busca e a lista de erros.
A terceira linha é o caminho onde os documentos que fazem parte da busca estão.

  As linhas restantes são divididas em 2 tipos de acordo com a flag. Se a flag for "PI.config.ok", faz a leitura da primeira string 
como parte do caminho e os 2 próximos números são os intervalos que correspondem aos arquivos de leitura, os intervalos são então,
armazenados em arquivos .txt juntamente com a terceira linha do arquivo de configuração inicial e a primeira string lida que faz
parte do caminho. A flag torna a busca "independente" da pasta programFILES. Por outro lado, se a flag for diferente de 
PI.config.ok, o arquivo não contém números para gerar os arquivos auxiliares e precisa necessariamente que eles já existam.

Pegando como exemplo o arquivo "comp.graphics.txt", localizado dentro de programFILES, a primeira linha é composta da terceira linha do
arquivo de configuração inicial junto do nome comp.graphics seguido por uma barra ao final. As próximas linhas são nomes de arquivos,
existentes ou não, dentro da pasta comp.graphics cujo nome é o mesmo do arquivo comp.graphics.txt.
