import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Random;

class SharedBufferCondition {
    private final int[] buffer;
    private final int BUFFER_SIZE;
    private int count = 0;
    private int in = 0;
    private int out = 0;
    
    private final Lock lock = new ReentrantLock();
    private final Condition notFull = lock.newCondition();
    private final Condition notEmpty = lock.newCondition();
    
    public SharedBufferCondition(int size) {
        this.BUFFER_SIZE = size;
        this.buffer = new int[BUFFER_SIZE];
    }
    
    public void produce(int item, int producerId) throws InterruptedException {
        lock.lock();
        try {
            while (count == BUFFER_SIZE) {
                notFull.await();
            }
            
            buffer[in] = item;
            System.out.printf("Produtor %d: produzido item %d na posição %d (count=%d)%n", 
                            producerId, item, in, count + 1);
            in = (in + 1) % BUFFER_SIZE;
            count++;
            
            notEmpty.signal();
        } finally {
            lock.unlock();
        }
    }
    
    public int consume(int consumerId) throws InterruptedException {
        lock.lock();
        try {
            while (count == 0) {
                notEmpty.await();
            }
            
            int item = buffer[out];
            System.out.printf("Consumidor %d: consumido item %d da posição %d (count=%d)%n", 
                            consumerId, item, out, count - 1);
            out = (out + 1) % BUFFER_SIZE;
            count--;
            
            notFull.signal();
            return item;
        } finally {
            lock.unlock();
        }
    }
}

class ProducerCondition extends Thread {
    private final SharedBufferCondition buffer;
    private final int producerId;
    private final int numItems;
    private final Random random = new Random();
    
    public ProducerCondition(SharedBufferCondition buffer, int id, int numItems) {
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

class ConsumerCondition extends Thread {
    private final SharedBufferCondition buffer;
    private final int consumerId;
    private final int numItems;
    private final Random random = new Random();
    
    public ConsumerCondition(SharedBufferCondition buffer, int id, int numItems) {
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

public class Atividade2 {
    private static final int BUFFER_SIZE = 10;
    private static final int NUM_ITEMS = 20;
    
    public static void main(String[] args) {
        int numProducers = 2;
        int numConsumers = 2;
        
        if (args.length == 2) {
            numProducers = Integer.parseInt(args[0]);
            numConsumers = Integer.parseInt(args[1]);
        }
        
        System.out.println("=== PRODUTOR/CONSUMIDOR - VARIÁVEIS DE CONDIÇÃO ===");
        System.out.println("Buffer size: " + BUFFER_SIZE);
        System.out.println("Items per thread: " + NUM_ITEMS);
        System.out.println("Produtores: " + numProducers);
        System.out.println("Consumidores: " + numConsumers);
        System.out.println("===================================================\n");
        
        SharedBufferCondition buffer = new SharedBufferCondition(BUFFER_SIZE);
        
        ProducerCondition[] producers = new ProducerCondition[numProducers];
        ConsumerCondition[] consumers = new ConsumerCondition[numConsumers];
        
        for (int i = 0; i < numProducers; i++) {
            producers[i] = new ProducerCondition(buffer, i + 1, NUM_ITEMS);
            producers[i].start();
        }
        
        for (int i = 0; i < numConsumers; i++) {
            consumers[i] = new ConsumerCondition(buffer, i + 1, NUM_ITEMS);
            consumers[i].start();
        }
        
        try {
            for (ProducerCondition producer : producers) {
                producer.join();
            }
            
            for (ConsumerCondition consumer : consumers) {
                consumer.join();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        
        System.out.println("\n=== EXECUÇÃO FINALIZADA ===");
    }
}