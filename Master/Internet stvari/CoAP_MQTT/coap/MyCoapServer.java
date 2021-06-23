package coap;

import org.eclipse.californium.core.CoapServer;

public class MyCoapServer extends CoapServer {

    public static void main(String[] args) {
        // port 5683
        CoapServer server = new CoapServer();
        
        server.add(new SensorsResource());      
        server.start();
    }	
}
