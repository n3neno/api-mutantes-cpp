# Teste de Símios - Em C++

API Rest construída para dedectar símios e humanos, o usuário fornece o DNA que está em formato de matriz de string, aceitando somente as bases nitrogenadas: A, C, G e T. O símio é dedectado se uma ou mais sêquencias de quatro "letras" iguais nas direçoes horizontais, verticais ou nas diagonais.

## Tecnologias Utilizadas
* C++;
* Crow Framework : Framework em C++ para utilizar protocolo HTTP, foi utilizado pela sua facilidade de aplicação neste tipo de projeto;
* SQLite3 : Banco de dados leve e embutido;
* Docker : Portabilidade facilitada(Linux, macOS, Windows, nuvem..), e consistência em qualquer ambiente;
* Render : Hospedagem na nuvem;

## Como usar a API
A API possui duas rotas principais. É possível usar o PowerShell, cURL ou ferramentas como PostMan/Insonmia

## AVISO! Devido ao uso do Render, a API demorara para responder o primeiro comando pois ela está "acordando"! Este processo pode levar de 30 a 50s.
### Dedectar Símio ('POST/ simian')
Recebe um JSON com a matriz de DNA e verefica se pertence a um símio
* Retorno 200 OK: Símio
* Retorno 403 Forbidden: Humano

### Exemplo de requisição:
* No powershell (**'POST/ simian'**): Invoke-WebRequest -Uri "[https://api-mutantes-cpp.onrender.com/simian](https://api-mutantes-cpp.onrender.com/simian)" -Method POST -ContentType "application/json" -Body '{"dna":["ATGCGA","CAGTGC","TTATGT","AGAAGG","CCCCTA","TCACTG"]}'
  ### **Neste caso a resposta seria HTTP 200-OK (símio), deve-se escrever o DNA desta forma para a requisição ser executada!!**
* No powershell (**'GET / status'**): (Invoke-WebRequest -Uri "https://api-mutantes-cpp.onrender.com/stats" -Method GET).Content
* No site, quando este estiver totalmente carregado (**'GET / status'**): https://api-mutantes-cpp.onrender.com/stats

