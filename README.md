Este repositório contém implementações completas dos exercícios de programação com threads em Linux, incluindo versões em C (usando pthreads) e Java (usando java.util.concurrent).

# 💻 Exercícios de Threads Linux — POSIX

Este repositório contém implementações completas dos **exercícios de programação com threads em Linux**, incluindo versões em **C (usando pthreads)** e **Java (usando java.util.concurrent e monitores)**.  

---

## 📁 Estrutura do Projeto

```text
📁 Estrutura do Projeto

trabalhoso/
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
│   └── exercicios_posix/
│       ├── exercicio1_duplo_buffer.c
│       ├── exercicio2_filosofos.c
│       ├── exercicio3_robos.c
│       ├── exercicio4_rendezvous.c
│       └── exercicio5_barreira.c
│
├── linux-threads/
│   ├── parte1/src/threads_parte1.c
│   └── parte2/src/threads_parte2.c
│
├── entrega2/
│   └── BarbeiroDorminhocoMonitores.java
│
├── entrega3/
│   └── DeadlockBarbeiros.java
│
├── bins/
├── nomes.txt
├── Makefile
└── README.md
```

---

## 🔧 Pré-requisitos

### Windows (WSL)
- WSL instalado  
- GCC com pthread  
- Java JDK 8+  
- Make  

### Linux / macOS
- GCC com pthread  
- Java JDK 8+  
- Make  

---

## 🚀 Como Compilar e Executar

### Compilar
```bash
# Tudo
make all
# Apenas C
make c-all
# Apenas Java
make java-all
```

### Executar
```bash
# Tudo
make run-all
# Apenas C
make run-c
# Apenas Java
make run-java
```

### Limpeza
```bash
make clean
```

### Verificar WSL
```bash
make check-wsl
```

---

## 📚 Descrição dos Exercícios

### Parte 1 — Análise de Performance (`threads_parte1.c`)
- Objetivo: medir performance com múltiplas threads.  
- Função: f(x) = (x² + 1) / (2x + 3).  
- Conceitos: criação de threads, sincronização básica, medição de tempo.  

### Parte 2 — Barbeiro Adormecido (`threads_parte2.c`)
- Objetivo: implementar o problema clássico do barbeiro adormecido.  
- Sincronização: 1 mutex e 1 condition variable.  
- Conceitos: mutex, condition variables, buffer circular.  

---

### Atividades POSIX

**Atividade 1 — Produtor/Consumidor (Semáforos)**  
C: `sem_t` Java: `Semaphore` Buffer fixo e controle por semáforos.  

**Atividade 2 — Produtor/Consumidor (Condition Variables)**  
C: `pthread_cond_t` + `pthread_mutex_t` Java: `ReentrantLock` + `Condition`.  

**Atividade 3 — Leitores/Escritores (Semáforos)**  
C: prioridade para escritores Java: `Semaphore`.  

**Atividade 4 — Leitores/Escritores (RWLocks)**  
C: `pthread_rwlock_t` Java: `ReentrantReadWriteLock` (escritor preferencial).  

---

## 🧩 Entrega 2 — O Barbeiro Dorminhoco (Java / Monitores)

**Local:** `entrega2/BarbeiroDorminhocoMonitores.java`  
**Sincronização:** apenas monitores Java (`synchronized`, `wait`, `notifyAll`)  

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

## 🧩 Entrega 3 — Trabalho de Deadlocks (O Problema dos Barbeiros)

**Local:** `entrega3/DeadlockBarbeiros.java`  
**Base:** *Tanenbaum – Sistemas Operacionais Modernos*  

**Descrição:**
- Vários barbeiros e cadeiras de espera.  
- Clientes chegam → sentam se tiver vaga, senão saem.  
- Barbeiros dormem sem clientes.  
- Evita **deadlocks** e **starvation**.  
- Sincronização com monitores Java (`wait/notifyAll`).  

**Execução:**
```bash
javac entrega3/DeadlockBarbeiros.java
java -cp entrega3 DeadlockBarbeiros 2 4 10
```

**Saída esperada:**
```
Cliente 1 cortando cabelo com Barbeiro 2
Cliente 3 terminou e saiu da barbearia
Cliente 5 tentou entrar, mas estava lotada
Barbeiro 1 dormindo...
Barbeiro 2 acordou! Começando os trabalhos!
```

---

## 🎯 Conceitos Implementados

**Primitivas C:** `sem_t`, `pthread_mutex_t`, `pthread_cond_t`, `pthread_rwlock_t`  
**Primitivas Java:** `Semaphore`, `ReentrantLock`, `Condition`, `ReadWriteLock`, `wait/notifyAll`  

**Padrões de Concorrência:** Produtor/Consumidor, Leitores/Escritores, Exclusão Mútua, Sinalização, Controle de Deadlocks.  

---

## 🏃‍♂️ Exemplos de Execução

**C:**
```bash
wsl gcc -Wall -Wextra -pthread -lm -o bins/atividade1 entrega1/atividades_posix/C/atividade1.c
wsl ./bins/atividade1
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
wsl ./bins/atividade1 5 3
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

## 📝 Notas

- WSL é necessário no Windows para pthread.  
- A ordem das threads muda a cada execução.  
- Performance depende da carga do sistema.  
- C focado em Linux/Unix; Java multiplataforma.  

---

**Autor:** Equipe de Sistemas Operacionais  
**Linguagens:** C (POSIX) + Java (Concurrent / Monitores)  
**Ambiente:** Linux / WSL + Windows 
