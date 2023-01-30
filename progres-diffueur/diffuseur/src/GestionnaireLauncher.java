import java.util.Scanner;
public class GestionnaireLauncher {
    public static void main(String[] args) {
        System.out.println("Sur quel port souhaitez-vous que votre gestionnaire écoute ?\n(Par défaut on place l'écoute dur le port 5050.)");
        Scanner sc = new Scanner(System.in);
        String port = sc.nextLine();
        while (!port.matches("\\d\\d\\d\\d") && !port.equals("")){
            System.out.println("Le numéro de port doit contenir quatre chiffres ! Recommencez.");
            port = sc.nextLine();
        }
        if (port.equals("")) port = "5050";
        Gestionnaire gestionnaire = new Gestionnaire(Integer.parseInt(port));
        gestionnaire.gestionne();
    }
}
