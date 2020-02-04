package Controllers;

import Tasks.ReadTask;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.text.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class StartWindow {

    @FXML
    private Button startButton;

    @FXML
    private Text welcomeText;

    private Socket socket;
    private InputStream in;

    public InputStream getIn() {
        return in;
    }

    public OutputStream getOut() {
        return out;
    }

    private OutputStream out;

    public Socket getSocket() { return socket; }


    @FXML
    private void initialize() {
        startButton.setText("Play now!!!");
        startButton.setDisable(true);
        welcomeText.setText("In this time, there isn't any game. You must wait.");

        try {
            socket = new Socket("127.0.0.1", 2115);         //TODO: zmienic przyjmowane dane na Main.host, Main.port
            in = socket.getInputStream();
            out = socket.getOutputStream();

            ReadTask message = new ReadTask(in);
            new Thread(message).start();        //watek odczytywania zostal uruchomiony

            message.addEventHandler(WorkerStateEvent.WORKER_STATE_SUCCEEDED, new EventHandler<WorkerStateEvent>() {
                        @Override
                        public void handle(WorkerStateEvent t) {
                            String response = message.getValue();
                            if (response != null) {
                                //TODO:Serwer sie uruchomil co dalej?
                                Main.setGra(true);
                                startButton.setDisable(false);
                            }
                        }
                    });
        } catch (IOException e) {
            System.out.println("Problem z połączeniem z serwerem");
            Main.setGra(false);
        }

        //po przyjściu komunikatu:
        //Welcome to our game. If you want to play, please press the button below!
    }

    @FXML
    private void joinToPlay() throws IOException {
        if (!Main.getGra()){
            //TODO: zatrzymac gre - cos poszlo nie tak
            //poniewaz pojawila sie gra, bo odblowoano przycisk, wiec polaczenie zostalo zerwane
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setHeaderText(null);
            alert.setHeaderText("Game is unavailable");
            alert.showAndWait();
            Main.getMainStage().close();
        }

        //TODO: trzeba jeszcze przekazac parametry na in i out
        Main.setRoot("/resources/gameView");

        /*Selector sel = Selector.open();

        sock = SocketChannel.open();
        sock.configureBlocking(false);
        sock.connect(new InetSocketAddress(Main.host, Main.port));      //np. host: "localhost", port: 1234
        SelectionKey sockKey = sock.register(sel, SelectionKey.OP_READ); // rejestracja kanału w selektorze
                    //OP_READ ustawia do odczytu - można też   SelectionKey.OP_READ | SelectionKey.OP_WRITE                */

        System.out.println("Gniazdo uruchomione!");
    }
}
