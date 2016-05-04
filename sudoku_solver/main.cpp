#include <iostream>
#include <windows.h>
#include <math.h>

#define TAM 500
using namespace std;

void criaJogo(int qtdVertices,int matrizAdj[TAM][TAM]){

    int roots2QtdVertices = sqrt(qtdVertices);
    int roots4QtdVertices = sqrt(roots2QtdVertices);

    int colunaLimite = roots2QtdVertices * roots4QtdVertices;

    int linha, coluna;

    //Coluna
    for(int i=0;i<qtdVertices;i++){
        for(int j=0;j<roots2QtdVertices;j++){
            matrizAdj[i][(j*roots2QtdVertices)+i]=1;
            matrizAdj[(j*roots2QtdVertices)+i][i]=1;
        }

    }


    //Area

    for(int i=0;i<qtdVertices;i+=roots4QtdVertices){
		for(int j=0;j<=roots4QtdVertices;j++){
			for(int k=0;k<roots4QtdVertices;k++){
				for(int l=0;l<roots4QtdVertices;l++){

					linha = i+k;
					coluna = i+j*roots2QtdVertices+l;

					if((linha / colunaLimite) == (coluna / colunaLimite)){
						matrizAdj[linha][coluna]=1;
						matrizAdj[coluna][linha]=1;
					}
				}
			}
		}
    }


    //Linha
    for(int i=0;i<qtdVertices;i++){
        for(int j=0;j<qtdVertices;j++){
            if((i/roots2QtdVertices)  == (j/roots2QtdVertices)){
                matrizAdj[i][j] = 1;
            }
        }
    }

    for(int i=0;i<TAM;i++){
        for(int j=0;j<TAM;j++){
            if(i == j){
                matrizAdj[i][j] = -1;
            }
        }
    }

}

void mostraColoracao(int qtdVertices,int vertice[]){

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int roots2QtdVertices = sqrt(qtdVertices);
    cout << "\n\n\t";

    for(int j=0;j<qtdVertices;j++){

        if(j % roots2QtdVertices == 0){
            cout << "\n\t";
        }

        if(vertice[j] == -1){
            cout << 0<< "\t";
        }else{
            SetConsoleTextAttribute(hConsole, vertice[j]+1);
            cout << vertice[j]+1<< "\t";
        }
    }

    SetConsoleTextAttribute(hConsole, 7);

}

void mostraMatrizPosicoes(int qtdVertices){
    int roots2QtdVertices = sqrt(qtdVertices), counter = 1;
    cout << "\n\n\t";

    for(int j=0;j<roots2QtdVertices;j++){
        for(int i=0;i<roots2QtdVertices;i++){
            cout <<counter<< "\t";
            counter++;
        }
        cout << "\n\t";
    }
}

void mostraMatriz(int qtdVertices,int matrizAdj[TAM][TAM]){
    cout << " Matriz de adjacencia:\n\n\n"<< " ";
        for(int i=0;i<qtdVertices;i++){
            for(int j=0;j<qtdVertices;j++){
                if(matrizAdj[i][j] == -1){
                    cout << 0;
                }else{
                    cout << matrizAdj[i][j];
                }
            }
            cout << "\n"<< " ";
        }
}

int todosVerticeVisitados(int qtdVertices,int foiVisitado[]){

    for(int i=0;i<qtdVertices;i++){
        if(foiVisitado[i] < 0){
            return 0;
        }
    }
    return 1;
}

int encontraGrau(int qtdVertices, int verticeAdj, int matrizAdj[TAM][TAM]){

	int grau = 0;

	for(int j = 0; j < qtdVertices;j++){
		if(matrizAdj[verticeAdj][j] > 0){
			grau++;
		}
	}
	return grau;
}

int encontraProxVertice(int qtdVertices,int foiColorido[],int matrizAdj[TAM][TAM]){

    int maiorGrau = -1;
	int maiorGrauSat = -1;
	int grauSatAtual = 0;
	bool corUtilizada[qtdVertices];
	int grauSat[qtdVertices];

    //Inicializa vetores
	for(int h=0;h<qtdVertices;h++){
        grauSat[h]=0;
		corUtilizada[h] = false;
    }

    //Encontra um grau de saturação para cada vertice
	for (int i = 0; i < qtdVertices;i++){
		if(foiColorido[i] < 0){
			for(int j = 0; j < qtdVertices;j++){
				if((matrizAdj[i][j] > -1) && (foiColorido[j] > -1) && (!corUtilizada[foiColorido[j]])){
					corUtilizada[foiColorido[j]] = true;
					grauSatAtual++;
				}
			}
			grauSat[i] = grauSatAtual;
			grauSatAtual = 0;

            for(int o=0;o<qtdVertices;o++){
                corUtilizada[o] = false;
            }
		}
	}

    //Encontra o maior grau de saturacao entre os graus de saturacao armazenados
	for (int l = 0; l < qtdVertices;l++){
		if(grauSat[l] > maiorGrauSat){
			maiorGrauSat = grauSat[l];
		}
	}

    //Encontra o maior grau entre os vertices com o maior grau de saturacao
	for (int m = 0; m < qtdVertices;m++){
		if(grauSat[m] == maiorGrauSat){
            if(encontraGrau(qtdVertices,m,matrizAdj) > maiorGrau){
                maiorGrau = encontraGrau(qtdVertices,m,matrizAdj);
            }
		}
	}

    //Retorna o primeiro vertice que contem o maior grau de saturacao e o maior grau
	for (int n = 0; n < qtdVertices;n++){
		if((grauSat[n] == maiorGrauSat) && (encontraGrau(qtdVertices,n,matrizAdj) == maiorGrau)){
			return n;
		}
	}

}

int encontraCor(int qtdVertices,int verticeMaiorGrau,int foiColorido[],int matrizAdj[TAM][TAM]){

	bool corUtilizada[qtdVertices];

	for(int h=0;h<qtdVertices;h++){
		corUtilizada[h] = false;
    }

	for(int j = 0; j < qtdVertices;j++){
		if((matrizAdj[verticeMaiorGrau][j] > 0) && (foiColorido[j] > -1)){
			corUtilizada[foiColorido[j]] = true;
		}
	}

	for(int k=0;k<qtdVertices;k++){
		if(!corUtilizada[k]){
			return k;
		}
    }
}

int coloreGrafo(int qtdVertices,int foiColorido[],int matrizAdj[TAM][TAM]){

    for(int i=0;i<qtdVertices;i++){
        foiColorido[i]=-1;
    }
    int verticeAtual, valorInicial;

    cout << "\nPosicoes: ";

    mostraMatrizPosicoes(qtdVertices);
    cout << "\n\n\nInforme a posicao inicial: ";
	//Fazer conversao de posicao na matriz para vertice?
	cin >> verticeAtual;
    verticeAtual--;

    cout << "\nInforme o valor para preencher a posicao inicial (1-"<<sqrt(qtdVertices)<<"):";
	//Fazer conversao de posicao na matriz para vertice?
	cin >> valorInicial;
	valorInicial--;


	foiColorido[verticeAtual]=valorInicial;

    while(!todosVerticeVisitados(qtdVertices,foiColorido)){

		verticeAtual = encontraProxVertice(qtdVertices,foiColorido,matrizAdj);

		foiColorido[verticeAtual] = encontraCor(qtdVertices,verticeAtual,foiColorido,matrizAdj);

	}

	return 1;
}

int main(){
    system("mode CON: COLS=300");
    int option;
    int qtdVertices;
    int foiColorido[TAM];
    int matrizAdj [TAM][TAM];

    cout << "\n\t Chegou Aqui";

    while(1){
        cout    << "\n 1  - Novo jogo"
                << "\n 2  - Calcula resultado"
                << "\n 3  - Exibe jogo"
                << "\n 4  - Exibe matriz"
                << "\n 5  - Exibe coloracao do grafo armazenado"
                << "\n\n\n 0  - Sair\n\n ";

        cin >> option;
        system("CLS");
        switch(option){
            case 1:
                cout    << "\n\t Informe o tamanho do grafo: ";
                cin >> qtdVertices;
                for(int i = 0;i<TAM;i++){
                    for(int j = 0;j<TAM;j++){
                        matrizAdj[i][j] = -1;
                    }
                }
                qtdVertices = qtdVertices*qtdVertices;
                criaJogo(qtdVertices,matrizAdj);
            break;
            case 2:
                if(coloreGrafo(qtdVertices,foiColorido,matrizAdj)){
                    cout << "\n\t Resultado calculado com sucesso";
                }
            break;
            case 3:
                cout << " Cores no grafo:\n";
                mostraColoracao(qtdVertices,foiColorido);
            break;

            case 4:
                mostraMatriz(qtdVertices,matrizAdj);
            break;


            case 5:

                for(int i = 0;i<TAM;i++){
                    for(int j = 0;j<TAM;j++){
                        matrizAdj[i][j] = -1;
                    }
                }

                qtdVertices = 16;


matrizAdj[0][0]		=-1;
matrizAdj[0][1]		=1;
matrizAdj[0][2]		=1;
matrizAdj[0][3]		=1;
matrizAdj[0][4]		=1;
matrizAdj[0][5]		=1;
matrizAdj[0][8]		=1;
matrizAdj[0][12]	=1;

matrizAdj[1][0]		=1;
matrizAdj[1][1]		=-1;
matrizAdj[1][2]		=1;
matrizAdj[1][3]		=1;
matrizAdj[1][4]		=1;
matrizAdj[1][5]		=1;
matrizAdj[1][9]		=1;
matrizAdj[1][13]	=1;

matrizAdj[2][0]		=1;
matrizAdj[2][1]		=1;
matrizAdj[2][2]		=-1;
matrizAdj[2][3]		=1;
matrizAdj[2][6]		=1;
matrizAdj[2][7]		=1;
matrizAdj[2][10]	=1;
matrizAdj[2][14]	=1;

matrizAdj[3][0]		=1;
matrizAdj[3][1]		=1;
matrizAdj[3][2]		=1;
matrizAdj[3][3]		=-1;
matrizAdj[3][6]		=1;
matrizAdj[3][7]		=1;
matrizAdj[3][11]	=1;
matrizAdj[3][15]	=1;

matrizAdj[4][0]		=1;
matrizAdj[4][1]		=1;
matrizAdj[4][4]		=-1;
matrizAdj[4][5]		=1;
matrizAdj[4][6]		=1;
matrizAdj[4][7]		=1;
matrizAdj[4][8]		=1;
matrizAdj[4][12]	=1;

matrizAdj[5][0]		=1;
matrizAdj[5][1]		=1;
matrizAdj[5][4]		=1;
matrizAdj[5][5]		=-1;
matrizAdj[5][6]		=1;
matrizAdj[5][7]		=1;
matrizAdj[5][9]		=1;
matrizAdj[5][13]	=1;

matrizAdj[6][2]		=1;
matrizAdj[6][3]		=1;
matrizAdj[6][4]		=1;
matrizAdj[6][5]		=1;
matrizAdj[6][6]		=-1;
matrizAdj[6][7]		=1;
matrizAdj[6][10]	=1;
matrizAdj[6][14]	=1;

matrizAdj[7][2]		=1;
matrizAdj[7][3]		=1;
matrizAdj[7][4]		=1;
matrizAdj[7][5]		=1;
matrizAdj[7][6]		=1;
matrizAdj[7][7]		=-1;
matrizAdj[7][11]	=1;
matrizAdj[7][15]	=1;

matrizAdj[8][8]		=-1;
matrizAdj[8][9]		=1;
matrizAdj[8][10]	=1;
matrizAdj[8][11]	=1;
matrizAdj[8][12]	=1;
matrizAdj[8][13]	=1;
matrizAdj[8][0]		=1;
matrizAdj[8][4]		=1;

matrizAdj[9][8]		=1;
matrizAdj[9][9]		=-1;
matrizAdj[9][10]	=1;
matrizAdj[9][11]	=1;
matrizAdj[9][12]	=1;
matrizAdj[9][13]	=1;
matrizAdj[9][1]		=1;
matrizAdj[9][5]		=1;

matrizAdj[10][8]	=1;
matrizAdj[10][9]	=1;
matrizAdj[10][10]	=-1;
matrizAdj[10][11]	=1;
matrizAdj[10][14]	=1;
matrizAdj[10][15]	=1;
matrizAdj[10][2]	=1;
matrizAdj[10][6]	=1;

matrizAdj[11][8]	=1;
matrizAdj[11][9]	=1;
matrizAdj[11][10]	=1;
matrizAdj[11][11]	=-1;
matrizAdj[11][14]	=1;
matrizAdj[11][15]	=1;
matrizAdj[11][3]	=1;
matrizAdj[11][7]	=1;

matrizAdj[12][8]	=1;
matrizAdj[12][9]	=1;
matrizAdj[12][12]	=-1;
matrizAdj[12][13]	=1;
matrizAdj[12][14]	=1;
matrizAdj[12][15]	=1;
matrizAdj[12][0]	=1;
matrizAdj[12][4]	=1;

matrizAdj[13][8]	=1;
matrizAdj[13][9]	=1;
matrizAdj[13][12]	=1;
matrizAdj[13][13]	=-1;
matrizAdj[13][14]	=1;
matrizAdj[13][15]	=1;
matrizAdj[13][1]	=1;
matrizAdj[13][5]	=1;

matrizAdj[14][10]	=1;
matrizAdj[14][11]	=1;
matrizAdj[14][12]	=1;
matrizAdj[14][13]	=1;
matrizAdj[14][14]	=-1;
matrizAdj[14][15]	=1;
matrizAdj[14][2]	=1;
matrizAdj[14][6]	=1;

matrizAdj[15][10]	=1;
matrizAdj[15][11]	=1;
matrizAdj[15][12]	=1;
matrizAdj[15][13]	=1;
matrizAdj[15][14]	=1;
matrizAdj[15][15]	=-1;
matrizAdj[15][3]	=1;
matrizAdj[15][7]	=1;




            if(coloreGrafo(qtdVertices,foiColorido,matrizAdj)){
                mostraColoracao(qtdVertices,foiColorido);
            }

            break;


            default: exit(1);
        }
        cout << "\n\n\n";
        system("PAUSE");
        system("CLS");
    }


}
