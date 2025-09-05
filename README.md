# 🔐 CriptMark: Benchmarking de Criptografia

**Um projeto para benchmark e análise comparativa dos esquemas de criptografia DES, RSA e Goldwasser-Micali.**

Este projeto foi desenvolvido com o objetivo de realizar uma análise aprofundada e comparativa do desempenho de três algoritmos de criptografia notáveis: **DES (Data Encryption Standard)**, **RSA (Rivest-Shamir-Adleman)** e **Goldwasser-Micali (GM)**. A avaliação foca em métricas de eficiência, como tempo de execução para geração de chaves, criptografia e descriptografia, além de analisar as características computacionais de cada esquema.

Desenvolvido como projeto final para a disciplina de **FMCC II**, ministrada pelo Prof. **Tiago Massoni**, no curso de **Ciência da Computação** da **Universidade Federal de Campina Grande (UFCG)**.

---

## 🚀 Sobre o Projeto

O CriptMark oferece uma plataforma para:

- **Implementação e Teste:** Fornecer implementações claras e funcionais dos algoritmos DES, RSA e GM.
- **Benchmarking de Desempenho:** Realizar testes de desempenho para medir:
  - **Tempo de Geração de Chaves:** O custo computacional para criar chaves de segurança.
  - **Tempo de Criptografia:** A velocidade para codificar mensagens.
  - **Tempo de Descriptografia:** A eficiência para decodificar mensagens.
- **Análise Comparativa:** Gerar relatórios que permitem uma comparação direta do desempenho e das características de cada algoritmo, como o aumento do tamanho da cifra em relação à mensagem original.

---

## 🛠️ Começando

Siga estas instruções para obter uma cópia local do projeto e executá-lo.

### Pré-requisitos

- `g++` (compilador C++)
- `make`

### Instalação

1. **Clone o repositório:**

   ```sh
   git clone https://github.com/seu-usuario/criptmark.git
   cd criptmark
   ```

2. **Compile o projeto:**

   ```sh
   make
   ```

   Isso irá compilar o código-fonte e criar o executável em `build/cryptmark`.

---

## Uso

Para executar o benchmark, rode o seguinte comando:

```sh
./build/cryptmark
```

O programa executará uma série de testes para cada algoritmo e imprimirá um relatório com os resultados médios, incluindo:

- **Tempo de geração de chaves:** Medido em microssegundos (μs).
- **Tempo de criptografia:** Medido em microssegundos (μs).
- **Tempo de descriptografia:** Medido em microssegundos (μs).
- **Tamanho da mensagem original e da cifra:** Em bytes, para análise de overhead.

### Limpando os arquivos de compilação

Para remover os arquivos gerados pela compilação, execute:

```sh
make clean
```

---

## 🤝 Contribuindo

Contribuições são o que tornam a comunidade de código aberto um lugar incrível para aprender, inspirar e criar. Qualquer contribuição que você fizer será **muito apreciada**.

1. Faça um *fork* do projeto.
2. Crie uma *branch* para sua feature (`git checkout -b feature/nova-feature`).
3. Faça o *commit* de suas alterações (`git commit -m 'Adiciona nova feature'`).
4. Faça o *push* para a *branch* (`git push origin feature/nova-feature`).
5. Abra um *Pull Request*.

---

## 📄 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.

---

## 📧 Contato

- **Adley Silva Mendes**
- **Igor Lima dos Santos**
- **Iury Ruan do Nascimento Santos**
- **Bruno Weslley Borges Ribeiro**
