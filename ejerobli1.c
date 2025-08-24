#include <stdio.h>
#include <stdlib.h>

//IMPRESIÓN.

/*Implementar una función void imprimir_bcd(const char bcd[], size_t digitos); que dado bcd un número en formato BCD de digitos dígitos lo imprima por stdout. El número debe imprimirse utilizando _ como separador de miles, por ejemplo, la función llamada con el vector {7, 6, 5, 4, 3, 2, 1} y 7 como cantidad de dígitos imprimirá la cadena "1_234_567" (notar que no hay '\n' al final de la misma).*/

void imprimir_bcd(const char bcd[], size_t digitos){
    //Creo un nuevo array con los mismos elementos de anterior, pero invertido.
    char inver[digitos];
    for (size_t i = 0; i < digitos; i++) {
        inver[i] = bcd[digitos - 1 - i];
    }
    //Esto agrega el _ como separador de miles del número invertido.
    for(int i = 0; i < digitos; i++){
        printf("%d", inver[i]);

        // Acá se verifica si hay que agregar _ (cada 3 dígitos, menos al final).
        size_t digitos_res = digitos - i - 1;
        if (digitos_res > 0 && digitos_res % 3 == 0) {
            printf("_");
        }
        
    }
}

//CONVERSIÓN.

/*Implementar una función unsigned long bcd_a_binario(const char bcd[], size_t digitos); que dado un BCD con determinada cantidad de dígitos lo convierta a binario y devuelva esa representación. Por ejemplo, si esta función recibe {1, 1, 5, 9} como bcd y 4 como digitos debe devolver 9511.*/

//Utilizo una función de potenciación hecha en un ejercicio previo.
int exponencial(int x, int n){
    int i = 0;
    int resultado = 1;
    while (i < n){
        resultado *= x;
        i++;
    }
    
    return resultado;
}

unsigned long bcd_a_binario(const char bcd[], size_t digitos){
    unsigned long binario = 0;
    //El ciclo le incrementa el valor de la multiplicación de cada elemento del arreglo por 10 a la potencia de i.
    for (size_t i = 0; i < digitos; i++) {
        binario += bcd[i] * exponencial(10, i);
    }

    return binario;
}

/*Implementar una función size_t binario_a_bcd(unsigned long binario, char bcd[]); que reciba un número binario y lo almacene en representación BCD en el arreglo bcd. La función debe devolver la cantidad de dígitos que utilizó en el arreglo.*/

size_t binario_a_bcd(unsigned long binario, char bcd[]) {
    size_t digitos = 0;

    //Este código está por si justo alguien ingresa 0, pone el número 0 en la primera posición y devuelve 1 como la cantidad de digitos.
    if (binario == 0) {
        bcd[0] = 0;
        return 1;
    }

    //Este ciclo guarda los digitos del numero en bcd en orden inverso
    while (binario > 0) {
        bcd[digitos] = binario % 10; //Esto obtiene el último digito del número y lo guarda en bcd.
        binario /= 10; //Esto borra el último dígito del número.
        digitos++;
    }

    return digitos;
}

//OPERACIONES.

/*Implementar una función size_t sumar_bcd(const char a[], size_t na, const char b[], size_t nb, char r[]); que compute r = a + b. Siendo na y nb la cantidad de dígitos de a y b respectivamente. La función debe devolver la cantidad de dígitos de r.*/

size_t sumar_bcd(const char a[], size_t na, const char b[], size_t nb, char r[]){
    size_t i = 0;
    size_t num_len = (na > nb) ? na : nb;
    int acarreo = 0;

    for(i = 0; i < num_len; i++){
        int digito_a = (i < na) ? a[i] : 0; //Estos son equivalentes a operadoraciones if-else.
        int digito_b = (i < nb) ? b[i] : 0;

        int suma = digito_a + digito_b + acarreo;
        r[i] = suma % 10;
        acarreo = suma / 10;
    }

    if (acarreo > 0) {
        r[i] = acarreo;
        return i + 1;
    }

    return i;
}


/*Implementar una función size_t multiplicar_por_un_digito(const char a[], size_t na, char b, char r[]); que opere r = a * b. Notar que b es un número de sólo un dígito.*/

size_t multiplicar_por_un_digito(const char a[], size_t na, char b, char r[]){
    size_t i = 0;
    int acarreo = 0;
    for(i = 0; i < na; i++){
        int digito_a = (i < na) ? a[i] : 0;

        int multip = digito_a * b + acarreo;
        r[i] = multip % 10;
        acarreo = multip / 10;
    }

    if (acarreo > 0) {
        r[i] = acarreo;
        return i + 1;
    }

    return i;
}

//APLICACIÓN.

/*Implementar una aplicación que le pida indefinidamente números al usuario de a dos por vez (pueden ser en líneas separadas) hasta que se dispare la señal de final de archivo. El programa tiene que calcular e imprimir la suma de los dos números y la multiplicación entre el primer número y la unidad del segundo número.*/

int leer_numero(unsigned long* num) {
    char buffer[100];
    int i = 0;
    int c = getchar();

    if (c == EOF) return 0;

    //Esto verifica que lo ingresado no sea un End of File, espacio blanco, etc.
    while (c != EOF && c != ' ' && c != '\n' && c != '\t') {
        if (c < '0' || c > '9') {
            //Esto borra la linea y muestra error si es que se ingresan letras.
            while (c != '\n' && c != EOF) c = getchar();
            printf("Error. Solo se permiten números.\n");
            return 0;
        }

        if (i < 100 - 1)
            buffer[i++] = c;

        c = getchar();
    }

    buffer[i] = '\0';

    *num = (unsigned long)atoi(buffer);
    return 1;
}

int main(){
    unsigned long x, y;
    char bcdx[21], bcdy[21], bcdr_suma[21], bcdr_multi[21];
    
    while(1){
        printf("Ingresá dos números por vez. Usá Ctrl+D (o Ctrl+Z en Windows) para terminar.\n");
        if (!leer_numero(&x)) break;
        if (!leer_numero(&y)) break;

        int len_x = binario_a_bcd(x, bcdx);
        int len_y = binario_a_bcd(y, bcdy);
        int b = y % 10;

        imprimir_bcd(bcdx, len_x);
        printf(" + ");
        imprimir_bcd(bcdy, len_y);
        printf(" = ");

        size_t len_suma = sumar_bcd(bcdx, len_x, bcdy, len_y, bcdr_suma);
        imprimir_bcd(bcdr_suma, len_suma);
        printf("\n");

        imprimir_bcd(bcdx, len_x);
        printf(" * ");
        printf("%d", b);
        printf(" = ");

        size_t len_multi = multiplicar_por_un_digito(bcdx, len_x, b, bcdr_multi);
        imprimir_bcd(bcdr_multi, len_multi);
        printf("\n");
    }

    return 0;
}