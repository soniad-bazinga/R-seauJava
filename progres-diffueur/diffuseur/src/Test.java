import java.util.LinkedList;
import java.util.Stack;

public class Test {
    public static void main(String[] args)
    {
        // Creating an empty Stack
        LinkedList<String> stack = new LinkedList<>();

        // Use add() method to add elements into the Stack
        stack.add("Welcome");
        stack.add("To");
        stack.add("Geeks");
        stack.add("4");
        stack.add("Geeks");

        // Displaying the Stack
        for(int i= 0; i< 5; i++){
            System.out.println(stack.pop());
        }

        // Use add() method to add elements into the Stack
        stack.push("It ");
        stack.push("Should");
        stack.push("Be");
        stack.push("This");
        stack.push("Way");

        for(int i= 0; i< 5; i++){
            System.out.println(stack.pop());
        }
        System.out.println("Stack: " + stack);
    }
}
