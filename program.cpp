#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

string concatena_sequencia(string txt, int index, int length); //implementa array circular e cria sequencias
string add_ruido(string in, int percent); //adiciona ruido ? sequencia
int Levenshtein(string palavra1, string palavra2); //numero de deslocamentos necessarios para tornar as strings iguais
bool verifica_sequencia(vector<string> sequencias,string verificar,int index, int* found, bool inicio,int max_erro); //verifica se a sequencia ? concatenar est? dentro da margem de erro


int main()
{
    string txt="GCTACGTAGCTAGTCGATCGATGCTAGCTAGTCGGTCAGCATGCAGTCAACACGATCGGACTCATCAGAGCATC";
    int tam_cadeia=txt.length(); //tamanho da string de entrada
    int tam_sequencia=20; //tamanho de cada sequencia do banco de sequencias
    int tam_sequenciamento=5; //tamanho da substring de cada sequencia para ser comparada
    int max_sequencias=10000; //maximo de sequencias
    int max_erro = 2; //maximo numero de deslocamentos permitido
    int percent_ruido = 0; //porcentagem de ruido em cada sequencia

    srand (time(NULL));
    int iSecret;

    vector<string> banco_sequencias;
    int cont=1;

    cout<<"SEQUENCIAS"<<endl;
    do
    {
        iSecret = rand()%(tam_cadeia);
        string sequencia = concatena_sequencia(txt,iSecret,tam_sequencia);
        sequencia = add_ruido(sequencia,percent_ruido);
        banco_sequencias.push_back(sequencia);
        cout<<sequencia<<endl;
    } while(cont++<max_sequencias);

    int recontagem_anterior=banco_sequencias.size();
    int recontagem_atual=0;

    //VERIFICACAO DE COINCIDENCIA
    while(recontagem_atual!=recontagem_anterior)
    {
        recontagem_anterior=recontagem_atual;
        recontagem_atual=0;
        for(int i=0;i<banco_sequencias.size();i++)
        {
            if(banco_sequencias[i]!=""){
            string inicio = banco_sequencias[i].substr(0,tam_sequenciamento);
            string fim = banco_sequencias[i].substr(tam_sequencia-tam_sequenciamento,tam_sequenciamento);
            int found=0;
            if(verifica_sequencia(banco_sequencias,inicio,i,&found,true,max_erro))
            {
                banco_sequencias[found]=banco_sequencias[found].substr(0,tam_sequencia-tam_sequenciamento);
                banco_sequencias[found]+=banco_sequencias[i];
                banco_sequencias[i]="";
            }
            if(verifica_sequencia(banco_sequencias,fim,i,&found,false,max_erro))
            {
                banco_sequencias[i]=banco_sequencias[i].substr(0,tam_sequencia-tam_sequenciamento);
                banco_sequencias[i]+=banco_sequencias[found];
                banco_sequencias[found]="";
            }
        }

        for(int i=0;i<banco_sequencias.size();i++)
          if(banco_sequencias[i]!="") recontagem_atual++;
        }
    }  
    
    cout<<"SAIDAS"<<endl;
    int max_value=0,max_index=0;
    int peso=50000;
    for(int i=0;i<banco_sequencias.size();i++)
    {
        if(banco_sequencias[i]!="") cout<<banco_sequencias[i]<<endl;
        if(Levenshtein(banco_sequencias[i],txt)<peso) {peso=Levenshtein(banco_sequencias[i],txt); max_index=i;}
    }
    cout<<endl<<"ENTRA: "<<txt<<endl;
    cout<<"SAIDA: "<<banco_sequencias[max_index];
    return 0;
}

string concatena_sequencia(string txt, int index, int length)
{
    string out;
    for(int i=0;i<length;i++)
    {
        out+=txt[(index+i)%txt.size()];
    }
    return out;
}

string add_ruido(string in, int percent)
{
    string out = in;
    int maximo = (out.size())*percent*0.01; //porcentagem de ruido convertido em inteiro
    char banco_ruido[4] = {'A','T','G','C'};
    int iSecret;
    int ruido = rand()%(4); //letra que sera adicionada á sequencia como ruido

    for(int i=0;i<maximo;i++)
    {
        iSecret = rand()%(out.size()); //posição á se trocar o caractere pelo ruido
        out[iSecret]=banco_ruido[ruido];
    }
    return out;
}

int Levenshtein(string palavra1, string palavra2)
{
    int tam1 = palavra1.size();
    int tam2 = palavra2.size();
    int verif[tam1 + 1][tam2 + 1];

    if (tam1 == 0)
        return tam2;

    if (tam2 == 0)
        return tam1;

    int i = 0;
    while(i <= tam1)
        verif[i][0] = i++;

    int j = 0;
    while(j <= tam2)
        verif[0][j] = j++;

    for (int i = 1; i <= tam1; i++)
    {
        for (int j = 1; j <= tam2; j++)
        {
            int custo = (palavra2[j - 1] == palavra1[i - 1]) ? 0 : 1;

            verif[i][j] = min(
                min(verif[i - 1][j] + 1, verif[i][j - 1] + 1),
                verif[i - 1][j - 1] + custo);
        }
    }

    return verif[tam1][tam2];
 }

 bool verifica_sequencia(vector<string> sequencias,string verificar,int index, int* found, bool inicio,int max_erro)
 {
     for(int i=0;i<sequencias.size();i++)
     {
        if(i==index) continue;
        else if(sequencias[i].length()!=0)
        {
            if((Levenshtein(sequencias[i].substr(0,verificar.length()),verificar)<max_erro) && !inicio)
            {
                *found=i;
                return true;
            }
            else if((Levenshtein(sequencias[i].substr(sequencias[i].length()-verificar.length(),verificar.length()),verificar)<max_erro) && inicio)
            {
                *found=i;
                return true;
            }
        }
     }
 }
