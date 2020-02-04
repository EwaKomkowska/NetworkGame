package Controllers;

import Tasks.ReadTask;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.text.Text;

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
    private Text text1;

    @FXML
    private void initialize() throws IOException {

/*      tutaj trzeba odczytac dane z serwera i jakos kluczami podzielić na pytanie (historyjke) i poszczegolne opcje???
        Do tego trzeba pamietac, ze po wybraniu opcji moze byc konieczne dopisanie dalszej historyjki?

        sel.select();                               // oczekiwanie na zdarzenie

        // selectedKeys() zawiera listę kluczy dla których można wykonać żądaną operację
        // i ustawia im dostępne zdarzenia (readyOps, isReadable, is…)
        assert sel.selectedKeys().size() == 1;
        assert sel.selectedKeys().iterator().next() == sockKey;

        bb.clear();                                 // przygotowanie bufora do pracy
        int count = sock.read(bb);

        if(count == -1) {                           // -1 oznacza EOF
            sockKey.cancel();                       // cancel usuwa klucz z selektora
            return;
        }

        System.out.write(bb.array(), 0, bb.position());

        sel.selectedKeys().clear();                 // klucze są w zbiorze do momentu usunięcia*/

    /*tu jest druga wersja z buforami i kanałem
        ByteBuffer question = ByteBuffer.allocate(128);
        ByteBuffer option1   = ByteBuffer.allocate(128);
        ByteBuffer option2   = ByteBuffer.allocate(128);
        ByteBuffer option3   = ByteBuffer.allocate(128);
        ByteBuffer option4   = ByteBuffer.allocate(128);


        ByteBuffer[] bufferArray = { question, option1, option2, option3, option4 };

        int bytesRead = sock.read(bufferArray);
        if (byteRead == -1) System.out.println("The connection is closed");

        //a tu jeszcze inna wersja ze sprawdzaniem obecnie otwartych gniazd: http://tutorials.jenkov.com/java-nio/selectors.html

        //String zmienna = "Przyklad danych";
        //po zaalokowaniu pamieci i wyczyszczeniu bufora
        //zapisanie danych do bufora: buf.put(zmienna.getBytes());
        //na koniec buf.flip();
     */

    /*potwierdzenie odbioru zrobione na zasadzie wyslania inormacji i opcji???*/

        firstButton.setText(Main.getButton1());
        secondButton.setText(Main.getButton2());
        thirdButton.setText(Main.getButton3());
        fourthButton.setText(Main.getButton4());
        text1.setText("This is field to show a story");
    }

    @FXML
    private void chooseFirst() throws IOException {
        System.out.println("Zagłosowałam na opcje 1");

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        //Main.getMainStage().close();
    }

    @FXML
    private void chooseSecond() throws IOException {
        System.out.println("Zagłosowałam na opcje 2");
        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);
        /*String myAnswer = "2";

        ByteBuffer buf = ByteBuffer.allocate(8);
        buf.clear();
        buf.put(myAnswer.getBytes());

        buf.flip();

        while(buf.hasRemaining()) {
            sock.write(buf);
        }*/


        //Main.getMainStage().close();
    }

    @FXML
    private void chooseThird() throws IOException {
        System.out.println("Zagłosowałam na opcje 3");

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        //Main.getMainStage().close();
    }

    @FXML
    private void chooseFourth() throws IOException {
        System.out.println("Zagłosowałam na opcje 4");

        firstButton.setDisable(true);
        secondButton.setDisable(true);
        thirdButton.setDisable(true);
        fourthButton.setDisable(true);

        System.out.println("Wysyłam komunikat o zaglosowaniu");
        //Main.getMainStage().close();
    }


    private void receiveMessage(String[] responseParts) {
        //messageObservableList.add(new Message(responseParts[3], responseParts[1], responseParts[4], responseParts[2]));
        //TODO: dodac otrzymana wiadomosc do tekstu
        text1.setText(responseParts[0]);
        firstButton.setText(responseParts[1]);
        secondButton.setText(responseParts[2]);
        thirdButton.setText(responseParts[3]);
        fourthButton.setText(responseParts[4]);
        // po otrzymaniu wiadomości kontynuujemy nasłuchiwanie odpowiedzi od serwera
        listenResponses();
    }

    private void listenResponses() {
        ReadTask readTask = new ReadTask(in);
        new Thread(readTask).start();
        readTask.addEventHandler(WorkerStateEvent.WORKER_STATE_SUCCEEDED, new EventHandler<WorkerStateEvent>() {
            @Override
            public void handle(WorkerStateEvent t) {
                String response = readTask.getValue();
                if (response != null) {
                    //TODO: jaki znacznik rozdziela wiadomosci?
                    String[] responseParts = response.split("//divider@//");

                    // w zależności od typu odpowiedzi podejmujemy różne akcje
                    if (responseParts[0].equals("FOUND") || responseParts[0].equals("NOTFOUND")) {
                    /*
                        responseParts[0] --> nagłówek 'FOUND' lub 'NOTFOUND'
                        responseParts[1] --> nazwa tematu
                        responseParts[2] --> opis tematu
                    */
                        //TODO: tu byla inna funkcja, czy my takiej potrzebujemy
                        //searchAction(responseParts);
                    } else if (responseParts[0].equals("MESSAGE")) {
                    /*
                        responseParts[0] --> nagłówek 'MESSAGE'
                        responseParts[1] --> tytuł wiadomości
                        responseParts[2] --> treść wiadomości
                        responseParts[3] --> nazwa tematu
                        responseParts[4] --> autor
                    */
                        receiveMessage(responseParts);
                    }
                }
            }
        });
    }

    @FXML
    private void exit () {
        //TODO: zamknac gniazdo, zwolnic pamiec
        Main.getMainStage().close();
    }
}
