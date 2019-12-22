package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.awt.*;
import java.io.IOException;

public class Main extends Application {

    static Scene scene;
    static String button1;
    static String button2;
    static String button3;
    static String button4;
    static Stage mainStage;


    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(Main.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }

    @Override
    public void start(Stage stage) throws IOException{
        scene = new Scene(loadFXML("start"), 500, 400);
        stage.setScene(scene);
        stage.setTitle("Client choice");
        stage.show();
        mainStage = stage;
    }

    public static void setRoot(String fxml) throws IOException {
        scene.setRoot(loadFXML(fxml));
    }

    public static void main(String[] args) {
        button1 = "Option 1";
        button2 = "Option 2";
        button3 = "Option 3";
        button4 = "Option 4";
        launch(args);
    }
}
