import java.util.concurrent.Semaphore;
import java.util.Random;

class SharedBuffer {
    private final int[] buffer;
    private final int BUFFER_SIZE;
    private int in = 0;
    private int out = 0;
    
    private final Semaphore empty;
    private final Semaphore full;
    private final Object mutex = new Object();
    
    public SharedBuffer(int size) {
        this.BUFFER_SIZE = size;
        this.buffer = new int[BUFFER_SIZE];
        this.empty = new Semaphore(BUFFER_SIZE);
        this.full = new Semaphore(0);
    }
    
    public void produce(int item, int producerId) throws InterruptedException {
        empty.acquire();
        
        synchronized (mutex) {
            buffer[in] = item;
            System.out.printf("Produtor %d: produzido item %d na posição %d%n", 
                            producerId, item, in);
            in = (in + 1) % BUFFER_SIZE;
        }
        
        full.release();
    }
    
    public int consume(int consumerId) throws InterruptedException {
        full.acquire();
        
        int item;
        synchronized (mutex) {
            item = buffer[out];
            System.out.printf("Consumidor %d: consumido item %d da posição %d%n", 
                            consumerId, item, out);
            out = (out + 1) % BUFFER_SIZE;
        }
        
        empty.release();
        return item;
    }
}

class Producer extends Thread {
    private final SharedBuffer buffer;
    private final int producerId;
    private final int numItems;
    private final Random random = new Random();
    
    public Producer(SharedBuffer buffer, int id, int numItems) {
        this.buffer = buffer;
        this.producerId = id;
        this.numItems = numItems;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 0; i < numItems; i++) {
                int item = random.nextInt(100) + 1;
                buffer.produce(item, producerId);
                Thread.sleep(random.nextInt(100));
            }
            System.out.printf("Produtor %d: finalizando%n", producerId);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class Consumer extends Thread {
    private final SharedBuffer buffer;
    private final int consumerId;
    private final int numItems;
    private final Random random = new Random();
    
    public Consumer(SharedBuffer buffer, int id, int numItems) {
        this.buffer = buffer;
        this.consumerId = id;
        this.numItems = numItems;
    }
    
    @Override
    public void run() {
        try {
            for (int i = 0; i < numItems; i++) {
                buffer.consume(consumerId);
                Thread.sleep(random.nextInt(150));
            }
            System.out.printf("Consumidor %d: finalizando%n", consumerId);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class Atividade1 {
    private static final int BUFFER_SIZE = 10;
    private static final int NUM_ITEMS = 20;
    
    public static void main(String[] args) {
        int numProducers = 2;
        int numConsumers = 2;
        
        if (args.length == 2) {
            numProducers = Integer.parseInt(args[0]);
            numConsumers = Integer.parseInt(args[1]);
        }
        
        System.out.println("=== PRODUTOR/CONSUMIDOR - SEMÁFOROS ===");
        System.out.println("Buffer size: " + BUFFER_SIZE);
        System.out.println("Items per thread: " + NUM_ITEMS);
        System.out.println("Produtores: " + numProducers);
        System.out.println("Consumidores: " + numConsumers);
        System.out.println("========================================\n");
        
        SharedBuffer buffer = new SharedBuffer(BUFFER_SIZE);
        
        Producer[] producers = new Producer[numProducers];
        Consumer[] consumers = new Consumer[numConsumers];
        
        for (int i = 0; i < numProducers; i++) {
            producers[i] = new Producer(buffer, i + 1, NUM_ITEMS);
            producers[i].start();
        }
        
        for (int i = 0; i < numConsumers; i++) {
            consumers[i] = new Consumer(buffer, i + 1, NUM_ITEMS);
            consumers[i].start();
        }
        
        try {
            for (Producer producer : producers) {
                producer.join();
            }
            
            for (Consumer consumer : consumers) {
                consumer.join();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        
        System.out.println("\n=== EXECUÇÃO FINALIZADA ===");
    }
}