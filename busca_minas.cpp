#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX = 10;
const int MINA = -1;

int filas, columnas;
int tablero[MAX][MAX];
bool visible[MAX][MAX];

// Direcciones para buscar celdas adyacentes
int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

// Función para limpiar la pantalla
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Colocar minas aleatoriamente
void generarMinas() {
    int minas = columnas + 1;
    int colocadas = 0;
    while (colocadas < minas) {
        int f = rand() % filas;
        int c = rand() % columnas;
        if (tablero[f][c] != MINA) {
            tablero[f][c] = MINA;
            colocadas++;
        }
    }
}

// Contar minas adyacentes para cada celda
void contarAdyacentes() {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] == MINA) continue;
            int contador = 0;
            for (int d = 0; d < 8; ++d) {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas && tablero[ni][nj] == MINA) {
                    contador++;
                }
            }
            tablero[i][j] = contador;
        }
    }
}

// Mostrar el tablero oculto (para el jugador)
void mostrarTablero() {
    cout << "    ";
    for (int j = 0; j < columnas; ++j) cout << j << " ";
    cout << "\n   ";
    for (int j = 0; j < columnas; ++j) cout << "--";
    cout << "\n";

    for (int i = 0; i < filas; ++i) {
        cout << i << " | ";
        for (int j = 0; j < columnas; ++j) {
            if (visible[i][j]) {
                if (tablero[i][j] == MINA) cout << "M ";
                else cout << tablero[i][j] << " ";
            } else {
                cout << "- ";
            }
        }
        cout << "\n";
    }
}

// Descubrir celdas (incluye efecto cascada)
void destapar(int i, int j) {
    if (i < 0 || i >= filas || j < 0 || j >= columnas || visible[i][j]) return;
    visible[i][j] = true;
    if (tablero[i][j] == 0) {
        for (int d = 0; d < 8; ++d)
            destapar(i + dx[d], j + dy[d]);
    }
}

// Verificar si el jugador ganó
bool verificarVictoria() {
    for (int i = 0; i < filas; ++i)
        for (int j = 0; j < columnas; ++j)
            if (tablero[i][j] != MINA && !visible[i][j])
                return false;
    return true;
}

// Función principal
int main() {
    srand(time(NULL));

    cout << "Ingrese las dimensiones del tablero (filas columnas): ";
    cin >> filas >> columnas;

    // Inicializar
    for (int i = 0; i < filas; ++i)
        for (int j = 0; j < columnas; ++j) {
            tablero[i][j] = 0;
            visible[i][j] = false;
        }

    generarMinas();
    contarAdyacentes();

    // Mostrar el tablero real (opcional)
    cout << "\nTablero generado (con minas):\n";
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] == MINA) cout << "M ";
            else cout << tablero[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\nPresiona Enter para comenzar...";
    cin.ignore();
    cin.get();
    limpiarPantalla();

    // Loop principal
    while (true) {
        mostrarTablero();
        int f, c;
        cout << "\nIngrese fila y columna: ";
        cin >> f >> c;

        if (tablero[f][c] == MINA) {
            visible[f][c] = true;
            mostrarTablero();
            cout << "\n💥 GAME OVER: pisaste una mina.\n";
            break;
        }

        destapar(f, c);

        if (verificarVictoria()) {
            mostrarTablero();
            cout << "\n🎉 ¡Ganaste! Descubriste todas las celdas sin minas.\n";
            break;
        }
    }

    return 0;
}
