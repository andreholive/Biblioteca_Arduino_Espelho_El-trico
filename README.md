# Biblioteca Arduino Espelho Elétrico
Biblioteca para Arduino que usa servos motores para controlar espelho elétrico de um carro.

Para utilizar essa biblioteca você deve:

# Incluir a biblioteca no seu projeto:
```c++
    #include <Espelho.h>
```


# Instanciar um ou mais objetos Espelho:

```c++
    Espelho direito(PINO1, PINO2, ENDEREÇO1, ENDEREÇO2);
    Espelho esquerdo(PINO3, PINO4, ENDEREÇO3, ENDEREÇO4);
    Espelho *e = &esquerdo; // define o ponteiro do espelho esquerdo
    Espelho *d =  &direito; // define o ponteiro do espelho direito
```

Obs: Você também pode Instanciar um espelho definindo o mínimo e máximo para o posicionamento dos motores.
Por padrão o mínimo é 0 e o máximo 180 a menos que seja definido:

```c++
    Espelho direito(PINO1, PINO2, ENDEREÇO1, ENDEREÇO2, MIN, MAX);
    Espelho esquerdo((PINO3, PINO4, ENDEREÇO3, ENDEREÇO4, MIN, MAX);
    Espelho *e = &esquerdo; // define o ponteiro do espelho esquerdo
    Espelho *d =  &direito; // define o ponteiro do espelho direito
```
onde PINO é uma porta Digital PWM do arduino e ENDEREÇO é um endereço de memória EEPROM que salvará a posição do espelho na memória.
são dois endereços e dois pinos por espelho pois são 2 motores que controlam o espelho (Eixo X e Eixo Y).

# Habilitar a funçao TILTDOWN nos espelhos:
esse passo não é obrigatório!

```c++
    d->enableTiltDown(ENDEREÇO5, ENDEREÇO6); // "d" é o ponteiro de direito
```
onde ENDEREÇO é um endereço de memória EEPROM que salvará a posição em TILTDOWN

# Atenção!!!
Para que os motores não fiquem consumindo energia e por consequencia esquentando, ultilizar a função abaixo no loop().

```c++
    e->timeToDisable();
    d->timeToDisable();
```
essa função desabilita os motores depois de 5 segundos após o ultimo movimento.

# Funçoes para movimentar os espelhos:

```c++
    e->up(); 
    e->down();
    e->left();
    e->right();
```

