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
        byte[] buffer = new byte[200];
        StringBuilder textMessage = new StringBuilder();

        while((currentCount = in.read(buffer, 0, 200)) > 0) {
            textMessage.append(new String(buffer, 0, currentCount));

            if (textMessage.indexOf("&", 2) > -1)
                break;
        }

        if (textMessage.length() == 0) {
            //TODO: czy jesli serwer aktywny, ale nic nie wysyła, to akceptować gre - dodac jakies wyjscie jak nie ma info od serwer po raz kolejny???
            //System.out.println("Błąd podczas odczytu wiadomości od serwera.");
            return null;
        }

        //System.out.println(new String(textMessage));
        return new String(textMessage);
    }
}
