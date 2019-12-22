package sample;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.text.Text;

import java.io.IOException;

public class StartWindow {

    @FXML
    private Button startButton;

    @FXML
    private Text welcomeText;


    @FXML
    private void initialize() {
        startButton.setText("Play now!!!");
        welcomeText.setText("Welcome to our game. If you want to play, please press the button below!");
    }

    @FXML
    private void joinToPlay() throws IOException {
        Main.setRoot("sample");
    }
}
