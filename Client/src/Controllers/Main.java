package Controllers;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
/*import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;*/

public class Main extends Application {

    static Scene scene;
    private static String button1;
    private static String button2;
    private static String button3;
    private static String button4;
    private static Stage mainStage;
    private static boolean gra;

    public static Integer port;
    public static String host;

    /*static SocketChannel sock;
    static Selector sel;                           // selektor – opakowuje mechanizm typu 'select'
    static SelectionKey sockKey;
    static ByteBuffer bb = ByteBuffer.allocate(10); // bufor – odpowiednio opakowana tablica bajtów*/


    public static boolean getGra() {
        return gra;
    }

    public static void setGra(boolean gra) {
        Main.gra = gra;
    }

    public static Scene getScene() {
        return scene;
    }

    public static void setScene(Scene scene) {
        Main.scene = scene;
    }

    public static String getButton1() {
        return button1;
    }

    public static void setButton1(String button1) {
        Main.button1 = button1;
    }

    public static String getButton2() {
        return button2;
    }

    public static void setButton2(String button2) {
        Main.button2 = button2;
    }

    public static String getButton3() {
        return button3;
    }

    public static void setButton3(String button3) {
        Main.button3 = button3;
    }

    public static String getButton4() {
        return button4;
    }

    public static void setButton4(String button4) {
        Main.button4 = button4;
    }

    public static Stage getMainStage() {
        return mainStage;
    }

    public static void setMainStage(Stage mainStage) {
        Main.mainStage = mainStage;
    }

    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }

    @Override
    public void start(Stage stage) throws IOException{
        scene = new Scene(loadFXML("/resources/start"), 500, 400);
        stage.setScene(scene);
        stage.setTitle("Client choice");
        stage.show();
        Main.setMainStage(stage);
    }

    public static void setRoot(String fxml) throws IOException {
        scene.setRoot(loadFXML(fxml));
    }

    public static void main(String[] args) throws IOException {
        Main.setButton1("Option 1");            //to jest tylko zmienna przechowujaca tekst, a nie przycisk
        Main.setButton2("Option 2");
        Main.setButton3("Option 3");
        Main.setButton4("Option 4");

        //coś na zasadzie dopóki nie dołączy do gry to nie idź dalej!!!
        //while (!getGra()) {
            System.out.println("Czekam na start gry!");     //TODO: Dane z args
            Main.host = "localhost";        //args[0];
            Main.port = 1234;                    //Integer.parseInt(args[1]);
            launch(args);
        //}
    }
}
