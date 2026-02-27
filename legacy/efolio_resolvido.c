#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RETANGULOS 100 //Número máximo de retangulos
#define LARGURA_PLANO 80 //Largura do plano
#define ALTURA_PLANO 25//Altura do plano
#define TAMANHO_COMANDO 20 //Tamanho do comando

typedef struct { //definição de estrutura com as coordenadas e dimensões do retangulo
    int x;
    int y;
    int largura;
    int altura;
} Retangulo;

Retangulo retangulos[MAX_RETANGULOS]; // Array de retangulos, usado para armazenar retangulos
int num_retangulos = 0; //Controlo do número de retângulos

void opcoes(){ //Exibir as as instruções para operar com o mundo dos retangulos
    printf("Seja Bem vindo, para operar no mundo dos retângulos insira os seguintes comandos na linha de comandos:\n\n");
    printf("create x,y+l,h - cria um retângulo em que (x,y) são as coordenadas do canto inferior esquerdo e (l,h) o comprimento e altura\n");
    printf("moveright x,y+p - desloca o retângulo situado nas coordenadas (x,y) para a direita p posições\n");
    printf("moveleftx,y+p - desloca o retângulo que contém o ponto (x,y) para a esquerda p posições\n\n");
    return; 

}

bool colide(Retangulo r1, Retangulo r2) { //função que define a colisão dos retangulos usando as coordenadas e dimensões
    if (r1.x + r1.largura <= r2.x || r2.x + r2.largura <= r1.x) return false; //Verifica se os retangulos não se sobrepoem no eixo X
    if (r1.y + r1.altura <= r2.y || r2.y + r2.altura <= r1.y) return false;//Verifica se os retangulos não se sobrepoem no eixo y
    return true;//Se os retangulos se sobrepoem em ambos os eixos retorne true, há colisão
}

void ajustar_gravidade() { //Função responsável pela queda dos retangulos (gravidade)
    bool mudou;//Definição de variavel que identifica a mundança na posição dos retangulos
    do { //Executar enquanto houver mudança
        mudou = false; // Inicializa a variável mudou como false
        for (int i = 0; i < num_retangulos; i++) { //Iterar sobre todos os retangulos
            while (retangulos[i].y > 1) { //Enquanto o retangulo não estiver no fundo do plano
                retangulos[i].y--;//Mover para baixo
                bool colidiu = false; //Variavel bolena para identificar colisão, declarada como falsa a partida
                for (int j = 0; j < num_retangulos; j++) { //Iterar sobre todos os retangulos
                    if (i != j && colide(retangulos[i], retangulos[j])) {//define se houve colisão 
                        colidiu = true; //em caso de colisão, "colidiu" é declarada verdade 
                        break;//E a uma saida do loop
                    }
                }
                if (colidiu) { //Se colidiu
                    retangulos[i].y++; //O retangulo é iterado uma vez sobre o eixo y
                    break;//Condição cumprida
                } else {
                    mudou = true;
                }
            }
        }
    } while (mudou);
}

//
double calcular_instabilidade() { //função que calcula a instabilidade do sistema 
    if (num_retangulos < 2) return 0;//Controlo de instabilidade, só há alerta de instabilidade caso haja no minimo dois retangulos no sistema
    double instabilidade_total = 0;//armazenar a soma da instabilidade de todos os retângulos instáveis. inicializada com 0 
    int num_instaveis = 0;//Armazenar numero de retangulos instaveis
    for (int i = 0; i < num_retangulos; i++) { //Iterar Retangulos
        int base_apoiada = 0; //armazenar o comprimento da base do retângulo atual que está apoiada em outros retângulos
        for (int j = 0; j < num_retangulos; j++) {
            if (i != j && retangulos[i].y == retangulos[j].y + retangulos[j].altura) {//Verifica se o retângulo no indice i está diretamente acima do retângulo de indice j
                if (retangulos[j].x <= retangulos[i].x && retangulos[i].x < retangulos[j].x + retangulos[j].largura) {
                    base_apoiada += retangulos[j].x + retangulos[j].largura - retangulos[i].x;
                } else if (retangulos[i].x <= retangulos[j].x && retangulos[j].x < retangulos[i].x + retangulos[i].largura) { 
                    base_apoiada += retangulos[i].x + retangulos[i].largura - retangulos[j].x;
                }
            }
        }
        double proporcao_apoiada = (double)base_apoiada / retangulos[i].largura; //Calcula a proporção da base do retângulo i que está apoiada em outros retângulos
        double proporcao_nao_apoiada = 1 - proporcao_apoiada;//Calcula a proporção da base do retângulo i que não está apoiada em outros retângulos
        if (proporcao_nao_apoiada > proporcao_apoiada) {//Se a proporção não apoiada for maior que a proporção apoiada
            instabilidade_total += proporcao_nao_apoiada;//Adiciona a proporção não apoiada à instabilidade total
            num_instaveis++;//Incrementa o número de retângulos instáveis
        }
    }
    if (num_instaveis == 0) return 0; //se não houver nenhum retângulo instável, retorna 0
    return instabilidade_total / num_instaveis;//eetorna a media da instabilidade dos retângulos instáveis
}

void criar_retangulo(int x, int y, int largura, int altura) {//Função responsável pela criação de retangulos no sistema
    if (num_retangulos == MAX_RETANGULOS) { //Imprimir erro caso sejam criados mais retangulos que permitidos
        printf("Erro: número máximo de retângulos atingido\n");
        return;
    }
    if (x < 1 || x + largura > LARGURA_PLANO || y < 1 || y + altura > ALTURA_PLANO) {//Imprime mensagem de erro caso retangulo esteja fora do plano
        printf("Erro: retângulo fora do plano\n");
        return;
    }
    
    //Verifica se um novo retangulo se sobrepoem ao existente e 
    for (int i = 0; i < num_retangulos; i++) {
        if (retangulos[i].x < x + largura && retangulos[i].x + retangulos[i].largura > x &&
            retangulos[i].y < y + altura && retangulos[i].y + retangulos[i].altura > y) {
                printf("Erro: retângulo sobreposto a outro retângulo\n");
                return;//retorna sem criar o retangulo
            }
    }
    
    retangulos[num_retangulos++] = (Retangulo){x, y, largura, altura};//Adiciona um novo retangulo ao array e incrementa o numero de retangulos
    ajustar_gravidade();//Aplica a gravidade
    double instabilidade = calcular_instabilidade();//calcula a instabilidade
    if (instabilidade >= 0.7 && instabilidade < 1) {//se for maior que 70%, aciona o alerta 
        printf("Alerta: grau de instabilidade do sistema é %.2f%%\n", instabilidade * 100);
    }
}

void mover_direita(int x, int y, int p) {//Função para mover retangulo para direita
    for (int i = 0; i < num_retangulos; i++) {//Itera sobre todos os retangulos
        if (retangulos[i].x <= x && x < retangulos[i].x + retangulos[i].largura && retangulos[i].y <= y && y < retangulos[i].y + retangulos[i].altura)
 {//verifica se o ponto (x,y) está dentro do retângulo atual
            if (retangulos[i].x + p + retangulos[i].largura > LARGURA_PLANO) {//verifica se o movimento faria o retângulo sair do plano
                printf("Erro: movimento impossível\n");
                return;
            }
            retangulos[i].x += p;//move o retângulo para a direita
            ajustar_gravidade();//aplicar gravidade
            double instabilidade = calcular_instabilidade();//Calculo de instabilidade e alerta caso seja superior a 70%
            if (instabilidade >= 0.7 && instabilidade < 1 ) {
                printf("Alerta: grau de instabilidade do sistema é %.2f%%\n", instabilidade * 100);
            }
            return;
        }
    }
    printf("Erro: não existe retângulo na posição (%d,%d)\n", x, y);
}

void mover_esquerda(int x, int y, int p) {//Função para mover a esquerda
    for (int i = 0; i < num_retangulos; i++) {
        if (retangulos[i].x <= x && x < retangulos[i].x + retangulos[i].largura && retangulos[i].y <= y && y < retangulos[i].y + retangulos[i].altura) {
            if (retangulos[i].x - p < 1) {//verifica se o movimento faria o retangulo sair do plano
                printf("Erro: movimento impossível\n");
                return;//retorna sem mover
            }
            retangulos[i].x -= p;//move para a esquerda
            ajustar_gravidade();
            double instabilidade = calcular_instabilidade();
            if (instabilidade >= 0.7 && instabilidade < 1 ) {
                printf("Alerta: grau de instabilidade do sistema é %.2f%%\n", instabilidade * 100);
            }
            return;
        }
    }
    printf("Erro: não existe retângulo na posição (%d,%d)\n", x, y);
}

void visualizar() {
    
    char plano[ALTURA_PLANO][LARGURA_PLANO];//matriz que representa o plano, (dimensões)
    for (int y = 0; y < ALTURA_PLANO; y++) {
        for (int x = 0; x < LARGURA_PLANO; x++) {
            plano[y][x] = ' '; //Itera todas as linhas e colunas com o espaço em branco
        }
    }
    for (int i = 0; i < num_retangulos; i++) {// Itera sobre todos os retângulos existentes
        for (int y = retangulos[i].y - 1; y < retangulos[i].y + retangulos[i].altura - 1; y++) {//Itera sobre as linhas ocupadas pelo retângulo atual
            for (int x = retangulos[i].x - 1; x < retangulos[i].x + retangulos[i].largura - 1; x++) {// Itera sobre as colunas ocupadas pelo retangulo atual
                if (x == retangulos[i].x - 1 || x == retangulos[i].x + retangulos[i].largura - 2 || y == retangulos[i].y - 1 || y == retangulos[i].y + retangulos[i].altura - 2) {
                    plano[y][x] = 'x';
                } //Marca os elementos da matriz que representam as bordas do ret como 'x'
            }
        }
    }
    for (int y = ALTURA_PLANO - 1; y >= 0; y--) {//itera sobre todas as linhas da matriz em ordem decrescente
        for (int x = 0; x < LARGURA_PLANO; x++) {//itera sobre todas as colunas da matriz em ordem crescente
            putchar(plano[y][x]);//Imprime cada elemento da matriz na tela
        }
        putchar('\n');//quebra de linha após cada linha da matriz para formatar a saída corretamente
    }
}

int main(int argc, char *argv[]) {
    opcoes();//Chama a função opcoes() para exibir as opções de comando para o usuário
    for (int i = 1; i < argc; i++) {//Itera sobre todos os argumentos passados para o programa na linha de comando
        char *comando = argv[i];//Armazena o argumento atual na variável comando
        if (strcmp(comando, "create") == 0) {//Verifica se o comando é "create"
            int x, y, largura, altura;//Declara variáveis para armazenar as coordenadas e dimensões do retângulo a ser criado
            sscanf(argv[++i], "%d,%d+%d,%d", &x, &y, &largura, &altura);//Lê as coordenadas e dimensões do retângulo a partir do próximo argumento
            criar_retangulo(x, y, largura, altura);//Chama a função criar_retangulo() para criar um novo retângulo com as coordenadas e dimensões especificadas
        } else if (strcmp(comando, "moveleft") == 0) {// Verifica se o comando é "moveleft"
            int x, y, p;//Declara variáveis para armazenar as coordenadas do retângulo a ser movido e a distância a ser movida
            sscanf(argv[++i], "%d,%d+%d", &x, &y, &p); //Lê as coordenadas e a distância a partir do próximo argumento
            mover_esquerda(x, y, p);//Chama a função mover_esquerda() para mover o retângulo especificado para a direita
        } else if (strcmp(comando, "moveright") == 0) {// Declara variáveis para armazenar as coordenadas do retângulo a ser movido e a distância a ser movida
            int x, y, p;// Declara variáveis para armazenar as coordenadas do retângulo a ser movido e a distância a ser movida
            sscanf(argv[++i], "%d,%d+%d", &x,&y,&p);//Lê as coordenadas e a distância a partir do próximo argumento
            mover_direita(x,y,p);//Chama a função mover_direita() para mover o retângulo especificado para a direita
        }
    }
    visualizar();// Chama a função visualizar() para exibir o mundo dos retângulos na tela
    return 0;// Retorna 0 para indicar que o programa foi executado com sucesso
}