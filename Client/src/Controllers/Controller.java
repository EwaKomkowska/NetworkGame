package Controllers;

import Tasks.ReadTask;
import Tasks.SendTask;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.*;
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

   @FXML
   private TextArea wynikA;

   @FXML
   private TextArea wynikB;

   @FXML
   private TextArea wynikC;

   @FXML
   private TextArea wynikD;

   public static ObservableList <String> data;
   private ListView <String> list;


    @FXML
    private void initialize() throws IOException {
        in = StartWindow.getIn();
        firstButton.setText(Main.getButton1());
        secondButton.setText(Main.getButton2());
        thirdButton.setText(Main.getButton3());
        fourthButton.setText(Main.getButton4());

        list= new ListView<>();
        list.setPrefHeight(125);
        list.setPrefWidth(400);
        list.setLayoutX(150);
        list.setLayoutY(60);


        ScrollPane scrollPane = new ScrollPane();
        //scrollPane.prefWidthProperty().bind(list.widthProperty());
        //scrollPane.prefHeightProperty().bind(list.heightProperty());

        scrollPane.setLayoutX(90);
        scrollPane.setLayoutY(10);

        data = FXCollections.observableArrayList();
        data.addAll(Main.getText());
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


    private void receiveMessage(String[] responseParts, int i) {
        try {//TODO: jak na biezaco odswiezac tekst w buttonie
            switch (responseParts[i+1]) {
                case "Q":       //pytanie
                case "5":       //odpowiedz
                    Main.setText(responseParts[i+2]);
                    this.data.addAll(responseParts[i+2]);    //TODO: cos nie dziala z tym data - NULLPointer
                    list.setItems(data);
                    break;
                case "A":
                    Main.setButton1(responseParts[i+2]);
                    firstButton.setText(responseParts[i+2]);
                    wynikA.setText(responseParts[i+2]);
                    break;
                case "B":
                    Main.setButton2(responseParts[i+2]);
                    secondButton.setText(responseParts[i+2]);
                    wynikB.setText(responseParts[i+2]);
                    break;
                case "C":
                    Main.setButton3(responseParts[i+2]);
                    thirdButton.setText(responseParts[i+2]);
                    wynikC.setText(responseParts[i+2]);
                    break;
                case "D":
                    Main.setButton4(responseParts[i+2]);
                    fourthButton.setText(responseParts[i+2]);
                    wynikD.setText(responseParts[i+2]);
                    break;
            }
        }catch (Exception e) {
            e.printStackTrace();
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

                    for (int i = 0; i < responseParts.length; i++) {
                        if (responseParts[i].equals("1")) {
                            /*System.out.println("Dostałem pytania, odpowiedzi lub dalsza czesc z listen responses");
                            for (int j = 0; j < responseParts.length; j++) {
                                System.out.println(j + ": " + responseParts[j] + " dla i: " + i);
                            }*/
                            receiveMessage(responseParts, i);
                            break;
                        }
                    }
                }
                else listenResponses();
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
