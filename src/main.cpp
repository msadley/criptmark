#include "utils.h"
#include "rsa.h"
#include "des.h"
#include "gm.h"
#include <iostream>

using namespace std;

// Imprime resultados do benchmark
void imprimirResultados(const string& algoritmo, double tempoGeracaoChaves, 
                        double tempoCriptografia, double tempoDescriptografia,
                        int tamanhoDados, int tamanhoCifrado, bool compensado = false, 
                        double fatorGeracao = 1.0, double fatorCripto = 1.0) {
    if (compensado) {
        cout << algoritmo << " (Compensado com fatores: Geracao=" << fatorGeracao 
             << ", Cripto/Decripto=" << fatorCripto << ")" << endl
             << "Tempo de geração de chaves: " << tempoGeracaoChaves * fatorGeracao << " μs" << endl
             << "Tempo de criptografia: " << tempoCriptografia * fatorCripto << " μs" << endl
             << "Tempo de descriptografia: " << tempoDescriptografia * fatorCripto << " μs" << endl
             << "Tempo total: " << (tempoGeracaoChaves * fatorGeracao + tempoCriptografia * fatorCripto + tempoDescriptografia * fatorCripto) << " μs" << endl;
    } else {
        cout << algoritmo << endl
             << "Tempo de geração de chaves: " << tempoGeracaoChaves << " μs" << endl
             << "Tempo de criptografia: " << tempoCriptografia << " μs" << endl
             << "Tempo de descriptografia: " << tempoDescriptografia << " μs" << endl
             << "Tempo total: " << (tempoGeracaoChaves + tempoCriptografia + tempoDescriptografia) << " μs" << endl;
    }
    cout << "Tamanho da mensagem: " << tamanhoDados << " bytes" << endl
         << "Tamanho da cifra: " << tamanhoCifrado << " bytes" << endl << endl;
}

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
        somaTempoGeracaoRSA += Utils::medirTempo([&]() {
            chavesRSA = RSA::gerarChavesRSA(p_rsa, q_rsa);
        });
        
        auto [chavePublicaRSA, chavePrivadaRSA] = chavesRSA;
        vector<int> textoCifradoRSA;
        somaTempoCriptografiaRSA += Utils::medirTempo([&]() {
            textoCifradoRSA = RSA::criptografarRSA(mensagem, chavePublicaRSA);
        });
        
        string mensagemDecifradaRSA;
        somaTempoDescriptografiaRSA += Utils::medirTempo([&]() {
            mensagemDecifradaRSA = RSA::decriptografarRSA(textoCifradoRSA, chavePrivadaRSA);
        });
        
        if (rodada == 0) {
            tamanhoCifradoRSA = textoCifradoRSA.size() * sizeof(int);
        }
        
        // Teste DES
        uint64_t chaveDES;
        somaTempoGeracaoDES += Utils::medirTempo([&]() {
            chaveDES = DES::gerarChaveDES();
        });
        
        vector<uint64_t> textoCifradoDES;
        somaTempoCriptografiaDES += Utils::medirTempo([&]() {
            textoCifradoDES = DES::criptografarDES(mensagem, chaveDES);
        });
        
        string mensagemDecifradaDES;
        somaTempoDescriptografiaDES += Utils::medirTempo([&]() {
            mensagemDecifradaDES = DES::descriptografarDES(textoCifradoDES, chaveDES);
        });
        
        if (rodada == 0) {
            tamanhoCifradoDES = textoCifradoDES.size() * sizeof(uint64_t);
        }
        
        // Teste Goldwasser-Micali
        tuple<tuple<int, int>, tuple<int, int>> chavesGM;
        somaTempoGeracaoGM += Utils::medirTempo([&]() {
            chavesGM = GM::gerarChavesGM();
        });
        
        auto [chavePublicaGM, chavePrivadaGM] = chavesGM;
        vector<int> textoCifradoGM;
        somaTempoCriptografiaGM += Utils::medirTempo([&]() {
            textoCifradoGM = GM::criptografarGM(mensagem, chavePublicaGM);
        });
        
        string mensagemDecifradaGM;
        somaTempoDescriptografiaGM += Utils::medirTempo([&]() {
            mensagemDecifradaGM = GM::descriptografarGM(textoCifradoGM, chavePrivadaGM);
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

    // Fatores de compensação baseados na complexidade assintótica (k=14)
    const double K = 14.0;
    const double NORMALIZATION_CONSTANT = 500.0;
    const double FATOR_GERACAO_DES = NORMALIZATION_CONSTANT / (K * K * K * K); // 1000 / 38416 ≈ 0.02604
    const double FATOR_CRIPTO_DES = NORMALIZATION_CONSTANT / (K * K * K);      // 1000 / 2744 ≈ 0.3644

    // Imprime resultados
    cout << "Resultados médios após " << NUM_RODADAS << " rodadas:" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    
    imprimirResultados("RSA", mediaTempoGeracaoRSA, mediaTempoCriptografiaRSA, mediaTempoDescriptografiaRSA,
                       tamanhoMensagem, tamanhoCifradoRSA);
    imprimirResultados("DES", mediaTempoGeracaoDES, mediaTempoCriptografiaDES, mediaTempoDescriptografiaDES,
                       tamanhoMensagem, tamanhoCifradoDES, true, FATOR_GERACAO_DES, FATOR_CRIPTO_DES);
    imprimirResultados("Goldwasser-Micali", mediaTempoGeracaoGM, mediaTempoCriptografiaGM, mediaTempoDescriptografiaGM,
                       tamanhoMensagem, tamanhoCifradoGM);
    
    cout << "-----------------------------------------------------------------" << endl;
    
    return 0;
}