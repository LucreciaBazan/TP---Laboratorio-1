#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int id;
    int dni;
    char nombre[20];
    char apellido[20];
    char telefono[20];
    char domicilio[30];
    char baja;
}Cliente;

typedef struct{
    int id;
    char producto[20];
    int stock;
    float costo;
    float pvp;
    char baja;
}Producto;

typedef struct{
    int id;
    int idCliente;
    int idProducto;
    int cantidad;
    int dia;
    int mes;
    int anio;
    char pagado;
    char anular;
}Venta;
//CLIENTES-------------------------------------------------------------------------------------------------------
void CargarCliente();
void GenerarIDCliente (Cliente *c);
int BuscarDNI(Cliente c);
void GuardarCliente (Cliente c);
void BajaCliente ();
void DarDeBajaCliente (int posicion);
void ModificarCliente ();
void EditarCliente (int posicion);
void GuardarModificacionCliente (Cliente c, int posicion);
void ListarClientes ();
int CalcularClientes (Cliente c);
//PRODUCTOS------------------------------------------------------------------------------------------------------
void CargarProducto();
void GenerarIDProducto (Producto *p);
void GuardarProducto (Producto p);
int BuscarProducto(Producto p);
void BajaProducto ();
void DarDeBajaProducto (int posicion);
void ModificarProducto ();
void EditarProducto (int posicion);
void GuardarModificacionProducto (Producto p, int posicion);
void ListarProductos ();
int CalcularProductos (Producto p);
//VENTAS---------------------------------------------------------------------------------------------------------
void CargarVenta();
void GenerarIDVenta (Venta *v);
int ValidarStock (Venta v);
int ValidarFecha (int dia, int mes, int anio);
void GuardarVenta (Venta v);
void RestarAlStock (Venta v);
void AnularVenta ();
int CalcularVentas ();
void DarDeBajaVenta (int posicion);
void ListarVentasPorCliente();
void ListarVentasPorMes();
void PromedioDeVentasPorMes();
void UbicarGananciasDeMes (int mes, int anio, float matriz[][4]);
float VentasTotalesDeProducto(int id, int cantidad);
float SacarPromedioDeMes(float matriz[][4]);
//MENU----------------------------------------------------------------------------------------------------------
void Titulo();
void MenuPrincipal();
void Clientes();
void Productos();
void Ventas();
//CLIENTES-------------------------------------------------------------------------------------------------------

void CargarCliente(){ //
    int flag;
    Cliente c;
    char control;
    do{
        GenerarIDCliente(&c);
        do{
        printf("Ingrese el DNI: \n");
        fflush(stdin);
        scanf("%d",&c.dni);
        flag= BuscarDNI(c);
        if (flag!=-1){
            printf("ERROR: El DNI ingresado ya esta guardado, por favor ingrese otro.\n\n");
        }
        }while (flag!=-1);

        printf("Ingrese el nombre: \n");
        fflush(stdin);
        gets(c.nombre);

        printf("Ingrese el apellido: \n");
        fflush(stdin);
        gets(c.apellido);

        printf("Ingrese el tel%cfono: \n",130);
        fflush(stdin);
        gets(c.telefono);

        printf("Ingrese el domicilio: \n");
        fflush(stdin);
        gets(c.domicilio);

        c.baja='n';

        GuardarCliente(c);

        printf("Desea cargar otro cliente? s/n");
        fflush(stdin);
        scanf("%c", &control);
    }while (control=='s');
}

void GenerarIDCliente (Cliente *c){
    FILE *archi;
    archi = fopen("Clientes.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        c->id = (ftell(archi)/sizeof(Cliente))+1;
    }else{
        c->id = 1;
    }
    fclose(archi);
}

int BuscarDNI(Cliente c){
    int posicion=-1,comparar=c.dni;
    FILE *archi;
    archi = fopen("Clientes.dat","rb");
    if (archi!=NULL){
        while ((fread(&c,sizeof(Cliente),1,archi)>0)&&posicion==-1){
            if (comparar==c.dni){
                posicion=(ftell(archi)/sizeof(Cliente));
            }
        }
    }
    fclose(archi);
    return posicion;
}

void GuardarCliente (Cliente c){
    FILE *archi;
    archi = fopen("Clientes.dat","ab");
    if (archi!=NULL){
        fwrite(&c,sizeof(Cliente),1,archi);
    }
    fclose(archi);
}

void BajaCliente (){//
    Cliente c;
    int poscicion;
    char control;
    do{
    printf("Ingrese el DNI del cliente que desea dar de baja\n");
    fflush(stdin);
    scanf("%d", &c.dni);
    poscicion=BuscarDNI(c);
    if (poscicion==-1){
        printf("ERROR: DNI no encontrado, intente otra vez.\n\n");
    }
    }while (poscicion==-1);
    printf("Seguro que desea dar de baja a este cliente?\n");
    fflush(stdin);
    scanf("%c", &control);
    if (control=='s'){
        DarDeBajaCliente(poscicion);
    }
}

void DarDeBajaCliente (int posicion){
    Cliente c;
    FILE *archi;
    archi = fopen("Clientes.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Cliente)*(posicion-1),SEEK_SET);
        fread(&c,sizeof(Cliente),1,archi);
        c.baja='s';
        fseek(archi,sizeof(Cliente)*(posicion-1),SEEK_SET);
        fwrite(&c,sizeof(Cliente),1,archi);
    }
    fclose(archi);
}

void ModificarCliente (){//
    Cliente c;
    int posicion;
    char control;
    do{
        printf("Ingrese el DNI del cliente que desea editar su informacion:\n");
        fflush(stdin);
        scanf("%d", &c.dni);
        posicion=BuscarDNI(c);
        if (posicion==-1){
            printf("ERROR: DNI no encontrado, intete otra vez\n");
        }
    }while (posicion==-1);
    EditarCliente(posicion);
}

void EditarCliente (int posicion){
    Cliente c;
    FILE *archi;
    int opcion;
    char control='s';
    archi = fopen("Clientes.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Cliente)*(posicion-1),SEEK_SET);
        fread(&c,sizeof(Cliente),1,archi);
        while (control=='s'){
            printf("Usted va editar al cliente: \n");
            printf("Id: %d \n",c.id);
            printf("DNI: %d \n\n",c.dni);
            printf("Que desea editar?\n");
            printf("[1] Nombre: %s\n",c.nombre);
            printf("[2] Apellido: %s\n",c.apellido);
            printf("[3] Telefono: %s\n",c.telefono);
            printf("[4] Domicilio: %s\n\n",c.domicilio);
            printf("[0] para terminar\n");
            fflush(stdin);
            scanf("%i", &opcion);
            fflush(stdin);
            switch (opcion){
                case 1:
                    gets(c.nombre);
                    system("pause");
                    system("cls");
                break;
                case 2:
                    gets(c.apellido);
                    system("pause");
                    system("cls");
                break;
                case 3:
                    gets(c.telefono);
                    system("pause");
                    system("cls");
                break;
                case 4:
                    gets(c.domicilio);
                    system("pause");
                    system("cls");
                break;
                case 0:
                    control='n';
                break;
                default:
                    printf("ERROR: opcion no valida, intente otra vez\n");
                    system("pause");
                    system("cls");
                break;
            }
        }
    }
    fclose(archi);
    printf("\n\n\nDesea guardar los cambios (de lo contrario se descartaran): s/n \n");
    scanf("%c", &control);
    if (control='s'){
        GuardarModificacionCliente(c,posicion);
    }else{
        printf("Las modificaciones no fueron guardadas \n");
    }
    fclose(archi);
}

void GuardarModificacionCliente (Cliente c, int posicion){
    FILE *archi;
    archi = fopen("Clientes.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Cliente)*(posicion-1),SEEK_SET);
        fwrite(&c,sizeof(Cliente),1,archi);
    }
    fclose(archi);
}

void ListarClientes (){
    Cliente c;
    FILE *archi;
    char control='s';
    int cantidad;
    cantidad=CalcularClientes(c);
    if (cantidad!=0){
        archi = fopen("Clientes.dat","rb");
        if (archi!=NULL){
            while ((fread(&c,sizeof(Cliente),1,archi)>0)&& control=='s'){
                printf("Id: %d \n",c.id);
                printf("DNI: %d \n",c.dni);
                printf("Nombre: %s\n",c.nombre);
                printf("Apellido: %s\n",c.apellido);
                printf("Telefono: %s\n",c.telefono);
                printf("Domicilio: %s\n",c.domicilio);
                if (c.id!=cantidad){
                    printf("Quedan %d clientes desea continuar? s/n",cantidad-c.id);
                    fflush(stdin);
                    scanf("%c",&control);
                }
            }
        fclose(archi);
        }else{
            printf("Aun no se han ingresado clientes");
        }
    }
}

int CalcularClientes (Cliente c){
    int cantidad;
    FILE *archi;
    archi=fopen("Clientes.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        cantidad=ftell(archi)/sizeof(Cliente);
    }
    return cantidad;
}

//PRODUCTOS------------------------------------------------------------------------------------------------------

void CargarProducto(){//
    Producto p;
    char control;
    int flag;
    do{

        GenerarIDProducto(&p);

        do{
        printf("Ingrese el nombre del producto (todo en mayusculas por favor): \n");
        fflush(stdin);
        gets(p.producto);
        flag= BuscarProducto(p);
        if (flag!=-1){
            printf("ERROR: El producto ingresado ya esta guardado, por favor ingrese otro.\n");
        }
        }while (flag!=-1);

        printf("Ingrese el stock actual del producto: \n");
        fflush(stdin);
        scanf("%d",&p.stock);

        printf("Ingrese el costo: \n");
        fflush(stdin);
        scanf("%f",&p.costo);

        printf("Ingrese pvp: \n");
        fflush(stdin);
        scanf("%f",&p.pvp);

        p.baja='n';

        GuardarProducto(p);

        printf("Desea cargar otro producto? s/n");
        fflush(stdin);
        scanf("%c", &control);
    }while (control=='s');
}

void GenerarIDProducto (Producto *p){
    FILE *archi;
    archi = fopen("Productos.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        p->id = (ftell(archi)/sizeof(Producto))+1;
    }else{
        p->id = 1;
    }
    fclose(archi);
}

void GuardarProducto (Producto p){
    FILE *archi;
    archi = fopen("Productos.dat","ab");
    if (archi!=NULL){
        fwrite(&p,sizeof(Producto),1,archi);
    }
    fclose(archi);
}

int BuscarProducto(Producto p){
    int posicion=-1;
    char comparar[20];
    strcpy(comparar,p.producto);
    FILE *archi;
    archi = fopen("Productos.dat","rb");
    if (archi!=NULL){
        while ((fread(&p,sizeof(Producto),1,archi)>0)&&posicion==-1){
            if (strcmpi(comparar,p.producto)==0){
                posicion=(ftell(archi)/sizeof(Producto));
            }
        }
    }
    fclose(archi);
    return posicion;
}

void BajaProducto (){//
    Producto p;
    int poscicion;
    char control;
    do{
        printf("Ingrese el nombre del producto que desea dar de baja\n");
        fflush(stdin);
        gets(p.producto);
        poscicion=BuscarProducto(p);
        if (poscicion==-1){
            printf("ERROR: Producto no encontrado, intente otra vez\n");
        }
    }while (poscicion==-1);

    printf("Seguro que desea dar de baja a este producto?\n");
    fflush(stdin);
    scanf("%c", &control);
    if (control=='s'){
        DarDeBajaProducto(poscicion);
    }
}

void DarDeBajaProducto (int posicion){
    Producto p;
    FILE *archi;
    archi = fopen("Productos.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(posicion-1),SEEK_SET);
        fread(&p,sizeof(Producto),1,archi);
        p.baja='s';
        fseek(archi,sizeof(Producto)*(posicion-1),SEEK_SET);
        fwrite(&p,sizeof(Producto),1,archi);
    }
    fclose(archi);
}

void ModificarProducto (){
    Producto p;
    int posicion;
    char control;
    do{
        printf("Ingrese el producto que desea editar\n");
        fflush(stdin);
        gets(p.producto);
        posicion=BuscarProducto(p);
        if (posicion==-1){
            printf("ERROR: producto no encontrado, intente otra vez\n");
        }
    }while (posicion==-1);
    EditarProducto(posicion);
}

void EditarProducto (int posicion){
    Producto p;
    FILE *archi;
    int opcion;
    char control='s';
    archi = fopen("Productos.dat","rb");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(posicion-1),SEEK_SET);
        fread(&p,sizeof(Producto),1,archi);
        while (control=='s'){
            printf("Usted va editar al producto: \n");
            printf("Id: %d \n",p.id);
            printf("Que desea editar?\n");
            printf("[1] Producto: %s\n",p.producto);
            printf("[2] Stock: %d\n",p.stock);
            printf("[3] Costo: %.2f\n",p.costo);
            printf("[4] PVP: %.2f\n\n",p.pvp);
            printf("[0] para terminar\n");
            fflush(stdin);
            scanf("%i", &opcion);
            fflush(stdin);
            switch (opcion){
                case 1:
                    gets(p.producto);
                    system("pause");
                    system("cls");
                break;
                case 2:
                    scanf("%i",&p.stock);
                    system("pause");
                    system("cls");
                break;
                case 3:
                    scanf("%f",&p.costo);
                    system("pause");
                    system("cls");
                break;
                case 4:
                    scanf("%f",&p.pvp);
                    system("pause");
                    system("cls");
                break;
                case 0:
                    control='n';
                break;
                default:
                    printf("ERROR: opcion no valida, intente otra vez");
                    system("pause");
                    system("cls");
                break;
            }
        }
    }
    fclose(archi);
    if (control!='s')
    {
        printf("\n\n\nDesea guardar los cambios (de lo contrario se descartaran): s/n \n");
        fflush(stdin);
        scanf("%c", &control);
        if (control=='s'){
            GuardarModificacionProducto(p,posicion);
        }else{
            printf("Las modificaciones no fueron guardadas \n");
        }
    }
}

void GuardarModificacionProducto (Producto p, int posicion){
    FILE *archi;
    archi = fopen("Productos.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(posicion-1),SEEK_SET);
        fwrite(&p,sizeof(Producto),1,archi);
    }
    fclose(archi);
}

void ListarProductos (){
    Producto p;
    FILE *archi;
    char control='s';
    int cantidad;
    cantidad=CalcularProductos(p);
    if (cantidad!=0){
        archi = fopen("Productos.dat","rb");
        if (archi!=NULL){
            while ((fread(&p,sizeof(Producto),1,archi)>0)&& control=='s'){
                printf("Id: %d \n",p.id);
                printf("Producto: %s\n",p.producto);
                printf("Stock: %d\n",p.stock);
                printf("Costo: %.2f\n",p.costo);
                printf("PVP: %.2f\n\n",p.pvp);
                if (p.id!=cantidad){
                    printf("Quedan %d productos, desea continuar? s/n",cantidad-p.id);
                    fflush(stdin);
                    scanf("%c",&control);
                }
            }

        }else{
            printf("Aun no se han ingresado clientes");
        }
    }
    fclose(archi);
}

int CalcularProductos (Producto p){
    int cantidad;
    FILE *archi;
    archi=fopen("Productos.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        cantidad=ftell(archi)/sizeof(Producto);
    }
    return cantidad;
}

//VENTAS---------------------------------------------------------------------------------------------------------

void CargarVenta(){
    int flag,flagC,flagP,stock;
    Venta v;
    Cliente c;
    Producto p;
    char control;
    do{
        GenerarIDVenta(&v);

        do{
        printf("Ingrese el DNI del comprador: \n");
        fflush(stdin);
        scanf("%d",&c.dni);
        flagC= BuscarDNI(c);
        if (flagC==-1){
            printf("ERROR: El DNI ingresado ya esta guardado, por favor ingrese otro.\n");
        }else{
            v.idCliente=flagC;
        }
        }while (flagC==-1);

        do{
        printf("Ingrese el nombre del producto: \n");
        fflush(stdin);
        gets(p.producto);
        flagP= BuscarProducto(p);
        if (flagP==-1){
            printf("ERROR: El producto ingresado ya esta guardado, por favor ingrese otro.\n");
        }else{
            v.idProducto=flagP;
        }
        }while (flagP==-1);

        do{
        printf("Ingrese la cantidad del producto que desea comprar: \n");
        fflush(stdin);
        scanf("%i", &v.cantidad);
        flag= ValidarStock(v);
        if (flag==0){
            printf("ERROR: La cantidad excede el stock actual del producto, por favor ingrese otra.\n");
        }
        }while (flag==0);

        do{
        printf("Ingrese el dia de la venta: \n");
        fflush(stdin);
        scanf("%i", &v.dia);
        printf("Ingrese el mes de la venta: \n");
        fflush(stdin);
        scanf("%i", &v.mes);
        printf("Ingrese el anio de la venta: \n");
        fflush(stdin);
        scanf("%i", &v.anio);
        flag= ValidarFecha(v.dia,v.mes,v.anio);
        if (flag==0){
            printf("ERROR: La fecha no existe.\n");
        }
        }while (flag==0);

        do{
            printf("La venta fue pagada? s/n\n");
            fflush(stdin);
            scanf("%c", &v.pagado);
        }while ((v.pagado!='s')&&(v.pagado!='n'));
        v.anular='n';

        printf("Desea cargar esta venta (de lo contrario se perderan los datos)? s/n");
        fflush(stdin);
        scanf("%c", &control);
        if (control=='s'){
            GuardarVenta(v);
        }
        printf("Desea realizar otra operacion? s/n");
        fflush(stdin);
        scanf("%c", &control);
    }while (control=='s');
}

void GenerarIDVenta (Venta *v){
    FILE *archi;
    archi = fopen("Ventas.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        v->id = (ftell(archi)/sizeof(Venta))+1;
    }else{
        v->id = 1;
    }
    fclose(archi);
}

int ValidarStock (Venta v){
    int flag=0;
    Producto p;
    FILE *archi;
    archi = fopen("Productos.dat","rb");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(v.idProducto-1),SEEK_SET);
        fread(&p,sizeof(Producto),1,archi);
        if (p.stock>=v.cantidad){
            flag=1;
        }
    }
    fclose(archi);
    return flag;
}

int ValidarFecha (int dia, int mes, int anio){
    system("pause");
    int flag=0;
    int bi=(anio)%4;
    if (bi==0){
        switch (mes){
        case 1:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 2:
            if (dia>=1 && dia<=29){
                flag=1;
            }
        break;
        case 3:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 4:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 5:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 6:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 7:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 8:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 9:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 10:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 11:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 12:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        }
    }else{
        switch (mes){
        case 1:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 2:
            if (dia>=1 && dia<=28){
                flag=1;
            }
        break;
        case 3:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 4:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 5:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 6:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 7:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 8:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 9:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 10:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        case 11:
            if (dia>=1 && dia<=30){
                flag=1;
            }
        break;
        case 12:
            if (dia>=1 && dia<=31){
                flag=1;
            }
        break;
        }
    }
    return flag;
}

void GuardarVenta (Venta v){
    FILE *archi;
    RestarAlStock(v);
    archi = fopen("Ventas.dat","ab");
    if (archi!=NULL){
        fwrite(&v,sizeof(Venta),1,archi);
    }
    fclose(archi);
}

void RestarAlStock (Venta v){
    Producto p;
    FILE *archi;
    archi = fopen("Productos.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(v.idProducto-1),SEEK_SET);
        fread(&p,sizeof(Producto),1,archi);
        p.stock=p.stock-v.cantidad;
        fseek(archi,sizeof(Producto)*(v.idProducto-1),SEEK_SET);
        fwrite(&p,sizeof(Producto),1,archi);
    }
    fclose(archi);
}

void AnularVenta (){//
    int id,cantidad;
    char control;
    cantidad=CalcularVentas();
    do{
        printf("ingrese el ID de la venta que desea anular");
        fflush(stdin);
        scanf("%d", &id);
        if (id>cantidad || id<0){
            printf("ERROR: no existe ese ID, intente otra vez");
        }
    }while(id>cantidad || id<0);
    printf("Esta seguro de anular esta venta?");
    scanf("%c",&control);
    if (control=='s'){
        DarDeBajaVenta(id);
    }
}

int CalcularVentas (){
    Venta v;
    int cantidad;
    FILE *archi;
    archi=fopen("Ventas.dat","rb");
    if (archi!=NULL){
        fseek(archi,0,SEEK_END);
        cantidad=ftell(archi)/sizeof(Venta);
    }
    return cantidad;
}

void DarDeBajaVenta (int posicion){
    Venta v;
    FILE *archi;
    archi = fopen("Ventas.dat","r+b");
    if (archi!=NULL){
        fseek(archi,sizeof(Venta)*(posicion-1),SEEK_SET);
        fread(&v,sizeof(Venta),1,archi);
        v.anular='a';
        fseek(archi,sizeof(Venta)*(posicion-1),SEEK_SET);
        fwrite(&v,sizeof(Venta),1,archi);
    }
    fclose(archi);
}

void ListarVentasPorCliente(){//
    int flag;
    Cliente c;
    Venta v;
    FILE *archi;
    do{
        printf("Ingrese el DNI del comprador: \n");
        fflush(stdin);
        scanf("%d",&c.dni);
        flag= BuscarDNI(c);
        if (flag==-1){
            printf("ERROR: El DNI ingresado ya esta guardado, por favor ingrese otro.\n");
        }
    }while (flag==-1);
    if (flag!=-1){
        archi = fopen("Ventas.dat","rb");
        if (archi!=NULL){
            while ((fread(&v,sizeof(Venta),1,archi))>0){
                if (flag==v.idCliente){
                    printf("ID de la venta: %d\n",v.id);
                    printf("ID del cliente: %d\n",v.idCliente);
                    printf("ID del producto: %d\n",v.idProducto);
                    printf("Monto vendido: %d\n",v.cantidad);
                    printf("Fecha: %d/%d/%d\n",v.dia,v.mes,v.anio);
                    printf("Estado de pago: %d\n",v.pagado);
                    printf("Estado de anulacion: %d\n\n",v.anular);
                }
            }
        }
        fclose(archi);
    }
}

void ListarVentasPorMes(){//
    int mes,anio;
    FILE *archi;
    Venta v;
    int flag;
    do{
        v.dia=1;
        printf("Ingrese el mes de la venta: \n");
        fflush(stdin);
        scanf("%i", &v.mes);
        printf("Ingrese el año de la venta: \n");
        fflush(stdin);
        scanf("%i", &v.anio);
        flag= ValidarFecha(v.dia, v.mes, v.anio);
        if (flag==0){
            printf("ERROR: La fecha no existe.\n");
        }
    }while (flag==0);
    mes=v.mes;
    anio=v.anio;
    archi = fopen("Ventas.dat","rb");
    if (archi!=NULL){
        while ((fread(&v,sizeof(Venta),1,archi))>0){
            if (anio==v.anio){
                if (mes==v.mes){
                    printf("ID de la venta: %d\n",v.id);
                    printf("ID del cliente: %d\n",v.idCliente);
                    printf("ID del producto: %d\n",v.idProducto);
                    printf("Monto vendido: %d\n",v.cantidad);
                    printf("Fecha: %d/%d/%d\n",v.dia,v.mes,v.anio);
                    printf("Estado de pago: %c\n",v.pagado);
                    printf("Estado de anulacion: %c\n\n",v.anular);
                }
            }
        }
    }
    fclose(archi);
}

void PromedioDeVentasPorMes(){//
    float matriz[7][4],promedioTotal;
    int mes,anio;
    printf("Ingrese el año de que desea sacar el promedio:\n");
    fflush(stdin);
    scanf("%i",&anio);
    do{
        printf("Ingrese el mes que desea sacar promedio:\n");
        fflush(stdin);
        scanf("%i", &mes);
        if(mes<1 && mes>12){
            printf("ERROR: mes invalido, intentelo otra vez");
        }
    }while(mes<1 && mes>12);
    UbicarGananciasDeMes(mes,anio,matriz);
    promedioTotal=SacarPromedioDeMes(matriz);
    printf("El promedio de ventas del mes %i de %i es:\n\n%.2f\n\n",mes,anio,promedioTotal);
}

void UbicarGananciasDeMes (int mes, int anio, float matriz[][4]){
    int dia=1,i,j;
    Venta v;
    FILE *archi;
    archi = fopen("Ventas.dat","rb");
    if (archi!=NULL){
        for (j=0;j<4;j++){
            for (i=0;i<7;i++){
                fseek(archi,0,SEEK_SET);
                matriz[i][j]=0.0;
                while (fread(&v,sizeof(Venta),1,archi)>0){
                    if (anio==v.anio && mes==v.mes && dia==v.dia){
                        matriz[i][j]=matriz[i][j]+VentasTotalesDeProducto(v.idProducto,v.cantidad);
                    }
                }
                dia++;
            }
        }
    }
    fclose(archi);
}

float VentasTotalesDeProducto(int id, int cantidad){
    Producto p;
    FILE *archi;
    float retornar;
    archi = fopen("Productos.dat","rb");
    if (archi!=NULL){
        fseek(archi,sizeof(Producto)*(id-1),SEEK_SET);
        fread(&p,sizeof(Producto),1,archi);
        retornar=(float)p.pvp*cantidad;
    }
    fclose(archi);
    return retornar;
}

float SacarPromedioDeMes(float matriz[][4]){
    int j,i;
    float total=0.0;
    for (j=0;j<4;j++){
        for (i=0;i<7;i++){
            total=total+matriz[i][j];
        }
    }
    total=(float)total/28;
    return total;
}

//MENU----------------------------------------------------------------------------------------------------------

void Titulo(){
    printf("\n    _______________________________________________________________________\n");
    printf("                                                                          ");
	printf("\t\t\t\t           MI EMPRESA\n");
	printf("    _______________________________________________________________________\n");

}

void MenuPrincipal(){
  char repite = 1;

  int opcion = -1;

  do {

  	Titulo();

  	printf("\n\t\t\t\tMENU PRINCIPAL\n");
  	printf("\n\t\t[1]. Clientes\n");
  	printf("\t\t[2]. Productos\n");
  	printf("\t\t[3]. Ventas\n\n");
  	printf("\t\t[0]. Salir\n");
  	printf("\n\t\tIngrese su opci%cn: ",162);
    scanf(" %i",&opcion );

  	switch (opcion) {

  		case 1:
  			Clientes();
  			break;

  		case 2:
            Productos();
            break;

  		case 3:
            Ventas();
  			break;

  		case 0:
  			repite = 0;
  			break;

        default:
            printf("Ingreso una opcion invalida, reintente nuevamente\n");
            system("pause");
            system("cls");
            break;
  		}

  } while (repite);

}

void Clientes(){
  char repite = 1;
  int opcion = -1;
  system("cls");

  do {
  	Titulo();

  	printf("\n\t\t\t\tClientes\n");
  	printf("\n\t\t[1]. Alta Clientes\n");
  	printf("\t\t[2]. Baja Clientes\n");
  	printf("\t\t[3]. Modificar clientes\n");
  	printf("\t\t[4]. Lista clientes\n\n");
  	printf("\t\t[0]. Salir\n");
  	printf("\n\t\tIngrese su opci%cn: ",162);
    scanf(" %i",&opcion );

  	switch (opcion) {

  		case 1:
  		    system("cls");
  		    CargarCliente();
  		    system("pause");
            system("cls");
  			break;

  		case 2:
            system("cls");
            BajaCliente();
  		    system("pause");
            system("cls");
  			break;

  		case 3:
            system("cls");
            ModificarCliente();
  		    system("pause");
            system("cls");
        break;

        case 4:
            system("cls");
            ListarClientes();
  		    system("pause");
            system("cls");
        break;

  		case 0:
            system("cls");
  			repite = 0;
  			break;

        default:
            printf("Ingreso una opcion invalida, reintente nuevamente\n");
            system("pause");
            system("cls");
            break;
  		}

  } while (repite);
}

void Productos(){
  char repite = 1;
  int opcion = -1;
  system("cls");
  do {

  	Titulo();

  	printf("\n\t\t\t\tProductos\n");
  	printf("\n\t\t[1]. Ingresar producto\n");
  	printf("\t\t[2]. Baja producto\n");
    printf("\t\t[3]. Modificar productos\n");
    printf("\t\t[4]. Lista de productos\n\n");
    printf("\t\t[0]. Salir\n");
  	printf("\n\t\tIngrese su opci%cn: ",162);
    scanf(" %i",&opcion );

  	switch (opcion) {

  		case 1:
            system("cls");
            CargarProducto();
  		    system("pause");
            system("cls");
  			break;

  		case 2:
            system("cls");
            BajaProducto();
  		    system("pause");
            system("cls");
  			break;

        case 3:
            system("cls");
            ModificarProducto();
  		    system("pause");
            system("cls");
    		break;

        case 4:
            system("cls");
            ListarProductos();
  		    system("pause");
            system("cls");
    		break;

        case 0:
            system("cls");
  			repite = 0;
  			break;

        default:
            printf("Ingreso una opcion invalida, reintente nuevamente\n");
            system("pause");
            system("cls");
            break;
  		}

  } while (repite);
}

void Ventas(){
  char repite = 1;
  int opcion = -1;
  system("cls");
  do {

  	Titulo();

  	printf("\n\t\t\t\tVentas\n");
  	printf("\n\t\t[1]. Alta de Venta\n");
  	printf("\t\t[2]. Anular venta\n");
    printf("\t\t[3]. Lista de venta por cliente\n");
    printf("\t\t[4]. Lista venta del mes\n");
    printf("\t\t[5]. Promedio del mes\n\n");
    printf("\t\t[0]. Salir\n");
  	printf("\n\t\tIngrese su opci%cn: ",162);
    scanf(" %i",&opcion );

  	switch (opcion) {

  		case 1:
            system("cls");
            CargarVenta();
  		    system("pause");
            system("cls");
  			break;

  		case 2:
            system("cls");
            AnularVenta();
  		    system("pause");
            system("cls");
  			break;

        case 3:
            system("cls");
            ListarVentasPorCliente();
  		    system("pause");
            system("cls");
    		break;

        case 4:
            system("cls");
            ListarVentasPorMes();
  		    system("pause");
            system("cls");
    		break;

        case 5:
            system("cls");
            PromedioDeVentasPorMes();
  		    system("pause");
            system("cls");
    		break;

        case 0:
            system("cls");
  			repite = 0;
  			break;

        default:
            printf("Ingreso una opcion invalida, reintente nuevamente\n");
            system("pause");
            system("cls");
            break;
  		}
  }while (repite);
}

int main(){
    MenuPrincipal();
}
