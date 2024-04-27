# Controle Bluetooth NES - Tetris

O projeto desenvolvido na APS 2 foi um controle no estilo do clássico dos jogos, NES, mas com algumas diferenças:

- Uso de um joystick para simular as setas de direita e esquerda;
- 2 botões colocados um acima do outro (ao invés de um ao lado do outro);
- Um switch para ligar e desligar o controle (com um LED que acende assim que o controle liga e conecta com o bluetooth);
- Um potenciômetro linear usado para o ajuste do volume (abaixar e aumentar o volume de acordo com a necessidade do usuário).

Para isso, o projeto foi organizado da seguinte maneira:

- `hc06.c`: Arquivo `.c` com implementação das funções auxiliares para configurar o módulo bluetooth
    - `bool hc06_check_connection();`
    - `bool hc06_set_name(char name[]);`
    - `bool hc06_set_pin(char pin[]);`
    - `bool hc06_set_at_mode(int on);`
    - `bool hc06_init(char name[], char pin[]);`

- `main.c`: Arquivo principal com inicialização do módulo bluetooth

- `main.py`: Arquivo para emular o clique de teclas do teclado a partir do uso dos botões

- `main.h`: Arquivo com as funções utilizadas no projeto

## Modo de uso

Para utilizar o controle, basta seguir os seguintes passos:

1. Ligar o controle através do switch (considerando que o controle já conta com pilhas);
2. Conectar o módulo do bluetooth pelo terminal;
3. Rodar o programa ```main.py```;
4. Se divertir com o Tetris nesse link: https://www.freetetris.org/game.php


## Imagens e diagramas


![d1a116f5-9ff6-4eab-a381-5b5b70588b67](https://github.com/insper-classroom/24a-emb-aps-2-ban_chutebox/assets/103457916/0ab9![APS2](https://github.com/insper-classroom/24a-emb-aps-2-ban_chutebox/assets/103457916/2affbc07-d51e-40b4-8aab-65aa951d0f19)
fcb3-42bd-4d2d-91bc-f3aefe9d8df6)


![Uploading APS2.png…]()
