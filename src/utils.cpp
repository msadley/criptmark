#include "utils.h"

namespace Utils {
std::vector<bool> stringParaBits(const std::string& texto) {
    std::vector<bool> bits;
    for (char c : texto) {
        std::bitset<8> b(c);
        for (int i = 0; i < 8; i++) {
            bits.push_back(b[i]);
        }
    }
    return bits;
}

std::string bitsParaString(const std::vector<bool>& bits) {
    std::string resultado;
    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> b;
        for (int j = 0; j < 8 && i + j < bits.size(); j++) {
            b[j] = bits[i + j];
        }
        resultado += static_cast<char>(b.to_ulong());
    }
    return resultado;
}

std::string bitsParaStringBinaria(const std::vector<bool>& bits) {
    std::string resultado;
    for (bool bit : bits) {
        resultado += bit ? '1' : '0';
    }
    return resultado;
}
}