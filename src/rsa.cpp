#include "rsa.h"
#include <cstdlib>
#include <ctime>

namespace RSA {
int mdc(int a, int b) {
    return b == 0 ? a : mdc(b, a % b);
}

std::tuple<int, int, int> mdcEulerExtendido(int a, int b) {
    if (b == 0) return {a, 1, 0};
    auto [valorMdc, x1, y1] = mdcEulerExtendido(b, a % b);
    int x = y1;
    int y = x1 - (a / b) * y1;
    return {valorMdc, x, y};
}

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

std::tuple<std::tuple<int, int>, std::tuple<int, int>> gerarChavesRSA(int p, int q) {
    srand(time(0));
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    
    int e;
    do {
        e = rand() % (phi - 2) + 2;
    } while (mdc(e, phi) != 1);
    
    auto [_, x, __] = mdcEulerExtendido(e, phi);
    int d = x < 0 ? x + phi : x;
    
    return {{e, n}, {d, n}};
}

std::vector<int> criptografarRSA(const std::string& mensagem, const std::tuple<int, int>& chavePublica) {
    int e, n;
    std::tie(e, n) = chavePublica;
    std::vector<int> textoCifrado;
    for (char c : mensagem) {
        textoCifrado.push_back(expMod(c, e, n));
    }
    return textoCifrado;
}

std::string decriptografarRSA(const std::vector<int>& textoCifrado, const std::tuple<int, int>& chavePrivada) {
    int d, n;
    std::tie(d, n) = chavePrivada;
    std::string mensagemDecifrada;
    for (int c : textoCifrado) {
        mensagemDecifrada += static_cast<char>(expMod(c, d, n));
    }
    return mensagemDecifrada;
}
}