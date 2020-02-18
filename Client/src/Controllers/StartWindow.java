package Controllers;

import Tasks.ConnectTask;
import Tasks.ReadTask;
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

    private static Socket socket;
    private static InputStream in;
    private static OutputStream out;

    public static InputStream getIn() {
        return in;
    }

    public static OutputStream getOut() {
        return out;
    }

    public static Socket getSocket() { return socket; }

    public Button getStartButton() {
        return startButton;
    }

    public Text getWelcomeText() {
        return welcomeText;
    }

    public void setSocket(Socket socket) {
        StartWindow.socket = socket;
    }

    public void setIn(InputStream in) {
        StartWindow.in = in;
    }

    public void setOut(OutputStream out) {
        StartWindow.out = out;
    }

    @FXML
    private void initialize() {
        startButton.setText("Play now!!!");
        startButton.setDisable(true);
        welcomeText.setText("In this time, there isn't any game. You must wait.");

        ConnectTask ct = new ConnectTask(this);
        new Thread(ct).start();
        //Controller c = new Controller();
        //c.listenResponses();
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

        Main.setRoot("/resources/gameView");
    }
}
