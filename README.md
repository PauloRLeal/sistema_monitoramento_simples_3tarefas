# Unidade 1 - Tarefa Capítulo 2

Este repositório contém o código-fonte para a resolução da tarefa do Capítulo 2 da Unidade 1 da Residência Tecnológica em Software Embarcado - EmbarcaTech.

## Descrição da atividade

Criar uma aplicação embarcada no FreeRTOS com 3 tarefas que simulam o monitoramento de um sistema com sensores (como um botão e um LED). As tarefas irão cooperar para realizar diferentes funções, como ler o estado do botão e controlar o LED.

### Instruções:

#### Descrição do Sistema:

1. **Tarefa 1 (Leitura do Botão)**: Essa tarefa será responsável por simular a leitura de um botão. Ela será executada periodicamente, a cada 100ms, e enviará o estado do botão para a próxima tarefa.
2. **Tarefa 2 (Processamento do Botão)**: Receberá o estado do botão da Tarefa 1. Caso o botão seja pressionado (simulado com a variável), ela acionará a próxima tarefa (a de controlar o LED). Caso contrário, apenas aguardará o próximo ciclo de leitura.
3. **Tarefa 3 (Controle do LED)**: Controlará um LED (simulado como uma variável ou saída digital). Se o botão for pressionado, o LED será aceso, caso contrário, será apagado. A tarefa será executada sempre que for acionada pela Tarefa 2.

#### Detalhamento da Implementação:

1. Definições de variáveis:
   - Defina variáveis para armazenar o estado do botão e do LED.
   - Utilize filas ou variáveis globais para compartilhar o estado entre as tarefas.
2. Criação das tarefas no FreeRTOS:
   - Tarefa 1: Leitura do botão (criada com uma prioridade baixa, executada a cada 100ms).
   - Tarefa 2: Processamento e decisão (executada dependendo do estado do botão).
   - Tarefa 3: Controle do LED (executada apenas quando acionada pela Tarefa 2).
3. Sincronização entre as tarefas:
   - Pode-se usar semáforos ou filas para que as tarefas se comuniquem e cooperem entre si.
   - A Tarefa 2 pode enviar um sinal para a Tarefa 3 quando detectar o estado do botão.

## Estrutura do Repositório

```
/
├── sistema_monitoramento_simples_3tarefas.c           # Arquivo C principal
├── CMakeList.txt                                      # Arquivo de compilação
└── README.md                                          # Este arquivo
```

## Configuração e Uso

### Compatibilidade

- Placa de desenvolvimento BitDogLab.
- Raspberry Pi Pico W.

### Compilação e Upload

1. Clone este repositório:
   ```
   git clone https://github.com/PauloRLeal/sistema_monitoramento_simples_3tarefas.git
   ```
2. Faça o upload do firmware para o microcontrolador.

3. Inicialize a placa, e utilize o botão B para interagir com a aplicação.

## Direitos de Uso

Este projeto é disponibilizado para leitura e citação em outros trabalhos. Caso utilize este projeto como referência, por favor, forneça os devidos créditos ao autor.

## Autor

Desenvolvido por Paulo Roberto Araújo Leal.
