package coap;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class HumaResource extends CoapResource {

	public String humidity = "<=>€##408521538#node_01#0#HUMA:48#";
	
    public HumaResource(String topic) {

        // resource identifier
        super(topic);

        // set display name
        getAttributes().setTitle("Temperature Resource");
    }

    @Override
    public void handleGET(CoapExchange exchange) {
        exchange.respond(humidity);
    }
}
