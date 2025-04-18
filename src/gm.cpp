#include "gm.h"
#include "rsa.h"
#include "utils.h"
#include <random>

namespace GM {
bool ehPrimo(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int simboloLegendre(int n, int p) {
    if (n % p == 0) return 0;
    int result = RSA::expMod(n, (p - 1) / 2, p);
    return result == p - 1 ? -1 : result;
}

int encontrarNaoResiduo(int p) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, p - 1);
    
    while (true) {
        int x = dis(gen);
        if (simboloLegendre(x, p) == -1) {
            return x;
        }
    }
}

std::tuple<std::tuple<int, int>, std::tuple<int, int>> gerarChavesGM() {
    int p = 9967, q = 9973;
    int n = p * q;
    
    int y;
    do {
        y = encontrarNaoResiduo(p);
    } while (simboloLegendre(y, q) != -1);
    
    return {{n, y}, {p, q}};
}

bool saoCoprimos(int a, int b) {
    return RSA::mdc(a, b) == 1;
}

std::vector<int> criptografarGM(const std::string& mensagem, const std::tuple<int, int>& chavePublica) {
    int n, y;
    std::tie(n, y) = chavePublica;
    std::vector<bool> bits = Utils::stringParaBits(mensagem);
    std::vector<int> textoCifrado;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, n - 1);
    
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

std::string descriptografarGM(const std::vector<int>& textoCifrado, const std::tuple<int, int>& chavePrivada) {
    int p, q;
    std::tie(p, q) = chavePrivada;
    std::vector<bool> bits;
    
    for (int c : textoCifrado) {
        bool bit = simboloLegendre(c, p) == -1;
        bits.push_back(bit);
    }
    
    return Utils::bitsParaString(bits);
}
}