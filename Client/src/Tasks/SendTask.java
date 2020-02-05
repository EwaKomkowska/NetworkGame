package Tasks;

import javafx.concurrent.Task;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

public class SendTask extends Task<Void> {

    private OutputStream out;
    private String text;

    public SendTask(String text, OutputStream out){
        this.out = out;
        this.text = text;
    }

    @Override
    protected Void call() {
        try {
            String message = "&&" + text + "&&";    //TODO: czy to wystarczy do wysłania odpowierdzi?
            // czy ma być jeszcze coś odnośnie końca, czy można podać koniec tylko?
            out.write(message.getBytes(StandardCharsets.UTF_8));

        } catch (IOException e) {
            System.out.println("Error until sending a message!");
        }

        return null;
    }
}
