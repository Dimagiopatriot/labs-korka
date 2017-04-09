package Lab4;

/**
 * Created by troll on 09.04.2017.
 */
public class MyThread extends Thread {

    private int id;
    private Monitor monitor;

    MyThread(int id, Monitor monitor){
        this.id = id;
        this.monitor = monitor;
    }

    @Override
    public void run() {
        super.run();

        int locId = id;
        System.out.println("Thread " + (locId + 1) + " started!");
        switch (id) {
            case 0:
                for (int i = 0; i < Main.N; i++)
                    Main.B[i] = 1;
                //send info about input
                monitor.inputSignal();
                break;
            case 1:
                monitor.setC();
                monitor.setZ();
                for (int i = 0; i < Main.N; i++)
                    for (int j = 0; j < Main.N; j++)
                        Main.MK[i][j] = 1;
                //send info about input
                monitor.inputSignal();
                break;
            case 3:
                monitor.setT();
                monitor.setMO();
                //send info about input
                monitor.inputSignal();
                break;
        }
        // wait for signal
        monitor.waitInput();
        calculation();
        if (id == 0) {
            monitor.waitOutput();
            for (int i=0; i < Main.N; i++)
                System.out.print(Main.A[i] + " ");
            System.out.println("Thread " + (id + 1) + " finished!");
        } else {
            monitor.outputSignal();
            System.out.println("Thread " + (id + 1) + " finished!");
        }
    }

    private void calculation() {
        int start = id * Main.H;
        int end = start + Main.H;
        int[] buffV = new int[Main.N];
        int buff = 0;
        int[][] buffMatr = new int[Main.N][Main.N];

        int[] ZLocal = monitor.getZ();
        int[] CLocal = monitor.getC();
        int[] TLocal = monitor.getT();
        int[][] MOLocal = monitor.getMO();

        for (int i = id; i < end; i++){
            if (ZLocal[i] > monitor.getzMax())
                monitor.setzMax(ZLocal[i]);
        }

        int zMaxLocal = monitor.getzMax();

        for (int i = start; i < end; i++){
            buffV[i] = 0;
            for (int j = 0; j < Main.N; j++){
                buffMatr[i][j] = 0;
                for (int k = 0; k < Main.N; k++){
                    buffMatr[i][j] = buffMatr[i][j] + MOLocal[i][k] * Main.MK[k][j];
                }
                buffV[i] += buffMatr[i][j] * TLocal[j];
            }
            buffV[i] *= zMaxLocal;
            buff += Main.B[i] * CLocal[i];
        }

        monitor.addToBuff(buff);
        monitor.inputBuffSignal();
        monitor.waitInputBuff();
        buff = monitor.getBuff();

        for (int i = start; i < end; i++){
            Main.A[i] = ZLocal[i] * buff + buffV[i];
        }
    }


}
