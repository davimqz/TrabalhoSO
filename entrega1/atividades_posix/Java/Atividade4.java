import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.Random;

class SharedResourceRW {
    private int data;
    private final ReadWriteLock rwLock = new ReentrantReadWriteLock(true);
    
    public SharedResourceRW(int initialData) {
        this.data = initialData;
    }
    
    public void read(int readerId) throws InterruptedException {
        rwLock.readLock().lock();
        try {
            System.out.printf("Leitor %d: lendo dados = %d%n", readerId, data);
            Thread.sleep(new Random().nextInt(200));
        } finally {
            rwLock.readLock().unlock();
        }
    }
    
    public void write(int writerId, int newData) throws InterruptedException {
        rwLock.writeLock().lock();
        try {
            System.out.printf("Escritor %d: escrevendo dados %d -> %d%n", 
                            writerId, data, newData);
            data = newData;
            Thread.sleep(new Random().nextInt(250));
        } finally {
            rwLock.writeLock().unlock();
        }
    }
    
    public int getData() {
        rwLock.readLock().lock();
        try {
            return data;
        } finally {
            rwLock.readLock().unlock();
        }
    }
}

class ReaderRW extends Thread {
    private final SharedResourceRW resource;
    private final int readerId;
    private final int numOperations;
    private final Random random = new Random();
    
    public ReaderRW(SharedResourceRW resource, int id, int numOperations) {
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

class WriterRW extends Thread {
    private final SharedResourceRW resource;
    private final int writerId;
    private final int numOperations;
    private final Random random = new Random();
    
    public WriterRW(SharedResourceRW resource, int id, int numOperations) {
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

public class Atividade4 {
    private static final int NUM_OPERATIONS = 5;
    
    public static void main(String[] args) {
        int numReaders = 3;
        int numWriters = 2;
        
        if (args.length == 2) {
            numReaders = Integer.parseInt(args[0]);
            numWriters = Integer.parseInt(args[1]);
        }
        
        System.out.println("=== LEITORES/ESCRITORES - RWLOCKS (PRIORIDADE ESCRITORES) ===");
        System.out.println("Operações por thread: " + NUM_OPERATIONS);
        System.out.println("Leitores: " + numReaders);
        System.out.println("Escritores: " + numWriters);
        System.out.println("=============================================================\n");
        
        SharedResourceRW resource = new SharedResourceRW(42);
        
        WriterRW[] writers = new WriterRW[numWriters];
        ReaderRW[] readers = new ReaderRW[numReaders];
        
        for (int i = 0; i < numWriters; i++) {
            writers[i] = new WriterRW(resource, i + 1, NUM_OPERATIONS);
            writers[i].start();
        }
        
        for (int i = 0; i < numReaders; i++) {
            readers[i] = new ReaderRW(resource, i + 1, NUM_OPERATIONS);
            readers[i].start();
        }
        
        try {
            for (WriterRW writer : writers) {
                writer.join();
            }
            
            for (ReaderRW reader : readers) {
                reader.join();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        
        System.out.println("\n=== EXECUÇÃO FINALIZADA ===");
        System.out.println("Valor final dos dados: " + resource.getData());
    }
}