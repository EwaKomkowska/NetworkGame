package Controllers;

import Tasks.ReadTask;
import Tasks.SendTask;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.ScrollPane;
import javafx.scene.layout.AnchorPane;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Controller {

    private InputStream in;
    private OutputStream out;

    @FXML
    private Button firstButton;

    @FXML
    private Button secondButton;

    @FXML
    private Button thirdButton;

    @FXML
    private Button fourthButton;

   @FXML
   private AnchorPane anchorPane;

   public static ObservableList <String> data;


    @FXML
    private void initialize() throws IOException {
        firstButton.setText(Main.getButton1());
        secondButton.setText(Main.getButton2());
        thirdButton.setText(Main.getButton3());
        fourthButton.setText(Main.getButton4());

        ListView <String> list = new ListView<>();
        list.setPrefHeight(75);
        list.setPrefWidth(300);
        list.setLayoutX(100);
        list.setLayoutY(10);


        ScrollPane scrollPane = new ScrollPane();
        //scrollPane.prefWidthProperty().bind(list.widthProperty());
        //scrollPane.prefHeightProperty().bind(list.heightProperty());

        scrollPane.setLayoutX(90);
        scrollPane.setLayoutY(10);

        ObservableList <String> data = FXCollections.observableArrayList();
        data.addAll(Main.getText());
        System.out.println("Data: " + data);
        list.setItems(data);


        scrollPane.setContent(list);
        scrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        anchorPane.getChildren().add(scrollPane);

        listenResponses();
    }

    @FXML
    private void chooseFirst() throws IOException {
        System.out.println("Zagłosowałam na opcje 1");

        SendTask sendTask = new SendTask("1", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        //Main.getMainStage().close();
    }

    @FXML
    private void chooseSecond() throws IOException {
        System.out.println("Zagłosowałam na opcje 2");

        SendTask sendTask = new SendTask("2", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
    }

    @FXML
    private void chooseThird() throws IOException {
        System.out.println("Zagłosowałam na opcje 3");

        SendTask sendTask = new SendTask("3", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        //Main.getMainStage().close();
    }

    @FXML
    private void chooseFourth() throws IOException {
        System.out.println("Zagłosowałam na opcje 4");

        SendTask sendTask = new SendTask("4", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        System.out.println("Wysyłam komunikat o zaglosowaniu");
        //Main.getMainStage().close();
    }


    private void receiveMessage(String[] responseParts) {
        try {
            switch (responseParts[2]) {
                case "Q":       //pytanie
                    Main.setText(responseParts[3]);     //przekazujemy tez na main, żeby móc dodac odpowiedź
                    data.add(Main.getText());
                    break;
                case "A":
                    firstButton.setText(responseParts[3]);
                    break;
                case "B":
                    secondButton.setText(responseParts[3]);
                    break;
                case "C":
                    thirdButton.setText(responseParts[3]);
                    break;
                case "D":
                    fourthButton.setText(responseParts[3]);
                    break;
                case "5":       //odpowiedź
                    data.add(responseParts[3]);
            }
        }catch (Exception e) {
            System.out.println("To nie było pytanie, odpowiedź ani wynik głosowania");
        }
        // po otrzymaniu wiadomości kontynuujemy nasłuchiwanie odpowiedzi od serwera
        listenResponses();
    }



    public void listenResponses() {
        ReadTask readTask = new ReadTask(in);
        new Thread(readTask).start();
        readTask.addEventHandler(WorkerStateEvent.WORKER_STATE_SUCCEEDED, new EventHandler<WorkerStateEvent>() {
            @Override
            public void handle(WorkerStateEvent t) {
                String response = readTask.getValue();
                if (response != null) {
                    //TODO: jaki znacznik rozdziela wiadomosci?
                    String[] responseParts = response.split("&&");
                    if (responseParts[1].equals("1")) {
                        System.out.println("Dostałem pytania, odpowiedzi lub dalsza czesc");
                        for (String i : responseParts) {
                            System.out.println(i);
                        }
                        receiveMessage(responseParts);
                    }
                }
            }
        });
    }



    @FXML
    private void exit () {
        //TODO: zamknac gniazdo, zwolnic pamiec
        SendTask sendTask = new SendTask("Koniec", out);
        new Thread(sendTask).start();

        Main.getMainStage().close();
    }
}
