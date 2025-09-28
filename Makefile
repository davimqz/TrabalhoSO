# Makefile Principal para Exercícios de Threads Linux
# Utiliza WSL para compilação no ambiente Windows

CC = gcc
CFLAGS = -Wall -Wextra -pthread -lm
BINDIR = bins

# Definir os binários e seus arquivos fonte
BINARIES = threads_parte1 threads_parte2 atividade1 atividade2 atividade3 atividade4
JAVA_ACTIVITIES = Atividade1 Atividade2 Atividade3 Atividade4

# Regra padrão
all: create_bindir c-all

# Criar diretório de binários
create_bindir:
	@if [ ! -d "$(BINDIR)" ]; then mkdir -p $(BINDIR); fi

# Compilar todos os programas C
c-all: $(BINARIES)

# Compilar todos os programas Java
java-all:
	@echo "Compilando programas Java..."
	cd atividades_posix/Java && javac *.java

# Regras específicas de compilação para C
threads_parte1: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/threads_parte1 linux-threads/parte1/src/threads_parte1.c

threads_parte2: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/threads_parte2 linux-threads/parte2/src/threads_parte2.c

atividade1: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/atividade1 atividades_posix/C/atividade1.c

atividade2: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/atividade2 atividades_posix/C/atividade2.c

atividade3: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/atividade3 atividades_posix/C/atividade3.c

atividade4: create_bindir
	wsl $(CC) $(CFLAGS) -o $(BINDIR)/atividade4 atividades_posix/C/atividade4.c

# Executar todos os programas C
run-c: c-all
	@echo "=== THREADS PARTE 1 ==="
	wsl ./$(BINDIR)/threads_parte1
	@echo
	@echo "=== THREADS PARTE 2 ==="
	wsl ./$(BINDIR)/threads_parte2
	@echo
	@echo "=== ATIVIDADE 1: PRODUTOR/CONSUMIDOR COM SEMÁFOROS ==="
	wsl ./$(BINDIR)/atividade1
	@echo
	@echo "=== ATIVIDADE 2: PRODUTOR/CONSUMIDOR COM CONDITION VARIABLES ==="
	wsl ./$(BINDIR)/atividade2
	@echo
	@echo "=== ATIVIDADE 3: LEITORES/ESCRITORES COM SEMÁFOROS ==="
	wsl ./$(BINDIR)/atividade3
	@echo
	@echo "=== ATIVIDADE 4: LEITORES/ESCRITORES COM RWLOCKS ==="
	wsl ./$(BINDIR)/atividade4

# Executar programas Java
run-java: java-all
	@echo "=== JAVA ATIVIDADE 1: PRODUTOR/CONSUMIDOR COM SEMÁFOROS ==="
	cd atividades_posix/Java && java Atividade1
	@echo
	@echo "=== JAVA ATIVIDADE 2: PRODUTOR/CONSUMIDOR COM CONDITION VARIABLES ==="
	cd atividades_posix/Java && java Atividade2
	@echo
	@echo "=== JAVA ATIVIDADE 3: LEITORES/ESCRITORES COM SEMÁFOROS ==="
	cd atividades_posix/Java && java Atividade3
	@echo
	@echo "=== JAVA ATIVIDADE 4: LEITORES/ESCRITORES COM RWLOCKS ==="
	cd atividades_posix/Java && java Atividade4

# Executar todos os programas (C e Java)
run-all: run-c run-java

# Limpeza
clean:
	rm -rf $(BINDIR)
	cd atividades_posix/Java && rm -f *.class
	
# Alvo para verificar se o WSL está disponível
check-wsl:
	@wsl echo "WSL está funcionando!"

.PHONY: all create_bindir c-all java-all run-c run-java run-all clean check-wsl