import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

public class BarbeiroDorminhocoMonitores {
    static class Pessoa {
        int id;
        Pessoa(int id){ this.id=id; }
        int getID(){ return id; }
    }

    static class Cliente extends Thread {
        int id;
        Barbearia barbearia;
        final Object monitor = new Object();
        volatile boolean terminou=false;
        volatile boolean saiu=false;
        Barbeiro barbeiroAtual=null;
        Cliente(int id, Barbearia b){ this.id=id; this.barbearia=b; }
        public void run(){
            for(;;){
                boolean ok = barbearia.cortaCabelo(this);
                if(!ok) System.out.println("Cliente "+id+" tentou entrar na barbearia, mas está lotada… indo dar uma voltinha");
                synchronized(monitor){
                    while(ok && !terminou){
                        try{ monitor.wait(); }catch(Exception e){}
                    }
                    if(ok){
                        System.out.println("Cliente "+id+" terminou o corte… saindo da barbearia");
                        saiu=true;
                        monitor.notifyAll();
                    }
                }
                dorme(ThreadLocalRandom.current().nextInt(3000,5001));
            }
        }
    }

    static class Barbeiro extends Thread {
        int id;
        Barbearia barbearia;
        Barbeiro(int id, Barbearia b){ this.id=id; this.barbearia=b; }
        public void run(){
            for(;;){
                Cliente c = barbearia.proximoCliente(this);
                if(c!=null){
                    dorme(ThreadLocalRandom.current().nextInt(1000,3001));
                    barbearia.corteTerminado(this, c);
                }
            }
        }
    }

    static class Barbearia {
        final int nBarbeiros;
        final int cadeirasEspera;
        int atendendo=0;
        final Deque<Cliente> fila = new ArrayDeque<>();
        final Set<Barbeiro> dormindo = new HashSet<>();

        Barbearia(int nBarbeiros, int cadeirasEspera){
            this.nBarbeiros=nBarbeiros;
            this.cadeirasEspera=cadeirasEspera;
        }

        boolean cortaCabelo(Cliente c){
            synchronized(this){
                boolean lotada = (fila.size()+atendendo) >= (cadeirasEspera + nBarbeiros);
                if(lotada) return false;
                System.out.println("Cliente "+c.id+" esperando corte...");
                fila.addLast(c);
                notifyAll();
            }
            synchronized(c.monitor){
                c.terminou=false;
                c.saiu=false;
            }
            return true;
        }

        Cliente proximoCliente(Barbeiro b){
            synchronized(this){
                while(fila.isEmpty()){
                    dormindo.add(b);
                    System.out.println("Barbeiro "+b.id+" indo dormir um pouco… não há clientes na barbearia...");
                    try{ wait(); }catch(Exception e){}
                    dormindo.remove(b);
                    System.out.println("Barbeiro "+b.id+" acordou. Começando os trabalhos.");
                }
                Cliente c = fila.removeFirst();
                atendendo++;
                c.barbeiroAtual = b;
                System.out.println("Cliente "+c.id+" cortando cabelo com Barbeiro "+b.id);
                return c;
            }
        }

        void corteTerminado(Barbeiro b, Cliente c){
            synchronized(this){
                atendendo--;
                notifyAll();
            }
            synchronized(c.monitor){
                c.terminou=true;
                c.monitor.notifyAll();
                while(!c.saiu){
                    try{ c.monitor.wait(); }catch(Exception e){}
                }
            }
        }
    }

    static void dorme(int ms){
        try{ Thread.sleep(ms); }catch(Exception e){}
    }

    public static void main(String[] args){
        int n=2, m=4, total=6;
        if(args.length>0) n=Integer.parseInt(args[0]);
        if(args.length>1) m=Integer.parseInt(args[1]);
        if(args.length>2) total=Integer.parseInt(args[2]);

        Barbearia barbearia = new Barbearia(n,m);

        Barbeiro[] bs = new Barbeiro[n];
        for(int i=0;i<n;i++){ bs[i]=new Barbeiro(i+1, barbearia); bs[i].start(); }

        Cliente[] cs = new Cliente[total];
        for(int i=0;i<total;i++){ cs[i]=new Cliente(i+1, barbearia); cs[i].start(); }
    }
}
