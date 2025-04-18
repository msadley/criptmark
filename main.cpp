#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <bitset>
#include <random>
#include <iomanip>
#include <array>

using namespace std;

// ---------------- Funções Auxiliares Comuns ----------------

// Converte uma string para um vetor de bits
vector<bool> stringParaBits(const string& texto) {
    vector<bool> bits;
    for (char c : texto) {
        bitset<8> b(c);
        for (int i = 0; i < 8; i++) {
            bits.push_back(b[i]);
        }
    }
    return bits;
}

// Converte um vetor de bits para uma string
string bitsParaString(const vector<bool>& bits) {
    string resultado;
    for (size_t i = 0; i < bits.size(); i += 8) {
        bitset<8> b;
        for (int j = 0; j < 8 && i + j < bits.size(); j++) {
            b[j] = bits[i + j];
        }
        resultado += static_cast<char>(b.to_ulong());
    }
    return resultado;
}

// Converte um vetor de bits para uma string binária (0s e 1s)
string bitsParaStringBinaria(const vector<bool>& bits) {
    string resultado;
    for (bool bit : bits) {
        resultado += bit ? '1' : '0';
    }
    return resultado;
}

// Mede o tempo de execução de uma função
template<typename Func, typename... Args>
long long medirTempo(Func&& func, Args&&... args) {
    auto inicio = chrono::high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto fim = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(fim - inicio).count();
}

// ---------------- Implementação do RSA ----------------

// Calcula o máximo divisor comum
int mdc(int a, int b) {
    return b == 0 ? a : mdc(b, a % b);
}

// Algoritmo de Euclides Estendido para inverso modular
tuple<int, int, int> mdcEulerExtendido(int a, int b) {
    if (b == 0) return {a, 1, 0};
    auto [valorMdc, x1, y1] = mdcEulerExtendido(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;
    return {valorMdc, x, y};
}

// Exponenciação modular (base^expoente mod modulo)
int expMod(int base, int expoente, int modulo) {
    int resultado = 1;
    base %= modulo;
    while (expoente > 0) {
        if (expoente & 1)
            resultado = (1LL * resultado * base) % modulo;
        expoente >>= 1;
        base = (1LL * base * base) % modulo;
    }
    return resultado;
}

// Gera chaves pública e privada para RSA
tuple<tuple<int, int>, tuple<int, int>> gerarChavesRSA(int p, int q) {
    srand(time(0));
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    
    // Encontra expoente público e
    int e;
    do {
        e = rand() % (phi - 2) + 2;
    } while (mdc(e, phi) != 1);
    
    // Calcula expoente privado d
    auto [_, x, __] = mdcEulerExtendido(e, phi);
    int d = x < 0 ? x + phi : x;
    
    return {{e, n}, {d, n}};
}

// Criptografa uma mensagem com RSA
vector<int> criptografarRSA(const string& mensagem, const tuple<int, int>& chavePublica) {
    int e, n;
    tie(e, n) = chavePublica;
    vector<int> textoCifrado;
    for (char c : mensagem) {
        textoCifrado.push_back(expMod(c, e, n));
    }
    return textoCifrado;
}

// Descriptografa uma mensagem com RSA
string decriptografarRSA(const vector<int>& textoCifrado, const tuple<int, int>& chavePrivada) {
    int d, n;
    tie(d, n) = chavePrivada;
    string mensagemDecifrada;
    for (int c : textoCifrado) {
        mensagemDecifrada += static_cast<char>(expMod(c, d, n));
    }
    return mensagemDecifrada;
}

// ---------------- Implementação do DES ----------------

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
    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
     13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
     13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
     3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
     14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
     11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
     10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
     4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
     13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
     6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
     1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
     2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

// Máscaras de permutação pré-computadas para DES
struct DESPermutations {
    static array<uint64_t, 64> IP_MASK;
    static array<uint64_t, 64> IP_1_MASK;
    static array<uint64_t, 56> PC1_MASK;
    static array<uint64_t, 48> PC2_MASK;
    static array<uint64_t, 48> E_MASK;
    static array<uint32_t, 32> P_MASK;

    static void initialize() {
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
};

array<uint64_t, 64> DESPermutations::IP_MASK;
array<uint64_t, 64> DESPermutations::IP_1_MASK;
array<uint64_t, 56> DESPermutations::PC1_MASK;
array<uint64_t, 48> DESPermutations::PC2_MASK;
array<uint64_t, 48> DESPermutations::E_MASK;
array<uint32_t, 32> DESPermutations::P_MASK;

// Inicializa as máscaras de permutação
static struct DESInitializer {
    DESInitializer() { DESPermutations::initialize(); }
} desInitializer;

// Aplica permutação usando máscaras pré-computadas
inline uint64_t aplicarPermutacao(uint64_t entrada, const array<uint64_t, 64>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

inline uint64_t aplicarPermutacao(uint64_t entrada, const array<uint64_t, 56>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

inline uint64_t aplicarPermutacao(uint64_t entrada, const array<uint64_t, 48>& mascara, int tamanhoSaida) {
    uint64_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1ULL << i);
        }
    }
    return saida;
}

inline uint32_t aplicarPermutacao(uint32_t entrada, const array<uint32_t, 32>& mascara, int tamanhoSaida) {
    uint32_t saida = 0;
    for (int i = 0; i < tamanhoSaida; i++) {
        if (entrada & mascara[i]) {
            saida |= (1U << i);
        }
    }
    return saida;
}

// Rotação à esquerda para 28 bits
inline uint32_t rotacaoEsquerda28(uint32_t bits, int deslocamento) {
    return ((bits << deslocamento) | (bits >> (28 - deslocamento))) & 0x0FFFFFFF;
}

// Gera 16 subchaves para DES
array<uint64_t, 16> gerarSubchavesDES(uint64_t chave) {
    array<uint64_t, 16> subchaves;
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

// Função F do DES
inline uint32_t funcaoF(uint32_t R, uint64_t subchave) {
    uint64_t expandido = aplicarPermutacao(R, DESPermutations::E_MASK, 48);
    uint64_t xorResultado = expandido ^ subchave;
    uint32_t sBoxResultado = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t seisBits = (xorResultado >> (i * 6)) & 0x3F;
        sBoxResultado |= (S_BOX[i][seisBits] << (28 - i * 4));
    }
    return aplicarPermutacao(sBoxResultado, DESPermutations::P_MASK, 32);
}

// Gera chave aleatória de 64 bits para DES com paridade
uint64_t gerarChaveDES() {
    static mt19937 gen(random_device{}());
    uint64_t chave = 0;
    uniform_int_distribution<> dis(0, 1);
    
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

// Criptografa um bloco de 64 bits com DES
uint64_t criptografarBloco(uint64_t bloco, const array<uint64_t, 16>& subchaves) {
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

// Descriptografa um bloco de 64 bits com DES
uint64_t descriptografarBloco(uint64_t bloco, const array<uint64_t, 16>& subchaves) {
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

// Criptografa uma mensagem com DES
vector<uint64_t> criptografarDES(const string& mensagem, uint64_t chave) {
    array<uint64_t, 16> subchaves = gerarSubchavesDES(chave);
    vector<bool> bits = stringParaBits(mensagem);
    
    while (bits.size() % 64 != 0) {
        bits.push_back(false);
    }
    
    vector<uint64_t> blocos;
    for (size_t i = 0; i < bits.size(); i += 64) {
        uint64_t bloco = 0;
        for (int j = 0; j < 64 && i + j < bits.size(); j++) {
            if (bits[i + j]) {
                bloco |= (1ULL << j);
            }
        }
        blocos.push_back(bloco);
    }
    
    vector<uint64_t> resultado;
    resultado.reserve(blocos.size());
    for (uint64_t bloco : blocos) {
        resultado.push_back(criptografarBloco(bloco, subchaves));
    }
    return resultado;
}

// Descriptografa uma mensagem com DES
string descriptografarDES(const vector<uint64_t>& textoCifrado, uint64_t chave) {
    array<uint64_t, 16> subchaves = gerarSubchavesDES(chave);
    vector<bool> bitsDecifrados;
    bitsDecifrados.reserve(textoCifrado.size() * 64);
    
    for (uint64_t bloco : textoCifrado) {
        uint64_t blocoDecifrado = descriptografarBloco(bloco, subchaves);
        for (int j = 0; j < 64; j++) {
            bitsDecifrados.push_back((blocoDecifrado >> j) & 1);
        }
    }
    
    return bitsParaString(bitsDecifrados);
}

// ---------------- Implementação Goldwasser-Micali ----------------

// Verifica se um número é primo
bool ehPrimo(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Calcula o símbolo de Legendre (n|p)
int simboloLegendre(int n, int p) {
    if (n % p == 0) return 0;
    int result = expMod(n, (p - 1) / 2, p);
    return result == p - 1 ? -1 : result;
}

// Encontra um não-resíduo quadrático módulo p
int encontrarNaoResiduo(int p) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(2, p - 1);
    
    while (true) {
        int x = dis(gen);
        if (simboloLegendre(x, p) == -1) {
            return x;
        }
    }
}

// Gera chaves para Goldwasser-Micali
tuple<tuple<int, int>, tuple<int, int>> gerarChavesGM() {
    int p = 9967, q = 9973;
    int n = p * q;
    
    int y;
    do {
        y = encontrarNaoResiduo(p);
    } while (simboloLegendre(y, q) != -1);
    
    return {{n, y}, {p, q}};
}

// Verifica se dois números são coprimos
bool saoCoprimos(int a, int b) {
    return mdc(a, b) == 1;
}

// Criptografa uma mensagem com Goldwasser-Micali
vector<int> criptografarGM(const string& mensagem, const tuple<int, int>& chavePublica) {
    int n, y;
    tie(n, y) = chavePublica;
    vector<bool> bits = stringParaBits(mensagem);
    vector<int> textoCifrado;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, n - 1);
    
    for (bool bit : bits) {
        int r;
        do {
            r = dis(gen);
        } while (!saoCoprimos(r, n));
        
        long long c = (1LL * r * r) % n;
        if (bit) {
            c = (c * y) % n;
        }
        textoCifrado.push_back(static_cast<int>(c));
    }
    
    return textoCifrado;
}

// Descriptografa uma mensagem com Goldwasser-Micali
string descriptografarGM(const vector<int>& textoCifrado, const tuple<int, int>& chavePrivada) {
    int p, q;
    tie(p, q) = chavePrivada;
    vector<bool> bits;
    
    for (int c : textoCifrado) {
        bool bit = simboloLegendre(c, p) == -1;
        bits.push_back(bit);
    }
    
    return bitsParaString(bits);
}

// Imprime resultados do benchmark
void imprimirResultados(const string& algoritmo, long long tempoGeracaoChaves, 
                        long long tempoCriptografia, long long tempoDescriptografia,
                        int tamanhoDados, int tamanhoCifrado) {
    cout << algoritmo << endl
         << "Tempo de geração de chaves: " << tempoGeracaoChaves << " μs" << endl
         << "Tempo de criptografia: " << tempoCriptografia << " μs" << endl
         << "Tempo de descriptografia: " << tempoDescriptografia << " μs" << endl
         << "Tempo total: " << (tempoGeracaoChaves + tempoCriptografia + tempoDescriptografia) << " μs" << endl
         << "Tamanho da mensagem: " << tamanhoDados << " bytes" << endl
         << "Tamanho da cifra: " << tamanhoCifrado << " bytes" << endl << endl;
}

// Função principal para comparar algoritmos
int main() {
    string mensagem = "Teste de diferentes algoritmos de criptografia";
    int tamanhoMensagem = mensagem.size();
    const int NUM_RODADAS = 10000;
    
    long long somaTempoGeracaoRSA = 0, somaTempoCriptografiaRSA = 0, somaTempoDescriptografiaRSA = 0;
    long long somaTempoGeracaoDES = 0, somaTempoCriptografiaDES = 0, somaTempoDescriptografiaDES = 0;
    long long somaTempoGeracaoGM = 0, somaTempoCriptografiaGM = 0, somaTempoDescriptografiaGM = 0;
    
    int tamanhoCifradoRSA = 0, tamanhoCifradoDES = 0, tamanhoCifradoGM = 0;

    for (int rodada = 0; rodada < NUM_RODADAS; rodada++) {
        // Teste RSA
        int p_rsa = 9967, q_rsa = 9973;
        tuple<tuple<int, int>, tuple<int, int>> chavesRSA;
        somaTempoGeracaoRSA += medirTempo([&]() {
            chavesRSA = gerarChavesRSA(p_rsa, q_rsa);
        });
        
        auto [chavePublicaRSA, chavePrivadaRSA] = chavesRSA;
        vector<int> textoCifradoRSA;
        somaTempoCriptografiaRSA += medirTempo([&]() {
            textoCifradoRSA = criptografarRSA(mensagem, chavePublicaRSA);
        });
        
        string mensagemDecifradaRSA;
        somaTempoDescriptografiaRSA += medirTempo([&]() {
            mensagemDecifradaRSA = decriptografarRSA(textoCifradoRSA, chavePrivadaRSA);
        });
        
        if (rodada == 0) {
            tamanhoCifradoRSA = textoCifradoRSA.size() * sizeof(int);
        }
        
        // Teste DES
        uint64_t chaveDES;
        somaTempoGeracaoDES += medirTempo([&]() {
            chaveDES = gerarChaveDES();
        });
        
        vector<uint64_t> textoCifradoDES;
        somaTempoCriptografiaDES += medirTempo([&]() {
            textoCifradoDES = criptografarDES(mensagem, chaveDES);
        });
        
        string mensagemDecifradaDES;
        somaTempoDescriptografiaDES += medirTempo([&]() {
            mensagemDecifradaDES = descriptografarDES(textoCifradoDES, chaveDES);
        });
        
        if (rodada == 0) {
            tamanhoCifradoDES = textoCifradoDES.size() * sizeof(uint64_t);
        }
        
        // Teste Goldwasser-Micali
        tuple<tuple<int, int>, tuple<int, int>> chavesGM;
        somaTempoGeracaoGM += medirTempo([&]() {
            chavesGM = gerarChavesGM();
        });
        
        auto [chavePublicaGM, chavePrivadaGM] = chavesGM;
        vector<int> textoCifradoGM;
        somaTempoCriptografiaGM += medirTempo([&]() {
            textoCifradoGM = criptografarGM(mensagem, chavePublicaGM);
        });
        
        string mensagemDecifradaGM;
        somaTempoDescriptografiaGM += medirTempo([&]() {
            mensagemDecifradaGM = descriptografarGM(textoCifradoGM, chavePrivadaGM);
        });
        
        if (rodada == 0) {
            tamanhoCifradoGM = textoCifradoGM.size() * sizeof(int);
        }
    }

    // Calcula médias
    double mediaTempoGeracaoRSA = somaTempoGeracaoRSA / (double)NUM_RODADAS;
    double mediaTempoCriptografiaRSA = somaTempoCriptografiaRSA / (double)NUM_RODADAS;
    double mediaTempoDescriptografiaRSA = somaTempoDescriptografiaRSA / (double)NUM_RODADAS;
    
    double mediaTempoGeracaoDES = somaTempoGeracaoDES / (double)NUM_RODADAS;
    double mediaTempoCriptografiaDES = somaTempoCriptografiaDES / (double)NUM_RODADAS;
    double mediaTempoDescriptografiaDES = somaTempoDescriptografiaDES / (double)NUM_RODADAS;
    
    double mediaTempoGeracaoGM = somaTempoGeracaoGM / (double)NUM_RODADAS;
    double mediaTempoCriptografiaGM = somaTempoCriptografiaGM / (double)NUM_RODADAS;
    double mediaTempoDescriptografiaGM = somaTempoDescriptografiaGM / (double)NUM_RODADAS;

    // Imprime resultados
    cout << "Resultados médios após " << NUM_RODADAS << " rodadas:" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    
    imprimirResultados("RSA", mediaTempoGeracaoRSA, mediaTempoCriptografiaRSA, mediaTempoDescriptografiaRSA,
                       tamanhoMensagem, tamanhoCifradoRSA);
    imprimirResultados("DES", mediaTempoGeracaoDES, mediaTempoCriptografiaDES, mediaTempoDescriptografiaDES,
                       tamanhoMensagem, tamanhoCifradoDES);
    imprimirResultados("Goldwasser-Micali", mediaTempoGeracaoGM, mediaTempoCriptografiaGM, mediaTempoDescriptografiaGM,
                       tamanhoMensagem, tamanhoCifradoGM);
    
    cout << "-----------------------------------------------------------------" << endl;
    
    return 0;
}