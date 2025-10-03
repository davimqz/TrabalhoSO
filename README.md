Este repositório contém implementações completas dos exercícios de programação com threads em Linux, incluindo versões em C (usando pthreads) e Java (usando java.util.concurrent).

📁 Estrutura do Projeto
trabalhoso/
├── linux-threads/
│   ├── parte1/src/threads_parte1.c
│   └── parte2/src/threads_parte2.c
├── exercicios_posix/
│   ├── exercicio1_duplo_buffer.c
│   ├── exercicio2_filosofos.c
│   ├── exercicio3_robos.c
│   ├── exercicio4_rendezvous.c
│   └── exercicio5_barreira.c
├── atividades_posix/
│   ├── C/
│   │   ├── atividade1.c
│   │   ├── atividade2.c
│   │   ├── atividade3.c
│   │   └── atividade4.c
│   └── Java/
│       ├── Atividade1.java
│       ├── Atividade2.java
│       ├── Atividade3.java
│       └── Atividade4.java
├── entrega2/
│   └── atividade2/
│       └── BarbeiroDorminhocoMonitores.java
├── bins/
├── nomes.txt
└── Makefile

🔧 Pré-requisitos
Windows (WSL)

WSL instalado

GCC com pthread no WSL

Java JDK (8+)

Make

Linux/macOS

GCC com pthread

Java JDK (8+)

Make

🚀 Como Compilar e Executar
Compilar Todos os Programas
# Apenas C
make c-all

# Apenas Java
make java-all

# Tudo
make all

Executar Programas
# Apenas C
make run-c

# Apenas Java
make run-java

# Tudo
make run-all

Limpeza
make clean

Verificar WSL (Windows)
make check-wsl

📚 Descrição dos Exercícios
Parte 1: Análise de Performance (threads_parte1.c)

Objetivo: medir performance com múltiplas threads

Conceitos: criação de threads, sincronização básica, medição de tempo

Função: f(x) = (x² + 1) / (2x + 3)

Parte 2: Barbeiro Adormecido (threads_parte2.c)

Objetivo: implementar o problema clássico do barbeiro adormecido

Conceitos: mutex, condition variables, buffer circular

Sincronização: 1 mutex e 1 condition variable

Atividade 1: Produtor/Consumidor com Semáforos

C: sem_t

Java: Semaphore

Buffer fixo

Sincronização por semáforos

Atividade 2: Produtor/Consumidor com Condition Variables

C: pthread_cond_t + pthread_mutex_t

Java: ReentrantLock + Condition

Controle por contadores/condições

Atividade 3: Leitores/Escritores com Semáforos

C: prioridade para escritores

Java: Semaphore

Política: leitores simultâneos, escritor exclusivo

Atividade 4: Leitores/Escritores com RWLocks

C: pthread_rwlock_t

Java: ReentrantReadWriteLock

Política de escritor preferencial

🧩 Atividade 2 (Arquitetura) — Barbeiro Dorminhoco (Java, Monitores)

Local: entrega2/atividade2/BarbeiroDorminhocoMonitores.java
Sincronização: apenas monitores Java (synchronized, wait, notifyAll)
Parâmetros (CLI): n_barbeiros m_cadeiras total_clientes
Mensagens: estados de cliente (esperando, cortando, terminou/saindo, barbearia lotada) e barbeiro (dormindo/acordou)

Compilar
javac entrega2/atividade2/BarbeiroDorminhocoMonitores.java

Executar (exemplo)
java -cp entrega2/atividade2 BarbeiroDorminhocoMonitores 2 4 6

🎯 Conceitos Implementados
Primitivas de Sincronização em C

sem_t, pthread_mutex_t, pthread_cond_t, pthread_rwlock_t

Equivalentes em Java

Semaphore, ReentrantLock, Condition, ReadWriteLock

Padrões de Concorrência

Produtor/Consumidor, Leitores/Escritores, Exclusão Mútua, Sinalização

🏃‍♂️ Exemplos de Execução
C (exemplo)
wsl gcc -Wall -Wextra -pthread -lm -o bins/atividade1 atividades_posix/C/atividade1.c
wsl ./bins/atividade1

Java (exemplo)
cd atividades_posix/Java
javac Atividade4.java
java Atividade4

Parâmetros customizáveis
# C
wsl ./bins/atividade1 5 3

# Java
java Atividade4 4 2

📊 Características

Medição de tempo

Saída organizada

join e limpeza de recursos

Números de threads, sleeps e buffers configuráveis

🔍 Debug/Análise

Evita deadlocks por ordem de locks e sinalização correta

Dicas: valgrind --tool=helgrind (Linux), observar logs

📝 Notas

WSL necessário no Windows para pthread

Intercalação de threads varia por execução

Performance depende da carga do sistema

C focado em Linux/Unix; Java multiplataforma

Autor: Exercícios de Sistemas Operacionais
Linguagens: C (POSIX) + Java (concurrent + monitores)
Ambiente: Linux/WSL + Windows