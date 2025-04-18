#ifndef RSA_H
#define RSA_H

#include <tuple>
#include <vector>
#include <string>

namespace RSA {
// Calcula o máximo divisor comum
int mdc(int a, int b);

// Algoritmo de Euclides Estendido para inverso modular
std::tuple<int, int, int> mdcEulerExtendido(int a, int b);

// Exponenciação modular (base^expoente mod modulo)
int expMod(int base, int expoente, int modulo);

// Gera chaves pública e privada para RSA
std::tuple<std::tuple<int, int>, std::tuple<int, int>> gerarChavesRSA(int p, int q);

// Criptografa uma mensagem com RSA
std::vector<int> criptografarRSA(const std::string& mensagem, const std::tuple<int, int>& chavePublica);

// Descriptografa uma mensagem com RSA
std::string decriptografarRSA(const std::vector<int>& textoCifrado, const std::tuple<int, int>& chavePrivada);
}

#endif // RSA_H