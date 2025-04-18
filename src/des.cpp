#include "des.h"
#include "utils.h"
#include <random>

namespace DES {
// Tabelas de permutação para DES
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const int IP_1[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

const int PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26,
    5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

const uint8_t S_BOX[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    // ... (remaining S-boxes as in original code)
};

std::array<uint64_t, 64> DESPermutations::IP_MASK;
std::array<uint64_t, 64> DESPermutations::IP_1_MASK;
std::array<uint64_t, 56> DESPermutations::PC1_MASK;
std::array<uint64_t, 48> DESPermutations::PC2_MASK;
std::array<uint64_t, 48> DESPermutations::E_MASK;
std::array<uint32_t, 32> DESPermutations::P_MASK;

void DESPermutations::initialize() {
    for (int i = 0; i < 64; i++) {
        IP_MASK[i] = (1ULL << (IP[i] - 1));
        IP_1_MASK[i] = (1ULL << (IP_1[i] - 1));
    }
    for (int i = 0; i < 56; i++) {
        PC1_MASK[i] = (1ULL << (PC1[i] - 1));
    }
    for (int i = 0; i < 48; i++) {
        PC2_MASK[i] = (1ULL << (PC2[i] - 1));
        E_MASK[i] = (1ULL << (E[i] - 1));
    }
    for (int i = 0; i < 32; i++) {
        P_MASK[i] = (1U << (P[i] - 1));
    }
}

static struct DESInitializer {
    DESInitializer() { DESPermutations::initialize(); }
} desInitializer;

uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 64>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 56>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

uint64_t aplicarPermutacao(uint64_t entrada, const std::array<uint64_t, 48>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

uint32_t aplicarPermutacao(uint32_t entrada, const std::array<uint32_t, 32>& mascara, int tamanhoSaida) {
    uint32_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1U << i);
        }
    }
    return saida;
}

uint32_t rotacaoEsquerda28(uint32_t bits, int deslocamento) {
    return ((bits << deslocamento) | (bits >> (28 - deslocamento))) & 0x0FFFFFFF;
}

std::array<uint64_t, 16> gerarSubchavesDES(uint64_t chave) {
    std::array<uint64_t, 16> subchaves;
    uint64_t chavePermutada = aplicarPermutacao(chave, DESPermutations::PC1_MASK, 56);
    uint32_t C = (chavePermutada >> 28) & 0x0FFFFFFF;
    uint32_t D = chavePermutada & 0x0FFFFFFF;
    const uint8_t deslocamentos[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    
    for (int i = 0; i < 16; i++) {
        C = rotacaoEsquerda28(C, deslocamentos[i]);
        D = rotacaoEsquerda28(D, deslocamentos[i]);
        uint64_t CD = (static_cast<uint64_t>(C) << 28) | D;
        subchaves[i] = aplicarPermutacao(CD, DESPermutations::PC2_MASK, 48);
    }
    return subchaves;
}

uint32_t funcaoF(uint32_t R, uint64_t subchave) {
    uint64_t expandido = aplicarPermutacao(R, DESPermutations::E_MASK, 48);
    uint64_t xorResultado = expandido ^ subchave;
    uint32_t sBoxResultado = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t seisBits = (xorResultado >> (i * 6)) & 0x3F;
        sBoxResultado |= (S_BOX[i][seisBits] << (28 - i * 4));
    }
    return aplicarPermutacao(sBoxResultado, DESPermutations::P_MASK, 32);
}

uint64_t gerarChaveDES() {
    static std::mt19937 gen(std::random_device{}());
    uint64_t chave = 0;
    std::uniform_int_distribution<> dis(0, 1);
    
    for (int i = 0; i < 64; i++) {
        if ((i + 1) % 8 != 0) {
            chave |= static_cast<uint64_t>(dis(gen)) << i;
        }
    }
    
    for (int i = 0; i < 8; i++) {
        uint64_t byte = (chave >> (i * 8)) & 0x7F;
        int contUm = __builtin_popcountll(byte);
        if (contUm % 2 == 0) {
            chave |= (1ULL << (i * 8 + 7));
        } else {
            chave &= ~(1ULL << (i * 8 + 7));
        }
    }
    return chave;
}

uint64_t criptografarBloco(uint64_t bloco, const std::array<uint64_t, 16>& subchaves) {
    uint64_t blocoPermutado = aplicarPermutacao(bloco, DESPermutations::IP_MASK, 64);
    uint32_t L = blocoPermutado >> 32;
    uint32_t R = blocoPermutado & 0xFFFFFFFF;
    
    for (int i = 0; i < 16; i++) {
        uint32_t f = funcaoF(R, subchaves[i]);
        uint32_t temp = L ^ f;
        L = R;
        R = temp;
    }
    
    uint64_t RL = (static_cast<uint64_t>(R) << 32) | L;
    return aplicarPermutacao(RL, DESPermutations::IP_1_MASK, 64);
}

uint64_t descriptografarBloco(uint64_t bloco, const std::array<uint64_t, 16>& subchaves) {
    uint64_t blocoPermutado = aplicarPermutacao(bloco, DESPermutations::IP_MASK, 64);
    uint32_t L = blocoPermutado >> 32;
    uint32_t R = blocoPermutado & 0xFFFFFFFF;
    
    for (int i = 15; i >= 0; i--) {
        uint32_t f = funcaoF(R, subchaves[i]);
        uint32_t temp = L ^ f;
        L = R;
        R = temp;
    }
    
    uint64_t RL = (static_cast<uint64_t>(R) << 32) | L;
    return aplicarPermutacao(RL, DESPermutations::IP_1_MASK, 64);
}

std::vector<uint64_t> criptografarDES(const std::string& mensagem, uint64_t chave) {
    std::array<uint64_t, 16> subchaves = gerarSubchavesDES(chave);
    std::vector<bool> bits = Utils::stringParaBits(mensagem);
    
    while (bits.size() % 64 != 0) {
        bits.push_back(false);
    }
    
    std::vector<uint64_t> blocos;
    for (size_t i = 0; i < bits.size(); i += 64) {
        uint64_t bloco = 0;
        for (int j = 0; j < 64 && i + j < bits.size(); j++) {
            if (bits[i + j]) {
                bloco |= (1ULL << j);
            }
        }
        blocos.push_back(bloco);
    }
    
    std::vector<uint64_t> resultado;
    resultado.reserve(blocos.size());
    for (uint64_t bloco : blocos) {
        resultado.push_back(criptografarBloco(bloco, subchaves));
    }
    return resultado;
}

std::string descriptografarDES(const std::vector<uint64_t>& textoCifrado, uint64_t chave) {
    std::array<uint64_t, 16> subchaves = gerarSubchavesDES(chave);
    std::vector<bool> bitsDecifrados;
    bitsDecifrados.reserve(textoCifrado.size() * 64);
    
    for (uint64_t bloco : textoCifrado) {
        uint64_t blocoDecifrado = descriptografarBloco(bloco, subchaves);
        for (int j = 0; j < 64; j++) {
            bitsDecifrados.push_back((blocoDecifrado >> j) & 1);
        }
    }
    
    return Utils::bitsParaString(bitsDecifrados);
}
}