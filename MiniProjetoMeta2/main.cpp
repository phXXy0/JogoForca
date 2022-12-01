#include <chrono>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int gameon = 1, abandona, randtheme, randtime, tempomaxjogo = 1, testa, timed, respostanick = 0;

string nick;

void atsettingsfile(){
    remove("settings.txt");

    ofstream settings;

    settings.open("settings.txt");
    settings << randtheme << endl << randtime << endl << tempomaxjogo << endl << abandona << endl << timed << endl << nick;
    settings.close();
}

void guardaprogressogeral(int tentativas, string word, string progresso, string falhadas, string nome, int tempo){
    ofstream guarda;

    guarda.open(nome+"_ERRO.txt");

    if(!guarda){
        cout << "Erro a guardar progresso silencioso." << endl;
    }

    guarda << falhadas << " " << tentativas << " " << word << " " << progresso << " " << tempo <<  endl;
    guarda.close();
}

typedef struct{
    string nickname;
    int score;
    int saved;
    long int highscore;
}jogadores;

class players{
private:

public:
    int playerarray();
    int go, verifica;
    void preenchejogador();
    int score;
    void carregatab(jogadores []);
    int testanick(jogadores [], int);
    void organizatab(jogadores [], int);
    bool compare(jogadores, jogadores);
};

class jogaforca{
private:
    string palavra, temaatual, passagem;
    void apresentaPalavra();
    const char *meses[8] = {"janeiro","fevereiro","abril","maio","agosto","setembro","outubro","dezembro"};
    const char *transportes[8] = {"carro","barco","aviao","bicicleta","mota","trotineta","metro","autocarro"};
    const char *instrumentos[8] = {"guitarra","flauta","tambor","piano","baixo","violino","saxofone","trompete"};
    void boneco();
    int testaletra();
    char testarletra;
    float pausa = 0, tempototaldejogo = 0;
    int calcpont();
    void guardaprogresso();
    int guardatempo = 0;
public:
    void apresentacao();
    void settingsdisp();
    char telaini();
    void escolhetemapalavra();
    void escolhepalavra(int);
    void jogo(jogadores [], int);
    void jogountimed();
    string triesletras = "_________";
    int tries = 9, Eletra = 0, diversao, tema, indicenatabela, score;
    void registapont(jogadores [], int);
    void registapontnew(int, int);
    void mostrascore(jogadores [], int);
    void carregaprogress();
    void removefiles(int, int);
    bool verificasejogoperdido();
    void carregaprogressperdido();
};

class settings{
public:
    void timerandom();
    void settingsdisp(int, int);
    void setting(int&, int&);
    void settingsfile();
    int escolhamenu;
private:
};

//############################################################################################################################
int players::playerarray(){                                              //FAZ CONTAGEM DE MEMBROS DA BASE DE DADOS
    ifstream scoreboard("scoreboard.txt");
    int count=0, x, y, j;
    string temp;
    while(scoreboard >> x >> temp >> y >> j)
    {
        ++count;
    }
    return count;
};
//############################################################################################################################
char jogaforca::telaini(){                                               //MENU INICIAL DO JOGO
    char escolha;
    cout<<" -------------------------------------------------- "<<endl;
    cout<<"|1)Comecar um jogo aleatorio modo diversao         |"<<endl;
    cout<<"|2)Comecar um novo jogo competitivo                |"<<endl;
    cout<<"|3)Ver a tabela de pontuacoes                      |"<<endl;
    cout<<"|4)Definicoes                                      |"<<endl;
    cout<<"|5)SAIR                                            |"<<endl;
    cout<<" -------------------------------------------------- "<<endl;
    cout<<"Escolha aqui a opcao pretendida. >";
    cin>>escolha;
    return(escolha);
}
//############################################################################################################################
void jogaforca::escolhetemapalavra(){                                    //FUNC QUE ESCOLHE O TEMA E A PALAVRA
    srand(time(NULL));
    int rt = rand()% 3;
    int rp = rand()% 8;

    if (rt == 0){
        palavra = meses[rp];
        temaatual = "Meses do ano.";
        apresentaPalavra();
    }
    if (rt == 1){
        palavra = transportes[rp];
        temaatual = "Meios de Transporte.";
        apresentaPalavra();
    }
    if (rt == 2){
        palavra = instrumentos[rp];
        temaatual = "Instrumentos musicais.";
        apresentaPalavra();
    }
}
//############################################################################################################################
void jogaforca::apresentaPalavra(){                                      //FUNC QUE ESCREVE A PALAVRA E QUE GERA A PASSAGEM SECRETA
    string secret(palavra.length(),'_');
    cout << "A palavra tem " << palavra.length() << " caracteres e o tema e: " << temaatual << endl << "--> " << secret << "<--" << endl;
    passagem = secret;
}
//############################################################################################################################
void players::carregatab(jogadores *tab){                                //FUNC QUE RECEBE TABELA POR REF E PREENCHE CONFORME DADOS DA BD
    string nickname;
    int pontos, i = 0, high;
    ifstream scoreboard;
    scoreboard.open("scoreboard.txt");
    if(!scoreboard) {
        cout << "O ficheiro da scoreboard nao foi lido." << endl;
        exit(1);
    }
    while(scoreboard >> pontos >> nickname >> verifica >> high){
        tab[i].nickname = nickname;
        tab[i].score = pontos;
        tab[i].saved = verifica;
        tab[i].highscore = high;
        i++;
    }
    scoreboard.close();
};
//############################################################################################################################
void players::preenchejogador(){                                         //FUNC QUE ESCOLHE OU INSERE NOVO JOGADOR
    go = 0, respostanick = 0;
    cout << "Pretende continuar com o nick " << nick << " ? 1 -> SIM 2-> NAO" << "  :" ;
    while((respostanick != 1) && (respostanick != 2)){
        cin >> respostanick;
    }
    if (respostanick == 2){
        cout << "Qual o nick? ";
        cin >> nick;
    }
    else go = 1;
    atsettingsfile();
}
//############################################################################################################################
int players::testanick(jogadores *tab, int dim){                         //FUNC QUE VERIFICA SE ESTE USER JA ESTA NA BD
    for (int i = 0; i < dim; i++){
        if (nick == tab[i].nickname)
            return i;
    }
    return -1;
};
//############################################################################################################################
bool jogaforca::verificasejogoperdido(){                                 //FUNC QUE VERIFICA SE ALGUM JOGO FOI ENCERRADO INESPERADAMENTE
    ifstream ERRO;
    ERRO.open(nick+"_ERRO.txt");
    if(!ERRO) {
        return 0;
    }
    ERRO.close();
    return 1;
}
//############################################################################################################################
void jogaforca::removefiles(int silencio, int guarda){                   //FUNC QUE REMOVE FICHEIROS DA BD
    auto filenamesilensioso = nick + "_ERRO.txt";
    auto filenameguarda = nick + ".txt";
    if (silencio == 1) remove(filenamesilensioso.c_str());
    if (guarda == 1)remove(filenameguarda.c_str());
}
//############################################################################################################################
void jogaforca::carregaprogressperdido(){                                //FUNC QUE CARREGA O PROGRESSO DE UM JOGO FECHADO N/ GUARDADO
    ifstream carrega;
    carrega.open(nick+"_ERRO.txt");

    if (carrega.is_open()){
        carrega >> triesletras >> tries >> palavra >> passagem >> guardatempo;
    }
    carrega.close();
}
//############################################################################################################################
void jogaforca::jogo(jogadores* tab, int dim){                           //FUNC PRINCIPAL DO JOGO DA FORCA
    int resposta = 0, pausaint, t = 0;
    auto start = high_resolution_clock::now();                                                                                  //regista tempo no inicio da func
    while (tries!=0) {
        system("cls");
        cout << "A palavra tem " << palavra.length() << " caracteres e o tema e: " << temaatual << endl;
        cout << "O jogador possui: " << tries << " vidas." << endl;
        cout << endl << endl << "--> " <<  passagem << " <-- " << endl << endl;
        cout << "Letras falhadas: " << triesletras << "Tempo utilizado: " << t << endl;
        cout << endl << endl;
        boneco ();
        cout << endl << endl;
        cout << "Insira uma letra(apenas sao reconhecidas letras minusculas): " << endl;
        cout << "PRIMA ASTERISCO (*) PARA MENU DE PAUSA" << endl << "Testar letra: ";
        cin >> testarletra;
        pausaint = testaletra();
        auto stopF4 = high_resolution_clock::now();
        auto temp = duration_cast<seconds>(stopF4 - start);
        t = (temp.count());

        guardaprogressogeral(tries, palavra, passagem, triesletras, nick, t);

        if (pausaint != 0){                                                                                                     // SE LÊ * METE JOGO EM PAUSA
            auto startp = high_resolution_clock::now();                                                                         //REGISTA TEMPO INICIAL DA PAUSA
            auto durationatepausa = duration_cast<seconds>(startp - start);                                                     //calcula tempo que esteve ate dar pausa
            guardatempo = (durationatepausa.count());                                                                            //guarda o tempo que esteve ate a pausa na var guardatempo
            if (diversao == 0){
                while((resposta != 1) && (resposta != 2)){
                    cout << "JOGO EM PAUSA" << endl <<"Tempo jogado: " << guardatempo << endl << "Deseja continuar? 1->SIM 2->GUARDAR E IR PARA MENU" << endl;
                    cin >> resposta;
                }
            }
            else{
                while((resposta != 1) && (resposta != 2)){
                    cout << "JOGO EM PAUSA" << endl << "Deseja continuar? 1->SIM 2->IR PARA MENU" << endl;
                    cin >> resposta;
                }
            }
            if (resposta == 2){                                                                               //asijdfbawiubdiABIFBIBFIEBIFUS
                if (diversao == 0) guardaprogresso();
                if (testa != -1) tab[testa].saved = 1;
                if (indicenatabela == -1) registapontnew(0,1);
                else registapont(tab,dim);
                atsettingsfile();
                removefiles(1,0);
                system("cls");
                break;
            }
            else{ resposta = 0, guardatempo = 0;}
            auto stopp = high_resolution_clock::now();                                                                                // regista tempo final da pausa
            auto durationp = duration_cast<seconds>(stopp - startp);                                                                  // calcula o tempo de esteve em pausa
            pausa += (durationp.count());                                                                                            // acrescenta o tempo que o jogo esteve pausado em PAUSA

        }
        if (passagem == palavra && tries > 0){
            system("cls");
            auto stop = high_resolution_clock::now();                                                                            //regista tempo final func TOTAL
            auto duration = duration_cast<seconds>(stop - start);                                                                // calcula tempo que o programa correu em
            tempototaldejogo = (duration.count());
            if ((tempototaldejogo + guardatempo) > (tempomaxjogo + pausa)){
                cout << "Atingiste o limite maximo de tempo[" << tempomaxjogo << "]! Fizeste: " << (tempototaldejogo - pausa + guardatempo) << " S! Melhor sorte na proxima!" << endl;
                if (diversao == 0){
                    if (testa != -1) registapont(tab, dim);
                    else registapontnew(0,0);
                }
                removefiles(1,1);
                atsettingsfile();
                break;
            }
            else{
                cout << "Parabens! Ganhaste o jogo com " << tries << " vidas e demoraste: " << (guardatempo + tempototaldejogo - pausa) << " segundos a concluir a palavra!" << endl << "A palavra era: " << palavra << endl;
                if (indicenatabela != -1) score = tab[indicenatabela].score;
                int mostrascoreronda = calcpont();
                score += mostrascoreronda;
                if (score > tab[testa].highscore) tab[testa].highscore = score;
                cout << "Obtiveste um score de " << mostrascoreronda << " pontos!!" << endl;
                removefiles(1,1);
                if (diversao == 0){
                    if (indicenatabela != -1){
                        tab[indicenatabela].score = score;
                        tab[indicenatabela].saved = 0;
                        registapont(tab, dim);
                    }
                    else registapontnew(score,0);
                }
            }
            atsettingsfile();
            break;
        }
        if (tries == 0){
            removefiles(1,1);
            system("cls");
            cout << "Chegaste a 0 tentativas... perdeste o jogo." << endl << "A palavra era: " << palavra << endl << endl << endl;
            boneco();
            cout << endl << endl;
            if (diversao == 0){
                abandona = 0;
                if (indicenatabela != -1){
                    tab[indicenatabela].score = 0;
                    registapont(tab, dim);
                }
                else registapontnew(0,0);
            }
        }
    }
    tries = 9;
    triesletras = "_________";
}
//############################################################################################################################
void jogaforca::boneco(){                                                //FUNC DE DESENHO DO BONECO
    if(tries == 9)
    {
        cout << "                " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==8)
    {
        cout << " ___________    " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==7)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==6)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |              " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==5)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |         |    " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==4)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |         |\\  " << endl;
        cout << " |               " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==3)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |        /|\\  " << endl;
        cout << " |              " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==2)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |        /|\\  " << endl;
        cout << " |        /     " << endl;
        cout << " |_____________ " << endl;
    }
    else if(tries==1)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         0    " << endl;
        cout << " |        /|\\  " << endl;
        cout << " |        / \\  " << endl;
        cout << " |_____________ " << endl;
    }
    else if (tries == 0)
    {
        cout << " ___________    " << endl;
        cout << " |         |    " << endl;
        cout << " |         X    " << endl;
        cout << " |        /|\\  " << endl;
        cout << " |        / \\  " << endl;
        cout << " |________DEAD_ " << endl;
    }
}
//############################################################################################################################
int jogaforca::testaletra(){                                             //FUNC QUE PREENCHE A PASSAGEM SECRETA E TESTA SE A LETRA PERTENCE
    int comp = palavra.length();
    int cletraparam = 0;
    if (testarletra == '*') return -1;
    for (int i = 0; i < comp; i++){
        if (testarletra == triesletras[i]){
            return 0;
        }
    }
    for (int i = 0; i < comp; i++){
        if (testarletra == passagem[i]){
            return 0;
        }

        if (testarletra == palavra[i])
        {
            passagem[i] = testarletra;
            cletraparam++;
        }
    }
    if (cletraparam == 0){
        tries--;
        Eletra++;
        triesletras[8-tries] = testarletra;
    }
    return (0);
}
//############################################################################################################################
void jogaforca::guardaprogresso(){                                       //GUARDA PROGRESSO
    ofstream guarda;

    guarda.open(nick+".txt");

    if(!guarda){
        cout<<"ERRO A GUARDAR O PROGRESSO" << endl;
    }

    guarda << triesletras << " " << tries << " " << palavra << " " << passagem << " " << guardatempo <<  " " << nick << endl;
    guarda.close();
}
//############################################################################################################################
void jogaforca::carregaprogress(){                                       //CARREGA PROGRESSO
    ifstream carrega;
    carrega.open(nick+".txt");

    if(!carrega){
        cout<<"Error opening players progress!!!";
    }

    if (carrega.is_open()){
        carrega >> triesletras >> tries >> palavra >> passagem >> guardatempo >> nick;
    }
    carrega.close();
}
//############################################################################################################################
void jogaforca::apresentacao(){
    cout << setw(60) << "::::::::::BEM-VINDO AO JOGO DA FORCA::::::::::" << endl << right << endl << setw(60) << right;
    cout << "Este jogo foi desenvolvido por 2016227279 Pedro Henriques no ambito do primeiro" << endl << setw(15);
    cout  << " mini projeto para a cadeira de EDA do LEC." << endl << endl << endl;
};
//############################################################################################################################
void jogaforca::escolhepalavra(int p){                                   //ESCOLHE PALAVRA RANDOM
    srand(time(NULL));
    int rp = rand()% 8;

    if (p == 1){
        palavra = meses[rp];
        temaatual = "Meses do ano.";
        apresentaPalavra();
    }
    if (p == 2){
        palavra = transportes[rp];
        temaatual = "Meios de Transporte.";
        apresentaPalavra();
    }
    if (p == 3){
        palavra = instrumentos[rp];
        temaatual = "Instrumentos musicais.";
        apresentaPalavra();
    }
}
//############################################################################################################################
void jogaforca::registapont(jogadores *tab, int dim){                    //REGISTA PONT EM BD
    remove("scoreboard.txt");

    ofstream scoreboard;

    scoreboard.open("scoreboard.txt");

    if(!scoreboard){
        cout<<"Error in creating file!!!";
    }
    for (int i = 0; i < dim; i++){
        scoreboard << tab[i].score << " " << tab[i].nickname << " " << tab[i].saved << " " << tab[i].highscore << endl;
    }

    scoreboard.close();
}
//############################################################################################################################
void jogaforca::jogountimed(){                                           //MODELO DE JOGO PARA MODO DIVERSAO
    int resposta = 0, pausaint;
    tries = 9;
    triesletras = "_________";
    while (tries!=0) {
        system("cls");
        cout << "A palavra tem " << palavra.length() << " caracteres e o tema e: " << temaatual << endl;
        cout << "O jogador possui: " << tries << " vidas." << endl;
        cout << endl << endl << "--> " <<  passagem << " <-- " << endl << endl;
        cout << "Letras falhadas: " << triesletras << endl;
        cout << endl << endl;
        boneco ();
        cout << endl << endl;
        cout << "Insira uma letra(apenas sao reconhecidas letras minusculas): " << endl;
        cout << "PRIMA ASTERISCO (*) PARA MENU DE PAUSA" << endl << "Testar letra: ";
        cin >> testarletra;
        pausaint = testaletra();

        if (pausaint != 0){                                                                                                     // SE LÊ * METE JOGO EM PAUSA
            while((resposta != 1) && (resposta != 2)){
                cout << "JOGO EM PAUSA" << endl << "Deseja continuar? 1->SIM 2->SAIR PARA O MENU" << endl;
                cin >> resposta;
            }
            if (resposta == 2){                                                                               //asijdfbawiubdiABIFBIBFIEBIFUS
                system("cls");
                break;
            }
        }
        if (passagem != palavra && tries > 0){
            system("cls");
            cout << "A palavra tem " << palavra.length() << " caracteres e o tema e: " << temaatual << endl;
            cout << "O jogador possui: " << tries << " vidas." << endl;
            cout << endl << endl << "--> " <<  passagem << " <-- " << endl << endl;
            cout << "Letras falhadas: " << triesletras << endl;
            cout << endl << endl;
        }
        if (passagem == palavra && tries > 0){
            system("cls");
            cout << "Parabens! Acabaste a palavra com " << tries << " vidas!" << endl << "A palavra era: " << palavra << endl;
            break;
        }
    }
    if (tries == 0){
        system("cls");
        cout << "Chegaste a 0 tentativas... perdeste o jogo." << endl << "A palavra era: " << palavra << endl << endl << endl;
        boneco();
        cout << endl << endl;
    }
}
//############################################################################################################################
void jogaforca::registapontnew(int d, int saida){                        //REGISTA PONT EM BD DE UMA NOVA ENTRADA
    ofstream scoreboard;
    scoreboard.open("scoreboard.txt", std::ios_base::app);
    if(!scoreboard) {
        cout << "O ficheiro da scoreboard nao foi lido." << endl;
        exit(1);
    }
    if(scoreboard.is_open()){
        scoreboard << d << " " << nick << " " << saida << " 0" << endl;
    }
    scoreboard.close();
}
//############################################################################################################################
int jogaforca::calcpont(){                                               //CALC PONT DA JOGADA
    int pontuacao;
    if (tempomaxjogo == 10){
        pontuacao = (tempomaxjogo - tempototaldejogo - guardatempo + pausa) * 15 - (Eletra * 3);
        cout << "tempomaxjogo - tempototaldejogo + guardatempo - pausa" << endl;
        cout << tempomaxjogo << " - " << tempototaldejogo << " + " << guardatempo << " - " << pausa << "*15" << endl;
        return(pontuacao);
    }
    if (tempomaxjogo == 20){
        pontuacao = (tempomaxjogo - tempototaldejogo - guardatempo + pausa) * 10 - (Eletra * 3);
        return(pontuacao);
    }
    else{
        pontuacao = (tempomaxjogo - tempototaldejogo - guardatempo + pausa) * 5 - (Eletra * 3);
        return(pontuacao);
    }
}
//############################################################################################################################
void jogaforca::mostrascore(jogadores *tab, int dim){                    //MOSTRA O SCORE DA BD
    cout << endl;
    cout << "SCORE " << setw(0) << " NICK " << setw(30) << " MAXSCORE " << endl;

    for (int i = 0; i < dim; i++)
    {
        cout << setw(7) << left << tab[i].score << "|" << setw(30) << left << tab[i].nickname << "|" << "  " << setw(20) << left << tab[i].highscore << "|" << endl;
    }
    cout << endl;
}
//############################################################################################################################
void settings::settingsfile(){
    ifstream settings;
    settings.open("settings.txt");
    if(!settings) {
        cout << "O ficheiro das definicoes nao foi lido." << endl;
        exit(1);
    }
    if (settings.is_open()){
        settings >> randtheme >> randtime >> tempomaxjogo >> abandona >> timed >> nick;
    }

    settings.close();
}
//############################################################################################################################
void settings::setting(int &t, int &ti){
    do {
        cin >> escolhamenu;
        if ((escolhamenu < 0) || (escolhamenu > 5))
            cout << "Introduza um numero valido." << endl << "Novo numero: ";
    } while ((escolhamenu < 0) || (escolhamenu > 5));
    if (escolhamenu == 1){
        if (t == true) {
            t = false;
        }
        else {
            t = true;
        }
        system("cls");
        atsettingsfile();
        cout << "Foi alterado o funcionamento dos temas." << endl;
    }
    if (escolhamenu == 2){
        if (ti == true) {
            ti = false;
            tempomaxjogo = 60;
        }
        else {
            ti = true;
            timerandom();
        }
        system("cls");
        atsettingsfile();
        cout << "Foi alterada a escolha de tempo random." << endl;
    }
    if (escolhamenu == 4){
        int temp1;
        cout << "Qual o tempo pretendido?" << endl << "1 para 1min30sec || 2 para 1min || 3 para 30sec   ->";
        do{
            cin >> temp1;
            if ((temp1 < 0) || (escolhamenu > 3))
                cout << "Introduza um numero valido." << endl << "Novo numero: ";
        } while ((temp1 < 0) || (temp1 > 3));
        if (temp1 == 1) tempomaxjogo = 90;
        if (temp1 == 2) tempomaxjogo = 60;
        if (temp1 == 3) tempomaxjogo = 30;
        system("cls");
        atsettingsfile();
        cout << "Foi alterado o tempo de cada jogo para " << tempomaxjogo << endl;

    }
    if (escolhamenu == 3){
        if (timed == 1) {
            timed = 0;
        }
        else {
            timed = 1;
        }
        system("cls");
        atsettingsfile();
        cout << "Foi alterado o modo de tempo do jogo diversão." << endl;
    }
    if (escolhamenu == 5){
        system("cls");
    }
}
//############################################################################################################################
void settings::settingsdisp(int t, int ti){
    system("cls");
    cout << "Executando o modo 4..." << endl;
    cout << "DEFINICOES DE JOGO" << endl << "O que pretende alterar?" << endl << endl;
    cout << "1- ESCOLHA DE TEMA RANDOM  (0 off 1 on) [" << t << "]" << endl;
    cout << "2- ESCOLHA DE TEMPO RANDOM (0 off 1 on) [" << ti << "]" << endl;
    cout << "3- MODO DIVERSAO C/ TEMPO  (0 off 1 on) [" << timed << "]" << endl;
    cout << "4- DIFICULDADE[" << tempomaxjogo << "s]" << endl;
    cout << "5- VOLTAR AO MENU INICIAL" << endl;
}
//############################################################################################################################
void settings::timerandom(){                                             //TEMPO RANDOM PARA JOGADA
    srand(time(NULL));
    int rt = rand()% 200;
    tempomaxjogo = rt;
}
//############################################################################################################################
bool compare(jogadores a, jogadores b){
    if(a.score > b.score)
        return 1;
    else
        return 0;
}
//############################################################################################################################
int main()
{
    players players;
    jogaforca jogaforca;
    settings settings;
    jogadores* array;
    jogaforca.apresentacao();
    int p;
    settings.settingsfile();
    do{
        switch (jogaforca.telaini()){

        case '1':
            system("cls");
            cout << "Executando o modo 1......" << endl;
            cout << "Foi escolhido uma tema pelo computador!" << endl;
            jogaforca.escolhetemapalavra();
            jogaforca.jogountimed();
            break;

        case '2':
            p = players.playerarray();
            array = new (nothrow) jogadores[p];
            system("cls");
            cout << "Executando o modo 2..." << endl;
            jogaforca.diversao = 0;
            players.preenchejogador();
            jogaforca.score = 0;
            players.carregatab(array);

            if (jogaforca.verificasejogoperdido() == 1){                  //VERIFICA SE HA ALGUM JOGO PARA ESTE USER PENDENTE
                cout << "Foi recuperado o progresso do jogador. Deseja recuperar? 1->SIM 2->NAO" << endl;
                do{
                    cin >> players.go;
                    if ((players.go < 1) || (players.go > 2))
                        cout << "Introduza um numero valido." << endl << "Novo numero: ";
                }while((players.go <  1) || (players.go > 2));
                if (players.go == 2){
                    jogaforca.removefiles(1,0);
                    system("cls");
                    break;
                }
                if (players.go == 1){
                    jogaforca.carregaprogressperdido();
                    jogaforca.indicenatabela = players.testanick(array, p);
                }
                jogaforca.jogo(array, p);
                break;
            }

            testa = players.testanick(array, p);
            if (testa != -1){
                if(array[testa].saved == 1){
                    cout << "Foi encontrado um ficheiro de progresso deste jogador. Continuar? 1->SIM 2->APAGAR" << endl;
                    do{
                        cin >> players.go;
                        if ((players.go < 1) || (players.go > 2))
                            cout << "Introduza um numero valido." << endl << "Novo numero: ";
                    }while((players.go <  1) || (players.go > 2));
                    if (players.go == 2){
                        players.carregatab(array);
                        array[testa].saved = 0;
                        jogaforca.registapont(array, p);
                        jogaforca.removefiles(1,1);
                        system("cls");
                        break;
                    }
                    jogaforca.carregaprogress();
                    jogaforca.jogo(array, p);
                    break;
                }
                else {
                    cout << "Utilizador ja encontrado! Dejesa continuar com o progresso do seu score? 1->SIM 2->MENU" << endl;
                    do{
                        cin >> players.go;
                        if ((players.go < 1) || (players.go > 2))
                            cout << "Introduza um numero valido." << endl << "Novo numero: ";
                    }while((players.go <  1) || (players.go > 3));
                }}
            if (players.go == 2){
                system("cls");
                jogaforca.indicenatabela = -1; //-1?
                break;
            }
            else (jogaforca.indicenatabela = players.testanick(array, p));
            if (randtime == 1) settings.timerandom();
            if (randtheme == 0){
                cout << "::Escolha de Temas::" << endl << "1 para meses // 2 para meios de transporte // 3 para instrumentos musicais" << endl;
                cout << "Escolha o tema pretendido (1 a 3): ";
                do {
                    cin >> jogaforca.tema;
                    if ((jogaforca.tema < 1) || (jogaforca.tema > 3))
                        cout << "Introduza um numero valido." << endl << "Novo numero: ";
                } while ((jogaforca.tema < 1) || (jogaforca.tema > 3));
                cout << "Tema verificado com sucesso" << endl;
                jogaforca.escolhepalavra(jogaforca.tema);
                jogaforca.jogo(array, p);
                break;
            }
            if (randtheme == 1){
                jogaforca.escolhetemapalavra();
                jogaforca.jogo(array, p);
                break;
            }
            delete[] array;

        case '3':
            p = players.playerarray();
            array = new (nothrow) jogadores[p];
            players.carregatab(array);
            system("cls");
            cout << "Executando o modo 3..." << endl;
            cout << "Sera exibida a tabela de pontuacoes!" << endl;
            players.carregatab(array);
            p = players.playerarray();
            sort(array, array+p, compare);
            jogaforca.registapont(array, p);
            jogaforca.mostrascore(array, p);
            delete[] array;
            break;

        case '4':
            system("cls");
            do{
                settings.settingsdisp(randtheme, randtime);
                settings.setting(randtheme, randtime);
            }while (settings.escolhamenu != 5);
            break;

        case '5':
            system("cls");
            cout << "SAINDO DO JOGO..." << endl;
            cout << "Ate a proxima..." << endl;
            gameon = 0;
            break;
        default:
            system("cls");
            break;
        }
    }while (gameon == 1);
    delete[] array;
    return 0;
}
//############################################################################################################################
