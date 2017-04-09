package Lab4;

/**
 * Created by troll on 09.04.2017.
 */
public class Monitor {

    private int[] C = new int[Main.N];
    private int[] Z = new int[Main.N];
    private int zMax = Integer.MIN_VALUE;
    private int[] T = new int[Main.N];
    private int[][] MO = new int[Main.N][Main.N];
    private int F1, F2, F3;
    private int buff = 0;

    Monitor(){
        F1 = 0;
        F2 = 0;
        F3 = 0;
    }

    synchronized void setC() {
        inputVector(C);
    }

    synchronized void setZ() {
        inputVector(Z);
    }

    synchronized void setzMax(int zMax) {
        this.zMax = zMax;
    }

    synchronized void setT() {
        inputVector(T);
    }

    synchronized void setMO() {
        inputMatrix(MO);
    }

    synchronized int[] getC() {

        return C;
    }

    synchronized int[] getZ() {
        return Z;
    }

    synchronized int getzMax() {
        return zMax;
    }

    synchronized int[] getT() {
        return T;
    }

    synchronized int[][] getMO() {
        return MO;
    }

    synchronized int getBuff(){
        return buff;
    }

    synchronized void addToBuff(int add){
        buff += add;
    }

    private void inputVector(int[] V){
        for (int i=0; i<Main.N; i++)
            V[i] = 1;
    }

    private void inputMatrix(int[][] M){
        for (int i=0; i<Main.N; i++)
            for (int j=0; j<Main.N; j++)
                M[i][j] = 1;
    }

    //waiting scalar vector multiplying in all threads
    synchronized void waitInputBuff() {
        while (F3 < 4){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    //input in vector buff signal
    synchronized void inputBuffSignal(){
        F3++;
        notifyAll();
    }

    // waiting input from T1, T2, T4
    synchronized void waitInput() {
        while (F2 < 3){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    // send signal about inputing to other threads
    synchronized void inputSignal(){
        F2++;
        notifyAll();
    }

    // for waiting end results from threads
    synchronized void waitOutput(){
        while (F1 < Main.P - 1){
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    // send signal for ending calculations
    synchronized void outputSignal(){
        F1++;
        notifyAll();
    }
}
