#include <iostream>
#include "menus/Menus.h"

using namespace std;

int main() {
    int opcion;

    do {
        cout << "\n====================================\n";
        cout << "        MY CHEMICAL MUSIC\n";
        cout << "====================================\n";
        cout << "1. Gestionar productos\n";
        cout << "2. Gestionar clientes\n";
        cout << "3. Gestionar proveedores\n";
        cout << "4. Gestionar ventas\n";
        cout << "5. Gestionar servicios\n";
        cout << "6. Gestionar facturacion\n";
        cout << "7. Compras a proveedores\n";
        cout << "8. Configuracion general\n";
        cout << "0. Salir\n";
        cout << "====================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: menuProductos(); break;
            case 2: menuClientes(); break;
            case 3: menuProveedores(); break;
            case 4: menuVentas(); break;
            case 5: menuServicios(); break;
            case 6: menuFacturacion(); break;
            case 7: menuComprasProveedores(); break;
            case 8: menuConfiguracion(); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}
