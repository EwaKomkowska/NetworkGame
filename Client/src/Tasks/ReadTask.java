package Tasks;

import javafx.concurrent.Task;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;

public class ReadTask extends Task<String> {

    private InputStream in;

    public ReadTask(InputStream in) {
        this.in = in;
    }


    @Override
    protected String call() throws Exception {
        //TODO: obsluga przyjmowania tekstu

        int currentCount = 0;
        byte[] buffer = new byte[128];      //TODO: czy odczytujemy długość?
        StringBuilder textMessage = new StringBuilder();

        while((currentCount = in.read(buffer, 0, 128)) > 0) {
            //totalCount = totalCount + currentCount;
            textMessage.append(new String(buffer, 0, currentCount, StandardCharsets.US_ASCII));
            //if(totalCount == length) break;
        }
        if(currentCount < 0) {
            System.out.println("Błąd podczas odczytu wiadomości od serwera.");
            return null;
        }


        return new String(textMessage);
    }
}
