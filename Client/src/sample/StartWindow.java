package sample;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.text.Text;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;

import static sample.Main.sock;

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
        Main.setGra(true);
        Main.setRoot("sample");

        /*Selector sel = Selector.open();

        sock = SocketChannel.open();
        sock.configureBlocking(false);
        sock.connect(new InetSocketAddress(Main.host, Main.port));      //np. host: "localhost", port: 1234
        SelectionKey sockKey = sock.register(sel, SelectionKey.OP_READ); // rejestracja kanału w selektorze
                    //OP_READ ustawia do odczytu - można też   SelectionKey.OP_READ | SelectionKey.OP_WRITE                */

        System.out.println("Gniazdo uruchomione!");
    }
}
