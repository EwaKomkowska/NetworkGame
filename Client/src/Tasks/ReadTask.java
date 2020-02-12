package Tasks;

import Controllers.Main;
import javafx.concurrent.Task;

import java.io.InputStream;

public class ReadTask extends Task<String> {

    private InputStream in;
    public ReadTask(InputStream in) {
        this.in = in;
    }


    @Override
    protected String call() throws Exception {
        //TODO: obsluga przyjmowania tekstu

        int currentCount = 0;
        byte[] buffer = new byte[100];      //TODO: czy odczytujemy długość?
        StringBuilder textMessage = new StringBuilder();
        int ind = -1;

        while((currentCount = in.read(buffer, 0, 100)) > 0) {

            textMessage.append(new String(buffer, 0, currentCount));     //, StandardCharsets.US_ASCII)
            //System.out.println(currentCount + new String(textMessage));

            //TODO: co zrobic zeby skonczyc sluchanie i przetworzyc
            if ((ind =textMessage.indexOf("&", 2)) > -1)   //tODO: czy tak moze zostac, bo minimum wiadomsci to 5?
                break;
        }

        if (textMessage.length() == 0) {
            //TODO: czy jesli serwer aktywny, ale nic nie wysyła, to akceptować gre?
            System.out.println("Błąd podczas odczytu wiadomości od serwera.");
            return null;
        }

        return new String(textMessage);
    }
}
