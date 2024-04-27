# Documentação do Código - Controle de Casa via Página Web com ESP32

## Visão Geral

Este documento descreve um projeto de controle remoto residencial utilizando o ESP32, um microcontrolador com suporte Wi-Fi. O projeto permite controlar relés (utilizados para dispositivos elétricos) por meio de uma página web acessível em qualquer dispositivo conectado à mesma rede Wi-Fi que o ESP32. O sistema também inclui a funcionalidade de atualização OTA (Over-The-Air), permitindo atualizar o firmware do ESP32 remotamente.

## Componentes Necessários

- **ESP32**: Microcontrolador principal com suporte Wi-Fi.
- **Módulo de Relé**: Utilizado para controlar dispositivos elétricos.
- **Switches**: Utilizados localmente para alternar os relés.
- **LEDs RGB**: Indicam o estado do sistema.
- **Conexão Wi-Fi**: Necessária para comunicação com a página web.

## Pinagem

- **Pino do Relé 1**: GPIO2
- **Pino do Relé 2**: GPIO3
- **Pino do Switch 1**: GPIO4
- **Pino do Switch 2**: GPIO5
- **Pino do LED Vermelho**: GPIO12
- **Pino do LED Verde**: GPIO13
- **Pino do LED Azul**: GPIO14

## Funcionamento

- **Inicialização**:
  - Conexão automática ao Wi-Fi utilizando WiFiManager.
  - Configuração dos pinos como entrada/saída.
  - Início do servidor web na porta 80.
  - Início da atualização OTA para permitir atualizações remotas.

- **Servidor Web**:
  - A página HTML servida pelo ESP32 permite controlar os relés.
  - Cada relé possui um botão na página para ligar/desligar.

- **Atualização OTA**:
  - Suporta a atualização do firmware remotamente.

- **Controle dos Relés**:
  - Os relés podem ser alternados por meio da página web ou localmente pelos switches.
  - Uma tarefa do FreeRTOS monitora os switches para alterar os estados dos relés.

- **Indicação de Status**:
  - Um LED RGB indica o estado do sistema:
    - Vermelho: Sem conexão Wi-Fi.
    - Verde: Conectado ao Wi-Fi.
    - Azul: Durante a inicialização.

## Detalhes da Implementação

- **Função `toggleRelayTask`**:
  - Tarefa que monitora os switches e altera os estados dos relés conforme necessário.

- **Função `handleRoot`**:
  - Manipula a requisição GET para a página principal (`/`) e exibe os botões de controle dos relés.

- **Função `handleToggleRelay`**:
  - Manipula as requisições GET para os endpoints `/toggle1` e `/toggle2` para alternar os estados dos relés.

- **Funcionamento da Interface Web**:
  - Os botões na página HTML enviam requisições HTTP para alternar os relés.
  - As respostas das requisições atualizam dinamicamente o estado dos botões na página.

## Uso do Código

- **Configuração Inicial**:
  - Conectar o ESP32 à rede Wi-Fi utilizando WiFiManager.
  - Definir os pinos para relés, switches e LEDs.
  - Iniciar o servidor web e a atualização OTA.

- **Loop Principal (`loop`)**:
  - Gerencia as requisições OTA e do servidor web.
  - Atualiza o estado do LED RGB com base na conexão Wi-Fi.

Este código fornece uma base sólida para projetos de automação residencial, permitindo controle remoto e atualização do firmware sem fio. Para uso prático, é necessário adaptar a pinagem e a lógica de controle conforme necessário para os dispositivos específicos a serem controlados em sua aplicação.
