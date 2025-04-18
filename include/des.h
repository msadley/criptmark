#ifndef DES_H
#define DES_H

#include <array>
#include <vector>
#include <string>
#include <cstdint>

namespace DES {
// Tabelas de permutação para DES
extern const int IP[64];
extern const int IP_1[64];
extern const int PC1[56];
extern const int PC2[48];
extern const int E[48];
extern const int P[32];
extern const uint8_t S_BOX[8][64];

// Máscaras de permutação pré-computadas para DES
struct DESPermutations {
    static std::array<uint64_t, 64> IP_MASK;
    static std::array<uint64_t, 64> IP_1_MASK;
    static std::array<uint64_t, 56> PC1_MASK;
    static std::array<uint64_t, 48> PC2_MASK;
    static std::array<uint64_t, 48> E_MASK;
    static std::array<uint32_t, 32> P_MASK;

    static void initialize();
};

// Aplica permutação usando máscaras pré-computadas
uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 64>& mascara, int tamanhoSaida);
uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 56>& mascara, int tamanhoSaida);
uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 48>& mascara, int tamanhoSaida);
uint32_t aplicarPermutacao(uint32_t entrada, const std::array<uint32_t, 32>& mascara, int tamanhoSaida);

// Rotação à esquerda para 28 bits
uint32_t rotacaoEsquerda28(uint32_t bits, int deslocamento);

// Gera 16 subchaves para DES
std::array<uint64_t, 16> gerarSubchavesDES(uint64_t chave);

// Função F do DES
uint32_t funcaoF(uint32_t R, uint64_t subchave);

// Gera chave aleatória de 64 bits para DES com paridade
uint64_t gerarChaveDES();

// Criptografa um bloco de 64 bits com DES
uint64_t criptografarBloco(uint64_t bloco, const std::array<uint64_t, 16>& subchaves);

// Descriptografa um bloco de 64 bits com DES
uint64_t descriptografarBloco(uint64_t bloco, const std::array<uint64_t, 16>& subchaves);

// Criptografa uma mensagem com DES
std::vector<uint64_t> criptografarDES(const std::string& mensagem, uint64_t chave);

// Descriptografa uma mensagem com DES
std::string descriptografarDES(const std::vector<uint64_t>& textoCifrado, uint64_t chave);
}

#endif // DES_H