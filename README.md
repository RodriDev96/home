#Documento: Projeto de Controle Remoto via Web com ESP32
## 1. Introdução: Este documento descreve um projeto completo de controle remoto via web utilizando o microcontrolador ESP32. O projeto permite controlar dispositivos conectados aos relés remotamente por meio de uma interface web acessível em qualquer dispositivo conectado à mesma rede Wi-Fi que o ESP32. Além disso, o projeto inclui a funcionalidade de atualização OTA (Over-The-Air), que permite atualizar o firmware do ESP32 sem a necessidade de conexão física.
## 2. Componentes Necessários:
## •	ESP32: O ESP32 é um microcontrolador Wi-Fi e Bluetooth de baixo custo e alto desempenho, ideal para projetos de IoT.
## •	Módulo de relé: Um módulo de relé com pelo menos dois canais é utilizado para controlar dispositivos elétricos.
## •	Switches: Switches (ou botões) são usados para controle local dos relés.
## •	LEDs RGB: LEDs RGB são utilizados para indicar o status do sistema.
## •	Acesso à rede Wi-Fi: Uma rede Wi-Fi é necessária para a comunicação entre o ESP32 e os dispositivos conectados.
## 3. Bibliotecas Utilizadas: O projeto faz uso das seguintes bibliotecas:
## •	Arduino.h: Biblioteca principal do Arduino para programação do ESP32.
## •	WiFiManager.h: Biblioteca para gerenciamento de conexão Wi-Fi, permitindo a configuração de redes sem a necessidade de recompilar o código.
## •	WebServer.h: Biblioteca para criação de um servidor web no ESP32.
## •	ArduinoOTA.h: Biblioteca para atualização OTA do firmware, permitindo atualizar o firmware remotamente.
## •	freertos/FreeRTOS.h e freertos/task.h: Bibliotecas para suporte ao FreeRTOS, que é um sistema operacional em tempo real.
## 4. Configurações dos Pinos: Os pinos do ESP32 são configurados da seguinte maneira:
## •	Relés: Dois pinos são configurados para controlar os relés.
## •	Switches: Dois pinos são configurados como entradas para ler o estado dos switches.
## •	LEDs RGB: Três pinos são configurados para controlar os LEDs RGB.
## 5. Funcionamento:
## •	Inicialização: No método setup(), o ESP32 inicia o WiFiManager para conectar-se à rede Wi-Fi. Em seguida, configura os pinos, inicia o servidor web, a atualização OTA e cria uma tarefa do FreeRTOS para alternar os relés com base no estado dos switches.
## •	Servidor Web: O servidor web é responsável por servir uma página HTML que permite controlar os relés remotamente. Os usuários podem ligar ou desligar os relés clicando em botões na página.
## •	Atualização OTA: O ESP32 suporta atualização OTA, permitindo que o firmware seja atualizado remotamente.
## •	Controle dos Relés: Uma tarefa do FreeRTOS monitora o estado dos switches. Quando um switch é acionado, a tarefa alterna o estado correspondente do relé.
## •	Indicação de Status: O LED RGB é usado para indicar o status do sistema. Ele acende em vermelho quando não está conectado à rede Wi-Fi, em verde quando está conectado, e em azul durante o processo de inicialização.
## 6. Funcionamento da Interface Web:
## •	A página HTML servida pelo ESP32 contém dois botões para cada relé. Cada botão exibe o estado atual do relé (ligado ou desligado) e permite alternar esse estado.
## •	Os botões são estilizados com CSS para fornecer feedback visual ao usuário.
## •	Quando um botão é clicado, uma solicitação HTTP é enviada para o ESP32, que então altera o estado correspondente do relé e atualiza a página.
## 7. Conclusão: Este projeto oferece uma solução completa e flexível para controlar dispositivos remotamente via web usando o ESP32. Ele combina a versatilidade do ESP32 com a conveniência da conectividade web e a flexibilidade do FreeRTOS, proporcionando uma plataforma robusta para projetos de automação residencial e IoT.

