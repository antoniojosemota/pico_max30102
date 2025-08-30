# Pico MAX30102 - Monitor de SpO2 e Frequência Cardíaca

Este projeto implementa um monitor de saturação de oxigênio no sangue (SpO2) e frequência cardíaca usando um Raspberry Pi Pico W e um sensor MAX30102. Os dados são lidos do sensor via I2C, processados e, em seguida, os valores de SpO2 e frequência cardíaca são enviados para o monitor serial.

[https://github.com/antoniojosemota/pico_max30102](https://github.com/antoniojosemota/pico_max30102)

## Funcionalidades

- Leitura de dados dos LEDs infravermelho e vermelho do sensor MAX30102.
- Cálculo dos valores de SpO2 e da frequência cardíaca.
- Exibição dos resultados em um monitor serial via USB.

## Requisitos de Hardware

- Raspberry Pi Pico W
- Sensor de pulso e oxímetro MAX30102
- Fios de conexão

## Conexões

| MAX30102 | Raspberry Pi Pico W |
| -------- | ------------------- |
| VIN      | 3V3 (OUT)           |
| GND      | GND                 |
| SDA      | GP2 (SDA)           |
| SCL      | GP3 (SCL)           |

## Requisitos de Software

É necessário ter o SDK do Raspberry Pi Pico e o MinGW configurados em seu ambiente de desenvolvimento. (Seguindo as instruções abaixo)

<aside> ➡️

[VsCode](https://www.notion.so/VsCode-25d67aa6595380bebe5ef3aa90cd6be2?pvs=21)

</aside>

## Compilando e Executando o Projeto

1. *Clone o repositório:*
    
    bash
    git clone <https://github.com/antoniojosemota/pico_max30102.git>
    
    
2. *Execute o CMake para gerar os arquivos de compilação:*
    
    ![alt text](image.png)
    
    - Clicando em “Compile” ele compila o código para gerar o executável “.uf2”.
    - Antes de enviar dê BOOTSEL na placa.
    - Em “Run” realiza a transferência do código para a placa.

## Utilização

Após carregar o firmware, abra um monitor serial (como o minicom ou o monitor serial da IDE do Arduino) para visualizar os dados de SpO2 e frequência cardíaca.

- *Taxa de transmissão:* 115200

A saída será semelhante a:

bash
HR: 75 bpm, SpO2: 98 %


## Estrutura de Arquivos

bash
├── CMakeLists.txt
├── algorithm
│   ├── algorithm.c
│   └── algorithm.h
├── max30100
│   ├── max30100.c
│   └── max30100.h
├── oxi.c
└── pico_sdk_import.cmake


- oxi.c: Arquivo principal da aplicação.
- max30100/: Biblioteca para o sensor MAX30102.
- algorithm/: Algoritmo para cálculo de SpO2 e frequência cardíaca.
- CMakeLists.txt: Arquivo de configuração de compilação do CMake.
- pico_sdk_import.cmake: Script de importação do SDK do Pico.

Desenvolvido por @Antonio José Mota Ferreira Brandão @Arthur Vieira @Nicolas Rafael
---
