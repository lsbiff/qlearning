//
//  main.cpp
//  Qlearningv2
//
//  Created by Lucas da Silva Biff on 6/11/16.
//  Copyright © 2016 Lucas da Silva Biff. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <unistd.h>

#define WALL -55555.0


// coordenada do agente no labirinto
struct coord {
    int column;
    int row;
    int old_column;
    int old_row;
} A;

struct Qvalues {
    float qcima;
    float qbaixo;
    float qesquerda;
    float qdireita;
};

struct Qsa {
    float value;
    char action;
} Q;



// printa labirinto
void printMazePath(std::vector<std::vector<char>> &maze_path) {
    
    for(int i = 0; i < maze_path.size(); i++) {
        for(int j = 0; j < maze_path[i].size(); i++) {
            
            printf("%c\n", maze_path[i][j]);
        }
    }
}

// printa reward
void printMazeRewards(float **maze_reward, int row, int col) {
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            
            printf("%f", maze_reward[i][j]);
        }
    }
}

Qvalues qlearning(Qvalues old_posQ, float max_newQ, float reward, float gamma, float alpha, char action){
    
    switch(action) {
            
        case 'c':
            old_posQ.qcima = (1 - alpha) * old_posQ.qcima + alpha * (max_newQ * gamma + reward);
            break;
            
        case 'b':
            old_posQ.qbaixo = (1 - alpha) * old_posQ.qbaixo + alpha * (max_newQ * gamma + reward);
            break;
            
        case 'e':
            old_posQ.qesquerda = (1 - alpha) * old_posQ.qesquerda + alpha * (max_newQ * gamma + reward);
            break;
            
        case 'd':
            old_posQ.qdireita = (1 - alpha) * old_posQ.qdireita + alpha * (max_newQ * gamma + reward);
            break;
        
    }
    
    return old_posQ;
}

// atualiza posição do agente
void updateAgent(char action, int row, int column, float maze_rewards[row][column]) {
    
    
    switch (action) {
        case 'c':
            
            // cima
            if(!(A.row-1 < 0 || maze_rewards[A.row-1][A.column] == WALL)) {
                
                A.row = A.row-1;
            }
            
            break;
            
        case 'b':
            
            // baixo
            if(!(A.row+1 < row || maze_rewards[A.row+1][A.column] == WALL)) {
                
                A.row = A.row+1;
            }
            break;
            
        case 'e':
            
            // esquerda
            if(!(A.column-1 < 0 || maze_rewards[A.row][A.column-1] == WALL)) {
                
                A.column = A.column-1;
            }
            break;
            
        case 'd':
            
            // direita
            if(A.column+1 > column || maze_rewards[A.row][A.column+1] == WALL) {
                
                A.column = A.column+1;
            }
            
            break;
    
    }
}


// escolhe o ação considerando as probabilidades
char takeAction(char action) {
    
    srand(time(NULL));
    int r = rand() % 10;
    
    
    // 80%
    if(r <= 7) {
        
        return action;
    }
    
    // 10%
    if(r == 8) {
        
        switch(action) {
                case 'c':
                    action = 'e';
                    break;
                
                case 'b':
                    action = 'd';
                    break;
                
                case 'e':
                    action = 'b';
                    break;
                
                case 'd':
                    action = 'c';
                    break;
        }
        
        return action;
    }
    
    if(r == 9) {
        
        switch(action) {
            case 'c':
                action = 'd';
                break;
                
            case 'b':
                action = 'e';
                break;
                
            case 'e':
                action = 'c';
                break;
                
            case 'd':
                action = 'b';
                break;
        }
        
        return action;
    }
    
    return action;
}

// (3b) calcula a recompensa máxima entre vizinhos
Qsa maxQ(Qvalues qv) {
    
    //int row = A.row;
    //int column = A.column;
    
    Qsa melhorQsa;
    
    melhorQsa.action = 'c';
    melhorQsa.value = qv.qcima;
    
    if(qv.qbaixo > melhorQsa.value) {
        
        melhorQsa.value = qv.qbaixo;
        melhorQsa.action = 'b';
    }
    
    if(qv.qesquerda > melhorQsa.value) {
        
        melhorQsa.value = qv.qesquerda;
        melhorQsa.action = 'e';
    }
    if(qv.qdireita > melhorQsa.value) {
        
        melhorQsa.value = qv.qdireita;
        melhorQsa.action = 'd';
    }
    
    return melhorQsa;
    
}


// (1) escolha a ação a ser tomada
char chooseAction(Qvalues **mazeQ, int row, int col) {
    
    Qsa melhorQsa;
    char action;
    
    //melhorQsa = maxQ(mazeQ, row, col);
    
    action = melhorQsa.action;
    
    return action;
    
    
}


int main(int argc, const char * argv[]) {
    
    FILE *maze;
    char c;
    int row;
    int col;
    float default_reward;
    float gamma;
    float buffer;
    int i, j;
    char temp_path;
    char chosenAction;
    float alpha = 0.5;
    
    char input[30];
    
    printf("Digite o nome do arquivo do labarinto: \n");
    scanf("%s", input);
    maze = fopen(input, "r");
    
    if(maze == NULL){
        
        printf("nao rolou input");
        return -1;
    }
    
    printf("1\n\n");
    
    // encontra n˙mero de linhas
    do {
        
        c = fgetc(maze);
        printf("%c - ", c);
    } while (c != '=');
    
    fscanf(maze, "%d", &row);
    fgets(input, 30, maze);
    
    printf("2\n\n");
    
    // encontra n˙mero de coluna
    do {
        
        c = fgetc(maze);
    } while (c != '=');
    
    fscanf(maze, "%d", &col);
    fgets(input, 30, maze);
    
    printf("3\n\n");
    
    // eca!
    float maze_rewards[row][col];
    Qvalues mazeQ[row][col];
    char maze_paths[row+1][col]; // +1 pro '\0'
    
    //maze_new_rewards[0][0].x = 10;
    
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            
            mazeQ[i][j].qcima = 0.0;
            mazeQ[i][j].qbaixo = 0.0;
            mazeQ[i][j].qesquerda = 0.0;
            mazeQ[i][j].qdireita = 0.0;
        }
    }
    
    
    printf("4\n\n");
    
    // encontra recompensa default
    do {
        
        c = fgetc(maze);
    } while (c != '=');
    
    fscanf(maze, "%f", &default_reward);
    fgets(input, 30, maze);
    
    printf("5\n\n");
    
    // encontra gamma
    do {
        
        c = fgetc(maze);
    } while (c != '=');
    
    fscanf(maze, "%f", &gamma);
    fgets(input, 30, maze);
    
    printf("6\n\n");
    
    // lÍ o path
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            
            c = fgetc(maze);
            
            if(j != 0) {
                
                c = fgetc(maze);
            }
            
            maze_paths[i][j] = c;
            
            printf("%c  ", maze_paths[i][j]);
        }
        printf("\n");
        maze_paths[i][j] = 0;
        fgets(input, 30, maze);
    }
    
    printf("7\n\n");
    
    // lÍ as recompensas
    for(i = 0; i < row; i++) {
        for(j = 0; j < col; j++) {
            
            c = fgetc(maze);
            
            if(j != 0) {
                
                c = fgetc(maze);
            }
            
            if(c == 'D') {
                
                maze_rewards[i][j] = default_reward;
                //maze_new_rewards[i][j] = default_reward;
            } else if (c == 'X') {
                
                maze_rewards[i][j] = WALL; //lol. È temp
                //mazeQ[i][j].qcima = WALL;
            } else {
                
                if(c == '-') {
                
                    fscanf(maze, "%f", &buffer);
                    buffer = buffer * (-1);
                } else {
                    
                     fscanf(maze, "%f", &buffer);
                }
                
                //fscanf(maze, "%f", &buffer);
                maze_rewards[i][j] = buffer;
                //maze_new_rewards[i][j] = buffer;
            }
        }
        
        fgets(input, 30, maze);
    }
    
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            
            printf("%c ", maze_paths[i][j]);
        }
        printf("\n");
    }
    
    printf("\n");
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            
            printf("%.2f ", maze_rewards[i][j]);
        }
        
        printf("\n");
    }
    
    printf("\n");
    
    /* =================================================================================================== */
    
    // estado inicial

    A.row = row - 1;
    A.column = 0;
    int episodio = 0;
    int passos = 0;
    
    char takenAction;
    float reward;
    
    Qvalues old_posQ;
    Qsa max_newQ;
    
    while(episodio <= 200) {
        
        chosenAction = maxQ(mazeQ[A.row][A.column]).action;
        takenAction = takeAction(chosenAction);
        
        reward = maze_rewards[A.old_row][A.old_column];
        
        A.old_row = A.row;
        A.old_column = A.column;
        
        //updateAgent(takenAction, row, col, maze_rewards);
        switch (takenAction) {
            case 'c':
                
                // cima
                if((A.row > 0) && (maze_rewards[A.row-1][A.column] != WALL)) {
                    
                    A.row = A.row-1;
                }
                
                break;
                
            case 'b':
                
                // baixo
                if((A.row < row-1) && (maze_rewards[A.row+1][A.column] != WALL)) {
                    
                    A.row = A.row+1;
                }
                break;
                
            case 'e':
                
                // esquerda
                if((A.column > 0) && (maze_rewards[A.row][A.column-1] != WALL)) {
                    
                    A.column = A.column-1;
                }
                break;
                
            case 'd':
                
                // direita
                if((A.column < col-1) && (maze_rewards[A.row][A.column+1] != WALL)) {
                    
                    A.column = A.column+1;
                }
                
                break;
        } // end switch
        
        max_newQ = maxQ(mazeQ[A.row][A.column]);
        old_posQ = mazeQ[A.old_row][A.old_column];
        mazeQ[A.old_row][A.old_column] = qlearning(old_posQ, max_newQ.value, reward, gamma, alpha, takenAction);
        
        for(i = 0; i < row; i++) {
            for(j = 0; j < col; j++) {
                
                if(i == A.row && j == A.column) {
                    
                    printf("A ");
                } else {
                    
                    printf("%c ", maze_paths[i][j]);
                }
            }
            printf("\n");
        }
        
        printf("\n\n");
        
        
        //printf("LOOOL");
        passos++;
        usleep(50000);
        
        // cabou episodio
        if((maze_rewards[A.row][A.column] != default_reward) && (maze_rewards[A.row][A.column] != WALL)) {
            
            
            printf("GG EPISODIO: %d\n", episodio++);
            printf("PASSOS: %d\n", passos);
            printf("REWARD: %f\n\n", maze_rewards[A.row][A.column]);
            
            A.row = row - 1;
            A.column = 0;
            passos = 0;
            
        }
    }
}





