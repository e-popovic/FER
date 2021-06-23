package coap;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.eclipse.californium.core.server.resources.Resource;
import static org.eclipse.californium.core.coap.CoAP.ResponseCode.*;

import java.nio.charset.StandardCharsets;

public class SensorsResource extends CoapResource {

	public SensorsResource() {
        // resource identifier
        super("sensors");

        // set display name
        getAttributes().setTitle("Sensor Resource");
    }
	
	@Override
    public void handleGET(CoapExchange exchange) {
        exchange.respond(METHOD_NOT_ALLOWED);
    }
	
	@Override
    public void handlePOST(CoapExchange exchange) {
        byte[] payload = exchange.getRequestPayload();
        
        try {
        	String topic = new String(payload, StandardCharsets.UTF_8);
            Resource child = getChild(getName());
            String childPath = getName() + "/" + topic;
            add(new TempResource(topic));
            child = getChild(topic);
            child.setPath(childPath);
            
            exchange.respond(CREATED, topic);
            
        } catch (Exception e) {
            exchange.respond(BAD_REQUEST, "Bad request!");
        }
    }

}