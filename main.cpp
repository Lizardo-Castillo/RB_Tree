#include <iostream>
using namespace std;

// Definición de los colores para los nodos del árbol rojo-negro
enum Color { ROJO, NEGRO };

// Estructura Nodo para el árbol rojo-negro
struct Nodo {
    int dato;          // Valor almacenado en el nodo
    Color color;       // Color del nodo (ROJO o NEGRO)
    Nodo* izq;         // Puntero al hijo izquierdo
    Nodo* der;         // Puntero al hijo derecho
    Nodo* padre;       // Puntero al nodo padre
    
    // Constructor para crear un nuevo nodo con un valor, por defecto ROJO y sin hijos ni padre
    Nodo(int val) : dato(val), color(ROJO), izq(nullptr), der(nullptr), padre(nullptr) {}
};

// Clase para manejar el árbol rojo-negro
class ArbolRN {
private:
    Nodo* raiz;  // Raíz del árbol
    
    // Rotación a la izquierda en el nodo x
    void rotarIzq(Nodo* x) {
        Nodo* y = x->der;          // y es el hijo derecho de x
        x->der = y->izq;           // El hijo izquierdo de y pasa a ser el hijo derecho de x
        if (y->izq) y->izq->padre = x; // Actualizamos padre si existe
        y->padre = x->padre;       // y toma el lugar de x en el árbol
        if (!x->padre) raiz = y;   // Si x era la raíz, ahora y es la nueva raíz
        else if (x == x->padre->izq) x->padre->izq = y;
        else x->padre->der = y;
        y->izq = x;                // x se vuelve hijo izquierdo de y
        x->padre = y;              // Actualizamos padre de x
    }
    
    // Rotación a la derecha en el nodo x
    void rotarDer(Nodo* x) {
        Nodo* y = x->izq;          // y es el hijo izquierdo de x
        x->izq = y->der;           // El hijo derecho de y pasa a ser el hijo izquierdo de x
        if (y->der) y->der->padre = x; // Actualizamos padre si existe
        y->padre = x->padre;       // y toma el lugar de x en el árbol
        if (!x->padre) raiz = y;   // Si x era la raíz, ahora y es la nueva raíz
        else if (x == x->padre->der) x->padre->der = y;
        else x->padre->izq = y;
        y->der = x;                // x se vuelve hijo derecho de y
        x->padre = y;              // Actualizamos padre de x
    }
    
    // Método para corregir las propiedades del árbol rojo-negro después de una inserción
    void corregirInsercion(Nodo* z) {
        // Mientras z no sea la raíz y el padre de z sea rojo (violación de regla)
        while (z != raiz && z->padre->color == ROJO) {
            // Si el padre de z es el hijo izquierdo del abuelo
            if (z->padre == z->padre->padre->izq) {
                Nodo* tio = z->padre->padre->der; // El "tío" es el hijo derecho del abuelo
                // Caso 1: si el tío es rojo, recoloreamos y subimos el problema hacia arriba
                if (tio && tio->color == ROJO) {
                    z->padre->color = NEGRO;
                    tio->color = NEGRO;
                    z->padre->padre->color = ROJO;
                    z = z->padre->padre;
                } else {
                    // Caso 2: z es hijo derecho, se rota a la izquierda para formar caso 3
                    if (z == z->padre->der) {
                        z = z->padre;
                        rotarIzq(z);
                    }
                    // Caso 3: rotación derecha y recoloreado para arreglar el árbol
                    z->padre->color = NEGRO;
                    z->padre->padre->color = ROJO;
                    rotarDer(z->padre->padre);
                }
            } else { // Caso simétrico cuando el padre es el hijo derecho del abuelo
                Nodo* tio = z->padre->padre->izq;
                if (tio && tio->color == ROJO) {
                    z->padre->color = NEGRO;
                    tio->color = NEGRO;
                    z->padre->padre->color = ROJO;
                    z = z->padre->padre;
                } else {
                    if (z == z->padre->izq) {
                        z = z->padre;
                        rotarDer(z);
                    }
                    z->padre->color = NEGRO;
                    z->padre->padre->color = ROJO;
                    rotarIzq(z->padre->padre);
                }
            }
        }
        raiz->color = NEGRO; // La raíz siempre debe ser negra
    }
    
    // Encontrar el nodo mínimo a partir de un nodo dado (hijo más a la izquierda)
    Nodo* minimo(Nodo* x) {
        while (x->izq) x = x->izq;
        return x;
    }
    
    // Reemplaza el subárbol en u por el subárbol en v
    void transplante(Nodo* u, Nodo* v) {
        if (!u->padre) raiz = v;          // Si u es raíz, v pasa a ser raíz
        else if (u == u->padre->izq) u->padre->izq = v;  // Si u es hijo izquierdo
        else u->padre->der = v;           // Si u es hijo derecho
        if (v) v->padre = u->padre;       // Actualizamos el padre de v
    }
    
    // Corrige el árbol después de una eliminación para mantener propiedades rojo-negro
    void corregirEliminacion(Nodo* x) {
        while (x != raiz && (!x || x->color == NEGRO)) {
            if (x == x->padre->izq) {
                Nodo* w = x->padre->der; // Hermano de x
                if (w->color == ROJO) {
                    // Caso 1: Hermano rojo, rotar y recolorear
                    w->color = NEGRO;
                    x->padre->color = ROJO;
                    rotarIzq(x->padre);
                    w = x->padre->der;
                }
                // Caso 2: Ambos hijos del hermano son negros
                if ((!w->izq || w->izq->color == NEGRO) && 
                    (!w->der || w->der->color == NEGRO)) {
                    w->color = ROJO;
                    x = x->padre;
                } else {
                    // Caso 3: El hijo derecho del hermano es negro
                    if (!w->der || w->der->color == NEGRO) {
                        if (w->izq) w->izq->color = NEGRO;
                        w->color = ROJO;
                        rotarDer(w);
                        w = x->padre->der;
                    }
                    // Caso 4: Rotación final y recoloreado
                    w->color = x->padre->color;
                    x->padre->color = NEGRO;
                    if (w->der) w->der->color = NEGRO;
                    rotarIzq(x->padre);
                    x = raiz;
                }
            } else { // Caso simétrico cuando x es hijo derecho
                Nodo* w = x->padre->izq;
                if (w->color == ROJO) {
                    w->color = NEGRO;
                    x->padre->color = ROJO;
                    rotarDer(x->padre);
                    w = x->padre->izq;
                }
                if ((!w->der || w->der->color == NEGRO) && 
                    (!w->izq || w->izq->color == NEGRO)) {
                    w->color = ROJO;
                    x = x->padre;
                } else {
                    if (!w->izq || w->izq->color == NEGRO) {
                        if (w->der) w->der->color = NEGRO;
                        w->color = ROJO;
                        rotarIzq(w);
                        w = x->padre->izq;
                    }
                    w->color = x->padre->color;
                    x->padre->color = NEGRO;
                    if (w->izq) w->izq->color = NEGRO;
                    rotarDer(x->padre);
                    x = raiz;
                }
            }
        }
        if (x) x->color = NEGRO; // Finalmente, aseguramos que x sea negro
    }
    
    // Liberar memoria recursivamente (destructor)
    void liberar(Nodo* x) {
        if (!x) return;
        liberar(x->izq);
        liberar(x->der);
        delete x;
    }
    
    // Imprimir el árbol en orden (izq-raiz-der) mostrando los valores y colores
    void imprimirEnOrden(Nodo* x) {
        if (!x) return;
        imprimirEnOrden(x->izq);
        cout << x->dato << "(" << (x->color == ROJO ? "R" : "N") << ") ";
        imprimirEnOrden(x->der);
    }
    
public:
    ArbolRN() : raiz(nullptr) {}    // Constructor inicializa árbol vacío
    ~ArbolRN() { liberar(raiz); }  // Destructor libera memoria
    
    // INSERTAR un valor en el árbol
    bool insertar(int valor) {
        if (buscar(valor)) return false; // Si ya existe, no insertar
        
        Nodo* z = new Nodo(valor); // Nuevo nodo rojo
        Nodo* y = nullptr;
        Nodo* x = raiz;
        
        // Buscar dónde insertar
        while (x) {
            y = x;
            x = (valor < x->dato) ? x->izq : x->der;
        }
        
        z->padre = y;
        if (!y) raiz = z;              // Si el árbol estaba vacío, z es la raíz
        else if (valor < y->dato) y->izq = z;
        else y->der = z;
        
        if (!z->padre) {               // Si z es raíz, se pone negro y termina
            z->color = NEGRO;
            return true;
        }
        if (!z->padre->padre) return true;  // Si el padre es raíz, no hay más corrección
        
        corregirInsercion(z);          // Corregimos propiedades del árbol
        return true;
    }
    
    // BUSCAR un valor en el árbol
    bool buscar(int valor) {
        Nodo* x = raiz;
        while (x && x->dato != valor) {
            x = (valor < x->dato) ? x->izq : x->der;
        }
        return x != nullptr;
    }
    
    // ELIMINAR un valor del árbol
    bool eliminar(int valor) {
        Nodo* z = raiz;
        // Buscar el nodo a eliminar
        while (z && z->dato != valor) {
            z = (valor < z->dato) ? z->izq : z->der;
        }
        if (!z) return false; // No encontrado
        
        Nodo* y = z;
        Color colorOriginal = y->color;
        Nodo* x;
        
        // Caso 1: z no tiene hijo izquierdo, reemplazar con derecho
        if (!z->izq) {
            x = z->der;
            transplante(z, z->der);
        } 
        // Caso 2: z no tiene hijo derecho, reemplazar con izquierdo
        else if (!z->der) {
            x = z->izq;
            transplante(z, z->izq);
        } 
        // Caso 3: z tiene dos hijos
        else {
            y = minimo(z->der);  // Encontrar sucesor (mínimo en subárbol derecho)
            colorOriginal = y->color;
            x = y->der;
            if (y->padre == z) {
                if (x) x->padre = y;
            } else {
                transplante(y, y->der);
                y->der = z->der;
                y->der->padre = y;
            }
            transplante(z, y);
            y->izq = z->izq;
            y->izq->padre = y;
            y->color = z->color;
        }
        
        delete z; // Eliminamos nodo
        
        // Si el color original era negro, corregimos el árbol
        if (colorOriginal == NEGRO && x) {
            corregirEliminacion(x);
        }
        return true;
    }
    
    // Mostrar el árbol en orden con los colores de los nodos
    void mostrar() {
        if (!raiz) {
            cout << "Arbol vacio" << endl;
            return;
        }
        cout << "Arbol: ";
        imprimirEnOrden(raiz);
        cout << endl;
    }
};

// Ejemplo de uso en main
int main() {
    ArbolRN arbol;
    
    // Insertando varios elementos
    cout << "Insertando: 10, 5, 15, 3, 7, 12, 18" << endl;
    arbol.insertar(10);
    arbol.insertar(5);
    arbol.insertar(15);
    arbol.insertar(3);
    arbol.insertar(7);
    arbol.insertar(12);
    arbol.insertar(18);
    arbol.mostrar();
    cout << endl;
    
    // Buscar elemento
    cout << "Buscar 7: " << (arbol.buscar(7) ? "Encontrado" : "No encontrado") << endl;
    arbol.mostrar();
    cout << endl;
    
    // Insertar nuevo elemento
    cout << "Insertando 11..." << endl;
    arbol.insertar(11);
    arbol.mostrar();
    cout << endl;
    
    // Eliminar elemento
    cout << "Eliminando 5..." << endl;
    arbol.eliminar(5);
    arbol.mostrar();
    
    return 0;
}
