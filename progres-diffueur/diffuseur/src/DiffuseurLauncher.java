import java.io.FileNotFoundException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Scanner;

public class DiffuseurLauncher {
    public static void main(String[] args) throws FileNotFoundException, UnknownHostException {
        /*TODO: il faut lire les informations concernant le diffuseur à partir d'un fichier */
        System.out.println("Sur quel port souhaitez-vous écouter les clients ? (Par défaut 5151)");
        Scanner sc = new Scanner(System.in);
        String port1 = sc.nextLine();
        while (!port1.matches("\\d\\d\\d\\d") && !port1.equals("")){
            System.out.println("Le numéro de port doit contenir quatre chiffres ! Recommencez.");
            port1 = sc.nextLine();
        }
        if (port1.equals("")) port1 = "5151";
        System.out.println("Sur quel port souhaitez-vous placer la multi-diffusion ? (Par défaut 5252)");
        String port2 = sc.nextLine();
        while (!port2.matches("\\d\\d\\d\\d") && !port2.equals("")){
            System.out.println("Le numéro de port doit contenir quatre chiffres ! Recommencez.");
            port2 = sc.nextLine();
        }
        if (port2.equals("")) port2 = "5252";
        
        Diffuseur diffuseur= new Diffuseur(Integer.parseInt(port1), Integer.parseInt(port2), "DIF_TWO", "225.010.020.030");
        diffuseur.setPile("crash2.txt");
        diffuseur.diffuse();
        diffuseur.connect();
        System.out.println("-------------------------------------------------------\n"
                        +  "        Le diffuseur diffuse sur le port "+port2
                        +  "\n            et écoute sur le port "+port1
                        +  ".\n   Il est sur la machine d'ip : "+Util.ip(InetAddress.getLocalHost().getHostAddress())
                        + "\n-------------------------------------------------------");
        String msg; 
        while(true){
            msg = sc.nextLine();
            if(msg.equals("REGI")) {
                /*pour s'enregistrer auprès du gestionnaire*/
                diffuseur.register();
            }else if(msg.equals("LIST")){
                System.out.println("Choisissez l'adresse ip du gestionnaire que vous souhaitez interroger.");
                String ip = Util.ip(sc.nextLine());
                System.err.println("Sur quel port tourne le gestionnaire ?\n(Par défaut 5050.)");
                String port = "initialise";
                port = sc.nextLine();
                while (!port.matches("\\d\\d\\d\\d") && !port.equals("")){
                    System.err.println("Le numéro de port doit contenir quatre chiffres ! Recommencez.");
                    port = sc.nextLine();
                }
                if (port.equals("")) port="5050";
                    diffuseur.list(Util.ip(ip), Integer.parseInt(port));
            }else{
                System.out.println("Les commandes acceptées sont :\n- REGI : enregistrement du diffuseur auprès du gestionnaire.\n- LIST : lister les diffuseurs enregistrés.");
            }
        }
    }
}
