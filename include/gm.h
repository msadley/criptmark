#ifndef GM_H
#define GM_H

#include <tuple>
#include <vector>
#include <string>

namespace GM {
// Verifica se um número é primo
bool ehPrimo(int n);

// Calcula o símbolo de Legendre (n|p)
int simboloLegendre(int n, int p);

// Encontra um não-resíduo quadrático módulo p
int encontrarNaoResiduo(int p);

// Gera chaves para Goldwasser-Micali
std::tuple<std::tuple<int, int>, std::tuple<int, int>> gerarChavesGM();

// Verifica se dois números são coprimos
bool saoCoprimos(int a, int b);

// Criptografa uma mensagem com Goldwasser-Micali
std::vector<int> criptografarGM(const std::string& mensagem, const std::tuple<int, int>& chavePublica);

// Descriptografa uma mensagem com Goldwasser-Micali
std::string descriptografarGM(const std::vector<int>& textoCifrado, const std::tuple<int, int>& chavePrivada);
}

#endif // GM_H