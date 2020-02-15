package Tasks;

import javafx.concurrent.Task;

import java.io.InputStream;

public class ReadTask extends Task<String> {

    private InputStream in;
    public ReadTask(InputStream in) {
        this.in = in;
    }


    @Override
    protected String call() throws Exception {
        int currentCount = 0;
        byte[] buffer = new byte[200];      //TODO: czy odczytujemy długość?
        StringBuilder textMessage = new StringBuilder();

        while((currentCount = in.read(buffer, 0, 200)) > 0) {
            textMessage.append(new String(buffer, 0, currentCount));

            //TODO: co zrobic zeby skonczyc sluchanie i przetworzyc
            if (textMessage.indexOf("&", 2) > -1)   //tODO: czy tak moze zostac, bo minimum wiadomsci to 5?
                break;
        }

        if (textMessage.length() == 0) {
            //TODO: czy jesli serwer aktywny, ale nic nie wysyła, to akceptować gre?
            //tODO: dodac jakies wyjscie jak nie ma info od serwer po raz kolejny???
            System.out.println("Błąd podczas odczytu wiadomości od serwera.");
            return null;
        }

        System.out.println(new String(textMessage));
        return new String(textMessage);
    }
}
