#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RETANGULOS 100
#define LARGURA_PLANO 80
#define ALTURA_PLANO 25
#define TAMANHO_COMANDO 20

double calcular_instabilidade();

typedef struct {
    int x;
    int y;
    int largura;
    int altura;
} Retangulo;

Retangulo retangulos[MAX_RETANGULOS];
int num_retangulos = 0;

bool colide(Retangulo r1, Retangulo r2) {
    if (r1.x + r1.largura <= r2.x || r2.x + r2.largura <= r1.x)
        return false;
    if (r1.y + r1.altura <= r2.y || r2.y + r2.altura <= r1.y)
        return false;
    return true;
}

void ajustar_gravidade() {
    bool mudou;
    do {
        mudou = false;
        for (int i = 0; i < num_retangulos; i++) {
            while (retangulos[i].y > 1) {
                retangulos[i].y--;
                bool colidiu = false;
                for (int j = 0; j < num_retangulos; j++) {
                    if (i != j && colide(retangulos[i], retangulos[j])) {
                        colidiu = true;
                        break;
                    }
                }
                if (colidiu) {
                    retangulos[i].y++;
                    break;
                } else {
                    mudou = true;
                }
            }
        }
    } while (mudou);
}

void criar_retangulo(int x, int y, int largura, int altura) {
    // código existente
    ajustar_gravidade();
    double instabilidade = calcular_instabilidade();
    if (instabilidade >= 0.7) {
        printf("Alerta: grau de instabilidade do sistema é %.2f%%\n", instabilidade * 100);
    }
}


void mover_direita(int x, int y, int p) {
    for (int i = 0; i < num_retangulos; i++) {
        if (retangulos[i].x <= x && x < retangulos[i].x + retangulos[i].largura &&
            retangulos[i].y <= y && y < retangulos[i].y + retangulos[i].altura) {
            if (retangulos[i].x + p + retangulos[i].largura > LARGURA_PLANO) {
                printf("Erro: movimento impossível\n");
                return;
            }
            retangulos[i].x += p;
            ajustar_gravidade();
            calcular_instabilidade();
            return;
        }
    }
    printf("Erro: não existe retângulo na posição (%d,%d)\n", x, y);
}

void mover_esquerda(int x, int y, int p) {
    for (int i = 0; i < num_retangulos; i++) {
        if (retangulos[i].x <= x && x < retangulos[i].x + retangulos[i].largura &&
            retangulos[i].y <= y && y < retangulos[i].y + retangulos[i].altura) {
            if (retangulos[i].x - p < 1) {
                printf("Erro: movimento impossível\n");
                return;
            }
            retangulos[i].x -= p;
            ajustar_gravidade();
            calcular_instabilidade();
            return;
        }
    }
    printf("Erro: não existe retângulo na posição (%d,%d)\n", x, y);
}

void visualizar() {
    char plano[ALTURA_PLANO][LARGURA_PLANO];
    for (int y = 0; y < ALTURA_PLANO; y++) {
        for (int x = 0; x < LARGURA_PLANO; x++) {
            plano[y][x] = ' ';
        }
    }
    for (int i = 0; i < num_retangulos; i++) {
        for (int y = retangulos[i].y - 1; y < retangulos[i].y + retangulos[i].altura - 1; y++) {
            for (int x = retangulos[i].x - 1; x < retangulos[i].x + retangulos[i].largura - 1; x++) {
                if (x == retangulos[i].x - 1 || x == retangulos[i].x + retangulos[i].largura - 2 ||
                    y == retangulos[i].y - 1 || y == retangulos[i].y + retangulos[i].altura - 2) {
                    plano[y][x] = 'x';
                }
            }
        }
    }
    for (int y = ALTURA_PLANO - 1; y >= 0; y--) {
        for (int x = 0; x < LARGURA_PLANO; x++) {
            putchar(plano[y][x]);
        }
        putchar('\n');
    }
   calcular_instabilidade();
}

double calcular_instabilidade() {
    double instabilidade_total = 0;
    int num_instaveis = 0;
    for (int i = 0; i < num_retangulos; i++) {
        int base_apoiada = 0;
        for (int j = 0; j < num_retangulos; j++) {
            if (i != j && retangulos[i].y == retangulos[j].y + retangulos[j].altura) {
                if (retangulos[j].x <= retangulos[i].x && retangulos[i].x < retangulos[j].x + retangulos[j].largura) {
                    base_apoiada += retangulos[j].x + retangulos[j].largura - retangulos[i].x;
                } else if (retangulos[i].x <= retangulos[j].x && retangulos[j].x < retangulos[i].x + retangulos[i].largura) {
                    base_apoiada += retangulos[i].x + retangulos[i].largura - retangulos[j].x;
                }
            }
        }
        double proporcao_apoiada = (double)base_apoiada / retangulos[i].largura;
        double proporcao_nao_apoiada = 1 - proporcao_apoiada;
        if (proporcao_nao_apoiada > proporcao_apoiada) {
            instabilidade_total += proporcao_nao_apoiada;
            num_instaveis++;
        }
    }
    if (num_instaveis == 0) return 0;
    return instabilidade_total / num_instaveis;
}


int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *comando = argv[i];
        if (strcmp(comando, "create") == 0) {
            int x, y, largura, altura;
            sscanf(argv[++i], "%d,%d+%d,%d", &x, &y, &largura, &altura);
            criar_retangulo(x, y, largura, altura);
        } else if (strcmp(comando, "moveleft") == 0) {
            int x, y, p;
            sscanf(argv[++i], "%d,%d+%d", &x, &y, &p);
            mover_esquerda(x, y, p);
        } else if (strcmp(comando, "moveright") == 0) {
            int x, y, p;
            sscanf(argv[++i], "%d,%d+%d", &x, &y, &p);
            mover_direita(x, y, p);
        }
        
        float grau_instabilidade = calcular_instabilidade();
        if (grau_instabilidade >= 0.7) {
            printf("Alerta: o grau de instabilidade é %.2f%%\n", grau_instabilidade * 100);
        }
        
    }
    
    visualizar();
    float grau_instabilidade = calcular_instabilidade();
    if (grau_instabilidade >= 0.7) {
        printf("Alerta: o grau de instabilidade é %.2f%%\n", grau_instabilidade * 100);
    }
    
    return 0;
}
