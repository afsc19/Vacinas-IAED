# Vacinas 💉
Projeto de Introdução aos Algoritmos e Estruturas de Dados (LEIC-A 24/25 - IST).

O projeto consiste na construção de um sistema de gestão de vacinas
e suas inoculações.

Consulte o [Enunciado](https://github.com/afsc19/Vacinas-IAED/blob/main/enunciado.md) para mais informações.

# Utilizar o programa 💉🩹
Basta correr utilizando `./proj` num sistema operativo Linux ([use WSL2 para Windows](https://learn.microsoft.com/en-us/windows/wsl/install)) ou em sistemas operativos baseados em UNIX (pode ser necessário recompilar).

| Comando | Acção |
|:---:|:---|
| __q__ | termina o programa |
| __c__ | introduz no sistema um novo lote de uma vacina |
| __l__ | lista as vacinas disponíveis |
| __a__ | aplica uma dose da vacina a um utente |
| __r__ | retira a disponibilidade de uma vacina |
| __d__ | apaga o registo de aplicações da vacina |
| __u__ | lista as aplicações a um utente |
| __t__ | avança o tempo simulado |

# Construir 🔨
Construa o binário com todos os ficheiros .c ([GCC](https://gcc.gnu.org/)):
```
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c
```

# Testes 🧪
Para correr os testes (fornecidos pelo professor), extraia a pasta zipada [public-tests.zip](https://github.com/afsc19/Vacinas-IAED/blob/main/public-tests.zip) e use o comando `make` na pasta `public-tests/`.
O resultado dos testes deverá aparecer no fim do output.
