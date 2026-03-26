# Teste de Símios - Em C++

API Rest construída para detectar símios e humanos, o usuário fornece o DNA que está em formato de matriz de string, aceitando somente as bases nitrogenadas: A, C, G e T. O símio é detectado se uma ou mais sequências de quatro "letras" iguais nas direções horizontais, verticais ou nas diagonais.

## Tecnologias Utilizadas

* C++
* Crow Framework : Framework em C++ para utilizar protocolo HTTP, foi utilizado pela sua facilidade de aplicação neste tipo de projeto
* SQLite3 : Banco de dados leve e embutido
* Docker : Portabilidade facilitada (Linux, macOS, Windows, nuvem) e consistência em qualquer ambiente
* Render : Hospedagem na nuvem

## Como usar a API

A API possui duas rotas principais. É possível usar o PowerShell, cURL ou ferramentas como Postman/Insomnia.

> ** AVISO!** Devido ao uso do Render, a API demorará para responder o primeiro comando pois ela está "acordando". Este processo pode levar de 30 a 50s.

> ** Devido ao uso do plano gratuito do Render, caso o servidor reinicie por inatividade, os dados salvos no SQLite serão resetados.

---

### POST /simian — Detectar Símio

Recebe um JSON com a matriz de DNA e verifica se pertence a um símio.

* **200 OK** → Símio
* **403 Forbidden** → Humano

#### PowerShell

```powershell
Invoke-WebRequest -Uri "https://api-mutantes-cpp.onrender.com/simian" -Method POST -ContentType "application/json" -Body '{"dna":["ATGCGA","CAGTGC","TTATGT","AGAAGG","CCCCTA","TCACTG"]}'
```

#### cURL (Linux / macOS / Git Bash)

```bash
curl -X POST https://api-mutantes-cpp.onrender.com/simian \
  -H "Content-Type: application/json" \
  -d '{"dna":["ATGCGA","CAGTGC","TTATGT","AGAAGG","CCCCTA","TCACTG"]}'
```

> Neste caso a resposta seria **HTTP 200-OK (símio)**.

---

### GET /stats — Estatísticas

Retorna a quantidade de DNAs símios, humanos e a proporção entre eles.

#### PowerShell

```powershell
(Invoke-WebRequest -Uri "https://api-mutantes-cpp.onrender.com/stats" -Method GET).Content
```

#### cURL (Linux / macOS / Git Bash)

```bash
curl https://api-mutantes-cpp.onrender.com/stats
```

#### Navegador

Acesse diretamente: https://api-mutantes-cpp.onrender.com/stats

Exemplo de resposta:

```json
{"count_mutant_dna": 40, "count_human_dna": 100, "ratio": 0.4}
```

