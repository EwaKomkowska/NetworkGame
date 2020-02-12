package Controllers;

import Tasks.ConnectTask;
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


    public Button getStartButton() {
        return startButton;
    }

    public Text getWelcomeText() {
        return welcomeText;
    }

    public void setStartButton(Button startButton) {
        this.startButton = startButton;
    }

    public void setWelcomeText(Text welcomeText) {
        this.welcomeText = welcomeText;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }

    public void setIn(InputStream in) {
        this.in = in;
    }

    public void setOut(OutputStream out) {
        this.out = out;
    }

    @FXML
    private void initialize() {
        startButton.setText("Play now!!!");
        startButton.setDisable(true);
        welcomeText.setText("In this time, there isn't any game. You must wait.");

        ConnectTask ct = new ConnectTask(this);
        new Thread(ct).start();
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
    }
}
