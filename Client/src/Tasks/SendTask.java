package Tasks;

import javafx.concurrent.Task;

import java.io.OutputStream;

public class SendTask extends Task<Void> {

    private OutputStream out;
    private String text;

    public SendTask(String text, OutputStream out){
        this.out = out;
        this.text = text;
    }

    @Override
    protected Void call() throws Exception {
        //TODO: tu musi byc cała obsluga wysyłanej wiadomosci
        return null;
    }
}
