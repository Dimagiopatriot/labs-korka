package Lab4;

/**
 * Created by troll on 09.04.2017.
 */
public class Main {

    static final int N = 4;
    static final int P = 4;
    static final int H = N/P;

    static int[] A = new int[N];
    static int[] B = new int[N];
    static int[][] MK = new int[N][N];

    private static Monitor monitor = new Monitor();

    public static void main(String[] args) {

        System.out.println("Program started!");
        MyThread thread1 = new MyThread(0, monitor);
        MyThread thread2 = new MyThread(1, monitor);
        MyThread thread3 = new MyThread(2, monitor);
        MyThread thread4 = new MyThread(3, monitor);

        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();

        System.out.println("Program finished!");
    }
}
