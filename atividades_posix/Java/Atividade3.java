import java.util.concurrent.Semaphore;
import java.util.Random;

class SharedResource {
    private int data;
    private int readCount = 0;
    
    private final Semaphore mutex = new Semaphore(1);
    private final Semaphore writeSem = new Semaphore(1);
    private final Semaphore readCountMutex = new Semaphore(1);
    
    public SharedResource(int initialData) {
        this.data = initialData;
    }
    
    public void read(int readerId) throws InterruptedException {
        readCountMutex.acquire();
        readCount++;
        if (readCount == 1) {
            writeSem.acquire();
        }
        readCountMutex.release();
        
        System.out.printf("Leitor %d: lendo dados = %d (leitores ativos: %d)%n", 
                        readerId, data, readCount);
        Thread.sleep(new Random().nextInt(200));
        
        readCountMutex.acquire();
        readCount--;
        if (readCount == 0) {
            writeSem.release();
        }
        readCountMutex.release();
    }
    
    public void write(int writerId, int newData) throws InterruptedException {
        writeSem.acquire();
        
        System.out.printf("Escritor %d: escrevendo dados %d -> %d%n", 
                        writerId, data, newData);
        data = newData;
        Thread.sleep(new Random().nextInt(250));
        
        writeSem.release();
    }
    
    public int getData() {
        return data;
    }
}

class Reader extends Thread {
    private final SharedResource resource;
    private final int readerId;
    private final int numOperations;
    private final Random random = new Random();
    
    public Reader(SharedResource resource, int id, int numOperations) {
        this.resource = resource;
        this.readerId = id;
        this.numOperations = numOperations;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 0; i < numOperations; i++) {
                resource.read(readerId);
                Thread.sleep(random.nextInt(300));
            }
            System.out.printf("Leitor %d: finalizando%n", readerId);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class Writer extends Thread {
    private final SharedResource resource;
    private final int writerId;
    private final int numOperations;
    private final Random random = new Random();
    
    public Writer(SharedResource resource, int id, int numOperations) {
        this.resource = resource;
        this.writerId = id;
        this.numOperations = numOperations;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 0; i < numOperations; i++) {
                int newData = random.nextInt(1000) + 1;
                resource.write(writerId, newData);
                Thread.sleep(random.nextInt(400));
            }
            System.out.printf("Escritor %d: finalizando%n", writerId);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class Atividade3 {
    private static final int NUM_OPERATIONS = 5;
    
    public static void main(String[] args) {
        int numReaders = 3;
        int numWriters = 2;
        
        if (args.length == 2) {
            numReaders = Integer.parseInt(args[0]);
            numWriters = Integer.parseInt(args[1]);
        }
        
        System.out.println("=== LEITORES/ESCRITORES - SEMÁFOROS (PRIORIDADE ESCRITORES) ===");
        System.out.println("Operações por thread: " + NUM_OPERATIONS);
        System.out.println("Leitores: " + numReaders);
        System.out.println("Escritores: " + numWriters);
        System.out.println("===============================================================\n");
        
        SharedResource resource = new SharedResource(42);
        
        Writer[] writers = new Writer[numWriters];
        Reader[] readers = new Reader[numReaders];
        
        for (int i = 0; i < numWriters; i++) {
            writers[i] = new Writer(resource, i + 1, NUM_OPERATIONS);
            writers[i].start();
        }
        
        for (int i = 0; i < numReaders; i++) {
            readers[i] = new Reader(resource, i + 1, NUM_OPERATIONS);
            readers[i].start();
        }
        
        try {
            for (Writer writer : writers) {
                writer.join();
            }
            
            for (Reader reader : readers) {
                reader.join();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        
        System.out.println("\n=== EXECUÇÃO FINALIZADA ===");
        System.out.println("Valor final dos dados: " + resource.getData());
    }
}