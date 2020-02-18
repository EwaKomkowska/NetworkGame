package Tasks;

import Controllers.Main;
import Controllers.StartWindow;
import javafx.concurrent.Task;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;

import java.io.IOException;
import java.net.Socket;

public class ConnectTask extends Task<Void> {

    private StartWindow st;

    public ConnectTask (StartWindow st) {
        this.st = st;
    }

    @Override
    protected Void call() throws Exception {
        while (!Main.getGra()) {
            try {
                st.setSocket(new Socket(Main.host, Main.port));         //TODO: zmienic przyjmowane dane na Main.host, Main.port
                st.setIn(StartWindow.getSocket().getInputStream());
                st.setOut(StartWindow.getSocket().getOutputStream());

                ReadTask message = new ReadTask(StartWindow.getIn());
                new Thread(message).start();        //watek odczytywania zostal uruchomiony

                message.addEventHandler(WorkerStateEvent.WORKER_STATE_SUCCEEDED, new EventHandler<WorkerStateEvent>() {
                    @Override
                    public void handle(WorkerStateEvent t) {
                        String response = message.getValue();
                        //System.out.println("response" + response);
                        while (response != null) {
                            String [] messageValue = response.split("&&");  //TODO: jaki mamy znak oddzielajacy?

                            switch (messageValue[1]) {
                                case "0":
                                    //TODO:Serwer sie uruchomil co dalej?
                                    System.out.println("Start game");
                                    break;
                                default:
                                    //System.out.println("Możliwe, że dostałem pytania, odpowiedzi lub dalsza czesc");
                                    //TODO: czy moze tak zostac, ze jest default'owe założenie, że przyszło pytanie?
                                    try {
                                        switch (messageValue[2]) {
                                            case "Q":       //pytanie lub odpowiedź
                                                Main.setText(Main.getText() + messageValue[3] + "\n");
                                                break;
                                            case "A":
                                                Main.setButton1(messageValue[3]);
                                                break;
                                            case "B":
                                                Main.setButton2(messageValue[3]);
                                                break;
                                            case "C":
                                                Main.setButton3(messageValue[3]);
                                                break;
                                            case "D":
                                                Main.setButton4(messageValue[3]);
                                                break;
                                            case "s1":
                                                Main.setWynikA(messageValue[3]);
                                                break;
                                            case "s2":
                                                Main.setWynikB(messageValue[3]);
                                                break;
                                            case "s3":
                                                Main.setWynikC(messageValue[3]);
                                                break;
                                            case "s4":
                                                Main.setWynikD(messageValue[3]);
                                                break;
                                        }
                                    }catch (Exception e) {
                                        System.out.println("It isn't a question\n");
                                    }
                                    break;
                            }

                        response = response.substring(5, response.length());
                        if (!response.contains("&&"))       //jesli nie zawiera wiecej wiadomosci - kazda wiadomosc co najmniej &&1&&
                            break;
                        else response = response.substring(response.indexOf("&&"), response.length());
                        }
                    }
                });
                Main.setGra(true);      //TODO: czy na pewno randomowa wiadomosc uruchamia gre czy tylko pytanie/odpowiedz lub komunikat
                st.getStartButton().setDisable(false);
                st.getWelcomeText().setText("Welcome to our game. If you want to play, please press the button below!");
            } catch (IOException e) {
                //nie mozna na razie polaczyc z serwerwm
            }
        }
        return null;
    }
}
