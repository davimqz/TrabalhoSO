# 🗒 Exercícios de Threads Linux — POSIX
<sup>Este repositório contém implementações completas dos exercícios de programação com threads em Linux, incluindo versões em C (usando pthreads) e Java (usando java.util.concurrent).</sup>

---

## 📁 Estrutura do Projeto

```text
📁 Estrutura do Projeto

trabalhoso/
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── launch.json
│   └── settings.json
│
├── entrega1/
│   ├── atividades_posix/
│   │   ├── C/
│   │   │   ├── atividade1.c
│   │   │   ├── atividade2.c
│   │   │   ├── atividade3.c
│   │   │   └── atividade4.c
│   │   └── Java/
│   │       ├── Atividade1.java
│   │       ├── Atividade2.java
│   │       ├── Atividade3.java
│   │       └── Atividade4.java
│   │
│   ├── exercicios_posix/
│   │   ├── exercicio1_duplo_buffer.c
│   │   ├── exercicio2_filosofos.c
│   │   ├── exercicio3_robos.c
│   │   ├── exercicio4_rendezvous.c
│   │   └── exercicio5_barreira.c
│   │
│   └── linux-threads/
│       ├── parte1/
│       │   ├── scripts/
│       │   └── src/
│       │       └── threads_parte1.c
│       └── parte2/
│           └── src/
│               └── threads_parte2.c
│
├── entrega2/
│   └── BarbeiroDorminhocoMonitores.java
│
├── nomes.txt
├── Makefile
└── README.md
```

---

## 🔧 Pré-requisitos
### 💻 Para Windows
* **WSL (Subsistema do Windows para Linux)** devidamente instalado.
* Compilador **GCC** com suporte a `pthread` (instalado no WSL).
* **Java JDK** (versão 8 ou superior).
* Utilitário **Make**.

### 💻 Para Linux / macOS
* Compilador **GCC** com suporte a `pthread`.
* **Java JDK** (versão 8 ou superior).
* Utilitário **Make**.

<br>

## ♟️ Comandos Disponíveis

Use os seguintes comandos `make` no seu terminal para gerenciar o projeto.

| Comando         | Descrição                                                  |
| :-------------- | :--------------------------------------------------------- |
| `make all`      | Compila todos os códigos-fonte (C e Java).                 |
| `make c-all`    | Compila apenas os códigos em C.                            |
| `make java-all` | Compila apenas os códigos em Java.                         |
| `make run-all`  | Executa o projeto principal.                               |
| `make run-c`    | Executa apenas a parte feita em C.                         |
| `make run-java` | Executa apenas a parte feita em Java.                      |
| `make clean`    | Remove todos os arquivos gerados pela compilação.          |
| `make check-wsl`| **(Apenas Windows)** Verifica a configuração do WSL.       |

<br>

## 📚 Descrição dos Exercícios

### Parte 1 — Análise de Performance
**Arquivo:** `entrega1/linux-threads/parte1/src/threads_parte1.c`  
- Objetivo: medir performance com múltiplas threads.  
- Função: f(x) = (x² + 1) / (2x + 3).  
- Conceitos: criação de threads, sincronização básica, medição de tempo.  

### Parte 2 — Barbeiro Adormecido
**Arquivo:** `entrega1/linux-threads/parte2/src/threads_parte2.c`  
- Objetivo: implementar o problema clássico do barbeiro adormecido.  
- Sincronização: 1 mutex e 1 condition variable.  
- Conceitos: mutex, condition variables, buffer circular.  

---

### Atividades POSIX (entrega1)

**Atividade 1 — Produtor/Consumidor (Semáforos)**  
C: `sem_t` Java: `Semaphore` Buffer fixo e controle por semáforos.  

**Atividade 2 — Produtor/Consumidor (Condition Variables)**  
C: `pthread_cond_t` + `pthread_mutex_t` Java: `ReentrantLock` + `Condition`.  

**Atividade 3 — Leitores/Escritores (Semáforos)**  
C: prioridade para escritores Java: `Semaphore`.  

**Atividade 4 — Leitores/Escritores (RWLocks)**  
C: `pthread_rwlock_t` Java: `ReentrantReadWriteLock` (escritor preferencial).  

---

<br>

## 💈 Entrega 2 — O Barbeiro Dorminhoco (Java / Semáforos)
<sup>Esta implementação resolve o problema clássico do **Barbeiro Dorminhoco** utilizando **Semáforos** para controlar a concorrência entre as threads de Barbeiros e Clientes em Java.</sup>

**Local do Arquivo:** `entrega2/BarbeiroDorminhoco.java`

### 📝 Descrição do Problema

A barbearia possui `n` barbeiros, uma cadeira de corte para cada um, e `m` cadeiras de espera para os clientes.
- Se não há clientes, os barbeiros dormem.
- Ao chegar um cliente, ele acorda um barbeiro para cortar o cabelo.
- Se todos os barbeiros estiverem ocupados, o cliente senta em uma cadeira de espera vaga.
- Se todas as cadeiras de espera estiverem ocupadas, o cliente vai embora e volta outra hora.

O desafio é coordenar essas ações usando threads sem gerar condições de corrida ou deadlocks, garantindo que os clientes sejam atendidos na ordem em que chegaram.

<br>

**Parâmetros (CLI):**
```
n_barbeiros m_cadeiras total_clientes
```

**Compilar:**
```bash
javac entrega2/BarbeiroDorminhocoMonitores.java
```

**Executar (exemplo):**
```bash
java -cp entrega2 BarbeiroDorminhocoMonitores 2 4 6
```

---

## 🎯 Conceitos Implementados

**Primitivas C:** `sem_t`, `pthread_mutex_t`, `pthread_cond_t`, `pthread_rwlock_t`  
**Primitivas Java:** `Semaphore`, `ReentrantLock`, `Condition`, `ReadWriteLock`, `wait/notifyAll`  

**Padrões de Concorrência:** Produtor/Consumidor, Leitores/Escritores, Exclusão Mútua, Sinalização.  

---

## 🏃‍♂️ Exemplos de Execução

**C:**
```bash
gcc -Wall -Wextra -pthread -lm -o atividade1 entrega1/atividades_posix/C/atividade1.c
./atividade1
```

**Java:**
```bash
cd entrega1/atividades_posix/Java
javac Atividade4.java
java Atividade4
```

**Parâmetros:**
```bash
# C
./atividade1 5 3
# Java
java Atividade4 4 2
```

---

## 📊 Características

- Medição de tempo e saída organizada.  
- join() e liberação de recursos correta.  
- Evita condições de corrida e deadlocks.  
- Número de threads, sleep e buffers configuráveis.  

---

## 🔍 Debug / Análise

- Use `valgrind --tool=helgrind` no Linux.  
- Observe os logs para verificar interleaving.  
- Ordem de locks garante segurança contra deadlocks.  


---


## 👩‍💻 Membros

<ul>
  <li>Erick Belo — eab2@cesar.school 📩</li>
  <li>Gabriel Belo — gnbo2@cesar.school 📩</li>
  <li>Davi Marques — dmo3@cesar.school 📩</li>
  <li>Luiz Claudio — lcpmf@cesar.school 📩</li>
  <li>João Lucas Robalinho — jlvrf@cesar.school 📩</li>
</ul>

**Autor:** Equipe de Sistemas Operacionais  
**Linguagens:** C (POSIX) + Java (Concurrent / Monitores)  
**Ambiente:** Linux / WSL + Window
