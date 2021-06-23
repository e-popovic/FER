package coap;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.coap.CoAP.Type;
import org.eclipse.californium.core.server.resources.CoapExchange;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

public class TempResource extends CoapResource  {

	public String temperature = "";
	public boolean firstReading = true;
	
    public TempResource(String topic) {

        super(topic);
        getAttributes().setTitle("Temperature Resource");
        
        setObservable(true);
		setObserveType(Type.CON);
		getAttributes().setObservable();

        Timer timer = new Timer();
		timer.schedule(new tempReading(), 0, 1000);	
    }

    @Override
    public void handleGET(CoapExchange exchange) {
    	exchange.setMaxAge(1);
        exchange.respond(temperature);
        if (firstReading) firstReading = false;      
    }
    
    // vrati vrijednost temperature iz waspmote poruke
    static public String parser(String nonparsed) {
    	return nonparsed.substring(nonparsed.indexOf(":")+1, nonparsed.indexOf(":")+6);
    }
    
    private class tempReading extends TimerTask {
		@Override
		public void run() {					
			if (firstReading) {
				String waspmoteTemp = "<=>€##408521538#node_01#0#TCA:26.13#";	//prvo postavljanje na ocitanu temp
		        temperature = parser(waspmoteTemp);		        
			}
			else {
				//temperatura 18 - 30	
				Random r = new Random();		
				double newTemp = 18.0 + r.nextDouble() * (30.0 - 18.0);
				temperature = Double.toString(newTemp);
				temperature = temperature.substring(0,5);
			}			
			changed();
		}
	}
}
