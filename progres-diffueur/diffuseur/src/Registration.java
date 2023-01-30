import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;

public class Registration implements Runnable{
    private final String id_diff;
    private final String address;
    private final int port1;
    private final int port2;
    private final int portGest;
    private final String ipGest;


    Registration(String id_diff, String address, int port1, int port2, int portGest, String ipGest){
        this.id_diff= id_diff;
        this.address= address;
        this.port1= port1;
        this.port2= port2;
        this.portGest = portGest;
        this.ipGest = ipGest;
    }

    @Override
    public void run() {
        try{
            Socket socket= new Socket(ipGest, portGest);
            System.out.println("-------------------------------------------------------\n"
                            +  " Le diffuseur est enregistré au gestionnaire port 5050\n"
                            +  "-------------------------------------------------------\n");
            BufferedReader br= new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter pw= new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
            pw.print("REGI "+ id_diff+ " "+ address+ " "+ port1+ " "+Util.ip(InetAddress.getLocalHost().getHostAddress())+" "+ port2+"\r\n");
            pw.flush();
            String msg= br.readLine();
            if(msg.equals("RENO")){
                System.err.println(msg);

            }else{
                System.out.println(msg);
            }
            while(true){
                msg = br.readLine();
                if(msg.equals("RUOK")){
                    pw.print("IMOK\r\n");
                    pw.flush();
                }else{
                    pw.print("chuis le message print"+msg);
                    pw.flush();
                }
            }

        } catch (Exception e) {
            System.out.println("Il semblerait qu'il y ait une erreur, aucun gestionnaire ne tourne port "+portGest+".\nVeuillez réitérer votre demande.");
        }
    }
}
