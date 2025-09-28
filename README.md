# Exercícios de Threads Linux - POSIX

Este repositório contém implementações completas dos exercícios de programação com threads em Linux, incluindo versões em **C** (usando pthreads) e **Java** (usando java.util.concurrent).

## 📁 Estrutura do Projeto

```
trabalhoso/
├── linux-threads/
│   ├── parte1/src/threads_parte1.c    # Análise de performance com threads
│   └── parte2/src/threads_parte2.c    # Problema do barbeiro adormecido
├── atividades_posix/
│   ├── C/
│   │   ├── atividade1.c              # Produtor/Consumidor com Semáforos
│   │   ├── atividade2.c              # Produtor/Consumidor com Condition Variables
│   │   ├── atividade3.c              # Leitores/Escritores com Semáforos
│   │   └── atividade4.c              # Leitores/Escritores com RWLocks
│   └── Java/
│       ├── Atividade1.java           # Produtor/Consumidor com Semaphore
│       ├── Atividade2.java           # Produtor/Consumidor com Lock/Condition
│       ├── Atividade3.java           # Leitores/Escritores com Semaphore
│       └── Atividade4.java           # Leitores/Escritores com ReadWriteLock
├── bins/                             # Diretório para executáveis C
├── nomes.txt                         # Lista de nomes para os exercícios
└── Makefile                          # Build system completo
```

## 🔧 Pré-requisitos

### Windows (WSL)
- **WSL** (Windows Subsystem for Linux) instalado
- **GCC** com suporte a pthread no WSL
- **Java JDK** (versão 8 ou superior)
- **Make** (disponível no WSL)

### Linux/macOS
- **GCC** com suporte a pthread
- **Java JDK** (versão 8 ou superior)
- **Make**

## 🚀 Como Compilar e Executar

### Compilar Todos os Programas
```bash
# Compilar apenas programas C
make c-all

# Compilar apenas programas Java
make java-all

# Compilar tudo (padrão)
make all
```

### Executar Programas
```bash
# Executar apenas programas C
make run-c

# Executar apenas programas Java
make run-java

# Executar tudo
make run-all
```

### Limpeza
```bash
# Remover binários e arquivos .class
make clean
```

### Verificar WSL (Windows)
```bash
# Testar se WSL está funcionando
make check-wsl
```

## 📚 Descrição dos Exercícios

### Parte 1: Análise de Performance (`threads_parte1.c`)
- **Objetivo**: Medir performance de computação matemática com múltiplas threads
- **Conceitos**: Criação de threads, sincronização básica, medição de tempo
- **Função**: Cálculo de `f(x) = (x² + 1) / (2x + 3)` em paralelo

### Parte 2: Barbeiro Adormecido (`threads_parte2.c`)
- **Objetivo**: Implementar o problema clássico do barbeiro adormecido
- **Conceitos**: Mutex, condition variables, buffer circular
- **Sincronização**: 1 mutex e 1 condition variable

### Atividade 1: Produtor/Consumidor com Semáforos
- **C**: Usa `sem_t` (POSIX semaphores)
- **Java**: Usa `java.util.concurrent.Semaphore`
- **Buffer**: Tamanho fixo de 10 elementos
- **Sincronização**: Semáforos para controle de recursos

### Atividade 2: Produtor/Consumidor com Condition Variables
- **C**: Usa `pthread_cond_t` e `pthread_mutex_t`
- **Java**: Usa `ReentrantLock` e `Condition`
- **Controle**: Baseado em contadores e condições

### Atividade 3: Leitores/Escritores com Semáforos
- **C**: Implementação com prioridade para escritores
- **Java**: Equivalente usando `Semaphore` para coordenação
- **Política**: Múltiplos leitores simultâneos, escritor exclusivo

### Atividade 4: Leitores/Escritores com RWLocks
- **C**: Usa `pthread_rwlock_t` com `PTHREAD_RWLOCK_PREFER_WRITER_NP`
- **Java**: Usa `ReentrantReadWriteLock` com política de escritor preferencial
- **Otimização**: Locks otimizados para leitura/escrita

## 🎯 Conceitos Implementados

### Primitivas de Sincronização em C
- **Semáforos POSIX** (`sem_t`)
- **Mutex** (`pthread_mutex_t`)
- **Condition Variables** (`pthread_cond_t`)
- **Read-Write Locks** (`pthread_rwlock_t`)

### Equivalentes em Java
- **Semaphore** (`java.util.concurrent.Semaphore`)
- **ReentrantLock** (`java.util.concurrent.locks.ReentrantLock`)
- **Condition** (`java.util.concurrent.locks.Condition`)
- **ReadWriteLock** (`java.util.concurrent.locks.ReadWriteLock`)

### Padrões de Concorrência
- **Produtor/Consumidor**: Coordenação de produção e consumo de recursos
- **Leitores/Escritores**: Controle de acesso para leitura e escrita concorrente
- **Exclusão Mútua**: Acesso exclusivo a recursos compartilhados
- **Sinalização**: Comunicação entre threads

## 🏃‍♂️ Exemplos de Execução

### Executar Atividade Específica (C)
```bash
# Compilar e executar produtor/consumidor com semáforos
wsl gcc -Wall -Wextra -pthread -lm -o bins/atividade1 atividades_posix/C/atividade1.c
wsl ./bins/atividade1
```

### Executar Atividade Específica (Java)
```bash
# Compilar e executar leitores/escritores com ReadWriteLock
cd atividades_posix/Java
javac Atividade4.java
java Atividade4
```

### Parâmetros Customizáveis
Muitos programas aceitam parâmetros para customizar o número de threads:
```bash
# C: 5 produtores, 3 consumidores
wsl ./bins/atividade1 5 3

# Java: 4 leitores, 2 escritores
java Atividade4 4 2
```

## 📊 Características dos Programas

### Performance e Medição
- Todos os programas incluem medição de tempo de execução
- Saída formatada para análise de resultados
- Logs detalhados das operações das threads

### Robustez
- Tratamento adequado de erros
- Cleanup de recursos (join de threads)
- Inicialização correta de primitivas de sincronização

### Configurabilidade
- Número de threads personalizável
- Tempos de sleep ajustáveis
- Tamanhos de buffer configuráveis

## 🔍 Debugging e Análise

### Verificação de Deadlocks
Os programas são implementados seguindo padrões que evitam deadlocks:
- Ordem consistente de aquisição de locks
- Timeouts em operações críticas
- Liberação adequada de recursos

### Análise de Concorrência
Para analisar o comportamento:
- Observe a saída dos logs para verificar intercalação de threads
- Use ferramentas como `valgrind --tool=helgrind` (Linux)
- Monitor de performance para verificar utilização de CPU

## 📝 Notas Importantes

- **WSL**: Necessário no Windows para compilação com pthread
- **Ordem de Execução**: As threads podem executar em ordem diferente a cada execução
- **Performance**: Os resultados de tempo podem variar conforme a carga do sistema
- **Portabilidade**: Código C compatível com Linux/Unix, Java é multiplataforma

## 🎓 Valor Educacional

Este conjunto de exercícios cobre:
- **Programação Concorrente**: Conceitos fundamentais de threads
- **Sincronização**: Diferentes mecanismos e suas aplicações
- **Problemas Clássicos**: Produtor/consumidor, leitores/escritores, barbeiro
- **Comparação de Linguagens**: Implementações equivalentes em C e Java
- **Boas Práticas**: Código limpo, tratamento de erros, documentação

---
**Autor**: Exercícios de Sistemas Operacionais  
**Linguagens**: C (POSIX Threads) + Java (java.util.concurrent)  
**Ambiente**: Linux/WSL + Windows