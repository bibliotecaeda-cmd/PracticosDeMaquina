#define MAX 2000

struct{
    persona personas[MAX];
    int cantidad;
}lso;


void inicializarEstructura(lso *lso){
    lso->cantidad = 0;
}

/// 0=exito 1=fracaso
int perteneceLSO(int dnibuscado,lso lso,int *posicion, float *costo){
    if (lso.cantidad == 0){
        *posicion = 0;
        return 1;
    }
    int li = 0; // Inclusivo
    int ls = lso.cantidad-1; // Inclusivo
    int testigo;

    while(li<ls){
        testigo = floor((li+ls)/2); //Testigo Izquierda
        if(dnibuscado<=lso[testigo]){
            ls=testigo;
        }
        else{
            li=testigo+1; //Segmento mas grande izquierda por el +1
        }
    }

    *posicion=testigo;
    if(lso[testigo]==dnibuscado){
        return 0; //Exito
    }
    else{
        return 1; //Fracaso
    }
}

int altaLSO(persona alta, lso lso, float *costo){
    int posicion = 0;
    int estado;
    if(lso.cantidad-1==MAX){
        return 1; //Fracaso - lista llena
    }
    estado = perteneceLSO(alta.dni, lso, *posicion, *costo);
    if(estado == 0){
        return 1; // Fracaso - Elemento ya existe
    }else{
        for(int i = lso.cantidad-1,posicion<i, i--){
            lso[i+1] = lso[i];
        }
        lso[posicion]=alta;
        return 0; //exito
    }
}

int bajaLSO(persona baja, lso lso, float *costo){
    int posicion = 0;
    int estado;
    persona aux;
    if(lso.cantidad-1 == 0){
        return 1; // Fracaso - Lista vacia
    }
    estado = perteneceLSO(baja.dni, lso, *posicion, *costo);
    if(estado != 0){
        return 1; // Fracaso - Elemento no esta
    }else{
        for(int i = posicion,i<lso.cantidad-1, i++){
            lso[i] = lso[i+1];
        }
        lso.cantidad = lso.cantidad - 1; // El elemento queda duplicado pero fuera de la lista (PREGUNTAR)
        return 0; //Exito
    }
}

int evocarLSO(int dniBuscar,persona *encontrada, lso *lso, *costo){
    int posicion = 0;
    int estado;
    persona aux;
    if(lso.cantidad-1) == 0{
        return 1; //Fracaso - lista vacia
    }
    estado = perteneceLSO(dniBuscar, lso, *posicion, *costo);
    if(estado != 0){
        return 1; // Fracaso - Elemento no esta
    }else{
        //Devolver la persona
        encontrada.dni = lso[posicion].dni;
        strcpy(encontrada.nombreyapellido, lso[posicion].nombreyapellido);
        strcpy(encontrada.domicilio, lso[posicion].domicilio);
        encontrada.codigopostal = lso[posicion].codigopostal;
        encontrada.nmesa = lso[posicion].nmesa;
        encontrada.circuito= lso[posicion].circuito;
        return 0; // Exito
    }
}

void mostrarLSO(lso lso){
    for (int i=0; i < lso.cantidad; i++){
        printf("Dni: %d",lso[i].dni);
        printf("Nombre y Apellido: $s",lso[i].nombreyapellido);
        printf("Domicilio: %s",lso[i].domicilio);
        printf("Codigo postal: %d",lso[i].codigopostal);
        printf("Numero de Mesa: %d",lso[i].nmesa);
        printf("Circuito: %d",lso[i].circuito);
    }
}
