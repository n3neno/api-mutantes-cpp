#include "crow_all.h"
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

bool validade_dna(const std::vector<std::string>& dna) {
    int tamanho = dna.size();
    if (tamanho == 0) return false;

    for (int linha = 0; linha < tamanho; linha++) {
         if ((int)dna[linha].size() != tamanho) return false;
        for (int coluna = 0; coluna < tamanho; coluna++) {
            char letra = (char)std::toupper((unsigned char)dna[linha][coluna]);

            if (letra != 'A' && letra != 'T' && letra != 'C' && letra != 'G') {
                return false;
            }
        }
    }
    return true;
}

bool isSimian(const std::vector<std::string>& dna) {
    int tamanho = dna.size();
    for (int linha = 0; linha < tamanho; linha++) {
       
        for (int coluna = 0; coluna < tamanho; coluna++) {
            char letra = dna[linha][coluna];

            if (coluna + 3 < tamanho && linha + 3 < tamanho) {
                int c = 1;
                for (int i = 1; i < 4; i++) {
                    if (letra == dna[linha + i][coluna + i]) c++;
                    else c = 0;
                }
                if (c == 4) return true;
            }

            if (linha + 3 < tamanho) {
                int c = 1;
                for (int i = 1; i < 4; i++) {
                    if (letra == dna[linha + i][coluna]) c++;
                    else c = 0;
                }
                if (c == 4) return true;
            }

            if (coluna + 3 < tamanho) {
                int c = 1;
                for (int i = 1; i < 4; i++) {
                    if (letra == dna[linha][coluna + i]) c++;
                    else c = 0;
                }
                if (c == 4) return true;
            }

            if (coluna - 3 >= 0 && linha + 3 < tamanho) {
                int c = 1;
                for (int i = 1; i < 4; i++) {
                    if (letra == dna[linha + i][coluna - i]) c++;
                    else c = 0;
                }
                if (c == 4) return true;
            }
        }
    }
    return false;
}

int main() {
    crow::SimpleApp app;

    sqlite3* db;
    int rc = sqlite3_open("mutantes.db", &db);

    if (rc) {
        std::cerr << "ERRO! não foi possível abrir o banco de dados!";
        return 1;
    } else {
        std::cout << "Banco de dados conectados com sucesso!\n";
    }

    const char* sql_tabela =
        "CREATE TABLE IF NOT EXISTS verificacoes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "dna_texto TEXT UNIQUE NOT NULL, "
        "is_mutant INTEGER NOT NULL"
        ");";

    char* msg_erro = nullptr;
    rc = sqlite3_exec(db, sql_tabela, nullptr, nullptr, &msg_erro);

    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao criar tabela: " << msg_erro << "\n";
        sqlite3_free(msg_erro);
    } else {
        std::cout << "Tabela 'verificacoes' pronta para ser usada!!\n";
    }

    CROW_ROUTE(app, "/simian").methods(crow::HTTPMethod::Post)([db](const crow::request& req) {
        auto json_body = crow::json::load(req.body);

        if (!json_body || !json_body.has("dna")) {
            return crow::response(400, "ERRO, JSON invalido ou 'dna' ausente!");
        }

        std::vector<std::string> sequencia_dna;
        for (const auto& linha : json_body["dna"]) {
            sequencia_dna.push_back(linha.s());
        }

        if (!validade_dna(sequencia_dna)) {
            return crow::response(400, "DNA INVALIDO: use apenas as bases nitrogenadas A, T, C, G e matriz quadrada");
        }

        bool is_mutant = isSimian(sequencia_dna);

        std::string dna_texto = "";
        for (size_t i = 0; i < sequencia_dna.size(); i++) {
            dna_texto += sequencia_dna[i];
            if (i < sequencia_dna.size() - 1) {
                dna_texto += ",";
            }
        }

        const char* sql_insert = "INSERT INTO verificacoes (dna_texto, is_mutant) VALUES (?, ?);";
        sqlite3_stmt* stmt;

        sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, dna_texto.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, is_mutant ? 1 : 0);

        int rc_step = sqlite3_step(stmt);

        if (rc_step == SQLITE_CONSTRAINT) {
            std::cout << "DNA ja existe no banco! Nao foi duplicado\n";
        } else if (rc_step == SQLITE_DONE) {
            std::cout << "Sucesso! Novo DNA salvo no banco de dados\n";
        }

        sqlite3_finalize(stmt);
        
        if (is_mutant) {
            return crow::response(200);
        } else {
            return crow::response(403);
        }
    });

    CROW_ROUTE(app, "/stats").methods(crow::HTTPMethod::Get)([db]() {
        int c_humano = 0, c_simian = 0;
        
        const char* sql_simian = "SELECT COUNT(*) FROM verificacoes WHERE is_mutant = 1;";
        sqlite3_stmt* stmt_mutant;

        if (sqlite3_prepare_v2(db, sql_simian, -1, &stmt_mutant, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt_mutant) == SQLITE_ROW) {
                c_simian = sqlite3_column_int(stmt_mutant, 0);
            }
        }
        sqlite3_finalize(stmt_mutant);

        const char* sql_humano = "SELECT COUNT(*) FROM verificacoes WHERE is_mutant = 0;";
        sqlite3_stmt* stmt_humano;
        
        if (sqlite3_prepare_v2(db, sql_humano, -1, &stmt_humano, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt_humano) == SQLITE_ROW) {
                c_humano = sqlite3_column_int(stmt_humano, 0);
            }
        }
        sqlite3_finalize(stmt_humano);

        double proporcao = 0.0;
        if (c_humano > 0) {
            proporcao = static_cast<double>(c_simian) / c_humano;
        } else if (c_simian > 0) {
            proporcao = 1.0;
        }

        crow::json::wvalue resposta;
        resposta["count_mutant_dna"] = c_simian;
        resposta["count_human_dna"] = c_humano;
        resposta["ratio"] = proporcao;

        return crow::response(resposta);
    });

    app.port(18081).multithreaded().run();
}
