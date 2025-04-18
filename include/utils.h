#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <chrono>
#include <bitset>

namespace Utils {
// Converte uma string para um vetor de bits
std::vector<bool> stringParaBits(const std::string& texto);

// Converte um vetor de bits para uma string
std::string bitsParaString(const std::vector<bool>& bits);

// Converte um vetor de bits para uma string binária (0s e 1s)
std::string bitsParaStringBinaria(const std::vector<bool>& bits);

// Mede o tempo de execução de uma função
template<typename Func, typename... Args>
long long medirTempo(Func&& func, Args&&... args) {
    auto inicio = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto fim = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio).count();
}
}

#endif // UTILS_H