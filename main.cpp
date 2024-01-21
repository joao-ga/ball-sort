#include <iostream>
#include <time.h>
#include <Windows.h>

#define TAM 6

typedef int stack_element;

#include "pilha.h"

typedef struct {
    int numero_elementos;
    Stack pilha;
} Tubo;

using namespace std;

void mudacor(int cor){
    /*
    1-azul
    2-verde
    3-verde-agua
    4-vermelho
    5-roxo
    */

    HANDLE Saida;
    Saida = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Saida, cor);
}

void printa_cor(int I){
    int cor[5] = {1,2,3,4,5};
    mudacor(cor[I-1]);
    cout<< '0';
    mudacor(15);
}

void iniciar_vazias(Tubo T[]){
    for(int i=0; i< TAM; i++) {
        initStack(T[i].pilha);
        T[i].numero_elementos = 0;
    }
}

void distribuir(Tubo T[]) {
    int num;
    int freq[5] = {0};
    srand(time(0));

    // Tubo vazio
    T[0].numero_elementos = 0;

    // Preencher os outros tubos
    for (int i = 1; i < TAM; i++) {
        for (int j = 0; j < 5; j++) {
            do {
                num = rand() % 5 + 1;  // N�meros de 1 a 5
            } while (freq[num - 1] >= 5);

            freq[num - 1]++;
            push(T[i].pilha, num);
            T[i].numero_elementos++;
        }
    }
}

void mostrar(Tubo T[]) {
    stack_element v;
    Tubo aux[TAM];
    iniciar_vazias(aux);

    cout<<endl;
    // For dos elementos (come�ando de cima para baixo)
    for (int linha = 5; linha >= 1; linha--) {
        // For dos tubos (come�ando da esquerda para a direita)
        for (int a = 0; a < TAM; a++) {
            if (T[a].numero_elementos < linha) {
                cout << " |   | ";
            } else {
                if (!isEmpty(T[a].pilha)) {
                    v = pop(T[a].pilha);
                    cout << " | "; printa_cor(v); cout << " | ";
                    push(aux[a].pilha, v);
                }
            }
        }
        cout << endl;
    }
        cout << " -----  -----  -----  -----  -----  ----- " << endl;
        cout << " --1--  --2--  --3--  --4--  --5--  --6-- " << endl << endl;

    // Desempilha elementos de aux e empilha de volta em T
    for (int j = 0; j < TAM; j++) {
        while (!isEmpty(aux[j].pilha)) {
            v = pop(aux[j].pilha);
            push(T[j].pilha, v);
        }
    }
}

int validar(Tubo T[], int o, int d){
    int tamanho_origem = T[o-1].numero_elementos;
    int tamanho_destino = T[d-1].numero_elementos;

    if(tamanho_origem > 0 && tamanho_destino < TAM-1){
        return 1;
    } else
        return 0;
}

int validar_fim(Tubo T[]) {
    int flag = 0;
    Tubo aux[TAM];

    iniciar_vazias(aux);

    for (int i = 0; i < TAM; i++) {
        if (T[i].numero_elementos == (TAM - 1)) {
            stack_element primeiro_elemento;
            primeiro_elemento = pop(T[i].pilha);

            while (!isEmpty(T[i].pilha)) {
                stack_element comparador = pop(T[i].pilha);
                push(aux[i].pilha, comparador);

                if (comparador != primeiro_elemento) {
                    flag = 1; // Corrigido: usar '=' em vez de '=='
                }
            }

            while (!isEmpty(aux[i].pilha)) {
                stack_element v = pop(aux[i].pilha);
                push(T[i].pilha, v);
            }

            push(T[i].pilha, primeiro_elemento);

            if (flag == 1) {
                return 1;
            }
        } else {
            return 1;
        }
    }

    return 0;
}

int jogada(Tubo T[]) {
    int origem, destino, validacao, validacao_fim;
    stack_element v;

    do{
        do {
        // Solicita a entrada do usu�rio
        cout << "Digite -1 para parar" << endl;
        cout << "De qual tubo deseja retirar? " << endl;
        cin >> origem;
        cout << "Para qual tubo deseja colocar? " << endl;
        cin >> destino;
        // Verifica se o usu�rio deseja parar
        if (origem == -1 || destino == -1) {
            return 0;
        }
        // Garante que a entrada do usu�rio esteja dentro dos limites
    } while (origem < 1 || origem > TAM || destino < 1 || destino > TAM);


    // Valida a jogada
    validacao = validar(T, origem, destino);
    }while(validacao != 1);


    // Realiza a jogada se for v�lida
    if (validacao == 1) {
        if(!isEmpty(T[origem-1].pilha)){
        v = pop(T[origem - 1].pilha);
        push(T[destino - 1].pilha, v);


        T[origem - 1].numero_elementos--;
        T[destino - 1].numero_elementos++;

        // Verifica se o jogo chegou ao fim
        validacao_fim = validar_fim(T);
        }
    }

    cout << validacao_fim;
    return validacao_fim;
}

int main()
{
    Tubo T[TAM];
    int repetir = 1, retorno;
    do{
        iniciar_vazias(T);
        distribuir(T);
        mostrar(T);
        do {
            retorno = jogada(T);
            mostrar(T);
            if(retorno == 0) break;
        } while(retorno);
        mostrar(T);
        cout<< "\nPARABENS!";
        cout << "\nDeseja jogar novamente? 1(sim) ou 0 (nao):  "; cin >> repetir;
    }while(repetir);
    cout << "FIM DO JOGO\n";

    return 0;
}
