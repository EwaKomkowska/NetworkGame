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
                st.setIn(st.getSocket().getInputStream());
                st.setOut(st.getSocket().getOutputStream());

                ReadTask message = new ReadTask(st.getIn());
                new Thread(message).start();        //watek odczytywania zostal uruchomiony

                message.addEventHandler(WorkerStateEvent.WORKER_STATE_SUCCEEDED, new EventHandler<WorkerStateEvent>() {
                    @Override
                    public void handle(WorkerStateEvent t) {
                        String response = message.getValue();
                        System.out.println("response" + response);
                        while (response != null) {
                            String [] messageValue = response.split("&&");  //TODO: jaki mamy znak oddzielajacy?
                            System.out.println("wiadomosc" + messageValue[1]);
                            switch (messageValue[1]) {
                                case "0":
                                    //TODO:Serwer sie uruchomil co dalej?
                                    System.out.println("Przyszła wiadomosc o grze!");
                                    break;
                                case "1":
                                    System.out.println("Dostałem pytania, odpowiedzi lub dalsza czesc");
                                    //TODO: przekazac na wyswietlanie juz
                                    Main.setButton1(messageValue[2]);       //skoro 0 jest indeksem to dalej przekazujemy treść
                                    break;
                                default:
                                    System.out.println("Nieznana wiadomosc");
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
                System.out.println("Czekam na połączenie z serwerem");
            }
        }
        return null;
    }
}
