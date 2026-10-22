#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

// Estructura que representa un paquete de red
struct Paquete
{
    int id;
    int tamanoKB;
    long tiempoLlegada;
};

class RouterSimulacion
{
private:
    std::queue<Paquete> colaMemoria; // Bufer dinamico de red
    int capacidadMaxKB;
    int tamanoActualKB;
    int paquetesPerdidos;

public:
    RouterSimulacion(int capacidadKB) : capacidadMaxKB(capacidadKB), tamanoActualKB(0), paquetesPerdidos(0) {}

    // Intentar asignar recursos al paquete entrante
    bool recibirPaquete(Paquete p)
    {
        if (tamanoActualKB + p.tamanoKB <= capacidadMaxKB)
        {
            colaMemoria.push(p);
            tamanoActualKB += p.tamanoKB;
            std::cout << "[ACEPTADO] Paquete " << p.id << " (" << p.tamanoKB << " KB) en cola. "
                      << "Uso bufer: " << tamanoActualKB << "/" << capacidadMaxKB << " KB\n";
            return true;
        }
        else
        {
            paquetesPerdidos++;
            std::cout << "[PROCESO RECHAZADO / DROP] Paquete " << p.id << " (" << p.tamanoKB
                      << " KB) descartado por falta de ancho de banda/memoria.\n";
            return false;
        }
    }

    // Procesar/Transmitir paquetes de la cola
    void procesarTrafico(int anchoBandaKB)
    {
        std::cout << "\n--- Procesando canal de salida (Ancho de Banda: " << anchoBandaKB << " KB) ---\n";
        int liberado = 0;

        while (!colaMemoria.empty() && liberado < anchoBandaKB)
        {
            Paquete p = colaMemoria.front();
            if (liberado + p.tamanoKB <= anchoBandaKB)
            {
                tamanoActualKB -= p.tamanoKB;
                liberado += p.tamanoKB;
                std::cout << "[TRANSMITIDO] Paquete ID: " << p.id << "\n";
                colaMemoria.pop();
            }
            else
            {
                break; // El paquete no cabe en esta ventana de transmision
            }
        }
    }

    void reporte()
    {
        std::cout << "\n=== RESUMEN DE SIMULACIÓN ==="
                  << "\nPaquetes actualmente en bufer: " << colaMemoria.size()
                  << "\nOcupacion del bufer: " << tamanoActualKB << " KB"
                  << "\nPaquetes desbordados/perdidos: " << paquetesPerdidos << "\n";
    }
};

int main()
{
    srand(time(0));
    RouterSimulacion router(500); // Bufer maximo de 500 KB

    // Simular llegada de trafico
    for (int i = 1; i <= 8; i++)
    {
        Paquete p = {i, rand() % 150 + 50, time(0)}; // Paquetes entre 50 y 200 KB
        router.recibirPaquete(p);
    }

    // Procesar flujo de salida
    router.procesarTrafico(300); // Ancho de banda asignado de 300 KB

    // Reporte final de uso de recursos
    router.reporte();

    return 0;
}