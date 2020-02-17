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

import javax.swing.*;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Controller {

    private InputStream in;
    private OutputStream out;

    int numerPytania = 0;

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
        out = StartWindow.getOut();
        firstButton.setText(Main.getButton1());
        secondButton.setText(Main.getButton2());
        thirdButton.setText(Main.getButton3());
        fourthButton.setText(Main.getButton4());

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        wynikA.setText(Main.getWynikA());
        wynikB.setText(Main.getWynikB());
        wynikC.setText(Main.getWynikC());
        wynikD.setText(Main.getWynikD());

        list= new ListView<>();
        list.setPrefHeight(125);
        list.setPrefWidth(450);
        list.setLayoutX(80);
        list.setLayoutY(60);


        ScrollPane scrollPane = new ScrollPane();

        scrollPane.setLayoutX(80);
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
        System.out.println("I vote on option 1");

        //TODO: dodac numer pytania
        SendTask sendTask = new SendTask("&&" + numerPytania + "&&1", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
    }

    @FXML
    private void chooseSecond() throws IOException {
        System.out.println("I vote on option 2");

        SendTask sendTask = new SendTask("&&" + numerPytania + "&&2", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
    }

    @FXML
    private void chooseThird() throws IOException {
        System.out.println("I vote on option 3");

        SendTask sendTask = new SendTask("&&" + numerPytania + "&&3", out);
        new Thread(sendTask).start();

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
    }

    @FXML
    private void chooseFourth() throws IOException {
        SendTask sendTask = new SendTask("&&" + numerPytania + "&&4", out);
        new Thread(sendTask).start();

        System.out.println("I vote on option 4");

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
    }


    private void receiveMessage(String[] responseParts, int i) {
        try {
            switch (responseParts[i+1]) {
                case "Q":       //pytanie //odpowiedz
                    Main.setText(responseParts[i+2]);
                    data.addAll(responseParts[i+2]);
                    list.setItems(data);
                    numerPytania = Integer.parseInt(responseParts[i]);
                    if (responseParts[i+2].equals("Koniec")) {
                        firstButton.setVisible(false);
                        secondButton.setVisible(false);
                        thirdButton.setVisible(false);
                        fourthButton.setVisible(false);
                        Main.setGra(false);
                        Thread.sleep(1000);
                        exit();
                    } else {        //if question/answer - we can't vote
                        firstButton.setDisable(true);
                        secondButton.setDisable(true);
                        thirdButton.setDisable(true);
                        fourthButton.setDisable(true);
                    }
                    break;
                case "A":
                    Main.setButton1(responseParts[i+2]);
                    firstButton.setText(responseParts[i+2]);
                    firstButton.setDisable(false);
                    break;
                case "B":
                    Main.setButton2(responseParts[i+2]);
                    secondButton.setText(responseParts[i+2]);
                    secondButton.setDisable(false);
                    break;
                case "C":
                    Main.setButton3(responseParts[i+2]);
                    thirdButton.setText(responseParts[i+2]);
                    thirdButton.setDisable(false);
                    break;
                case "D":
                    Main.setButton4(responseParts[i+2]);
                    fourthButton.setText(responseParts[i+2]);
                    fourthButton.setDisable(false);
                    break;
                case "s1":
                    wynikA.setText(responseParts[i+2]);
                    break;
                case "s2":
                    wynikB.setText(responseParts[i+2]);
                    break;
                case "s3":
                    wynikC.setText(responseParts[i+2]);
                    break;
                case "s4":
                    wynikD.setText(responseParts[i+2]);
                    break;
            }
        }catch (Exception e) {
            System.out.println("Error with text messsages\n");
            System.out.println(responseParts[i+1]);
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
                            try {
                                Integer.parseInt(responseParts[i]);     //TODO: sprawdzamy czy jest numer pytania
                                receiveMessage(responseParts, i);       //tu był break, ale nie odczytuje wtedy więcej wiadomości
                            } catch (NumberFormatException e) {
                            }
                        }
                    }
                    listenResponses();
                }
            });
    }



    @FXML
    private void exit () {
        Main.getMainStage().close();
        try {
            in.close();
            out.close();
            StartWindow.getSocket().close();
            StartWindow.getIn().close();
            StartWindow.getOut().close();
        }catch (IOException e) {
            System.out.println("There is a problem with close connection.");
        }

    }
}
